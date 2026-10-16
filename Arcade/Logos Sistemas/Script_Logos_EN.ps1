# ==========================================================
#   UNIVERSAL MARQUEE CONVERTER - 24-BIT RGB (128x32)
# ==========================================================
Add-Type -AssemblyName System.Drawing

# --- DITHERING RGB565 (Floyd-Steinberg) ---
# The LED panel only supports 5 bits for red, 6 bits for green, and 5 bits for blue (RGB565).
# This function pre-quantizes the BMP using error diffusion so that,
# when the ESP32 truncates the color values, gradients do not show color banding.
function Get-QuantizedValue([double]$valor, [int]$bits) {
    $niveles = [math]::Pow(2, $bits) - 1
    $paso = 255.0 / $niveles
    $q = [math]::Round($valor / $paso) * $paso
    if ($q -lt 0) { $q = 0 }
    if ($q -gt 255) { $q = 255 }
    return $q
}

function Convert-ToRGB565Dithered([System.Drawing.Bitmap]$bmp) {
    $w = $bmp.Width
    $h = $bmp.Height
    $rect = New-Object System.Drawing.Rectangle(0, 0, $w, $h)
    $bmpData = $bmp.LockBits($rect, [System.Drawing.Imaging.ImageLockMode]::ReadWrite, [System.Drawing.Imaging.PixelFormat]::Format24bppRgb)
    $stride = $bmpData.Stride
    $bytes = $stride * $h
    $buffer = New-Object byte[] $bytes
    [System.Runtime.InteropServices.Marshal]::Copy($bmpData.Scan0, $buffer, 0, $bytes)

    # Copy to double-precision matrices so positive and negative error can accumulate.
    $r = New-Object double[] ($w * $h)
    $g = New-Object double[] ($w * $h)
    $b = New-Object double[] ($w * $h)
    for ($y = 0; $y -lt $h; $y++) {
        for ($x = 0; $x -lt $w; $x++) {
            $idx = $y * $stride + $x * 3
            $i = $y * $w + $x
            $b[$i] = $buffer[$idx]
            $g[$i] = $buffer[$idx + 1]
            $r[$i] = $buffer[$idx + 2]
        }
    }

    for ($y = 0; $y -lt $h; $y++) {
        for ($x = 0; $x -lt $w; $x++) {
            $i = $y * $w + $x
            $oldR = $r[$i]; $oldG = $g[$i]; $oldB = $b[$i]

            $newR = Get-QuantizedValue $oldR 5
            $newG = Get-QuantizedValue $oldG 6
            $newB = Get-QuantizedValue $oldB 5

            $r[$i] = $newR; $g[$i] = $newG; $b[$i] = $newB

            $errR = $oldR - $newR
            $errG = $oldG - $newG
            $errB = $oldB - $newB

            # Floyd-Steinberg error diffusion: right 7/16, bottom-left 3/16, bottom 5/16, bottom-right 1/16.
            if ($x + 1 -lt $w) {
                $j = $i + 1
                $r[$j] += $errR * 7 / 16; $g[$j] += $errG * 7 / 16; $b[$j] += $errB * 7 / 16
            }
            if ($y + 1 -lt $h) {
                if ($x - 1 -ge 0) {
                    $j = $i + $w - 1
                    $r[$j] += $errR * 3 / 16; $g[$j] += $errG * 3 / 16; $b[$j] += $errB * 3 / 16
                }
                $j = $i + $w
                $r[$j] += $errR * 5 / 16; $g[$j] += $errG * 5 / 16; $b[$j] += $errB * 5 / 16
                if ($x + 1 -lt $w) {
                    $j = $i + $w + 1
                    $r[$j] += $errR * 1 / 16; $g[$j] += $errG * 1 / 16; $b[$j] += $errB * 1 / 16
                }
            }
        }
    }

    for ($y = 0; $y -lt $h; $y++) {
        for ($x = 0; $x -lt $w; $x++) {
            $idx = $y * $stride + $x * 3
            $i = $y * $w + $x
            $buffer[$idx]     = [byte]([math]::Round($b[$i]))
            $buffer[$idx + 1] = [byte]([math]::Round($g[$i]))
            $buffer[$idx + 2] = [byte]([math]::Round($r[$i]))
        }
    }

    [System.Runtime.InteropServices.Marshal]::Copy($buffer, 0, $bmpData.Scan0, $bytes)
    $bmp.UnlockBits($bmpData)
}

# --- DIMENSION CONFIGURATION ---
$ancho = 128
$alto = 32

Write-Host "==========================================" -ForegroundColor Magenta
Write-Host "    BATCH IMAGE CONVERTER" -ForegroundColor White
Write-Host "==========================================" -ForegroundColor Magenta

# 1. Request the source folder
$inputOrigen = Read-Host "Enter the path to the folder containing the images"
$rutaOrigen = $inputOrigen.Trim('"')

if (!(Test-Path $rutaOrigen)) {
    Write-Host "`n[ERROR] The specified folder was not found: $rutaOrigen" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit
}

# 2. Request the destination folder (optional)
$inputDestino = Read-Host "Destination path [press Enter to save in the source folder]"
$rutaDestino = if ([string]::IsNullOrWhiteSpace($inputDestino)) { $rutaOrigen } else { $inputDestino.Trim('"') }

if (!(Test-Path $rutaDestino)) {
    New-Item -ItemType Directory -Force -Path $rutaDestino | Out-Null
}

# 3. Find all valid images (recursive search supported)
Write-Host "`nSearching for valid images..." -ForegroundColor Cyan
$imagenes = Get-ChildItem -Path $rutaOrigen -File -Recurse | Where-Object {
    $_.Extension -match '^\.(png|jpg|jpeg|bmp|gif)$'
}

if ($imagenes.Count -eq 0) {
    Write-Host "[WARNING] No images (.png, .jpg, .jpeg, .bmp, .gif) were found in the folder." -ForegroundColor Yellow
    Read-Host "Press Enter to exit"
    exit
}

Write-Host "Found $($imagenes.Count) images ready to process.`n" -ForegroundColor Green

# 4. Batch processing
$contadorExito = 0
foreach ($img in $imagenes) {
    # Convert the name to lowercase to stay consistent with the Recalbox scripts.
    $nombreSinExt = [System.IO.Path]::GetFileNameWithoutExtension($img.Name).Trim().ToLower()
    $finalPath = Join-Path $rutaDestino "$nombreSinExt.bmp"

    Write-Host "Processing: $($img.Name) -> $nombreSinExt.bmp ... " -NoNewline

    # Initialize control variables so they can be released if processing fails.
    $bmp = $null
    $g = $null
    $oldImg = $null

    try {
        # Force the canvas to exactly 128x32 using native 24-bit format (3 bytes per pixel).
        $bmp = New-Object System.Drawing.Bitmap($ancho, $alto, [System.Drawing.Imaging.PixelFormat]::Format24bppRgb)
        $g = [System.Drawing.Graphics]::FromImage($bmp)

        # High-quality bicubic interpolation to avoid jagged edges at 128x32.
        $g.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic
        $g.PixelOffsetMode = [System.Drawing.Drawing2D.PixelOffsetMode]::HighQuality
        $g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::HighQuality
        $g.CompositingQuality = [System.Drawing.Drawing2D.CompositingQuality]::HighQuality

        # Load and draw the original image at the scaled size.
        $oldImg = [System.Drawing.Image]::FromFile($img.FullName)
        $g.DrawImage($oldImg, 0, 0, $ancho, $alto)

        # Close the original file immediately to avoid file locks.
        $oldImg.Dispose()
        $g.Dispose()

        # Pre-quantize with RGB565 dithering to prevent color banding on the panel.
        Convert-ToRGB565Dithered $bmp

        # Save as a standard bitmap.
        $bmp.Save($finalPath, [System.Drawing.Imaging.ImageFormat]::Bmp)
        $bmp.Dispose()

        Write-Host "[OK]" -ForegroundColor Green
        $contadorExito++
    } catch {
        Write-Host "[ERROR]" -ForegroundColor Red
        Write-Host "    Details: $_" -ForegroundColor Yellow

        # Emergency cleanup if processing fails partway through a file.
        if ($null -ne $oldImg) { $oldImg.Dispose() }
        if ($null -ne $g) { $g.Dispose() }
        if ($null -ne $bmp) { $bmp.Dispose() }
    }
}

Write-Host "`n==========================================" -ForegroundColor Magenta
Write-Host "COMPLETED: successfully processed $contadorExito of $($imagenes.Count) images." -ForegroundColor Green
Write-Host "Marquees are ready in: $rutaDestino" -ForegroundColor White
Write-Host "==========================================" -ForegroundColor Magenta
Write-Host "Press any key to exit"
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
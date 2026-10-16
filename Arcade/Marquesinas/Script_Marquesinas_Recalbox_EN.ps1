# ==========================================================
#   RETRO PIXEL MARQUEE MANAGER - RECALBOX (COMPLETE)
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

# --- INITIAL CONFIGURATION ---
$defRutaRoms = "\\192.168.1.123\share\roms"
$defRutaSD = "C:\Export_Arcade"
$ancho = 128
$alto = 32

Write-Host "==========================================" -ForegroundColor Magenta
Write-Host "     PATH CONFIGURATION — RECALBOX" -ForegroundColor White
Write-Host "==========================================" -ForegroundColor Magenta

# 1. Request paths
$inputRoms = Read-Host "Recalbox ROMs path [press Enter for: $defRutaRoms]"
$rutaRomsRecalbox = if ([string]::IsNullOrWhiteSpace($inputRoms)) { $defRutaRoms } else { $inputRoms.Trim('"') }

$inputSD = Read-Host "SD destination path [press Enter for: $defRutaSD]"
$rutaSD = if ([string]::IsNullOrWhiteSpace($inputSD)) { $defRutaSD } else { $inputSD.Trim('"') }

if (!(Test-Path $rutaRomsRecalbox)) {
    Write-Host "`n[ERROR] Not found: $rutaRomsRecalbox" -ForegroundColor Red
    Read-Host "Press Enter to exit"
    exit
}

# 1.5. Select the source image type
Write-Host "`n==========================================" -ForegroundColor Magenta
Write-Host "   IMAGE SOURCE (SCRAPER)" -ForegroundColor White
Write-Host "==========================================" -ForegroundColor Magenta
Write-Host "1. Standard image (<image>)"
Write-Host "2. Thumbnail (<thumbnail>)"
Write-Host "3. Logo / wheel art (<logo>)"
Write-Host "=========================================="

$opcionImagen = Read-Host "Select the image type to use [press Enter for option 1]"
if ([string]::IsNullOrWhiteSpace($opcionImagen)) { $opcionImagen = "1" }

# 2. Get systems
$todosLosSistemas = Get-ChildItem -Path $rutaRomsRecalbox -Directory | Where-Object {
    Test-Path (Join-Path $_.FullName "gamelist.xml")
}

# 3. System selection menu
Write-Host "`n==========================================" -ForegroundColor Magenta
Write-Host "   SYSTEM SELECTION" -ForegroundColor White
Write-Host "==========================================" -ForegroundColor Magenta
Write-Host "0. [ALL SYSTEMS]" -ForegroundColor Yellow
$i = 1
foreach ($sys in $todosLosSistemas) {
    Write-Host "$i. $($sys.Name)"
    $i++
}
Write-Host "=========================================="

$seleccion = Read-Host "Select the system number to index"

$sistemasAProcesar = @()
if ($seleccion -eq "0") {
    $sistemasAProcesar = $todosLosSistemas
} elseif ($seleccion -gt 0 -and $seleccion -le $todosLosSistemas.Count) {
    $sistemasAProcesar = $todosLosSistemas[$seleccion - 1]
} else {
    Write-Host "Invalid selection." -ForegroundColor Red
    exit
}

# 4. Processing
foreach ($sysFolder in $sistemasAProcesar) {
    $sistemaNombre = $sysFolder.Name
    $xmlPath = Join-Path $sysFolder.FullName "gamelist.xml"
    Write-Host "`n>>> PROCESSING: $($sistemaNombre.ToUpper())" -ForegroundColor Cyan

    $destDirImagenes = Join-Path $rutaSD "Arcade\$sistemaNombre"
    if (!(Test-Path $destDirImagenes)) { New-Item -ItemType Directory -Force -Path $destDirImagenes | Out-Null }

    [xml]$xml = Get-Content $xmlPath -Raw -Encoding UTF8
    $listaJuegos = New-Object System.Collections.Generic.List[string]

    foreach ($game in $xml.gameList.game) {
        # Select the XML node according to the option chosen by the user.
        $nodoImagen = if ($opcionImagen -eq "2") {
            $game.thumbnail
        } elseif ($opcionImagen -eq "3") {
            $game.logo
        } else {
            $game.image
        }

        # Process the game if it exists and contains the selected tag.
        if ($game.path -and $nodoImagen) {
            $imageValue = $nodoImagen
            if ($imageValue -is [System.Array]) { $imageValue = $imageValue[0] }

            $romName = [System.IO.Path]::GetFileNameWithoutExtension($game.path).Trim().ToLower()
            $relImgPath = $imageValue.TrimStart('.').TrimStart('/').TrimStart('\')
            $imgPath = Join-Path $sysFolder.FullName $relImgPath

            if (Test-Path $imgPath) {
                try {
                    $bmp = New-Object System.Drawing.Bitmap($ancho, $alto, [System.Drawing.Imaging.PixelFormat]::Format24bppRgb)
                    $g = [System.Drawing.Graphics]::FromImage($bmp)
                    $g.InterpolationMode = [System.Drawing.Drawing2D.InterpolationMode]::HighQualityBicubic

                    $oldImg = [System.Drawing.Image]::FromFile($imgPath)
                    $g.DrawImage($oldImg, 0, 0, $ancho, $alto)
                    $oldImg.Dispose()

                    # Pre-quantize with RGB565 dithering to prevent color banding on the panel.
                    Convert-ToRGB565Dithered $bmp

                    $finalPath = Join-Path $destDirImagenes "$romName.bmp"
                    $bmp.Save($finalPath, [System.Drawing.Imaging.ImageFormat]::Bmp)
                    $bmp.Dispose()
                    $g.Dispose()

                    if (!$listaJuegos.Contains($romName)) { $listaJuegos.Add($romName) }
                    Write-Host " OK: $romName"
                } catch {
                    Write-Host " Error: $romName" -ForegroundColor Yellow
                }
            }
        }
    }

    if ($listaJuegos.Count -gt 0) {
        $listaOrdenada = $listaJuegos | Sort-Object -Unique
        $destDirArcadeRaiz = Join-Path $rutaSD "Arcade"
        if (!(Test-Path $destDirArcadeRaiz)) { New-Item -ItemType Directory -Force -Path $destDirArcadeRaiz | Out-Null }
        $txtPath = Join-Path $destDirArcadeRaiz "$sistemaNombre.txt"
        [System.IO.File]::WriteAllLines($txtPath, $listaOrdenada)
        Write-Host "Generated index $sistemaNombre.txt." -ForegroundColor Green
    }
}

Write-Host "`nPROCESS FINISHED" -ForegroundColor White
Write-Host "Press any key to exit"
$null = $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
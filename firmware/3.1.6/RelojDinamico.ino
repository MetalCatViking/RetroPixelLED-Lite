// ====================================================================
// DYNAMIC CLOCK — loop (new sketch tab: RelojDinamico.ino)
//
// Uses the same "contract" as mostrarRelojLite(): remains in the loop
// for clockDuration seconds, handling the button, IR, web server, and
// TCP marquee; exits if the menu opens, the sleep timer starts, or a request
// interrupts it. This lets all existing call sites continue to work as-is
// (clock-only mode, clock every N GIFs, etc.).
// ====================================================================

static unsigned long rdSalidaMs  = 0;   // when we last exited
static int           rdCaraVista = -1;  // which face was being shown
static int           rdRotIdx    = -1;  // current face in "rotate" mode
static unsigned long rdRotDesde  = 0;   // when it was selected

// relojDinamico: 0 = off (Lite clock), 1..RD_NUM_CARAS = face index + 1
void mostrarRelojDinamico(bool conTransicion) {
    (void)conTransicion;   // the particle transition belongs to the Lite clock
    if (!display) return;
    if (arcadeEnable > 0 && estadoActual == ESTADO_ARCADE) return;

    int idx = relojDinamico - 1;
    const int modoEntrada = dynModo;
    if (dynModo != 0 && RD_NUM_CARAS > 1) {
        const bool reentrada = (rdCaraVista >= 0) && (millis() - rdSalidaMs < 1500UL);
        if (dynModo == 1) {                                  // random: choose a different face each time the clock appears
            if (reentrada) idx = rdCaraVista;
            else { int nueva; do { nueva = random(RD_NUM_CARAS); } while (nueva == rdCaraVista); idx = nueva; }
        } else {                                             // rotate: switch to the next face every dynRotMin minutes
            if (rdRotIdx < 0) { rdRotIdx = idx; rdRotDesde = millis(); }
            if (millis() - rdRotDesde >= (unsigned long)dynRotMin * 60000UL) { rdRotIdx = (rdRotIdx + 1) % RD_NUM_CARAS; rdRotDesde = millis(); }
            idx = rdRotIdx;
        }
    }
    if (idx < 0 || idx >= RD_NUM_CARAS) idx = 0;
    interrumpirReproduccion = false;

    struct tm t;
    if (!getLocalTime(&t)) return;

    // With DOUBLE_BUFF=1, the engine redraws the full scene (two frames after
    // each change). This is the same condition setup() uses to enable double buffering.
    const bool doble = (doubleBuff == 1) && (arcadeEnable == 0) && (transitionEnable != 1);

    // In "clock only" mode, the main loop calls us again every
    // clockDuration seconds. If this is an immediate re-entry and the screen
    // is still ours, do NOT reset the scene (Mario will not jump again).
    const bool continuar = (rdCaraVista == idx) && (millis() - rdSalidaMs < 1500UL);
    if (!continuar) {
        rdIniciar(idx, t.tm_hour, t.tm_min, t.tm_sec, offset, PANEL_RES_X * panelChain, doble);
        display->flipDMABuffer();
        rdCaraVista = idx;
    }

    unsigned long startTime = millis();
    while (millis() - startTime < (clockDuration * 1000UL) && !interrumpirReproduccion) {

        gestionarBotonMenu();
        leerControlRemoto();
        server.handleClient();
        verificarMarquesinaTCP();

        if (isSleeping || estadoActual != ESTADO_GIFS) {
            interrumpirReproduccion = true;
            rdCaraVista = -1;               // the menu or another mode has overwritten the screen
            break;
        }

        // If the face changes or is disabled from the PWA/OSD, exit now
        // so the main loop can re-enter with the new settings.
        if (relojDinamico == 0 || dynModo != modoEntrada || (dynModo == 0 && relojDinamico != idx + 1)) { rdCaraVista = -1; break; }
        if (dynModo == 2 && (millis() - rdRotDesde) >= (unsigned long)dynRotMin * 60000UL) break;   // time to rotate to the next face

        if (!getLocalTime(&t)) break;

        if (rdActualizar(t.tm_hour, t.tm_min, t.tm_sec, millis()))
            display->flipDMABuffer();

        delay(10);
    }
    rdSalidaMs = millis();
}

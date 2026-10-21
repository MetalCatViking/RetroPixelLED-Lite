// ====================================================================
//  PortalWiFi.ino - Wi-Fi setup portal (configure the network without editing config.ini)
//
//  Open it from the OSD menu (Wi-Fi > Add Network), or automatically when
//  Wi-Fi is enabled but no SSID is configured. The flow is:
//
//    1. portalEscanearRedes()   scans before starting the access point
//    2. portalIniciarAP()       starts an AP with a random password + captive DNS
//    3. portalIniciarWeb()      starts a temporary server for portal routes
//    4. loop:                   handles requests, tests the submitted network, and
//                               checks whether to exit (web, IR remote, or physical button)
//    5. portalLimpiar()         releases resources; restarts or continues offline
//
//  LANGUAGE: portal text (panel and web) lives in PortalTextos.h
//  (ES, EN, and FR) and is selected using LANGUAGE in config.ini; any other language
//  falls back to English. The /idioma/*.json files are only used by the OSD menus.
//  Fixed HTML fragments are defined in PortalPaginas.h.
//
// ====================================================================
#include <DNSServer.h>
#include "PortalPaginas.h"
#include "PortalTextos.h"

#define PORTAL_AP_SSID     "RetroPixel-LED"
#define PORTAL_TIMEOUT_MS  300000UL    // maximum open time: 5 minutes
#define PORTAL_PRUEBA_MS   15000UL     // maximum time to test a network
#define PORTAL_MAX_REDES   12

// --------------------------------------------------------------------
// Portal state (used only while the portal is open)
// --------------------------------------------------------------------
struct RedWiFi { String ssid; int rssi; bool abierta; };

static std::vector<RedWiFi> portalRedes;
static WebServer*           portalWeb = nullptr;
static DNSServer            portalDns;

static char portalClave[9];
static char portalLinea2[24], portalLinea3[24], portalLinea4[24];
static char portalSsidNuevo[33];
static char portalPassNueva[64];
static int  portalIdioma = PORTAL_IDIOMA_EN;
static bool portalPendiente = false;    // user submitted the form
static bool portalCancelado = false;    // user clicked "Cancel" on the web page

// ====================================================================
//  UTILITIES
// ====================================================================

// Select the portal language from LANGUAGE in config.ini: ES, FR, or English by default
static void portalElegirIdioma() {
    if (strcasecmp(idiomaActivo, "ES") == 0)      portalIdioma = PORTAL_IDIOMA_ES;
    else if (strcasecmp(idiomaActivo, "FR") == 0) portalIdioma = PORTAL_IDIOMA_FR;
    else                                          portalIdioma = PORTAL_IDIOMA_EN;   // EN and any other value
    Serial.printf("[PORTAL] Idioma del portal: %s (config: %s)\n", PORTAL_LANG[portalIdioma], idiomaActivo);
}

// Portal text in the selected language (id is a PortalTexto enum value)
static const char* portalT(int id) {
    const char* t = PORTAL_TXT[portalIdioma][id];
    return t ? t : PORTAL_TXT[PORTAL_IDIOMA_EN][id];     // fallback in case a translation is missing
}

static String escHtml(const String& s) {
    String o;
    o.reserve(s.length() + 8);
    for (size_t i = 0; i < s.length(); i++) {
        char c = s[i];
        if (c == '&') o += F("&amp;");
        else if (c == '<') o += F("&lt;");
        else if (c == '>') o += F("&gt;");
        else if (c == '"') o += F("&quot;");
        else o += c;
    }
    return o;
}

// Four lines on the panel (default font, ASCII only, max. 21 characters per line)
static void portalPantalla(const char* l1, const char* l2, const char* l3, const char* l4) {
    display->fillScreen(0);
    display->setFont(nullptr);
    display->setTextSize(1);
    display->setTextWrap(false);
    printMenuCentrado(l1, 0, display->color565(0, 255, 255), offset);
    display->drawLine(offset, 7, offset + offset, 7, display->color565(100, 100, 100));
    display->setTextColor(display->color565(255, 255, 255));
    display->setCursor(offset + 2, 9);    display->print(l2);
    display->setCursor(offset + 2, 17);   display->print(l3);
    display->setCursor(offset + 2, 25);   display->print(l4);
    display->flipDMABuffer();
}

static void portalPantallaPrincipal() {
    portalPantalla(portalT(PT_P_TITULO), portalLinea2, portalLinea3, portalLinea4);
}

// ====================================================================
//  1. NETWORK SCAN
// ====================================================================
static void portalEscanearRedes() {
    portalRedes.clear();

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(200);

    int n = WiFi.scanNetworks();
    if (n <= 0) { delay(500); n = WiFi.scanNetworks(); }     // retry once if the first scan fails
    Serial.printf("[PORTAL] Redes encontradas: %d\n", n);

    for (int i = 0; i < n; i++) {
        String s = WiFi.SSID(i);
        if (s.length() == 0) continue;                        // hidden networks must be entered manually
        int rssi = WiFi.RSSI(i);
        bool abierta = (WiFi.encryptionType(i) == WIFI_AUTH_OPEN);

        bool repetida = false;
        for (RedWiFi& r : portalRedes) {
            if (r.ssid == s) { repetida = true; if (rssi > r.rssi) r.rssi = rssi; break; }
        }
        if (!repetida) portalRedes.push_back({s, rssi, abierta});
    }
    WiFi.scanDelete();

    // Sort from strongest to weakest signal (insertion sort; the list is short)
    for (size_t i = 1; i < portalRedes.size(); i++) {
        RedWiFi x = portalRedes[i];
        int j = (int)i - 1;
        while (j >= 0 && portalRedes[j].rssi < x.rssi) { portalRedes[j + 1] = portalRedes[j]; j--; }
        portalRedes[j + 1] = x;
    }
    if (portalRedes.size() > PORTAL_MAX_REDES) portalRedes.resize(PORTAL_MAX_REDES);

    for (const RedWiFi& r : portalRedes) Serial.printf("[PORTAL]   %s (%d dBm)\n", r.ssid.c_str(), r.rssi);
}

// ====================================================================
//  2. ACCESS POINT + DNS
// ====================================================================
static void portalIniciarAP() {
    WiFi.mode(WIFI_AP_STA);

    // Random password shown only on the panel
    snprintf(portalClave, sizeof(portalClave), "%08lu", (unsigned long)(esp_random() % 100000000UL));
    WiFi.softAP(PORTAL_AP_SSID, portalClave, 1, 0, 2);        // channel 1, visible, max. 2 clients
    delay(200);

    const IPAddress ip = WiFi.softAPIP();
    portalDns.start(53, "*", ip);   // captive portal: any hostname resolves to the panel

    snprintf(portalLinea2, sizeof(portalLinea2), "WiFi: %s", PORTAL_AP_SSID);
    snprintf(portalLinea3, sizeof(portalLinea3), "Pass: %s", portalClave);
    snprintf(portalLinea4, sizeof(portalLinea4), "IP: %s", ip.toString().c_str());

    Serial.printf("[PORTAL] AP %s, IP %s, heap libre %u, mayor bloque %u\n",
                  PORTAL_AP_SSID, ip.toString().c_str(), ESP.getFreeHeap(), ESP.getMaxAllocHeap());
}

// ====================================================================
//  3. TEMPORARY WEB SERVER
// ====================================================================

// Response header: <html lang>, styles, and <body>. Send in chunks to avoid large Strings.
static void portalInicioRespuesta(int codigo, bool formulario, bool titularRojo) {
    portalWeb->setContentLength(CONTENT_LENGTH_UNKNOWN);
    portalWeb->send(codigo, "text/html; charset=utf-8", "");
    portalWeb->sendContent_P(PORTAL_HTML_A);
    portalWeb->sendContent(PORTAL_LANG[portalIdioma]);
    portalWeb->sendContent_P(PORTAL_HTML_B);
    portalWeb->sendContent_P(PORTAL_CSS_BASE);
    if (formulario)  portalWeb->sendContent_P(PORTAL_CSS_FORM);
    if (titularRojo) portalWeb->sendContent_P(PORTAL_CSS_ROJO);
    portalWeb->sendContent_P(PORTAL_FIN_CABECERA);
}

static void portalFinRespuesta() {
    portalWeb->sendContent("");
}

// <tag>translated text</tag>
static void portalElemento(const char* etiqueta, int id) {
    String h = String("<") + etiqueta + ">" + escHtml(portalT(id)) + "</" + etiqueta + ">";
    portalWeb->sendContent(h);
}

static void portalRutaInicio() {
    portalInicioRespuesta(200, true, false);
    portalWeb->sendContent_P(PORTAL_H1);
    portalElemento("p", PT_W_INSTRUCCION);

    portalWeb->sendContent_P(PORTAL_FORM_INICIO);
    portalElemento("label", PT_W_REDES);
    portalWeb->sendContent("<div id=\"lista\">");
    if (portalRedes.empty()) {
        portalElemento("p", PT_W_SIN_REDES);
    }
    for (const RedWiFi& r : portalRedes) {
        portalWeb->sendContent("<button type=\"button\" class=\"red\" data-s=\"" + escHtml(r.ssid) + "\">" +
                               escHtml(r.ssid) + (r.abierta ? "" : " &#128274;") + "</button>");
    }
    portalWeb->sendContent("</div>");

    portalElemento("label", PT_W_RED);
    portalWeb->sendContent_P(PORTAL_INPUT_SSID);
    portalElemento("label", PT_W_PASS);
    portalWeb->sendContent_P(PORTAL_INPUT_PASS);
    portalWeb->sendContent("<button type=\"submit\">" + escHtml(portalT(PT_W_CONECTAR)) + "</button></form>");

    portalElemento("p", PT_W_AVISO_24);
    portalWeb->sendContent("<p><a href=\"/cancelar\">" + escHtml(portalT(PT_W_CANCELAR)) + "</a></p>");
    portalWeb->sendContent_P(PORTAL_SCRIPT_FIN);
    portalFinRespuesta();
}

static void portalPaginaEspera() {
    portalInicioRespuesta(200, false, false);
    portalElemento("h1", PT_W_ESPERA_TITULO);
    portalElemento("p", PT_W_ESPERA_1);
    portalElemento("p", PT_W_ESPERA_2);
    portalWeb->sendContent_P(PORTAL_FIN_BODY);
    portalFinRespuesta();
}

static void portalPaginaError() {
    portalInicioRespuesta(400, false, true);
    portalElemento("h1", PT_W_ERROR_TITULO);
    portalElemento("p", PT_W_ERROR_TXT);
    portalWeb->sendContent("<p><a href=\"/\">" + escHtml(portalT(PT_W_VOLVER)) + "</a></p>");
    portalWeb->sendContent_P(PORTAL_FIN_BODY);
    portalFinRespuesta();
}

static void portalPaginaCancelado() {
    portalInicioRespuesta(200, false, false);
    portalElemento("h1", PT_W_CANCELADO_TITULO);
    portalElemento("p", PT_W_CANCELADO_TXT);
    portalWeb->sendContent_P(PORTAL_FIN_BODY);
    portalFinRespuesta();
}

static void portalRutaGuardar() {
    String s = portalWeb->arg("ssid");
    String p = portalWeb->arg("pass");
    bool ok = (s.length() >= 1 && s.length() <= 32) &&
              (p.length() == 0 || (p.length() >= 8 && p.length() <= 63));
    if (!ok) {
        portalPaginaError();
        return;
    }
    strlcpy(portalSsidNuevo, s.c_str(), sizeof(portalSsidNuevo));
    strlcpy(portalPassNueva, p.c_str(), sizeof(portalPassNueva));
    portalPendiente = true;
    portalPaginaEspera();
}

static void portalRutaCancelar() {
    portalCancelado = true;
    portalPaginaCancelado();
}

// Any other path (Android, iOS, and Windows captive-portal probes) redirects to the form
static void portalRutaNoEncontrada() {
    portalWeb->sendHeader("Location", String("http://") + WiFi.softAPIP().toString() + "/", true);
    portalWeb->send(302, "text/plain", "");
}

static void portalIniciarWeb() {
    portalWeb = new WebServer(80);
    portalWeb->on("/", HTTP_GET, portalRutaInicio);
    portalWeb->on("/guardar", HTTP_POST, portalRutaGuardar);
    portalWeb->on("/cancelar", HTTP_GET, portalRutaCancelar);
    portalWeb->onNotFound(portalRutaNoEncontrada);
    portalWeb->begin();
}

// ====================================================================
//  4. LOOP: TEST THE NETWORK AND EXIT
// ====================================================================

// Test the submitted network and save it to config.ini on success. Returns true if saved.
static bool portalProbarYGuardar(int doubleBuffOriginal) {
    portalPantalla(portalT(PT_P_PROBANDO), portalSsidNuevo, "", "");

    WiFi.begin(portalSsidNuevo, portalPassNueva);
    unsigned long t0 = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - t0 < PORTAL_PRUEBA_MS) delay(250);

    if (WiFi.status() != WL_CONNECTED) {
        WiFi.disconnect(false);
        portalPantalla(portalT(PT_P_NO_CONECTA), portalT(PT_P_REVISA1), portalT(PT_P_REVISA2), portalT(PT_P_REINTENTA));
        delay(4000);
        portalPantallaPrincipal();
        return false;
    }

    wifiEnable = true;
    strlcpy(wifi_ssid, portalSsidNuevo, sizeof(wifi_ssid));
    strlcpy(wifi_pass, portalPassNueva, sizeof(wifi_pass));
    doubleBuff = doubleBuffOriginal;      // preserve the user's configured setting
    guardarConfigIni();

    char lIP[24];
    snprintf(lIP, sizeof(lIP), "IP: %s", WiFi.localIP().toString().c_str());
#ifdef MDNS_HOSTNAME
    portalPantalla(portalT(PT_P_OK), lIP, MDNS_HOSTNAME ".local", portalT(PT_P_REINICIANDO));
#else
    portalPantalla(portalT(PT_P_OK), lIP, "", portalT(PT_P_REINICIANDO));
#endif
    delay(6000);
    return true;
}

// True if the IR remote OK button was pressed (ignores repeat codes while held)
static bool portalTeclaSalirIR() {
    if (!IrReceiver.decode()) return false;
    bool salir = !(IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) &&
                 IrReceiver.decodedIRData.decodedRawData == ir_btn_ok;
    IrReceiver.resume();
    if (salir) Serial.println(F("[PORTAL] Tecla OK del mando: saliendo del portal"));
    return salir;
}

// True when closing without saving: web Cancel, IR remote, or physical button
static bool portalDebeSalir() {
    if (portalCancelado) {
        delay(500);                       // allow the browser to receive the response
        return true;
    }
    if (portalTeclaSalirIR()) return true;
    if (digitalRead(PIN_BOTON_MENU) == LOW) {
        while (digitalRead(PIN_BOTON_MENU) == LOW) delay(10);
        return true;
    }
    return false;
}

// ====================================================================
//  5. CLEANUP
// ====================================================================
static void portalLimpiar() {
    if (portalWeb) {                      // temporary server: do not leave routes exposed on the normal network
        portalWeb->stop();
        delete portalWeb;
        portalWeb = nullptr;
    }
    portalDns.stop();
    WiFi.softAPdisconnect(true);
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    delay(200);

    portalRedes.clear();
    portalRedes.shrink_to_fit();
}

// ====================================================================
//  ENTRY POINTS
// ====================================================================

// Called from setup(): consumes the OSD menu flag and decides whether to open the portal.
// Opens when requested from the menu, or when Wi-Fi is enabled but no network is configured.
bool portalWiFiNecesario(bool sdOk, bool entrarFTP) {
    Preferences p;
    p.begin("sistema", false);
    bool pedido = p.getBool("portal_wifi", false);
    if (pedido) p.putBool("portal_wifi", false);              // consume the flag so this is attempted only once
    p.end();

    return sdOk && !entrarFTP && (pedido || (wifiEnable == 1 && wifi_ssid[0] == '\0'));
}

// Called from the OSD menu: requests the portal on the next boot (same pattern as FTP mode).
// These messages belong to the OSD menu and therefore come from the language JSON (SUBMENU_WIFI).
void abrirPortalWiFiDesdeMenu() {
    display->fillScreen(0);
    printMenuCentrado(msgOr("SUBMENU_WIFI", "portal_abriendo", "Opening Portal"), 8, display->color565(255, 255, 255), offset);
    printMenuCentrado(msgOr("SUBMENU_WIFI", "portal_reinicio", "Restarting..."), 18, display->color565(255, 255, 255), offset);
    display->flipDMABuffer();

    Serial.println(F("[MENU] Wi-Fi portal requested: restarting..."));
    Preferences p;
    p.begin("sistema", false);
    p.putBool("portal_wifi", true);
    p.end();

    delay(2000);
    ESP.restart();
}

// Called from setup() after initializing the panel.
// doubleBuffOriginal: DOUBLE_BUFF before disabling it for this boot (restored before saving)
void ejecutarPortalWiFi(int doubleBuffOriginal) {
    Serial.println(F("[PORTAL] Abriendo portal de configuración WiFi"));

    portalPendiente = false;
    portalCancelado = false;
    portalElegirIdioma();

    portalEscanearRedes();        // before starting the AP, so its channel does not change while the phone is connected
    portalIniciarAP();
    portalIniciarWeb();
    portalPantallaPrincipal();

    bool guardado = false;
    const unsigned long inicio = millis();
    while (!guardado && millis() - inicio < PORTAL_TIMEOUT_MS) {
        portalDns.processNextRequest();
        portalWeb->handleClient();

        if (portalPendiente) {
            portalPendiente = false;
            guardado = portalProbarYGuardar(doubleBuffOriginal);
        }

        if (portalDebeSalir()) break;
        delay(2);
    }

    portalLimpiar();

    // If a new network was saved, or a network was already configured, restart cleanly
    if (guardado || wifi_ssid[0] != '\0') {
        ESP.restart();
    }

    // With no configured network, continue booting offline
    doubleBuff = doubleBuffOriginal;
    display->fillScreen(0);
    display->flipDMABuffer();
}

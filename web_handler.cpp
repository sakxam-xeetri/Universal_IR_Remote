/*
 * =============================================================
 *  Web Handler Module - Implementation
 * =============================================================
 *  Hosts the web server and DNS captive-portal on the ESP32
 *  soft-AP.  All API endpoints are defined here.
 *
 *  Routes:
 *    GET  /                  → Serve UI
 *    GET  /api/send?code=HEX → Send IR code
 *    GET  /api/custom        → List custom buttons (JSON)
 *    GET  /api/custom/add    → Add button   (?name=…&code=…)
 *    GET  /api/custom/edit   → Edit button  (?id=…&name=…&code=…)
 *    GET  /api/custom/delete → Delete button (?id=…)
 *    *    (everything else)  → Captive-portal redirect
 * =============================================================
 */

#include "web_handler.h"
#include "config.h"
#include "web_ui.h"
#include "ir_sender.h"
#include "button_storage.h"

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>

static WebServer  server(WEB_SERVER_PORT);
static DNSServer  dnsServer;

// ── Serve the main UI page ──────────────────────────────────
static void handleRoot() {
    server.send_P(200, "text/html", INDEX_HTML);
}

// ── API: Send IR code ───────────────────────────────────────
static void handleSendIR() {
    if (!server.hasArg("code")) {
        server.send(200, "application/json",
                    "{\"ok\":false,\"msg\":\"Missing code parameter\"}");
        return;
    }

    String codeStr = server.arg("code");
    codeStr.replace("0x", "");
    codeStr.replace("0X", "");
    codeStr.toUpperCase();

    uint32_t hexCode = strtoul(codeStr.c_str(), NULL, 16);
    bool ok = sendIRCode(hexCode);

    String json = "{\"ok\":";
    json += ok ? "true" : "false";
    json += ",\"code\":\"";
    json += codeStr;
    json += "\"}";
    server.send(200, "application/json", json);
}

// ── API: List custom buttons ────────────────────────────────
static void handleCustomList() {
    server.send(200, "application/json", getCustomButtonsJSON());
}

// ── API: Add custom button ──────────────────────────────────
static void handleCustomAdd() {
    String name = server.arg("name");
    String code = server.arg("code");

    if (name.length() == 0 || code.length() == 0) {
        server.send(200, "application/json",
                    "{\"ok\":false,\"msg\":\"Name and code are required\"}");
        return;
    }

    code.replace("0x", "");
    code.replace("0X", "");
    code.toUpperCase();

    bool ok = addCustomButton(name, code);
    if (ok) {
        server.send(200, "application/json", "{\"ok\":true}");
    } else {
        server.send(200, "application/json",
                    "{\"ok\":false,\"msg\":\"Storage full (max "
                    + String(MAX_CUSTOM_BUTTONS) + ")\"}");
    }
}

// ── API: Edit custom button ─────────────────────────────────
static void handleCustomEdit() {
    if (!server.hasArg("id")) {
        server.send(200, "application/json",
                    "{\"ok\":false,\"msg\":\"Missing id\"}");
        return;
    }

    int    id   = server.arg("id").toInt();
    String name = server.arg("name");
    String code = server.arg("code");

    code.replace("0x", "");
    code.replace("0X", "");
    code.toUpperCase();

    bool ok = editCustomButton(id, name, code);
    server.send(200, "application/json",
                ok ? "{\"ok\":true}"
                   : "{\"ok\":false,\"msg\":\"Invalid button id\"}");
}

// ── API: Delete custom button ───────────────────────────────
static void handleCustomDelete() {
    if (!server.hasArg("id")) {
        server.send(200, "application/json",
                    "{\"ok\":false,\"msg\":\"Missing id\"}");
        return;
    }

    int id = server.arg("id").toInt();
    bool ok = deleteCustomButton(id);
    server.send(200, "application/json",
                ok ? "{\"ok\":true}"
                   : "{\"ok\":false,\"msg\":\"Invalid button id\"}");
}

// ── Captive-portal redirect ─────────────────────────────────
static void handleCaptiveRedirect() {
    String url = "http://" + WiFi.softAPIP().toString() + "/";
    server.sendHeader("Location", url, true);
    server.send(302, "text/plain", "");
}

// ═══════════════════════════════════════════════════════════
//  Public interface
// ═══════════════════════════════════════════════════════════

void setupWebServer() {
    // Start DNS server — redirects ALL domains to the AP IP
    // so that captive-portal detection opens our page.
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

    // ── Main page ───────────────────────────────────────────
    server.on("/",                HTTP_GET, handleRoot);

    // ── IR API ──────────────────────────────────────────────
    server.on("/api/send",        HTTP_GET, handleSendIR);

    // ── Custom-button API ───────────────────────────────────
    server.on("/api/custom",          HTTP_GET, handleCustomList);
    server.on("/api/custom/add",      HTTP_GET, handleCustomAdd);
    server.on("/api/custom/edit",     HTTP_GET, handleCustomEdit);
    server.on("/api/custom/delete",   HTTP_GET, handleCustomDelete);

    // ── Captive-portal detection URLs ───────────────────────
    server.on("/generate_204",        HTTP_GET, handleCaptiveRedirect);   // Android
    server.on("/gen_204",             HTTP_GET, handleCaptiveRedirect);   // Android alt
    server.on("/hotspot-detect.html", HTTP_GET, handleCaptiveRedirect);   // Apple / iOS
    server.on("/library/test/success.html", HTTP_GET, handleCaptiveRedirect); // Apple alt
    server.on("/connecttest.txt",     HTTP_GET, handleCaptiveRedirect);   // Windows
    server.on("/redirect",            HTTP_GET, handleCaptiveRedirect);   // Windows alt
    server.on("/fwlink",              HTTP_GET, handleCaptiveRedirect);   // Microsoft
    server.on("/canonical.html",      HTTP_GET, handleCaptiveRedirect);   // Firefox
    server.on("/success.txt",         HTTP_GET, handleCaptiveRedirect);   // Firefox alt

    // ── Catch-all → redirect to main page ───────────────────
    server.onNotFound(handleCaptiveRedirect);

    server.begin();
    Serial.println(F("[Web] Server started on port 80"));
    Serial.print(F("[Web] Open  http://"));
    Serial.print(WiFi.softAPIP());
    Serial.println(F("/  in your browser"));
}

void handleWebServer() {
    dnsServer.processNextRequest();
    server.handleClient();
}

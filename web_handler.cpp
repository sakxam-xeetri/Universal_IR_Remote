/*
 * =============================================================
 *  Web Handler Module - Implementation
 * =============================================================
 *  Hosts the web server on the local WiFi network.
 *  Any device on the same network can access the controller.
 *
 *  Routes:
 *    GET  /                  → Remote UI page
 *    GET  /custom            → Custom commands page
 *    GET  /api/send?code=HEX → Send IR code
 *    GET  /api/custom        → List custom buttons (JSON)
 *    GET  /api/custom/add    → Add button   (?name=…&code=…)
 *    GET  /api/custom/edit   → Edit button  (?id=…&name=…&code=…)
 *    GET  /api/custom/delete → Delete button (?id=…)
 * =============================================================
 */

#include "web_handler.h"
#include "config.h"
#include "web_ui.h"
#include "ir_sender.h"
#include "button_storage.h"

#include <WiFi.h>
#include <WebServer.h>

static WebServer server(WEB_SERVER_PORT);

// ── Serve the remote UI page ────────────────────────────────
static void handleRoot() {
    server.send_P(200, "text/html", INDEX_HTML);
}

// ── Serve the custom commands page ──────────────────────────
static void handleCustomPage() {
    server.send_P(200, "text/html", CUSTOM_HTML);
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

// ── 404 handler ─────────────────────────────────────────────
static void handleNotFound() {
    server.send(404, "text/plain", "404 - Not Found");
}

// ═══════════════════════════════════════════════════════════
//  Public interface
// ═══════════════════════════════════════════════════════════

void setupWebServer() {
    // ── Pages ───────────────────────────────────────────────
    server.on("/",                HTTP_GET, handleRoot);
    server.on("/custom",          HTTP_GET, handleCustomPage);

    // ── IR API ──────────────────────────────────────────────
    server.on("/api/send",        HTTP_GET, handleSendIR);

    // ── Custom-button API ───────────────────────────────────
    server.on("/api/custom",          HTTP_GET, handleCustomList);
    server.on("/api/custom/add",      HTTP_GET, handleCustomAdd);
    server.on("/api/custom/edit",     HTTP_GET, handleCustomEdit);
    server.on("/api/custom/delete",   HTTP_GET, handleCustomDelete);

    // ── 404 ─────────────────────────────────────────────────
    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println(F("[Web] Server started on port 80"));
    Serial.print(F("[Web] Open  http://"));
    Serial.print(WiFi.localIP());
    Serial.println(F("/  in your browser"));
}

void handleWebServer() {
    server.handleClient();
}

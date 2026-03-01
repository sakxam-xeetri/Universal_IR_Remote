/*
 * =============================================================
 *  WiFi Manager Module - Implementation
 * =============================================================
 *  A self-contained WiFi configuration portal.
 *
 *  Flow:
 *   1. Read saved SSID/password from NVS (Preferences).
 *   2. If credentials exist → try to connect (STA mode).
 *   3. If connection fails or no creds → start AP + web portal.
 *   4. User scans networks, picks one, enters password.
 *   5. Credentials are saved to NVS and ESP32 reboots into STA.
 *
 *  The AP creates an open network called "RGB_IR_Setup".
 *  A DNS server redirects all domains to the portal page,
 *  so it works like a captive portal on phones and laptops.
 *
 *  NVS namespace: "wifi_cfg"
 *    Keys: "ssid"  (String, max 32)
 *           "pass"  (String, max 64)
 * =============================================================
 */

#include "wifi_manager.h"
#include "config.h"

#include <WiFi.h>
#include <WebServer.h>
#include <DNSServer.h>
#include <Preferences.h>

// ── Constants ───────────────────────────────────────────────
static const char* NVS_NS          = "wifi_cfg";
static const char* AP_SSID         = "RGB_IR_Setup";
static const unsigned long PORTAL_TIMEOUT = 300000;  // 5 minutes

// ── Forward declarations ────────────────────────────────────
static String savedSSID();
static String savedPass();
static void   saveCredentials(const String& ssid, const String& pass);
static bool   tryConnect(const String& ssid, const String& pass);
static void   runPortal();

// ═════════════════════════════════════════════════════════════
//  PUBLIC
// ═════════════════════════════════════════════════════════════

bool wifiManagerConnect() {
    String ssid = savedSSID();
    String pass = savedPass();

    // If we have saved credentials, try them first
    if (ssid.length() > 0) {
        Serial.print(F("[WiFiMgr] Saved network: "));
        Serial.println(ssid);
        if (tryConnect(ssid, pass)) {
            return true;
        }
        Serial.println(F("[WiFiMgr] Could not connect to saved network"));
    } else {
        Serial.println(F("[WiFiMgr] No saved WiFi credentials"));
    }

    // Launch config portal
    Serial.println(F("[WiFiMgr] Starting configuration portal..."));
    runPortal();

    // If portal saved new creds, we restart to connect cleanly
    ESP.restart();
    return false;  // never reached
}

void wifiManagerReset() {
    Preferences p;
    p.begin(NVS_NS, false);
    p.clear();
    p.end();
    Serial.println(F("[WiFiMgr] Credentials erased"));
}

// ═════════════════════════════════════════════════════════════
//  NVS HELPERS
// ═════════════════════════════════════════════════════════════

static String savedSSID() {
    Preferences p;
    p.begin(NVS_NS, true);
    String s = p.getString("ssid", "");
    p.end();
    return s;
}

static String savedPass() {
    Preferences p;
    p.begin(NVS_NS, true);
    String s = p.getString("pass", "");
    p.end();
    return s;
}

static void saveCredentials(const String& ssid, const String& pass) {
    Preferences p;
    p.begin(NVS_NS, false);
    p.putString("ssid", ssid);
    p.putString("pass", pass);
    p.end();
    Serial.print(F("[WiFiMgr] Credentials saved for: "));
    Serial.println(ssid);
}

// ═════════════════════════════════════════════════════════════
//  STA CONNECTION
// ═════════════════════════════════════════════════════════════

static bool tryConnect(const String& ssid, const String& pass) {
    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false);
    WiFi.setHostname(WIFI_HOSTNAME);
    WiFi.begin(ssid.c_str(), pass.c_str());

    Serial.print(F("[WiFiMgr] Connecting"));
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        attempts++;
        if (attempts >= WIFI_CONNECT_TIMEOUT * 2) {
            Serial.println(F(" TIMEOUT"));
            WiFi.disconnect(true);
            return false;
        }
    }
    Serial.println(F(" OK"));
    return true;
}

// ═════════════════════════════════════════════════════════════
//  CAPTIVE PORTAL
// ═════════════════════════════════════════════════════════════

// ── Portal HTML page ────────────────────────────────────────
static const char PORTAL_HTML[] PROGMEM = R"==(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no">
<title>WiFi Setup - RGB IR Remote</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{
  font-family:'Segoe UI',system-ui,-apple-system,sans-serif;
  background:#1a1a2e;min-height:100vh;
  display:flex;align-items:center;justify-content:center;
  padding:16px;color:#e0e0e0;
}
.card{
  background:linear-gradient(180deg,#1e1e3a,#16162e);
  border-radius:24px;padding:30px 22px;
  max-width:380px;width:100%;
  border:1px solid rgba(255,255,255,.06);
  box-shadow:0 20px 60px rgba(0,0,0,.5);
}
.logo{
  text-align:center;margin-bottom:6px;
  font-size:2rem;
}
h1{
  text-align:center;font-size:1.15rem;font-weight:800;
  letter-spacing:1px;margin-bottom:4px;
  background:linear-gradient(90deg,#ff6b6b,#feca57,#48dbfb,#ff9ff3);
  -webkit-background-clip:text;-webkit-text-fill-color:transparent;
}
.sub{text-align:center;font-size:.72rem;color:#555;margin-bottom:20px}
label{display:block;font-size:.78rem;color:#888;margin-bottom:5px;font-weight:600}
select,input[type=password],input[type=text]{
  width:100%;padding:12px 14px;border-radius:10px;
  border:1px solid #2a2a48;background:#0e0e20;color:#ddd;
  font-size:.9rem;outline:none;margin-bottom:14px;
  transition:border-color .2s;
  -webkit-appearance:none;
}
select{cursor:pointer;background-image:url("data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='12' height='8'%3E%3Cpath d='M1 1l5 5 5-5' stroke='%23666' fill='none'/%3E%3C/svg%3E");
  background-repeat:no-repeat;background-position:right 12px center;padding-right:34px}
select:focus,input:focus{border-color:#48dbfb}
input::placeholder{color:#3a3a5a}
.btn{
  width:100%;padding:14px;border:none;border-radius:12px;
  font-size:.95rem;font-weight:700;color:#fff;cursor:pointer;
  background:linear-gradient(180deg,#48dbfb,#2e9ab8);
  box-shadow:0 4px 16px rgba(72,219,251,.3);
  transition:transform .1s;margin-top:4px;
}
.btn:active{transform:scale(.96)}
.btn:disabled{opacity:.5;cursor:not-allowed}
.scan{
  width:100%;padding:10px;border:none;border-radius:10px;
  font-size:.78rem;font-weight:600;color:#48dbfb;cursor:pointer;
  background:transparent;border:1px solid rgba(72,219,251,.25);
  margin-bottom:14px;transition:all .2s;
}
.scan:hover{background:rgba(72,219,251,.08)}
.msg{
  text-align:center;padding:12px 8px 0;
  font-size:.78rem;min-height:24px;
  font-weight:500;transition:color .3s;
}
.ok{color:#2ecc71}.err{color:#ff6b6b}.info{color:#48dbfb}
.toggle{
  display:flex;align-items:center;gap:8px;margin-bottom:14px;
  font-size:.75rem;color:#666;cursor:pointer;
}
.toggle input{width:16px;height:16px;accent-color:#48dbfb}
.net-info{font-size:.65rem;color:#555;margin:-8px 0 14px 2px}
</style>
</head>
<body>
<div class="card">
  <div class="logo">&#128225;</div>
  <h1>WiFi Setup</h1>
  <div class="sub">RGB IR Remote Controller</div>

  <form id="f" action="/save" method="POST">
    <label>WiFi Network</label>
    <select id="ssid" name="ssid"><option value="">Scanning...</option></select>
    <div id="ni" class="net-info"></div>

    <button type="button" class="scan" onclick="scan()">&#128260; Scan Again</button>

    <label>Password</label>
    <div style="position:relative">
      <input type="password" id="pw" name="pass" placeholder="Enter WiFi password" autocomplete="off">
    </div>
    <label class="toggle"><input type="checkbox" onclick="togglePw()"> Show password</label>

    <button type="submit" class="btn" id="go">Connect</button>
  </form>
  <div id="msg" class="msg"></div>
</div>

<script>
function scan(){
  document.getElementById('ssid').innerHTML='<option value="">Scanning...</option>';
  document.getElementById('ni').textContent='';
  fetch('/scan').then(r=>r.json()).then(d=>{
    var s=document.getElementById('ssid');
    s.innerHTML='';
    if(!d.length){s.innerHTML='<option value="">No networks found</option>';return;}
    d.forEach(n=>{
      var o=document.createElement('option');
      o.value=n.ssid;
      o.textContent=n.ssid+' ('+n.rssi+'dBm'+(n.enc?', \ud83d\udd12':'')+')';
      s.appendChild(o);
    });
    updateInfo();
  }).catch(()=>{
    document.getElementById('ssid').innerHTML='<option value="">Scan failed</option>';
  });
}
function updateInfo(){
  var s=document.getElementById('ssid');
  var v=s.value;
  document.getElementById('ni').textContent=v?'Selected: '+v:'';
}
document.getElementById('ssid').onchange=updateInfo;
function togglePw(){
  var p=document.getElementById('pw');
  p.type=p.type==='password'?'text':'password';
}
document.getElementById('f').onsubmit=function(e){
  e.preventDefault();
  var ssid=document.getElementById('ssid').value;
  var pass=document.getElementById('pw').value;
  if(!ssid){document.getElementById('msg').className='msg err';document.getElementById('msg').textContent='Select a network';return;}
  document.getElementById('go').disabled=true;
  document.getElementById('msg').className='msg info';
  document.getElementById('msg').textContent='Connecting to '+ssid+'...';
  fetch('/save',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},
    body:'ssid='+encodeURIComponent(ssid)+'&pass='+encodeURIComponent(pass)})
    .then(r=>r.json()).then(d=>{
      if(d.ok){
        document.getElementById('msg').className='msg ok';
        document.getElementById('msg').textContent='\u2714 Saved! Rebooting...';
      } else {
        document.getElementById('msg').className='msg err';
        document.getElementById('msg').textContent='\u2718 '+(d.msg||'Error');
        document.getElementById('go').disabled=false;
      }
    }).catch(()=>{
      document.getElementById('msg').className='msg err';
      document.getElementById('msg').textContent='\u2718 Connection error';
      document.getElementById('go').disabled=false;
    });
};
scan();
</script>
</body>
</html>
)==";

// ── Portal: scan networks ───────────────────────────────────
static String scanNetworksJSON() {
    int n = WiFi.scanNetworks();
    String json = "[";
    // Deduplicate SSIDs, keep strongest signal
    for (int i = 0; i < n; i++) {
        String ssid = WiFi.SSID(i);
        if (ssid.length() == 0) continue;

        // Check for duplicate
        bool dup = false;
        for (int j = 0; j < i; j++) {
            if (WiFi.SSID(j) == ssid) { dup = true; break; }
        }
        if (dup) continue;

        if (json.length() > 1) json += ",";
        // Escape quotes in SSID
        String safe = ssid;
        safe.replace("\\", "\\\\");
        safe.replace("\"", "\\\"");
        json += "{\"ssid\":\"" + safe + "\""
              + ",\"rssi\":" + String(WiFi.RSSI(i))
              + ",\"enc\":" + String(WiFi.encryptionType(i) != WIFI_AUTH_OPEN ? "true" : "false")
              + "}";
    }
    json += "]";
    WiFi.scanDelete();
    return json;
}

// ── Run the captive portal ──────────────────────────────────
static void runPortal() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID);
    delay(100);

    IPAddress apIP = WiFi.softAPIP();
    Serial.print(F("[WiFiMgr] AP started: "));
    Serial.println(AP_SSID);
    Serial.print(F("[WiFiMgr] Portal IP : "));
    Serial.println(apIP);

    DNSServer  dns;
    WebServer  portal(80);
    dns.start(53, "*", apIP);

    volatile bool shouldRestart = false;

    // ── Routes ──────────────────────────────────────────────
    portal.on("/", HTTP_GET, [&portal]() {
        portal.send_P(200, "text/html", PORTAL_HTML);
    });

    portal.on("/scan", HTTP_GET, [&portal]() {
        portal.send(200, "application/json", scanNetworksJSON());
    });

    portal.on("/save", HTTP_POST, [&portal, &shouldRestart]() {
        String ssid = portal.arg("ssid");
        String pass = portal.arg("pass");

        if (ssid.length() == 0) {
            portal.send(200, "application/json",
                        "{\"ok\":false,\"msg\":\"SSID is required\"}");
            return;
        }

        saveCredentials(ssid, pass);
        portal.send(200, "application/json", "{\"ok\":true}");
        shouldRestart = true;
    });

    // Captive portal redirects
    portal.on("/generate_204",        HTTP_GET, [&portal, &apIP]() { portal.sendHeader("Location","http://"+apIP.toString()+"/",true); portal.send(302,"text/plain",""); });
    portal.on("/gen_204",             HTTP_GET, [&portal, &apIP]() { portal.sendHeader("Location","http://"+apIP.toString()+"/",true); portal.send(302,"text/plain",""); });
    portal.on("/hotspot-detect.html", HTTP_GET, [&portal, &apIP]() { portal.sendHeader("Location","http://"+apIP.toString()+"/",true); portal.send(302,"text/plain",""); });
    portal.on("/connecttest.txt",     HTTP_GET, [&portal, &apIP]() { portal.sendHeader("Location","http://"+apIP.toString()+"/",true); portal.send(302,"text/plain",""); });
    portal.on("/fwlink",              HTTP_GET, [&portal, &apIP]() { portal.sendHeader("Location","http://"+apIP.toString()+"/",true); portal.send(302,"text/plain",""); });
    portal.on("/canonical.html",      HTTP_GET, [&portal, &apIP]() { portal.sendHeader("Location","http://"+apIP.toString()+"/",true); portal.send(302,"text/plain",""); });
    portal.on("/success.txt",         HTTP_GET, [&portal, &apIP]() { portal.sendHeader("Location","http://"+apIP.toString()+"/",true); portal.send(302,"text/plain",""); });

    portal.onNotFound([&portal, &apIP]() {
        portal.sendHeader("Location", "http://" + apIP.toString() + "/", true);
        portal.send(302, "text/plain", "");
    });

    portal.begin();
    Serial.println(F("[WiFiMgr] Portal running — connect to WiFi 'RGB_IR_Setup'"));

    unsigned long start = millis();
    while (!shouldRestart) {
        dns.processNextRequest();
        portal.handleClient();

        // Timeout — restart to retry
        if (millis() - start > PORTAL_TIMEOUT) {
            Serial.println(F("[WiFiMgr] Portal timeout — restarting"));
            ESP.restart();
        }
        delay(1);
    }

    // Small delay to let the HTTP response finish
    for (int i = 0; i < 20; i++) {
        portal.handleClient();
        delay(50);
    }

    portal.stop();
    dns.stop();
    Serial.println(F("[WiFiMgr] Restarting to connect..."));
    delay(500);
    ESP.restart();
}

/*
 * =============================================================
 *  Web UI – Remote Page (matches physical 24-key remote)
 * =============================================================
 *  Layout mirrors the actual remote control image exactly:
 *    Row 1: ☀+  ☀-  OFF  ON
 *    Row 2:  R   G    B   W
 *    Row 3: 🟠  🟢  🔵  FLASH
 *    Row 4: 🟠  🟢  🟣  STROBE
 *    Row 5: 🟡  🔵  🩷  FADE
 *    Row 6: 🟡  🔵  🩷  SMOOTH
 * =============================================================
 */

#ifndef WEB_UI_H
#define WEB_UI_H

#include <Arduino.h>

// ═════════════════════════════════════════════════════════════
//  PAGE 1 — REMOTE CONTROL
// ═════════════════════════════════════════════════════════════
const char INDEX_HTML[] PROGMEM = R"==(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no">
<title>RGB IR Remote</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
html,body{height:100%}
body{
  font-family:'Segoe UI',system-ui,-apple-system,sans-serif;
  background:#1a1a2e;
  display:flex;flex-direction:column;align-items:center;
  min-height:100vh;
  color:#e0e0e0;
  -webkit-tap-highlight-color:transparent;
  user-select:none;
  padding:0 0 20px;
}

/* ── Top Bar ─────────────────────────────────── */
.topbar{
  width:100%;padding:12px 16px;
  display:flex;justify-content:space-between;align-items:center;
  background:rgba(0,0,0,.3);
  border-bottom:1px solid rgba(255,255,255,.05);
}
.topbar .title{
  font-size:1.05rem;font-weight:800;letter-spacing:1.5px;
  background:linear-gradient(90deg,#ff6b6b,#feca57,#48dbfb,#ff9ff3);
  -webkit-background-clip:text;-webkit-text-fill-color:transparent;
  background-clip:text;
}
.topbar .nav-link{
  color:#48dbfb;text-decoration:none;font-size:.82rem;font-weight:600;
  padding:6px 14px;border-radius:8px;
  border:1px solid rgba(72,219,251,.3);
  transition:all .2s;
}
.topbar .nav-link:hover{background:rgba(72,219,251,.12)}

/* ── Remote Body ─────────────────────────────── */
.remote-wrap{
  flex:1;display:flex;align-items:center;justify-content:center;
  padding:12px 10px;width:100%;
}
.remote{
  background:linear-gradient(180deg,#f5f5f5 0%,#e8e8e8 50%,#ddd 100%);
  border-radius:30px;
  padding:24px 18px 20px;
  max-width:320px;width:100%;
  box-shadow:0 25px 80px rgba(0,0,0,.6),
             0 2px 0 #fff inset,
             0 -2px 4px rgba(0,0,0,.15) inset;
  border:2px solid #ccc;
  position:relative;
}
.remote::before{
  content:'';position:absolute;top:12px;left:50%;transform:translateX(-50%);
  width:20px;height:4px;border-radius:2px;background:rgba(0,0,0,.12);
}

/* ── Grid ────────────────────────────────────── */
.grid{
  display:grid;
  grid-template-columns:repeat(4,1fr);
  gap:10px;
  padding:8px 0;
}

/* ── Separator ───────────────────────────────── */
.sep{
  grid-column:1/-1;height:0;margin:2px 0;
}

/* ── Base Button ─────────────────────────────── */
.btn{
  display:flex;align-items:center;justify-content:center;
  border:none;cursor:pointer;
  font-weight:700;color:#fff;
  text-shadow:0 1px 2px rgba(0,0,0,.4);
  transition:transform .12s ease,box-shadow .2s;
  position:relative;overflow:hidden;
  -webkit-tap-highlight-color:transparent;
}
.btn:active{transform:scale(.85)!important}

/* ── Circle Buttons (colors) ─────────────────── */
.btn.circle{
  width:58px;height:58px;border-radius:50%;
  margin:0 auto;
  box-shadow:0 4px 10px rgba(0,0,0,.3),
             inset 0 2px 4px rgba(255,255,255,.25),
             inset 0 -2px 4px rgba(0,0,0,.15);
  font-size:.75rem;letter-spacing:.5px;
}

/* ── Rounded Rect Buttons (controls) ─────────── */
.btn.rect{
  border-radius:10px;
  padding:10px 4px;
  min-height:44px;
  font-size:.72rem;letter-spacing:.3px;
  box-shadow:0 3px 8px rgba(0,0,0,.25),
             inset 0 1px 2px rgba(255,255,255,.2);
}

/* ── Click Flash ─────────────────────────────── */
.btn.flash-fx{animation:flashpulse .35s ease}
@keyframes flashpulse{
  0%{box-shadow:0 0 0 0 rgba(255,255,255,.7)}
  100%{box-shadow:0 0 0 16px rgba(255,255,255,0)}
}

/* ── Brightness Buttons ──────────────────────── */
.btn.bright{
  background:linear-gradient(180deg,#e0e0e0,#c4c4c4);
  color:#333;text-shadow:none;
  border-radius:50%;width:58px;height:58px;margin:0 auto;
  box-shadow:0 3px 8px rgba(0,0,0,.2),inset 0 1px 3px rgba(255,255,255,.6);
  font-size:1.3rem;
}

/* ── Row 1: OFF / ON ─────────────────────────── */
.c-off{background:linear-gradient(180deg,#e8e8e8,#c8c8c8);color:#333;text-shadow:none;
  box-shadow:0 3px 8px rgba(0,0,0,.2),inset 0 1px 3px rgba(255,255,255,.5)}
.c-on{background:linear-gradient(180deg,#ff4444,#cc2222);
  box-shadow:0 4px 12px rgba(255,0,0,.3),inset 0 1px 3px rgba(255,255,255,.2)}

/* ── Row 2: R G B W ──────────────────────────── */
.c-red{background:linear-gradient(180deg,#ff2020,#cc0000);
  box-shadow:0 4px 14px rgba(255,0,0,.4),inset 0 2px 4px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.15)}
.c-green{background:linear-gradient(180deg,#22cc22,#009900);
  box-shadow:0 4px 14px rgba(0,200,0,.35),inset 0 2px 4px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.15)}
.c-blue{background:linear-gradient(180deg,#2244ff,#0022cc);
  box-shadow:0 4px 14px rgba(0,50,255,.4),inset 0 2px 4px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.15)}
.c-white{background:linear-gradient(180deg,#f0f0f0,#cccccc);color:#333;text-shadow:none;
  box-shadow:0 4px 10px rgba(0,0,0,.2),inset 0 2px 4px rgba(255,255,255,.5),inset 0 -2px 4px rgba(0,0,0,.1)}

/* ── Row 3 Colors ────────────────────────────── */
.c-r3{background:linear-gradient(180deg,#ff8833,#dd6600);
  box-shadow:0 4px 12px rgba(255,120,0,.35),inset 0 2px 4px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.15)}
.c-g3{background:linear-gradient(180deg,#33dd66,#11aa44);
  box-shadow:0 4px 12px rgba(50,220,100,.3),inset 0 2px 4px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.15)}
.c-b3{background:linear-gradient(180deg,#3355dd,#1133bb);
  box-shadow:0 4px 12px rgba(50,80,220,.35),inset 0 2px 4px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.15)}

/* ── Row 4 Colors ────────────────────────────── */
.c-r4{background:linear-gradient(180deg,#ff5522,#dd3300);
  box-shadow:0 4px 12px rgba(255,80,30,.35),inset 0 2px 4px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.15)}
.c-g4{background:linear-gradient(180deg,#22cccc,#009999);
  box-shadow:0 4px 12px rgba(30,200,200,.3),inset 0 2px 4px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.15)}
.c-b4{background:linear-gradient(180deg,#9944ee,#7722cc);
  box-shadow:0 4px 12px rgba(150,60,230,.35),inset 0 2px 4px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.15)}

/* ── Row 5 Colors ────────────────────────────── */
.c-r5{background:linear-gradient(180deg,#ffcc00,#ddaa00);color:#333;text-shadow:none;
  box-shadow:0 4px 12px rgba(255,200,0,.35),inset 0 2px 4px rgba(255,255,255,.3),inset 0 -2px 4px rgba(0,0,0,.1)}
.c-g5{background:linear-gradient(180deg,#33bbee,#1199cc);
  box-shadow:0 4px 12px rgba(50,180,230,.35),inset 0 2px 4px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.15)}
.c-b5{background:linear-gradient(180deg,#ff44aa,#dd2288);
  box-shadow:0 4px 12px rgba(255,60,160,.35),inset 0 2px 4px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.15)}

/* ── Row 6 Colors ────────────────────────────── */
.c-r6{background:linear-gradient(180deg,#ffee66,#ddcc33);color:#333;text-shadow:none;
  box-shadow:0 4px 12px rgba(255,230,80,.3),inset 0 2px 4px rgba(255,255,255,.3),inset 0 -2px 4px rgba(0,0,0,.1)}
.c-g6{background:linear-gradient(180deg,#66ddff,#44bbdd);color:#1a3a4a;text-shadow:none;
  box-shadow:0 4px 12px rgba(100,220,255,.3),inset 0 2px 4px rgba(255,255,255,.3),inset 0 -2px 4px rgba(0,0,0,.1)}
.c-b6{background:linear-gradient(180deg,#ff88cc,#dd66aa);
  box-shadow:0 4px 12px rgba(255,130,200,.3),inset 0 2px 4px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.15)}

/* ── Function Buttons (FLASH/STROBE/FADE/SMOOTH) */
.c-func{
  background:linear-gradient(180deg,#d8d8d8,#b8b8b8);
  color:#333;text-shadow:none;
  box-shadow:0 3px 8px rgba(0,0,0,.2),inset 0 1px 3px rgba(255,255,255,.5);
}

/* ── Status Bar ──────────────────────────────── */
.status{
  text-align:center;padding:8px 4px 0;
  font-size:.7rem;color:#888;min-height:22px;
  transition:color .3s;font-weight:500;
}

/* ── Footer info ─────────────────────────────── */
.info{
  text-align:center;padding:10px;
  font-size:.6rem;color:#444;letter-spacing:.5px;
}
.info a{color:#48dbfb;text-decoration:none}

/* ── Responsive tweaks ───────────────────────── */
@media(max-width:340px){
  .remote{padding:18px 12px 16px}
  .grid{gap:7px}
  .btn.circle{width:50px;height:50px;font-size:.65rem}
  .btn.bright{width:50px;height:50px;font-size:1.1rem}
  .btn.rect{padding:8px 3px;font-size:.65rem}
}
@media(min-width:400px){
  .btn.circle{width:64px;height:64px}
  .btn.bright{width:64px;height:64px}
}
</style>
</head>
<body>

<!-- TOP BAR -->
<div class="topbar">
  <span class="title">RGB IR REMOTE</span>
  <a href="/custom" class="nav-link">&#9881; Custom</a>
</div>

<!-- REMOTE -->
<div class="remote-wrap">
<div class="remote">
  <div class="grid">

    <!-- Row 1: Brightness + / - / OFF / ON -->
    <button class="btn bright" onclick="S(this,'00F700FF')" title="Brightness Up">&#9728;</button>
    <button class="btn bright" onclick="S(this,'00F7807F')" title="Brightness Down" style="font-size:1rem">&#9788;</button>
    <button class="btn rect c-off" onclick="S(this,'00F740BF')">OFF</button>
    <button class="btn rect c-on"  onclick="S(this,'00F7C03F')">ON</button>

    <div class="sep"></div>

    <!-- Row 2: R / G / B / W -->
    <button class="btn circle c-red"   onclick="S(this,'00F720DF')">R</button>
    <button class="btn circle c-green" onclick="S(this,'00F7A05F')">G</button>
    <button class="btn circle c-blue"  onclick="S(this,'00F7609F')">B</button>
    <button class="btn circle c-white" onclick="S(this,'00F7E01F')">W</button>

    <div class="sep"></div>

    <!-- Row 3 -->
    <button class="btn circle c-r3" onclick="S(this,'00F710EF')"></button>
    <button class="btn circle c-g3" onclick="S(this,'00F7906F')"></button>
    <button class="btn circle c-b3" onclick="S(this,'00F750AF')"></button>
    <button class="btn rect c-func" onclick="S(this,'00F7D02F')">FLASH</button>

    <!-- Row 4 -->
    <button class="btn circle c-r4" onclick="S(this,'00F730CF')"></button>
    <button class="btn circle c-g4" onclick="S(this,'00F7B04F')"></button>
    <button class="btn circle c-b4" onclick="S(this,'00F7708F')"></button>
    <button class="btn rect c-func" onclick="S(this,'00F7F00F')">STROBE</button>

    <!-- Row 5 -->
    <button class="btn circle c-r5" onclick="S(this,'00F708F7')"></button>
    <button class="btn circle c-g5" onclick="S(this,'00F78877')"></button>
    <button class="btn circle c-b5" onclick="S(this,'00F748B7')"></button>
    <button class="btn rect c-func" onclick="S(this,'00F7C837')">FADE</button>

    <!-- Row 6 -->
    <button class="btn circle c-r6" onclick="S(this,'00F728D7')"></button>
    <button class="btn circle c-g6" onclick="S(this,'00F7A857')"></button>
    <button class="btn circle c-b6" onclick="S(this,'00F76897')"></button>
    <button class="btn rect c-func" onclick="S(this,'00F7E817')">SMOOTH</button>

  </div>
  <div class="status" id="st">Ready</div>
</div>
</div>

<div class="info">ESP32 Universal IR Remote &bull; <a href="https://sakshyambastakoti.com.np" target="_blank">Sakshyam Bastakoti</a></div>

<script>
var st=document.getElementById('st');
function S(el,code){
  el.classList.remove('flash-fx');void el.offsetWidth;el.classList.add('flash-fx');
  st.style.color='#48dbfb';st.textContent='Sending 0x'+code+' \u2026';
  fetch('/api/send?code='+code)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#2ecc71':'#ff6b6b';
      st.textContent=d.ok?'\u2714 Sent 0x'+code:'\u2718 Failed';
    })
    .catch(function(){st.style.color='#ff6b6b';st.textContent='\u2718 Connection error';});
}
</script>
</body>
</html>
)==";


// ═════════════════════════════════════════════════════════════
//  PAGE 2 — CUSTOM COMMANDS
// ═════════════════════════════════════════════════════════════
const char CUSTOM_HTML[] PROGMEM = R"==(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no">
<title>Custom Commands - RGB IR Remote</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
body{
  font-family:'Segoe UI',system-ui,-apple-system,sans-serif;
  background:#1a1a2e;
  min-height:100vh;
  color:#e0e0e0;
  -webkit-tap-highlight-color:transparent;
  user-select:none;
}

/* ── Top Bar ─────────────────────────────────── */
.topbar{
  width:100%;padding:12px 16px;
  display:flex;justify-content:space-between;align-items:center;
  background:rgba(0,0,0,.3);
  border-bottom:1px solid rgba(255,255,255,.05);
}
.topbar .title{
  font-size:1.05rem;font-weight:800;letter-spacing:1.5px;
  background:linear-gradient(90deg,#ff6b6b,#feca57,#48dbfb,#ff9ff3);
  -webkit-background-clip:text;-webkit-text-fill-color:transparent;
  background-clip:text;
}
.topbar .nav-link{
  color:#48dbfb;text-decoration:none;font-size:.82rem;font-weight:600;
  padding:6px 14px;border-radius:8px;
  border:1px solid rgba(72,219,251,.3);
  transition:all .2s;
}
.topbar .nav-link:hover{background:rgba(72,219,251,.12)}

/* ── Content ─────────────────────────────────── */
.content{
  max-width:440px;margin:0 auto;padding:20px 14px 30px;
}

/* ── Section Card ────────────────────────────── */
.card{
  background:linear-gradient(180deg,#1e1e3a,#16162e);
  border-radius:18px;padding:20px 16px;
  border:1px solid rgba(255,255,255,.06);
  box-shadow:0 12px 40px rgba(0,0,0,.4);
  margin-bottom:18px;
}
.card h2{
  font-size:.95rem;margin-bottom:14px;color:#48dbfb;
  letter-spacing:.5px;display:flex;align-items:center;gap:8px;
}

/* ── Status Bar ──────────────────────────────── */
.status{
  text-align:center;padding:10px 4px;
  font-size:.75rem;color:#48dbfb;min-height:28px;
  transition:color .3s;font-weight:500;
}

/* ── Quick Send Section ──────────────────────── */
.qrow{display:flex;gap:8px}
.qrow input{
  flex:1;padding:12px 14px;border-radius:10px;
  border:1px solid #2a2a48;background:#0e0e20;color:#ddd;
  font-size:.95rem;font-family:'Courier New',monospace;
  outline:none;transition:border-color .2s;letter-spacing:1.5px;
}
.qrow input:focus{border-color:#48dbfb}
.qrow input::placeholder{color:#3a3a5a;letter-spacing:0}

/* ── Buttons ─────────────────────────────────── */
.abtn{
  padding:10px 18px;border-radius:10px;border:none;cursor:pointer;
  font-size:.8rem;font-weight:700;color:#fff;white-space:nowrap;
  transition:transform .1s,filter .2s;
  box-shadow:0 3px 10px rgba(0,0,0,.3);
}
.abtn:active{transform:scale(.93)}
.abtn.send{background:linear-gradient(180deg,#0abde3,#0897b4)}
.abtn.primary{background:linear-gradient(180deg,#48dbfb,#2e9ab8)}
.abtn.edit-btn{background:linear-gradient(180deg,#feca57,#d4a520);color:#222;font-size:.7rem;padding:8px 12px}
.abtn.del-btn{background:linear-gradient(180deg,#ff6b6b,#cc4444);font-size:.7rem;padding:8px 12px}

/* ── Add Button Form ─────────────────────────── */
.form-row{display:flex;gap:8px;margin-bottom:10px;flex-wrap:wrap}
.form-row input{
  flex:1;min-width:100px;padding:11px 14px;border-radius:10px;
  border:1px solid #2a2a48;background:#0e0e20;color:#ddd;
  font-size:.85rem;outline:none;transition:border-color .2s;
}
.form-row input:focus{border-color:#48dbfb}
.form-row input::placeholder{color:#3a3a5a}
.form-row .hex-input{font-family:'Courier New',monospace;letter-spacing:1px}

/* ── Custom Button List ──────────────────────── */
.clist{margin-top:6px}
.citem{
  display:flex;align-items:center;gap:10px;
  padding:12px 12px;margin-bottom:8px;border-radius:12px;
  background:rgba(255,255,255,.03);
  border:1px solid rgba(255,255,255,.05);
  transition:background .2s;
}
.citem:hover{background:rgba(255,255,255,.06)}
.citem .cname{flex:1;font-size:.85rem;font-weight:600}
.citem .ccode{
  font-size:.78rem;color:#666;font-family:'Courier New',monospace;
  letter-spacing:.5px;
}
.citem .actions{display:flex;gap:6px;flex-shrink:0}
.empty-msg{
  text-align:center;padding:18px 10px;
  font-size:.8rem;color:#3a3a5a;
}

/* ── Info Footer ─────────────────────────────── */
.info{
  text-align:center;padding:10px;
  font-size:.6rem;color:#444;letter-spacing:.5px;
}
.info a{color:#48dbfb;text-decoration:none}
</style>
</head>
<body>

<!-- TOP BAR -->
<div class="topbar">
  <span class="title">CUSTOM COMMANDS</span>
  <a href="/" class="nav-link">&#9664; Remote</a>
</div>

<div class="content">

  <!-- Status -->
  <div class="status" id="st">Ready</div>

  <!-- QUICK SEND -->
  <div class="card">
    <h2>&#9889; Quick Send</h2>
    <div class="qrow">
      <input type="text" id="qhex" placeholder="HEX  e.g. 00FF20DF" maxlength="10"
             oninput="this.value=this.value.replace(/[^0-9a-fA-Fx]/g,'')">
      <button class="abtn send" onclick="QS()">SEND</button>
    </div>
  </div>

  <!-- ADD CUSTOM BUTTON -->
  <div class="card">
    <h2>&#10133; Add Custom Button</h2>
    <div class="form-row">
      <input type="text" id="cn" placeholder="Button name" maxlength="20">
      <input type="text" id="cc" class="hex-input" placeholder="HEX code" maxlength="10"
             oninput="this.value=this.value.replace(/[^0-9a-fA-Fx]/g,'')">
      <button class="abtn primary" onclick="AB()">ADD</button>
    </div>
  </div>

  <!-- SAVED BUTTONS -->
  <div class="card">
    <h2>&#128203; Saved Buttons</h2>
    <div id="cl" class="clist">
      <div class="empty-msg">Loading...</div>
    </div>
  </div>

  <!-- WIFI SETTINGS -->
  <div class="card" style="border-color:rgba(255,100,100,.15)">
    <h2 style="color:#ff6b6b">&#128274; WiFi Settings</h2>
    <p style="font-size:.75rem;color:#666;margin-bottom:12px">Erase saved WiFi credentials and reboot into setup mode to connect to a different network.</p>
    <button class="abtn" style="background:linear-gradient(180deg,#ff6b6b,#cc4444);width:100%;padding:12px" onclick="RW()">Reset WiFi &amp; Reboot</button>
  </div>

</div>

<div class="info">ESP32 Universal IR Remote &bull; <a href="https://sakshyambastakoti.com.np" target="_blank">Sakshyam Bastakoti</a></div>

<script>
var st=document.getElementById('st');

/* ── Quick Send ──────────────────────────────── */
function QS(){
  var c=document.getElementById('qhex').value.trim().replace(/^0x/i,'').toUpperCase();
  if(!c||!/^[0-9A-F]{1,8}$/.test(c)){st.style.color='#ff6b6b';st.textContent='Enter a valid HEX code';return;}
  while(c.length<8)c='0'+c;
  st.style.color='#48dbfb';st.textContent='Sending 0x'+c+' \u2026';
  fetch('/api/send?code='+c)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#2ecc71':'#ff6b6b';
      st.textContent=d.ok?'\u2714 Sent 0x'+c:'\u2718 Failed';
    })
    .catch(function(){st.style.color='#ff6b6b';st.textContent='\u2718 Connection error';});
}

/* ── Load custom buttons ─────────────────────── */
function LC(){
  fetch('/api/custom')
    .then(function(r){return r.json()})
    .then(function(d){
      var h='';
      d.forEach(function(b){
        h+='<div class="citem">';
        h+='<span class="cname">'+E(b.name)+'</span>';
        h+='<span class="ccode">0x'+E(b.code)+'</span>';
        h+='<div class="actions">';
        h+='<button class="abtn send" style="font-size:.7rem;padding:8px 12px" onclick="SC(\''+EA(b.code)+'\')">SEND</button>';
        h+='<button class="abtn edit-btn" onclick="EB('+b.id+',\''+EA(b.name)+'\',\''+EA(b.code)+'\')">EDIT</button>';
        h+='<button class="abtn del-btn" onclick="DB('+b.id+')">DEL</button>';
        h+='</div></div>';
      });
      if(!d.length) h='<div class="empty-msg">No custom buttons yet \u2014 add one above!</div>';
      document.getElementById('cl').innerHTML=h;
    })
    .catch(function(){document.getElementById('cl').innerHTML='<div class="empty-msg" style="color:#ff6b6b">Failed to load</div>';});
}

/* ── Send custom code ────────────────────────── */
function SC(code){
  st.style.color='#48dbfb';st.textContent='Sending 0x'+code+' \u2026';
  fetch('/api/send?code='+code)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#2ecc71':'#ff6b6b';
      st.textContent=d.ok?'\u2714 Sent 0x'+code:'\u2718 Failed';
    })
    .catch(function(){st.style.color='#ff6b6b';st.textContent='\u2718 Connection error';});
}

/* ── Add custom button ───────────────────────── */
function AB(){
  var n=document.getElementById('cn').value.trim();
  var c=document.getElementById('cc').value.trim().replace(/^0x/i,'').toUpperCase();
  if(!n||!c){st.style.color='#ff6b6b';st.textContent='Enter name and HEX code';return;}
  if(!/^[0-9A-F]{1,8}$/.test(c)){st.style.color='#ff6b6b';st.textContent='Invalid HEX code';return;}
  while(c.length<8)c='0'+c;
  fetch('/api/custom/add?name='+encodeURIComponent(n)+'&code='+c)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#2ecc71':'#ff6b6b';
      st.textContent=d.ok?'\u2714 Button added':'\u2718 '+(d.msg||'Error');
      if(d.ok){document.getElementById('cn').value='';document.getElementById('cc').value='';LC();}
    })
    .catch(function(){st.style.color='#ff6b6b';st.textContent='\u2718 Connection error';});
}

/* ── Edit custom button ──────────────────────── */
function EB(id,name,code){
  var n=prompt('Button name:',name);if(n===null)return;
  var c=prompt('HEX code (without 0x):',code);if(c===null)return;
  c=c.replace(/^0x/i,'').toUpperCase();
  if(!/^[0-9A-F]{1,8}$/.test(c)){st.style.color='#ff6b6b';st.textContent='Invalid HEX code';return;}
  while(c.length<8)c='0'+c;
  fetch('/api/custom/edit?id='+id+'&name='+encodeURIComponent(n)+'&code='+c)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#2ecc71':'#ff6b6b';
      st.textContent=d.ok?'\u2714 Updated':'\u2718 '+(d.msg||'Error');
      LC();
    })
    .catch(function(){st.style.color='#ff6b6b';st.textContent='\u2718 Connection error';});
}

/* ── Delete custom button ────────────────────── */
function DB(id){
  if(!confirm('Delete this button?'))return;
  fetch('/api/custom/delete?id='+id)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#2ecc71':'#ff6b6b';
      st.textContent=d.ok?'\u2714 Deleted':'\u2718 '+(d.msg||'Error');
      LC();
    })
    .catch(function(){st.style.color='#ff6b6b';st.textContent='\u2718 Connection error';});
}

/* ── Helpers ─────────────────────────────────── */
function E(s){var d=document.createElement('div');d.appendChild(document.createTextNode(s));return d.innerHTML;}
function EA(s){return String(s).replace(/\\/g,'\\\\').replace(/'/g,"\\'");}

/* ── Reset WiFi ──────────────────────────────── */
function RW(){
  if(!confirm('Reset WiFi credentials?\n\nThe ESP32 will reboot into setup mode.\nYou will need to connect to "RGB_IR_Setup" WiFi to reconfigure.'))return;
  st.style.color='#48dbfb';st.textContent='Resetting WiFi...';
  fetch('/api/wifi/reset')
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color='#feca57';
      st.textContent='\u2714 WiFi erased \u2014 rebooting into setup mode...';
    })
    .catch(function(){st.style.color='#feca57';st.textContent='Rebooting...';});
}

LC();
</script>
</body>
</html>
)==";

#endif

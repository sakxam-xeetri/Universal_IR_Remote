/*
 * =============================================================
 *  Web UI – Embedded HTML / CSS / JavaScript
 * =============================================================
 *  Stored as a PROGMEM raw-string literal so it lives in flash.
 *  Served to the browser by the web-handler module.
 * =============================================================
 */

#ifndef WEB_UI_H
#define WEB_UI_H

#include <Arduino.h>

const char INDEX_HTML[] PROGMEM = R"==(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1,maximum-scale=1,user-scalable=no">
<title>RGB IR Remote</title>
<style>
*{margin:0;padding:0;box-sizing:border-box}
html{height:100%}
body{
  font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,Helvetica,Arial,sans-serif;
  background:linear-gradient(160deg,#0a0a14 0%,#111128 40%,#0d1b2a 100%);
  min-height:100vh;
  display:flex;flex-direction:column;align-items:center;
  padding:16px 8px 30px;
  color:#e0e0e0;
  -webkit-tap-highlight-color:transparent;
  user-select:none;
}

/* ── Header ─────────────────────────────────────── */
.hdr{text-align:center;margin-bottom:14px}
.hdr h1{
  font-size:1.5rem;letter-spacing:2px;font-weight:800;
  background:linear-gradient(90deg,#ff6b6b,#feca57,#48dbfb,#ff9ff3);
  -webkit-background-clip:text;-webkit-text-fill-color:transparent;
  background-clip:text;
}
.hdr p{font-size:.72rem;color:#555;margin-top:3px;letter-spacing:.5px}

/* ── Remote body ────────────────────────────────── */
.remote{
  background:linear-gradient(180deg,#1c1c32 0%,#14142a 100%);
  border-radius:28px;padding:20px 14px 14px;
  max-width:370px;width:100%;
  box-shadow:0 20px 60px rgba(0,0,0,.55),
             inset 0 1px 0 rgba(255,255,255,.04);
  border:1px solid rgba(255,255,255,.07);
}
.remote-label{
  text-align:center;font-size:.65rem;color:#444;
  margin-bottom:12px;letter-spacing:1px;text-transform:uppercase;
}

/* ── Button grid ────────────────────────────────── */
.grid{display:grid;grid-template-columns:repeat(4,1fr);gap:9px}
.sep{
  grid-column:1/-1;height:1px;margin:3px 0;
  background:linear-gradient(90deg,transparent,rgba(255,255,255,.08),transparent);
}
.row-label{
  grid-column:1/-1;font-size:.55rem;color:#444;
  padding:2px 4px 0;letter-spacing:.5px;text-transform:uppercase;
}

/* ── Button base ────────────────────────────────── */
.btn{
  display:flex;flex-direction:column;align-items:center;justify-content:center;
  padding:10px 3px;border-radius:11px;border:none;cursor:pointer;
  font-size:.7rem;font-weight:700;color:#fff;
  text-shadow:0 1px 3px rgba(0,0,0,.6);
  transition:transform .1s ease,box-shadow .25s ease;
  min-height:56px;position:relative;overflow:hidden;
  box-shadow:0 4px 12px rgba(0,0,0,.35),inset 0 1px 0 rgba(255,255,255,.12);
}
.btn:active{transform:scale(.88)}
.btn .lbl{pointer-events:none;line-height:1.1;text-align:center}
.btn .hex{
  font-size:.42rem;opacity:.55;margin-top:3px;
  font-weight:400;pointer-events:none;font-family:monospace;letter-spacing:.3px;
}

/* ── Click flash animation ──────────────────────── */
.btn.flash-fx{animation:pulse .4s ease}
@keyframes pulse{
  0%  {box-shadow:0 0 0 0 rgba(255,255,255,.55)}
  100%{box-shadow:0 0 0 18px rgba(255,255,255,0)}
}

/* ── Status bar ─────────────────────────────────── */
.status{
  text-align:center;padding:8px 4px;margin-top:8px;
  font-size:.72rem;color:#48dbfb;min-height:28px;
  transition:color .3s;
}

/* ── Section panels ─────────────────────────────── */
.panel{
  margin-top:18px;max-width:370px;width:100%;
  background:linear-gradient(180deg,#1c1c32,#14142a);
  border-radius:22px;padding:18px 14px;
  border:1px solid rgba(255,255,255,.06);
  box-shadow:0 12px 36px rgba(0,0,0,.35);
}
.panel h2{font-size:.92rem;margin-bottom:12px;color:#48dbfb;letter-spacing:.5px}

/* ── Input / action rows ────────────────────────── */
.irow{display:flex;gap:8px;margin-bottom:10px;flex-wrap:wrap}
.irow input{
  flex:1;min-width:90px;padding:9px 12px;border-radius:9px;
  border:1px solid #2a2a44;background:#0e0e1e;color:#ddd;
  font-size:.82rem;outline:none;transition:border-color .2s;
}
.irow input:focus{border-color:#48dbfb}
.irow input::placeholder{color:#444}

.abtn{
  padding:9px 16px;border-radius:9px;border:none;cursor:pointer;
  font-size:.75rem;font-weight:700;color:#fff;
  transition:transform .1s,filter .2s;
  box-shadow:0 3px 10px rgba(0,0,0,.3);
}
.abtn:active{transform:scale(.93)}
.abtn.primary{background:linear-gradient(180deg,#48dbfb,#2e9ab8)}
.abtn.send{background:linear-gradient(180deg,#0abde3,#0897b4)}
.abtn.edit-btn{background:linear-gradient(180deg,#feca57,#d4a520);color:#222;font-size:.65rem;padding:6px 10px}
.abtn.del-btn{background:linear-gradient(180deg,#ff6b6b,#cc4444);font-size:.65rem;padding:6px 10px}

/* ── Custom button list ─────────────────────────── */
.clist{margin-top:8px}
.citem{
  display:flex;align-items:center;gap:8px;
  padding:9px 10px;margin-bottom:6px;border-radius:10px;
  background:rgba(255,255,255,.025);
  border:1px solid rgba(255,255,255,.04);
  transition:background .2s;
}
.citem:hover{background:rgba(255,255,255,.05)}
.citem .cn{flex:1;font-size:.8rem;font-weight:600}
.citem .cc{font-size:.72rem;color:#666;font-family:monospace}
.citem .ca{display:flex;gap:5px}

/* ── Quick-send row ─────────────────────────────── */
.qrow{display:flex;gap:8px}
.qrow input{
  flex:1;padding:10px 14px;border-radius:9px;
  border:1px solid #2a2a44;background:#0e0e1e;color:#ddd;
  font-size:.9rem;font-family:monospace;outline:none;
  transition:border-color .2s;letter-spacing:1px;
}
.qrow input:focus{border-color:#0abde3}
.qrow input::placeholder{color:#444}

/* ── Footer ─────────────────────────────────────── */
.foot{text-align:center;padding:22px 0 6px;font-size:.55rem;color:#333}

/* ══════════════════════════════════════════════════
   BUTTON COLOR CLASSES  (gradient backgrounds)
   ══════════════════════════════════════════════════ */
.c-bup   {background:linear-gradient(180deg,#5a5a6e,#3e3e52)}
.c-bdn   {background:linear-gradient(180deg,#48485c,#32324a)}
.c-off   {background:linear-gradient(180deg,#a82020,#6e1414)}
.c-on    {background:linear-gradient(180deg,#1da81d,#126e12)}
.c-red   {background:linear-gradient(180deg,#ff1a1a,#cc0000)}
.c-green {background:linear-gradient(180deg,#00cc00,#009200)}
.c-blue  {background:linear-gradient(180deg,#1a5aff,#0044cc)}
.c-white {background:linear-gradient(180deg,#e8e8e8,#c0c0c0);color:#1a1a1a;text-shadow:none}
.c-r2    {background:linear-gradient(180deg,#ff7722,#cc5500)}
.c-g2    {background:linear-gradient(180deg,#22ff66,#00cc44);color:#1a1a1a;text-shadow:none}
.c-b2    {background:linear-gradient(180deg,#2266ff,#0044cc)}
.c-flash {background:linear-gradient(180deg,#daa520,#b38600)}
.c-r3    {background:linear-gradient(180deg,#ff4422,#cc2200)}
.c-g3    {background:linear-gradient(180deg,#00cccc,#008f8f)}
.c-b3    {background:linear-gradient(180deg,#9944ff,#7722cc)}
.c-strb  {background:linear-gradient(180deg,#c8c800,#8f8f00)}
.c-r4    {background:linear-gradient(180deg,#ffcc00,#b38f00);color:#1a1a1a;text-shadow:none}
.c-g4    {background:linear-gradient(180deg,#22ccff,#0099cc)}
.c-b4    {background:linear-gradient(180deg,#ff22cc,#cc0099)}
.c-fade  {background:linear-gradient(180deg,#44a844,#2e7a2e)}
.c-r5    {background:linear-gradient(180deg,#ffff55,#cccc22);color:#1a1a1a;text-shadow:none}
.c-g5    {background:linear-gradient(180deg,#55ccff,#2299cc)}
.c-b5    {background:linear-gradient(180deg,#ff55cc,#cc2299)}
.c-smooth{background:linear-gradient(180deg,#5555aa,#3a3a80)}
</style>
</head>
<body>

<!-- ═══════ HEADER ═══════ -->
<div class="hdr">
  <h1>RGB IR REMOTE</h1>
  <p>ESP32 WiFi IR Controller &bull; NEC 32-bit</p>
</div>

<!-- ═══════ REMOTE ═══════ -->
<div class="remote">
  <div class="remote-label">24-Key RGB LED Remote</div>
  <div class="grid">

    <!-- Row 1 – Controls -->
    <button class="btn c-bup"  onclick="S(this,'00FF00FF')"><span class="lbl">BRIGHT+</span><span class="hex">00FF00FF</span></button>
    <button class="btn c-bdn"  onclick="S(this,'00FF807F')"><span class="lbl">BRIGHT-</span><span class="hex">00FF807F</span></button>
    <button class="btn c-off"  onclick="S(this,'00FF40BF')"><span class="lbl">OFF</span><span class="hex">00FF40BF</span></button>
    <button class="btn c-on"   onclick="S(this,'00FFC03F')"><span class="lbl">ON</span><span class="hex">00FFC03F</span></button>

    <div class="sep"></div>

    <!-- Row 2 – Primary Colors -->
    <button class="btn c-red"  onclick="S(this,'00FF20DF')"><span class="lbl">RED</span><span class="hex">00FF20DF</span></button>
    <button class="btn c-green"onclick="S(this,'00FFA05F')"><span class="lbl">GREEN</span><span class="hex">00FFA05F</span></button>
    <button class="btn c-blue" onclick="S(this,'00FF609F')"><span class="lbl">BLUE</span><span class="hex">00FF609F</span></button>
    <button class="btn c-white"onclick="S(this,'00FFE01F')"><span class="lbl">WHITE</span><span class="hex">00FFE01F</span></button>

    <div class="sep"></div>

    <!-- Row 3 -->
    <button class="btn c-r2"   onclick="S(this,'00FF10EF')"><span class="lbl">ORANGE</span><span class="hex">00FF10EF</span></button>
    <button class="btn c-g2"   onclick="S(this,'00FF906F')"><span class="lbl">LT GREEN</span><span class="hex">00FF906F</span></button>
    <button class="btn c-b2"   onclick="S(this,'00FF50AF')"><span class="lbl">DK BLUE</span><span class="hex">00FF50AF</span></button>
    <button class="btn c-flash"onclick="S(this,'00FFD02F')"><span class="lbl">FLASH</span><span class="hex">00FFD02F</span></button>

    <!-- Row 4 -->
    <button class="btn c-r3"   onclick="S(this,'00FF30CF')"><span class="lbl">DK ORANGE</span><span class="hex">00FF30CF</span></button>
    <button class="btn c-g3"   onclick="S(this,'00FFB04F')"><span class="lbl">CYAN</span><span class="hex">00FFB04F</span></button>
    <button class="btn c-b3"   onclick="S(this,'00FF708F')"><span class="lbl">PURPLE</span><span class="hex">00FF708F</span></button>
    <button class="btn c-strb" onclick="S(this,'00FFF00F')"><span class="lbl">STROBE</span><span class="hex">00FFF00F</span></button>

    <!-- Row 5 -->
    <button class="btn c-r4"   onclick="S(this,'00FF08F7')"><span class="lbl">YELLOW</span><span class="hex">00FF08F7</span></button>
    <button class="btn c-g4"   onclick="S(this,'00FF8877')"><span class="lbl">LT BLUE</span><span class="hex">00FF8877</span></button>
    <button class="btn c-b4"   onclick="S(this,'00FF48B7')"><span class="lbl">PINK</span><span class="hex">00FF48B7</span></button>
    <button class="btn c-fade" onclick="S(this,'00FFC837')"><span class="lbl">FADE</span><span class="hex">00FFC837</span></button>

    <!-- Row 6 -->
    <button class="btn c-r5"   onclick="S(this,'00FF28D7')"><span class="lbl">LT YELLOW</span><span class="hex">00FF28D7</span></button>
    <button class="btn c-g5"   onclick="S(this,'00FFA857')"><span class="lbl">SKY BLUE</span><span class="hex">00FFA857</span></button>
    <button class="btn c-b5"   onclick="S(this,'00FF6897')"><span class="lbl">LT PINK</span><span class="hex">00FF6897</span></button>
    <button class="btn c-smooth"onclick="S(this,'00FFE817')"><span class="lbl">SMOOTH</span><span class="hex">00FFE817</span></button>

  </div>
  <div class="status" id="st">Ready</div>
</div>

<!-- ═══════ QUICK SEND ═══════ -->
<div class="panel">
  <h2>&#9889; Quick Send</h2>
  <div class="qrow">
    <input type="text" id="qhex" placeholder="HEX code  e.g. 00FF20DF" maxlength="10"
           oninput="this.value=this.value.replace(/[^0-9a-fA-Fx]/g,'')">
    <button class="abtn send" onclick="QS()">SEND</button>
  </div>
</div>

<!-- ═══════ CUSTOM BUTTONS ═══════ -->
<div class="panel">
  <h2>&#127919; Custom Buttons</h2>
  <div class="irow">
    <input type="text" id="cn" placeholder="Button name" maxlength="20">
    <input type="text" id="cc" placeholder="HEX code" maxlength="10"
           oninput="this.value=this.value.replace(/[^0-9a-fA-Fx]/g,'')">
    <button class="abtn primary" onclick="AB()">ADD</button>
  </div>
  <div id="cl" class="clist"></div>
</div>

<div class="foot">ESP32 Universal IR Remote &bull; v1.0</div>

<!-- ═══════ JAVASCRIPT ═══════ -->
<script>
var st=document.getElementById('st');

/* ── Send IR code (from remote button) ──────── */
function S(el,code){
  el.classList.remove('flash-fx');
  void el.offsetWidth;
  el.classList.add('flash-fx');
  st.style.color='#48dbfb';
  st.textContent='Sending 0x'+code+' ...';
  fetch('/api/send?code='+code)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#2ecc71':'#ff6b6b';
      st.textContent=d.ok?'\u2714 Sent 0x'+code:'\u2718 Send failed';
    })
    .catch(function(){st.style.color='#ff6b6b';st.textContent='\u2718 Connection error';});
}

/* ── Quick Send ─────────────────────────────── */
function QS(){
  var c=document.getElementById('qhex').value.trim().replace(/^0x/i,'').toUpperCase();
  if(!c||!/^[0-9A-F]{1,8}$/.test(c)){st.style.color='#ff6b6b';st.textContent='Enter a valid HEX code';return;}
  while(c.length<8)c='0'+c;
  st.style.color='#48dbfb';st.textContent='Sending 0x'+c+' ...';
  fetch('/api/send?code='+c)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#2ecc71':'#ff6b6b';
      st.textContent=d.ok?'\u2714 Sent 0x'+c:'\u2718 Send failed';
    })
    .catch(function(){st.style.color='#ff6b6b';st.textContent='\u2718 Connection error';});
}

/* ── Load custom buttons list ───────────────── */
function LC(){
  fetch('/api/custom')
    .then(function(r){return r.json()})
    .then(function(d){
      var h='';
      d.forEach(function(b){
        h+='<div class="citem">';
        h+='<span class="cn">'+E(b.name)+'</span>';
        h+='<span class="cc">0x'+E(b.code)+'</span>';
        h+='<div class="ca">';
        h+='<button class="abtn send" style="font-size:.65rem;padding:6px 10px" onclick="SC(\''+E(b.code)+'\')">SEND</button>';
        h+='<button class="abtn edit-btn" onclick="EB('+b.id+',\''+EA(b.name)+'\',\''+EA(b.code)+'\')">EDIT</button>';
        h+='<button class="abtn del-btn" onclick="DB('+b.id+')">DEL</button>';
        h+='</div></div>';
      });
      if(!d.length)h='<p style="color:#444;font-size:.75rem;text-align:center;padding:10px 0">No custom buttons yet — add one above!</p>';
      document.getElementById('cl').innerHTML=h;
    })
    .catch(function(){});
}

/* ── Send custom button code ────────────────── */
function SC(code){
  st.style.color='#48dbfb';st.textContent='Sending 0x'+code+' ...';
  fetch('/api/send?code='+code)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#2ecc71':'#ff6b6b';
      st.textContent=d.ok?'\u2714 Sent 0x'+code:'\u2718 Send failed';
    })
    .catch(function(){st.style.color='#ff6b6b';st.textContent='\u2718 Connection error';});
}

/* ── Add custom button ──────────────────────── */
function AB(){
  var n=document.getElementById('cn').value.trim();
  var c=document.getElementById('cc').value.trim().replace(/^0x/i,'').toUpperCase();
  if(!n||!c){st.style.color='#ff6b6b';st.textContent='Enter button name and HEX code';return;}
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

/* ── Edit custom button (prompt-based) ──────── */
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
      st.textContent=d.ok?'\u2714 Button updated':'\u2718 '+(d.msg||'Error');
      LC();
    })
    .catch(function(){st.style.color='#ff6b6b';st.textContent='\u2718 Connection error';});
}

/* ── Delete custom button ───────────────────── */
function DB(id){
  if(!confirm('Delete this custom button?'))return;
  fetch('/api/custom/delete?id='+id)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#2ecc71':'#ff6b6b';
      st.textContent=d.ok?'\u2714 Button deleted':'\u2718 '+(d.msg||'Error');
      LC();
    })
    .catch(function(){st.style.color='#ff6b6b';st.textContent='\u2718 Connection error';});
}

/* ── HTML-escape helper ─────────────────────── */
function E(s){
  var d=document.createElement('div');d.appendChild(document.createTextNode(s));return d.innerHTML;
}
/* ── Escape for JS attribute (single-quoted) ── */
function EA(s){return String(s).replace(/\\/g,'\\\\').replace(/'/g,"\\'");}

/* ── Initial load ───────────────────────────── */
LC();
</script>

</body>
</html>
)==";

#endif

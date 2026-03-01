/*
 * =============================================================
 *  Web UI - Liquid Glass Morphism Design
 * =============================================================
 *  Professional glassmorphism UI with SVG icons
 *  Page 1: Remote Control  |  Page 2: Custom Commands
 * =============================================================
 */

#ifndef WEB_UI_H
#define WEB_UI_H

#include <Arduino.h>

// =============================================================
//  PAGE 1 — REMOTE CONTROL
// =============================================================
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
  background:#0a0a1a;
  background-image:
    radial-gradient(ellipse at 20% 20%,rgba(99,102,241,.15) 0%,transparent 50%),
    radial-gradient(ellipse at 80% 80%,rgba(236,72,153,.12) 0%,transparent 50%),
    radial-gradient(ellipse at 50% 50%,rgba(6,182,212,.08) 0%,transparent 60%);
  display:flex;flex-direction:column;align-items:center;
  min-height:100vh;color:#e0e0e0;
  -webkit-tap-highlight-color:transparent;
  user-select:none;overflow-x:hidden;
}

/* -- Animated BG Orbs -- */
.orb{position:fixed;border-radius:50%;filter:blur(80px);opacity:.3;pointer-events:none;z-index:0}
.orb-1{width:300px;height:300px;background:#6366f1;top:-80px;left:-60px;animation:float1 8s ease-in-out infinite}
.orb-2{width:250px;height:250px;background:#ec4899;bottom:-60px;right:-50px;animation:float2 10s ease-in-out infinite}
.orb-3{width:200px;height:200px;background:#06b6d4;top:50%;left:60%;animation:float3 12s ease-in-out infinite}
@keyframes float1{0%,100%{transform:translate(0,0)}50%{transform:translate(30px,40px)}}
@keyframes float2{0%,100%{transform:translate(0,0)}50%{transform:translate(-25px,-35px)}}
@keyframes float3{0%,100%{transform:translate(0,0)}50%{transform:translate(-20px,25px)}}

/* -- Glass Utility -- */
.glass{
  background:rgba(255,255,255,.05);
  backdrop-filter:blur(20px);-webkit-backdrop-filter:blur(20px);
  border:1px solid rgba(255,255,255,.08);
}
.glass-strong{
  background:rgba(255,255,255,.08);
  backdrop-filter:blur(30px);-webkit-backdrop-filter:blur(30px);
  border:1px solid rgba(255,255,255,.12);
}

/* -- Top Bar -- */
.topbar{
  position:sticky;top:0;z-index:100;
  width:100%;padding:14px 20px;
  display:flex;justify-content:space-between;align-items:center;
  background:rgba(10,10,26,.6);
  backdrop-filter:blur(24px);-webkit-backdrop-filter:blur(24px);
  border-bottom:1px solid rgba(255,255,255,.06);
}
.topbar .brand{display:flex;align-items:center;gap:10px}
.topbar .brand svg{width:22px;height:22px;flex-shrink:0}
.topbar .title{
  font-size:.95rem;font-weight:700;letter-spacing:1.2px;
  background:linear-gradient(135deg,#818cf8,#c084fc,#f472b6);
  -webkit-background-clip:text;-webkit-text-fill-color:transparent;
  background-clip:text;
}
.nav-link{
  display:flex;align-items:center;gap:6px;
  color:rgba(255,255,255,.7);text-decoration:none;font-size:.78rem;font-weight:600;
  padding:7px 16px;border-radius:10px;
  background:rgba(255,255,255,.05);
  border:1px solid rgba(255,255,255,.08);
  backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px);
  transition:all .25s ease;letter-spacing:.3px;
}
.nav-link svg{width:14px;height:14px;opacity:.7}
.nav-link:hover{background:rgba(255,255,255,.1);color:#fff;border-color:rgba(255,255,255,.15)}

/* -- Remote Body -- */
.remote-wrap{
  flex:1;display:flex;align-items:center;justify-content:center;
  padding:16px 12px;width:100%;position:relative;z-index:1;
}
.remote{
  background:rgba(255,255,255,.06);
  backdrop-filter:blur(40px);-webkit-backdrop-filter:blur(40px);
  border:1px solid rgba(255,255,255,.1);
  border-radius:28px;
  padding:26px 18px 22px;
  max-width:340px;width:100%;
  box-shadow:
    0 8px 32px rgba(0,0,0,.4),
    0 0 0 1px rgba(255,255,255,.05) inset,
    0 1px 0 rgba(255,255,255,.1) inset;
  position:relative;overflow:hidden;
}
.remote::before{
  content:'';position:absolute;top:0;left:0;right:0;height:50%;
  background:linear-gradient(180deg,rgba(255,255,255,.04),transparent);
  pointer-events:none;border-radius:28px 28px 0 0;
}

/* -- Grid -- */
.grid{
  display:grid;
  grid-template-columns:repeat(4,1fr);
  gap:10px;
  padding:6px 0;
  position:relative;z-index:1;
}
.sep{grid-column:1/-1;height:1px;margin:4px 0;background:linear-gradient(90deg,transparent,rgba(255,255,255,.06),transparent)}

/* -- Base Button -- */
.btn{
  display:flex;align-items:center;justify-content:center;
  border:none;cursor:pointer;
  font-weight:700;color:#fff;
  text-shadow:0 1px 3px rgba(0,0,0,.4);
  transition:transform .15s cubic-bezier(.2,.8,.2,1),box-shadow .25s;
  position:relative;overflow:hidden;
  -webkit-tap-highlight-color:transparent;
  letter-spacing:.4px;
}
.btn::after{
  content:'';position:absolute;top:0;left:0;right:0;bottom:0;
  background:linear-gradient(180deg,rgba(255,255,255,.15) 0%,transparent 50%);
  pointer-events:none;border-radius:inherit;
}
.btn:active{transform:scale(.88)!important}

/* -- Ripple -- */
.btn.ripple::before{
  content:'';position:absolute;width:120%;padding-bottom:120%;
  border-radius:50%;background:rgba(255,255,255,.3);
  transform:scale(0);animation:rippleAnim .5s ease-out;
  pointer-events:none;z-index:2;
}
@keyframes rippleAnim{to{transform:scale(1);opacity:0}}

/* -- Circle Buttons -- */
.btn.circle{
  width:60px;height:60px;border-radius:50%;
  margin:0 auto;
  box-shadow:
    0 4px 15px rgba(0,0,0,.3),
    inset 0 1px 2px rgba(255,255,255,.2),
    inset 0 -2px 4px rgba(0,0,0,.2);
  font-size:.7rem;
}

/* -- Rect Buttons -- */
.btn.rect{
  border-radius:12px;
  padding:10px 4px;
  min-height:44px;
  font-size:.68rem;letter-spacing:.5px;
  box-shadow:
    0 4px 12px rgba(0,0,0,.25),
    inset 0 1px 2px rgba(255,255,255,.15);
}

/* -- Brightness Buttons -- */
.btn.bright{
  background:rgba(255,255,255,.1);
  backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px);
  border:1px solid rgba(255,255,255,.15);
  color:#fff;text-shadow:none;
  border-radius:50%;width:60px;height:60px;margin:0 auto;
  box-shadow:0 4px 15px rgba(0,0,0,.2),inset 0 1px 2px rgba(255,255,255,.1);
}
.btn.bright svg{width:22px;height:22px;fill:currentColor;position:relative;z-index:1}
.btn.bright:hover{background:rgba(255,255,255,.15)}

/* -- OFF / ON -- */
.c-off{
  background:rgba(255,255,255,.08);
  backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px);
  border:1px solid rgba(255,255,255,.1);
  color:rgba(255,255,255,.7);text-shadow:none;
  box-shadow:0 4px 12px rgba(0,0,0,.2),inset 0 1px 2px rgba(255,255,255,.08);
}
.c-on{
  background:linear-gradient(135deg,#ef4444,#dc2626);
  box-shadow:0 4px 20px rgba(239,68,68,.35),inset 0 1px 2px rgba(255,255,255,.2);
}

/* -- Color Buttons -- */
.c-red{background:linear-gradient(135deg,#ef4444,#b91c1c);box-shadow:0 4px 20px rgba(239,68,68,.35),inset 0 1px 2px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.2)}
.c-green{background:linear-gradient(135deg,#22c55e,#15803d);box-shadow:0 4px 20px rgba(34,197,94,.3),inset 0 1px 2px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.2)}
.c-blue{background:linear-gradient(135deg,#3b82f6,#1d4ed8);box-shadow:0 4px 20px rgba(59,130,246,.35),inset 0 1px 2px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.2)}
.c-white{background:linear-gradient(135deg,#f1f5f9,#cbd5e1);color:#1e293b;text-shadow:none;box-shadow:0 4px 20px rgba(255,255,255,.15),inset 0 1px 2px rgba(255,255,255,.5),inset 0 -2px 4px rgba(0,0,0,.08)}

/* Row 3 */
.c-r3{background:linear-gradient(135deg,#f97316,#c2410c);box-shadow:0 4px 18px rgba(249,115,22,.3),inset 0 1px 2px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.2)}
.c-g3{background:linear-gradient(135deg,#34d399,#059669);box-shadow:0 4px 18px rgba(52,211,153,.25),inset 0 1px 2px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.2)}
.c-b3{background:linear-gradient(135deg,#6366f1,#4338ca);box-shadow:0 4px 18px rgba(99,102,241,.3),inset 0 1px 2px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.2)}

/* Row 4 */
.c-r4{background:linear-gradient(135deg,#ea580c,#9a3412);box-shadow:0 4px 18px rgba(234,88,12,.3),inset 0 1px 2px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.2)}
.c-g4{background:linear-gradient(135deg,#06b6d4,#0e7490);box-shadow:0 4px 18px rgba(6,182,212,.3),inset 0 1px 2px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.2)}
.c-b4{background:linear-gradient(135deg,#a855f7,#7e22ce);box-shadow:0 4px 18px rgba(168,85,247,.3),inset 0 1px 2px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.2)}

/* Row 5 */
.c-r5{background:linear-gradient(135deg,#eab308,#a16207);color:#1c1917;text-shadow:none;box-shadow:0 4px 18px rgba(234,179,8,.3),inset 0 1px 2px rgba(255,255,255,.3),inset 0 -2px 4px rgba(0,0,0,.1)}
.c-g5{background:linear-gradient(135deg,#38bdf8,#0284c7);box-shadow:0 4px 18px rgba(56,189,248,.3),inset 0 1px 2px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.2)}
.c-b5{background:linear-gradient(135deg,#ec4899,#be185d);box-shadow:0 4px 18px rgba(236,72,153,.3),inset 0 1px 2px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.2)}

/* Row 6 */
.c-r6{background:linear-gradient(135deg,#fde047,#ca8a04);color:#1c1917;text-shadow:none;box-shadow:0 4px 18px rgba(253,224,71,.25),inset 0 1px 2px rgba(255,255,255,.3),inset 0 -2px 4px rgba(0,0,0,.08)}
.c-g6{background:linear-gradient(135deg,#67e8f9,#22d3ee);color:#134e4a;text-shadow:none;box-shadow:0 4px 18px rgba(103,232,249,.25),inset 0 1px 2px rgba(255,255,255,.3),inset 0 -2px 4px rgba(0,0,0,.08)}
.c-b6{background:linear-gradient(135deg,#f9a8d4,#ec4899);box-shadow:0 4px 18px rgba(249,168,212,.25),inset 0 1px 2px rgba(255,255,255,.2),inset 0 -2px 4px rgba(0,0,0,.15)}

/* -- Function Buttons -- */
.c-func{
  background:rgba(255,255,255,.07);
  backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px);
  border:1px solid rgba(255,255,255,.1);
  color:rgba(255,255,255,.85);text-shadow:none;
  box-shadow:0 4px 12px rgba(0,0,0,.2),inset 0 1px 2px rgba(255,255,255,.08);
}
.c-func:hover{background:rgba(255,255,255,.12)}

/* -- Status -- */
.status{
  text-align:center;padding:12px 4px 2px;
  font-size:.72rem;min-height:28px;
  transition:color .3s;font-weight:500;
  color:rgba(255,255,255,.4);
  position:relative;z-index:1;
}

/* -- Footer -- */
.info{
  text-align:center;padding:12px;position:relative;z-index:1;
  font-size:.6rem;color:rgba(255,255,255,.25);letter-spacing:.5px;
}
.info a{color:#818cf8;text-decoration:none;transition:color .2s}
.info a:hover{color:#a5b4fc}

/* -- Responsive -- */
@media(max-width:340px){
  .remote{padding:18px 12px 16px;border-radius:22px}
  .grid{gap:7px}
  .btn.circle{width:50px;height:50px;font-size:.6rem}
  .btn.bright{width:50px;height:50px}
  .btn.bright svg{width:18px;height:18px}
  .btn.rect{padding:8px 3px;font-size:.6rem}
}
@media(min-width:400px){
  .btn.circle{width:66px;height:66px}
  .btn.bright{width:66px;height:66px}
}
</style>
</head>
<body>

<!-- BG Orbs -->
<div class="orb orb-1"></div>
<div class="orb orb-2"></div>
<div class="orb orb-3"></div>

<!-- TOP BAR -->
<div class="topbar">
  <div class="brand">
    <svg viewBox="0 0 24 24" fill="none" stroke="#818cf8" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 2v4M12 18v4M4.93 4.93l2.83 2.83M16.24 16.24l2.83 2.83M2 12h4M18 12h4M4.93 19.07l2.83-2.83M16.24 7.76l2.83-2.83"/><circle cx="12" cy="12" r="3"/></svg>
    <span class="title">RGB IR REMOTE</span>
  </div>
  <a href="/custom" class="nav-link">
    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="3" y="3" width="7" height="7" rx="1"/><rect x="14" y="3" width="7" height="7" rx="1"/><rect x="3" y="14" width="7" height="7" rx="1"/><rect x="14" y="14" width="7" height="7" rx="1"/></svg>
    Custom
  </a>
</div>

<!-- REMOTE -->
<div class="remote-wrap">
<div class="remote">
  <div class="grid">

    <!-- Row 1: Brightness / OFF / ON -->
    <button class="btn bright" onclick="S(this,'00F700FF')" title="Brighter">
      <svg viewBox="0 0 24 24"><path d="M12 3v1m0 16v1m-8-9H3m18 0h-1m-2.636-6.364l-.707.707M6.343 17.657l-.707.707m0-12.728l.707.707m11.314 11.314l.707.707M12 8a4 4 0 100 8 4 4 0 000-8z"/></svg>
    </button>
    <button class="btn bright" onclick="S(this,'00F7807F')" title="Darker">
      <svg viewBox="0 0 24 24" style="width:20px;height:20px"><path d="M12 3a9 9 0 109 9c0-.46-.04-.92-.1-1.36a5.389 5.389 0 01-4.4 2.26 5.403 5.403 0 01-3.14-9.8c-.44-.06-.9-.1-1.36-.1z"/></svg>
    </button>
    <button class="btn rect c-off" onclick="S(this,'00F740BF')">
      <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="width:16px;height:16px;margin-right:3px"><path d="M18.36 6.64A9 9 0 115.64 18.36 9 9 0 0118.36 6.64M12 2v10"/></svg>OFF
    </button>
    <button class="btn rect c-on" onclick="S(this,'00F7C03F')">
      <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" style="width:16px;height:16px;margin-right:3px"><path d="M18.36 6.64A9 9 0 115.64 18.36 9 9 0 0118.36 6.64M12 2v10"/></svg>ON
    </button>

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
  el.classList.remove('ripple');void el.offsetWidth;el.classList.add('ripple');
  setTimeout(function(){el.classList.remove('ripple')},500);
  st.style.color='#818cf8';st.textContent='Sending 0x'+code+' ...';
  fetch('/api/send?code='+code)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#34d399':'#f87171';
      st.textContent=d.ok?'Sent 0x'+code+' successfully':'Failed to send';
    })
    .catch(function(){st.style.color='#f87171';st.textContent='Connection error';});
}
</script>
</body>
</html>
)==";


// =============================================================
//  PAGE 2 — CUSTOM COMMANDS
// =============================================================
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
  background:#0a0a1a;
  background-image:
    radial-gradient(ellipse at 30% 10%,rgba(99,102,241,.12) 0%,transparent 50%),
    radial-gradient(ellipse at 70% 90%,rgba(236,72,153,.1) 0%,transparent 50%),
    radial-gradient(ellipse at 50% 50%,rgba(6,182,212,.06) 0%,transparent 60%);
  min-height:100vh;color:#e0e0e0;
  -webkit-tap-highlight-color:transparent;
  user-select:none;overflow-x:hidden;
}

/* -- Animated BG Orbs -- */
.orb{position:fixed;border-radius:50%;filter:blur(80px);opacity:.25;pointer-events:none;z-index:0}
.orb-1{width:280px;height:280px;background:#6366f1;top:-80px;right:-60px;animation:f1 9s ease-in-out infinite}
.orb-2{width:220px;height:220px;background:#ec4899;bottom:-50px;left:-40px;animation:f2 11s ease-in-out infinite}
@keyframes f1{0%,100%{transform:translate(0,0)}50%{transform:translate(-20px,30px)}}
@keyframes f2{0%,100%{transform:translate(0,0)}50%{transform:translate(20px,-25px)}}

/* -- Top Bar -- */
.topbar{
  position:sticky;top:0;z-index:100;
  width:100%;padding:14px 20px;
  display:flex;justify-content:space-between;align-items:center;
  background:rgba(10,10,26,.6);
  backdrop-filter:blur(24px);-webkit-backdrop-filter:blur(24px);
  border-bottom:1px solid rgba(255,255,255,.06);
}
.topbar .brand{display:flex;align-items:center;gap:10px}
.topbar .brand svg{width:22px;height:22px;flex-shrink:0}
.topbar .title{
  font-size:.95rem;font-weight:700;letter-spacing:1.2px;
  background:linear-gradient(135deg,#818cf8,#c084fc,#f472b6);
  -webkit-background-clip:text;-webkit-text-fill-color:transparent;
  background-clip:text;
}
.nav-link{
  display:flex;align-items:center;gap:6px;
  color:rgba(255,255,255,.7);text-decoration:none;font-size:.78rem;font-weight:600;
  padding:7px 16px;border-radius:10px;
  background:rgba(255,255,255,.05);
  border:1px solid rgba(255,255,255,.08);
  backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px);
  transition:all .25s ease;letter-spacing:.3px;
}
.nav-link svg{width:14px;height:14px;opacity:.7}
.nav-link:hover{background:rgba(255,255,255,.1);color:#fff;border-color:rgba(255,255,255,.15)}

/* -- Content -- */
.content{
  max-width:480px;margin:0 auto;padding:20px 16px 40px;
  position:relative;z-index:1;
}

/* -- Glass Card -- */
.card{
  background:rgba(255,255,255,.05);
  backdrop-filter:blur(24px);-webkit-backdrop-filter:blur(24px);
  border:1px solid rgba(255,255,255,.08);
  border-radius:20px;padding:22px 18px;
  box-shadow:0 8px 32px rgba(0,0,0,.3),inset 0 1px 0 rgba(255,255,255,.06);
  margin-bottom:16px;
  position:relative;overflow:hidden;
}
.card::before{
  content:'';position:absolute;top:0;left:0;right:0;height:40%;
  background:linear-gradient(180deg,rgba(255,255,255,.03),transparent);
  pointer-events:none;border-radius:20px 20px 0 0;
}
.card h2{
  font-size:.88rem;margin-bottom:14px;
  color:rgba(255,255,255,.85);font-weight:700;letter-spacing:.5px;
  display:flex;align-items:center;gap:10px;
  position:relative;z-index:1;
}
.card h2 svg{width:18px;height:18px;flex-shrink:0;opacity:.7}

/* -- Status -- */
.status{
  text-align:center;padding:10px 4px 6px;
  font-size:.75rem;min-height:32px;
  transition:color .3s;font-weight:500;
  color:rgba(255,255,255,.35);
  position:relative;z-index:1;
}

/* -- Inputs -- */
.inp{
  flex:1;padding:11px 14px;border-radius:12px;
  border:1px solid rgba(255,255,255,.08);
  background:rgba(0,0,0,.3);color:#e0e0e0;
  font-size:.88rem;outline:none;
  transition:border-color .25s,box-shadow .25s;
  backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px);
}
.inp:focus{border-color:rgba(129,140,248,.4);box-shadow:0 0 0 3px rgba(129,140,248,.1)}
.inp::placeholder{color:rgba(255,255,255,.2)}
.inp.mono{font-family:'SF Mono','Fira Code','Courier New',monospace;letter-spacing:1.5px}

/* -- Buttons -- */
.abtn{
  padding:10px 18px;border-radius:12px;border:none;cursor:pointer;
  font-size:.78rem;font-weight:700;color:#fff;white-space:nowrap;
  transition:transform .12s,box-shadow .25s,background .25s;
  position:relative;overflow:hidden;letter-spacing:.3px;
}
.abtn::after{
  content:'';position:absolute;top:0;left:0;right:0;bottom:0;
  background:linear-gradient(180deg,rgba(255,255,255,.12) 0%,transparent 60%);
  pointer-events:none;border-radius:inherit;
}
.abtn:active{transform:scale(.94)}

.abtn.send{
  background:linear-gradient(135deg,#6366f1,#4f46e5);
  box-shadow:0 4px 15px rgba(99,102,241,.3);
}
.abtn.send:hover{box-shadow:0 4px 20px rgba(99,102,241,.45)}

.abtn.primary{
  background:linear-gradient(135deg,#818cf8,#6366f1);
  box-shadow:0 4px 15px rgba(129,140,248,.3);
}
.abtn.primary:hover{box-shadow:0 4px 20px rgba(129,140,248,.45)}

.abtn.edit-btn{
  background:linear-gradient(135deg,#fbbf24,#d97706);color:#1c1917;
  font-size:.7rem;padding:8px 12px;
  box-shadow:0 3px 10px rgba(251,191,36,.2);
}
.abtn.del-btn{
  background:linear-gradient(135deg,#f87171,#dc2626);
  font-size:.7rem;padding:8px 12px;
  box-shadow:0 3px 10px rgba(248,113,113,.2);
}
.abtn.danger{
  background:linear-gradient(135deg,#f87171,#dc2626);
  box-shadow:0 4px 15px rgba(248,113,113,.25);
  width:100%;padding:12px;
}

/* -- Quick Send -- */
.qrow{display:flex;gap:10px;position:relative;z-index:1}

/* -- Form -- */
.form-row{display:flex;gap:8px;flex-wrap:wrap;position:relative;z-index:1}
.form-row .inp{min-width:100px}

/* -- Custom Button List -- */
.clist{position:relative;z-index:1}
.citem{
  display:flex;align-items:center;gap:10px;
  padding:12px 14px;margin-bottom:8px;border-radius:14px;
  background:rgba(255,255,255,.03);
  border:1px solid rgba(255,255,255,.05);
  transition:background .2s,border-color .2s;
}
.citem:hover{background:rgba(255,255,255,.06);border-color:rgba(255,255,255,.1)}
.citem .cname{flex:1;font-size:.84rem;font-weight:600;color:rgba(255,255,255,.85)}
.citem .ccode{
  font-size:.74rem;color:rgba(255,255,255,.3);
  font-family:'SF Mono','Fira Code','Courier New',monospace;letter-spacing:.5px;
}
.citem .actions{display:flex;gap:6px;flex-shrink:0}
.empty-msg{
  text-align:center;padding:24px 10px;
  font-size:.8rem;color:rgba(255,255,255,.2);
}

/* -- WiFi Card Accent -- */
.card.wifi-card{border-color:rgba(248,113,113,.15)}
.card.wifi-card h2{color:#f87171}
.card.wifi-card p{font-size:.75rem;color:rgba(255,255,255,.35);margin-bottom:14px;position:relative;z-index:1;line-height:1.5}

/* -- Footer -- */
.info{
  text-align:center;padding:12px;position:relative;z-index:1;
  font-size:.6rem;color:rgba(255,255,255,.2);letter-spacing:.5px;
}
.info a{color:#818cf8;text-decoration:none;transition:color .2s}
.info a:hover{color:#a5b4fc}
</style>
</head>
<body>

<!-- BG Orbs -->
<div class="orb orb-1"></div>
<div class="orb orb-2"></div>

<!-- TOP BAR -->
<div class="topbar">
  <div class="brand">
    <svg viewBox="0 0 24 24" fill="none" stroke="#818cf8" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><rect x="3" y="3" width="7" height="7" rx="1"/><rect x="14" y="3" width="7" height="7" rx="1"/><rect x="3" y="14" width="7" height="7" rx="1"/><rect x="14" y="14" width="7" height="7" rx="1"/></svg>
    <span class="title">CUSTOM COMMANDS</span>
  </div>
  <a href="/" class="nav-link">
    <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 2v4M12 18v4M4.93 4.93l2.83 2.83M16.24 16.24l2.83 2.83M2 12h4M18 12h4M4.93 19.07l2.83-2.83M16.24 7.76l2.83-2.83"/><circle cx="12" cy="12" r="3"/></svg>
    Remote
  </a>
</div>

<div class="content">

  <!-- Status -->
  <div class="status" id="st">Ready</div>

  <!-- QUICK SEND -->
  <div class="card">
    <h2>
      <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polygon points="13 2 3 14 12 14 11 22 21 10 12 10 13 2"/></svg>
      Quick Send
    </h2>
    <div class="qrow">
      <input type="text" id="qhex" class="inp mono" placeholder="HEX e.g. F720DF" maxlength="10"
             oninput="this.value=this.value.replace(/[^0-9a-fA-Fx]/g,'')">
      <button class="abtn send" onclick="QS()">SEND</button>
    </div>
  </div>

  <!-- ADD CUSTOM BUTTON -->
  <div class="card">
    <h2>
      <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="10"/><line x1="12" y1="8" x2="12" y2="16"/><line x1="8" y1="12" x2="16" y2="12"/></svg>
      Add Custom Button
    </h2>
    <div class="form-row">
      <input type="text" id="cn" class="inp" placeholder="Button name" maxlength="20">
      <input type="text" id="cc" class="inp mono" placeholder="HEX code" maxlength="10"
             oninput="this.value=this.value.replace(/[^0-9a-fA-Fx]/g,'')">
      <button class="abtn primary" onclick="AB()">ADD</button>
    </div>
  </div>

  <!-- SAVED BUTTONS -->
  <div class="card">
    <h2>
      <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M19 21H5a2 2 0 01-2-2V5a2 2 0 012-2h11l5 5v11a2 2 0 01-2 2z"/><polyline points="17 21 17 13 7 13 7 21"/><polyline points="7 3 7 8 15 8"/></svg>
      Saved Buttons
    </h2>
    <div id="cl" class="clist">
      <div class="empty-msg">Loading...</div>
    </div>
  </div>

  <!-- WIFI SETTINGS -->
  <div class="card wifi-card">
    <h2>
      <svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M5 12.55a11 11 0 0114.08 0M1.42 9a16 16 0 0121.16 0M8.53 16.11a6 6 0 016.95 0M12 20h.01"/></svg>
      WiFi Settings
    </h2>
    <p>Erase saved WiFi credentials and reboot into setup mode to connect to a different network.</p>
    <button class="abtn danger" onclick="RW()">Reset WiFi &amp; Reboot</button>
  </div>

</div>

<div class="info">ESP32 Universal IR Remote &bull; <a href="https://sakshyambastakoti.com.np" target="_blank">Sakshyam Bastakoti</a></div>

<script>
var st=document.getElementById('st');

/* -- Quick Send -- */
function QS(){
  var c=document.getElementById('qhex').value.trim().replace(/^0x/i,'').toUpperCase();
  if(!c||!/^[0-9A-F]{1,8}$/.test(c)){st.style.color='#f87171';st.textContent='Enter a valid HEX code';return;}
  while(c.length<8)c='0'+c;
  st.style.color='#818cf8';st.textContent='Sending 0x'+c+' ...';
  fetch('/api/send?code='+c)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#34d399':'#f87171';
      st.textContent=d.ok?'Sent 0x'+c+' successfully':'Failed to send';
    })
    .catch(function(){st.style.color='#f87171';st.textContent='Connection error';});
}

/* -- Load custom buttons -- */
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
      if(!d.length) h='<div class="empty-msg">No custom buttons yet</div>';
      document.getElementById('cl').innerHTML=h;
    })
    .catch(function(){document.getElementById('cl').innerHTML='<div class="empty-msg" style="color:#f87171">Failed to load</div>';});
}

/* -- Send custom code -- */
function SC(code){
  st.style.color='#818cf8';st.textContent='Sending 0x'+code+' ...';
  fetch('/api/send?code='+code)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#34d399':'#f87171';
      st.textContent=d.ok?'Sent 0x'+code+' successfully':'Failed to send';
    })
    .catch(function(){st.style.color='#f87171';st.textContent='Connection error';});
}

/* -- Add custom button -- */
function AB(){
  var n=document.getElementById('cn').value.trim();
  var c=document.getElementById('cc').value.trim().replace(/^0x/i,'').toUpperCase();
  if(!n||!c){st.style.color='#f87171';st.textContent='Enter name and HEX code';return;}
  if(!/^[0-9A-F]{1,8}$/.test(c)){st.style.color='#f87171';st.textContent='Invalid HEX code';return;}
  while(c.length<8)c='0'+c;
  fetch('/api/custom/add?name='+encodeURIComponent(n)+'&code='+c)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#34d399':'#f87171';
      st.textContent=d.ok?'Button added':'Error: '+(d.msg||'Unknown');
      if(d.ok){document.getElementById('cn').value='';document.getElementById('cc').value='';LC();}
    })
    .catch(function(){st.style.color='#f87171';st.textContent='Connection error';});
}

/* -- Edit custom button -- */
function EB(id,name,code){
  var n=prompt('Button name:',name);if(n===null)return;
  var c=prompt('HEX code (without 0x):',code);if(c===null)return;
  c=c.replace(/^0x/i,'').toUpperCase();
  if(!/^[0-9A-F]{1,8}$/.test(c)){st.style.color='#f87171';st.textContent='Invalid HEX code';return;}
  while(c.length<8)c='0'+c;
  fetch('/api/custom/edit?id='+id+'&name='+encodeURIComponent(n)+'&code='+c)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#34d399':'#f87171';
      st.textContent=d.ok?'Updated':'Error: '+(d.msg||'Unknown');
      LC();
    })
    .catch(function(){st.style.color='#f87171';st.textContent='Connection error';});
}

/* -- Delete custom button -- */
function DB(id){
  if(!confirm('Delete this button?'))return;
  fetch('/api/custom/delete?id='+id)
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color=d.ok?'#34d399':'#f87171';
      st.textContent=d.ok?'Deleted':'Error: '+(d.msg||'Unknown');
      LC();
    })
    .catch(function(){st.style.color='#f87171';st.textContent='Connection error';});
}

/* -- Helpers -- */
function E(s){var d=document.createElement('div');d.appendChild(document.createTextNode(s));return d.innerHTML;}
function EA(s){return String(s).replace(/\\/g,'\\\\').replace(/'/g,"\\'");}

/* -- Reset WiFi -- */
function RW(){
  if(!confirm('Reset WiFi credentials?\n\nThe ESP32 will reboot into setup mode.\nYou will need to connect to "RGB_IR_Setup" WiFi to reconfigure.'))return;
  st.style.color='#818cf8';st.textContent='Resetting WiFi...';
  fetch('/api/wifi/reset')
    .then(function(r){return r.json()})
    .then(function(d){
      st.style.color='#fbbf24';
      st.textContent='WiFi erased - rebooting into setup mode...';
    })
    .catch(function(){st.style.color='#fbbf24';st.textContent='Rebooting...';});
}

LC();
</script>
</body>
</html>
)==";

#endif

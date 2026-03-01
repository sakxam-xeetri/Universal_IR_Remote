# ESP32 Universal IR Remote Controller

A professional WiFi-based IR blaster that fully replaces a **24-key RGB LED strip remote**. The ESP32 connects to your home WiFi network, and **any device on the same network** (phone, tablet, laptop) can control the RGB strip through a browser.

**Author:** [Sakshyam Bastakoti](https://sakshyambastakoti.com.np)

---

## Features

- **WiFi Station Mode** — Connects to your existing home WiFi; no separate network needed
- **Multi-Device Access** — Any device on the same network can control the strip
- **mDNS Support** — Access via `http://irremote.local` (no need to remember the IP)
- **Auto-Reconnect** — Automatically reconnects if WiFi drops
- **Remote Page** — Button layout matches the physical 24-key remote exactly
- **Custom Commands Page** — Separate page for Quick Send and custom buttons
- **Color-Coded Circular Buttons** — Looks like the real remote
- **Click Animation** — Visual pulse feedback on every tap
- **Quick Send** — Enter any arbitrary HEX code and transmit instantly
- **Custom Buttons** — Create, edit, delete your own buttons
- **Persistent Storage** — Custom buttons survive reboots (saved in ESP32 NVS flash)
- **Mobile Responsive** — Designed for phones, works on any screen size
- **Serial Logging** — Every IR transmission logged for debugging

---

## Hardware Required

| Component | Details |
|-----------|---------|
| ESP32 Dev Module | Any ESP32 board (e.g. ESP32-WROOM-32) |
| IR LED | 940nm or 850nm infrared LED |
| Resistor | 100Ω–220Ω (for IR LED current limiting) |
| NPN Transistor (optional) | 2N2222 or similar — boosts range significantly |

### Wiring Diagram

```
Simple (short range ~2m):

    ESP32 GPIO 4 ──── 100Ω ──── IR LED Anode (+)
                                IR LED Cathode (-) ──── GND


With transistor (long range ~8m+):

    ESP32 GPIO 4 ──── 1kΩ ──── Base (2N2222)
                               Collector ──── IR LED Cathode (-)
                               Emitter  ──── GND
    3.3V ──── 22Ω ──── IR LED Anode (+) ──── Collector (above)
```

> **Tip**: Using an NPN transistor dramatically improves IR range. The transistor switches the LED with more current than the ESP32 GPIO can provide directly.

---

## Required Libraries

Install via **Arduino IDE → Sketch → Include Library → Manage Libraries**:

| Library | Author | Version |
|---------|--------|---------|
| **IRremote** | Armin Joachimsmeyer | **4.x** (v4.2.0 or later) |

All other libraries are built into the ESP32 Arduino Core:
- `WiFi.h`, `WebServer.h`, `ESPmDNS.h`, `Preferences.h`

---

## Step-by-Step Upload Instructions

### 1. Install Arduino IDE
Download from [arduino.cc](https://www.arduino.cc/en/software) (version 2.x recommended).

### 2. Install ESP32 Board Support
1. Open **File → Preferences**
2. In "Additional Board Manager URLs", add:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Open **Tools → Board → Boards Manager**
4. Search for **esp32** and install **"esp32 by Espressif Systems"**

### 3. Install IRremote Library
1. Open **Sketch → Include Library → Manage Libraries**
2. Search for **IRremote**
3. Install **"IRremote" by Armin Joachimsmeyer** version **4.x**

### 4. Configure Your WiFi
Open `config.h` and change these two lines to your WiFi credentials:

```cpp
#define WIFI_SSID       "YOUR_WIFI_NAME"    // ← Your WiFi network name
#define WIFI_PASS       "YOUR_WIFI_PASS"    // ← Your WiFi password
```

### 5. Open the Project
1. Open **File → Open** and navigate to `Universal_IR_Remote.ino`
2. Arduino IDE will open all project files in tabs

### 6. Configure Board Settings

| Setting | Value |
|---------|-------|
| Board | ESP32 Dev Module |
| Upload Speed | 921600 |
| CPU Frequency | 240MHz (WiFi/BT) |
| Flash Frequency | 80MHz |
| Flash Mode | QIO |
| Flash Size | 4MB (32Mb) |
| Partition Scheme | Default 4MB with spiffs |
| Port | (your ESP32 COM port) |

### 7. Upload
1. Connect ESP32 via USB
2. Hold **BOOT** button (if needed)
3. Click **Upload** (→ arrow button)
4. Release BOOT once upload starts
5. Wait for "Done uploading"

### 8. Find the IP Address
1. Open **Tools → Serial Monitor** (baud: 115200)
2. Press the ESP32 **RST** button
3. You'll see output like:
   ```
   [WiFi] Connected!
           IP   : 192.168.1.42
   [mDNS] http://irremote.local
   ```
4. Note the IP address

### 9. Use It
1. Make sure your phone/laptop is on the **same WiFi network**
2. Open a browser and go to:
   - `http://192.168.1.42` (use your actual IP from step 8)
   - **or** `http://irremote.local` (mDNS — works on most devices)
3. You'll see the remote control UI!

---

## Web Pages

### Page 1: Remote Control (`/`)
- Looks exactly like the physical 24-key remote
- Circular color buttons + rounded control buttons
- Brightness ☀+/☀-, OFF/ON, R/G/B/W
- 16 color buttons + FLASH/STROBE/FADE/SMOOTH
- Tap any button to send its NEC IR code

### Page 2: Custom Commands (`/custom`)
- **Quick Send** — Enter any HEX code and send it
- **Add Custom Button** — Name + HEX code → saved to flash
- **Saved Buttons** — View, send, edit, or delete your custom buttons
- Buttons persist across reboots

Navigate between pages using the link in the top-right corner.

---

## How to Edit HEX Codes

### Method 1: Custom Commands Page (Easiest)
Go to `/custom` in your browser and add any new button with any HEX code.

### Method 2: Edit Source Code
To change the 24 default remote buttons, edit `web_ui.h`:

```html
<button class="btn circle c-red" onclick="S(this,'00FF20DF')">R</button>
```

Change `00FF20DF` to your desired NEC code, then re-upload.

---

## 24-Key RGB Remote — NEC HEX Codes

| Button | HEX Code | Button | HEX Code |
|--------|----------|--------|----------|
| BRIGHT+ | `00FF00FF` | BRIGHT- | `00FF807F` |
| OFF | `00FF40BF` | ON | `00FFC03F` |
| RED | `00FF20DF` | GREEN | `00FFA05F` |
| BLUE | `00FF609F` | WHITE | `00FFE01F` |
| Orange | `00FF10EF` | Lt Green | `00FF906F` |
| Dk Blue | `00FF50AF` | FLASH | `00FFD02F` |
| Dk Orange | `00FF30CF` | Cyan | `00FFB04F` |
| Purple | `00FF708F` | STROBE | `00FFF00F` |
| Yellow | `00FF08F7` | Lt Blue | `00FF8877` |
| Pink | `00FF48B7` | FADE | `00FFC837` |
| Lt Yellow | `00FF28D7` | Sky Blue | `00FFA857` |
| Lt Pink | `00FF6897` | SMOOTH | `00FFE817` |

---

## Project File Structure

```
Universal_IR_Remote/
├── Universal_IR_Remote.ino   ← Main sketch (WiFi STA + loop)
├── config.h                  ← WiFi credentials, GPIO pin, settings
├── ir_sender.h / .cpp        ← IR transmitter (NEC 32-bit)
├── button_storage.h / .cpp   ← Custom button NVS persistence
├── web_ui.h                  ← Both HTML pages (remote + custom)
├── web_handler.h / .cpp      ← Web server + API routes
└── README.md                 ← This file
```

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| ESP32 won't connect to WiFi | Check SSID/password in `config.h`. Open Serial Monitor for errors. |
| Can't find the IP address | Check Serial Monitor output after reset. |
| `irremote.local` doesn't work | mDNS may not work on all devices. Use the IP address instead. |
| IR not working | Check wiring. Verify IR LED lights up using phone camera. |
| Short IR range | Add an NPN transistor circuit. Use 940nm IR LED. |
| Wrong codes sent | Capture your remote's codes with an IR receiver (see below). |
| Upload fails | Hold BOOT button during upload. Check correct COM port. |
| Library errors | Ensure IRremote v4.x is installed. Remove conflicting IR libraries. |

---

## Capturing Codes from Your Remote

If the default codes don't match your specific remote:

1. Wire an **IR receiver** (e.g. VS1838B) to the ESP32
2. Open the IRremote library example: **ReceiveDemo**
3. Press each button on your physical remote
4. Note the HEX codes from Serial Monitor
5. Update via the Custom Commands page or edit `web_ui.h`

---

## License

MIT License — free to use, modify, and distribute.

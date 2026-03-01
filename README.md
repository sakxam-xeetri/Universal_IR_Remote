# ESP32 Universal IR Remote Controller

A professional WiFi-based IR blaster that fully replaces a **24-key RGB LED strip remote**. The ESP32 creates its own WiFi access point, hosts a beautiful mobile-friendly web interface, and transmits **NEC 32-bit** IR codes through an IR LED.

---

## Features

- **WiFi Access Point** — No router needed; ESP32 hosts its own network
- **Captive Portal** — Web UI opens automatically when you connect
- **24-Key Remote Layout** — Buttons arranged exactly like the physical remote
- **Color-Coded Buttons** — Each button matches the real remote's color
- **HEX Code Display** — Every button shows its NEC code
- **Click Animation** — Visual pulse feedback on button press
- **Quick Send** — Enter any arbitrary HEX code and transmit it
- **Custom Buttons** — Create, edit, delete your own buttons
- **Persistent Storage** — Custom buttons survive reboots (saved in NVS flash)
- **Mobile Responsive** — Designed for phones; works on any screen size
- **Serial Monitor Logging** — Every IR transmission is logged for debugging

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

> **Tip**: Using an NPN transistor dramatically improves IR range. The transistor
> switches the LED with more current than the ESP32 GPIO can provide directly.

---

## Required Libraries

Install these via **Arduino IDE → Sketch → Include Library → Manage Libraries**:

| Library | Author | Version |
|---------|--------|---------|
| **IRremote** | Armin Joachimsmeyer | **4.x** (v4.2.0 or later) |

All other libraries are built into the ESP32 Arduino Core:
- `WiFi.h`
- `WebServer.h`
- `DNSServer.h`
- `Preferences.h`

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
3. Find **"IRremote" by Armin Joachimsmeyer**
4. Install version **4.x**

### 4. Open the Project
1. Open **File → Open** and navigate to `Universal_IR_Remote.ino`
2. Arduino IDE will open all project files in tabs

### 5. Configure Board Settings
Under the **Tools** menu, set:

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

### 6. Upload
1. Connect ESP32 via USB
2. Press and hold the **BOOT** button on the ESP32 (if needed)
3. Click **Upload** (→ arrow button)
4. Release BOOT once upload starts
5. Wait for "Done uploading"

### 7. Connect and Use
1. Open WiFi settings on your phone/laptop
2. Connect to **`RGB_Controller`** (open network, no password)
3. The captive portal should open automatically
4. If not, open a browser and go to **`http://192.168.4.1`**

---

## How to Use

### Remote Buttons
Tap any button on the web remote to send its IR code. You'll see:
- A flash animation on the button
- Status text confirming the code was sent
- Serial monitor output for debugging

### Quick Send
Enter any 8-character HEX code (e.g., `00FF20DF`) in the Quick Send box and tap SEND. Useful for testing codes from other remotes.

### Custom Buttons
1. Enter a **name** and **HEX code** in the Custom Buttons section
2. Tap **ADD** to save
3. The button appears in your custom list
4. Use **SEND** to transmit, **EDIT** to change, **DEL** to remove
5. Custom buttons are saved to flash and persist across reboots

---

## How to Edit HEX Codes

### Method 1: Web UI (Easiest)
Use the **Custom Buttons** section to add new codes without touching any code.

### Method 2: Edit `web_ui.h` (Remote Layout)
To change the default 24 remote buttons:
1. Open `web_ui.h`
2. Find the button HTML, e.g.:
   ```html
   <button class="btn c-red" onclick="S(this,'00FF20DF')">
   ```
3. Change `00FF20DF` to your desired code
4. Update the displayed hex: `<span class="hex">00FF20DF</span>`
5. Re-upload to ESP32

### Method 3: Edit `config.h` (Reference)
The `config.h` file contains a reference table of all 24 codes. Update the comments to match your remote if different.

---

## 24-Key RGB Remote — NEC HEX Code Reference

| Button | HEX Code | Button | HEX Code |
|--------|----------|--------|----------|
| BRIGHT+ | `00FF00FF` | BRIGHT- | `00FF807F` |
| OFF | `00FF40BF` | ON | `00FFC03F` |
| RED | `00FF20DF` | GREEN | `00FFA05F` |
| BLUE | `00FF609F` | WHITE | `00FFE01F` |
| ORANGE | `00FF10EF` | LT GREEN | `00FF906F` |
| DK BLUE | `00FF50AF` | FLASH | `00FFD02F` |
| DK ORANGE | `00FF30CF` | CYAN | `00FFB04F` |
| PURPLE | `00FF708F` | STROBE | `00FFF00F` |
| YELLOW | `00FF08F7` | LT BLUE | `00FF8877` |
| PINK | `00FF48B7` | FADE | `00FFC837` |
| LT YELLOW | `00FF28D7` | SKY BLUE | `00FFA857` |
| LT PINK | `00FF6897` | SMOOTH | `00FFE817` |

> **Note**: These are the standard NEC codes for the most common 24-key RGB LED
> strip remotes. If your specific remote uses different codes, use an IR receiver
> module to capture them, then update via the web UI or source code.

---

## Project File Structure

```
Universal_IR_Remote/
├── Universal_IR_Remote.ino   ← Main sketch (setup + loop)
├── config.h                  ← WiFi, pin, and code configuration
├── ir_sender.h               ← IR transmitter header
├── ir_sender.cpp             ← IR transmitter (NEC protocol)
├── button_storage.h          ← NVS storage header
├── button_storage.cpp        ← Custom button persistence
├── web_ui.h                  ← Embedded HTML/CSS/JS interface
├── web_handler.h             ← Web server header
├── web_handler.cpp           ← Web server + captive portal + API
└── README.md                 ← This file
```

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| Can't find WiFi network | Check serial monitor for AP startup errors. Try restarting ESP32. |
| Page won't load | Make sure you're connected to `RGB_Controller`. Go to `http://192.168.4.1` |
| IR not working | Check wiring. Verify IR LED lights up using phone camera (phones can see IR). |
| Short IR range | Add an NPN transistor circuit. Use 940nm IR LED. |
| Wrong codes sent | Use an IR receiver to capture your remote's actual codes and update them. |
| Upload fails | Hold BOOT button during upload. Check correct COM port. |
| Library errors | Ensure IRremote v4.x is installed. Remove any conflicting IR libraries. |

---

## Capturing Codes from Your Remote

If the default codes don't match your remote, capture them:

1. Wire an **IR receiver module** (e.g., VS1838B) to the ESP32
2. Open the IRremote library example: **ReceiveDemo**
3. Point your physical remote at the receiver and press each button
4. Note the HEX codes from the Serial Monitor
5. Update the codes in the web UI (Custom Buttons) or in `web_ui.h`

---

## Author

**Sakshyam Bastakoti**  
🌐 [sakshyambastakoti.com.np](https://sakshyambastakoti.com.np)

---

## License

MIT License — free to use, modify, and distribute.

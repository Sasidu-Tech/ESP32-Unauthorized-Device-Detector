🔐 ESP32 Unauthorized Device Detector

![ESP32 Unauthorized Device Detector](images/Esp32%20Unauthorized%20Device%20Detector%20(5).jpeg)

A lightweight LAN security monitoring system built using an ESP32.
The system scans the local Wi-Fi network, detects active devices, retrieves their IP and MAC addresses, and compares detected MAC addresses against an authorized device list.

This project is part of my ESP32 Cybersecurity Project Series.

---

🚀 Features

- 📡 Wi-Fi network connectivity
- 🔍 Local network device scanning
- 🌐 Active IP address detection using ICMP Ping
- 🆔 MAC address detection using ARP
- ✅ Authorized device identification
- ⚠️ Unknown device detection
- 🚫 ESP32 self-detection exclusion
- 📊 Network information monitoring
- 🔄 Automatic network scanning every 10 seconds
- 🖥️ Serial Monitor security dashboard

---

🧠 How It Works

The ESP32 connects to the same Wi-Fi network as the devices being monitored.

             Wi-Fi Network
                   │
                   ▼
              ┌─────────┐
              │  ESP32  │
              │ Scanner │
              └────┬────┘
                   │
            Scan Local Network
                   │
                   ▼
            ┌──────────────┐
            │ Active Host? │
            └──────┬───────┘
                   │
                  YES
                   │
                   ▼
             Get IP Address
                   │
                   ▼
              Get MAC Address
                   │
                   ▼
          Compare Authorized MAC
                   │
          ┌────────┴────────┐
          ▼                 ▼
     AUTHORIZED          UNKNOWN
          │                 │
          ▼                 ▼
       SAFE ✅          ALERT ⚠️

---

🔧 Hardware Requirements

Component| Quantity
ESP32 DevKit V1| 1
USB Cable| 1
Wi-Fi Network| 1
Computer| 1

No external sensors are required.

---

💻 Software Requirements

- Arduino IDE
- ESP32 Board Package
- ESPping Library
- Wi-Fi enabled network

Libraries

#include <WiFi.h>
#include <ESPping.h>
#include "esp_netif.h"
#include "esp_netif_net_stack.h"
#include "lwip/netif.h"
#include "lwip/etharp.h"
#include "lwip/ip4_addr.h"

---

⚙️ Configuration

Before uploading the code, configure your Wi-Fi credentials:

const char* SSID     = "YOUR_WIFI_NAME";
const char* PASSWORD = "YOUR_WIFI_PASSWORD";

Add the MAC address of an authorized device:

const char* AUTHORIZED_MAC = "F4:A5:9D:14:53:96";

«Note: Replace the example MAC address with the MAC address of your own authorized device.»

---

📡 Network Information

The ESP32 displays:

ESP32 IP
Gateway IP
Subnet Mask
ESP32 MAC Address

Example:

ESP32 IP     : 10.239.161.66
Gateway      : 10.239.161.159
Subnet Mask  : 255.255.255.0
ESP32 MAC    : 20:E7:C8:68:AF:5C

---

🔍 Example Scan Output

Authorized Device

----------------------------------------------
Device No  : 1
IP Address : 10.239.161.100
MAC        : F4:A5:9D:14:53:96
Status     : AUTHORIZED ✅
Security   : SAFE
----------------------------------------------

Unknown Device

----------------------------------------------
Device No  : 2
IP Address : 10.239.161.120
MAC        : XX:XX:XX:XX:XX:XX
Status     : UNKNOWN ⚠️
Security   : ALERT
----------------------------------------------

---

🔄 Automatic Monitoring

The system automatically performs a network scan every:

10 Seconds

This allows the ESP32 to continuously monitor the local network for newly active devices.

---

🛡️ Security Concept

This project demonstrates basic concepts used in network security:

- IP address discovery
- MAC address identification
- ARP
- ICMP Ping
- LAN host discovery
- Device authorization
- Unauthorized device detection
- Network monitoring

The project is designed as an educational cybersecurity prototype.

---

📁 Project Structure

ESP32-Unauthorized-Device-Detector/
│
├── ESP32_Unauthorized_Device_Detector.ino
├── README.md
├── images/
│      └── demo.png
├── demo
       └── ESP32 Unauthorized Device Detector.mp4
    

---

🎯 Learning Objectives

Through this project, I learned how to:

- Use ESP32 for network security applications
- Identify devices on a local network
- Understand IP and MAC addresses
- Understand the basic role of ARP
- Use ICMP Ping for host discovery
- Implement authorized-device checking
- Build a simple network monitoring system

---

🚀 Future Improvements

Possible future upgrades:

- 🔴 Buzzer and LED security alerts
- 🌐 Web-based security dashboard
- 📱 Mobile notifications
- 💾 Event logging
- 📋 Multiple authorized devices
- 📊 Network activity statistics
- 🤖 AI-based anomaly detection
- 🛡️ Integration with an ESP32 Mini IDS

---

👨‍💻 Author

Sasidu-Tech

ESP32 • Networking • Cybersecurity • IoT

---

📌 Project Series

This project is part of my ongoing ESP32 Cybersecurity Project Series.

01. ESP32 Wi-Fi Scanner                  ✅
02. ESP32 Network Status Monitor        ✅
03. ESP32 Unauthorized Device Detector  ✅
04. ESP32 Honeypot                      🔜
05. ESP32 Mini IDS                      🔜
06. Secure IoT Gateway                  🔜

---

⚠️ Disclaimer

This project is developed for educational and authorized network-monitoring purposes only.

Only scan networks and devices that you own or have explicit permission to monitor.

---

⭐ If you found this project interesting, consider giving the repository a star!

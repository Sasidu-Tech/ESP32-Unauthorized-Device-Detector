#include <WiFi.h>
#include <ESPping.h>

#include "esp_netif.h"
#include "esp_netif_net_stack.h"

#include "lwip/netif.h"
#include "lwip/etharp.h"
#include "lwip/ip4_addr.h"

// =====================================================
// Wi-Fi SETTINGS
// =====================================================

const char* SSID     = "Infinix HOT 40 Pro";
const char* PASSWORD = "12345678";

// =====================================================
// AUTHORIZED DEVICE
// =====================================================

// Your phone Wi-Fi MAC
const char* AUTHORIZED_MAC = "f4:a5:9d:14:53:96";

// =====================================================
// SETTINGS
// =====================================================

const unsigned long SCAN_INTERVAL = 10000;

IPAddress localIP;
IPAddress gatewayIP;
IPAddress subnetMask;

unsigned long lastScan = 0;

// =====================================================
// FUNCTION: MAC FORMAT
// =====================================================

String formatMAC(const uint8_t* mac)
{
  char buffer[18];

  sprintf(
    buffer,
    "%02X:%02X:%02X:%02X:%02X:%02X",
    mac[0],
    mac[1],
    mac[2],
    mac[3],
    mac[4],
    mac[5]
  );

  return String(buffer);
}

// =====================================================
// FUNCTION: CHECK AUTHORIZED MAC
// =====================================================

bool isAuthorized(String mac)
{
  mac.toUpperCase();

  String authorized = String(AUTHORIZED_MAC);
  authorized.toUpperCase();

  return mac == authorized;
}

// =====================================================
// FUNCTION: GET MAC FROM ARP TABLE
// =====================================================

String getMACFromARP(IPAddress targetIP)
{
  // Get Wi-Fi STA network interface
  esp_netif_t* espNetif =
      esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");

  if (espNetif == NULL)
  {
    return "";
  }

  // Get underlying lwIP netif
  struct netif* lwipNetif =
      (struct netif*)esp_netif_get_netif_impl(espNetif);

  if (lwipNetif == NULL)
  {
    return "";
  }

  // Convert target IP to lwIP format
  ip4_addr_t wantedIP;

  IP4_ADDR(
    &wantedIP,
    targetIP[0],
    targetIP[1],
    targetIP[2],
    targetIP[3]
  );

  // Search ARP table
  for (uint8_t i = 0; i < 10; i++)
  {
    ip4_addr_t* arpIP = NULL;
    struct netif* arpNetif = NULL;
    struct eth_addr* arpMAC = NULL;

    u8_t valid = etharp_get_entry(
      i,
      &arpIP,
      &arpNetif,
      &arpMAC
    );

    if (!valid)
    {
      continue;
    }

    if (arpIP == NULL || arpMAC == NULL)
    {
      continue;
    }

    // Check IP match
    if (ip4_addr_cmp(arpIP, &wantedIP))
    {
      return formatMAC(arpMAC->addr);
    }
  }

  return "";
}

// =====================================================
// FUNCTION: PRINT DEVICE
// =====================================================

void printDevice(
  int deviceNumber,
  IPAddress ip,
  String mac
)
{
  Serial.println("----------------------------------------------");

  Serial.print("Device No  : ");
  Serial.println(deviceNumber);

  Serial.print("IP Address : ");
  Serial.println(ip);

  Serial.print("MAC        : ");

  if (mac.length() > 0)
  {
    Serial.println(mac);
  }
  else
  {
    Serial.println("NOT FOUND");
  }

  if (mac.length() > 0)
  {
    if (isAuthorized(mac))
    {
      Serial.println("Status     : AUTHORIZED ✅");
      Serial.println("Security   : SAFE");
    }
    else
    {
      Serial.println("Status     : UNKNOWN ⚠️");
      Serial.println("Security   : ALERT");
    }
  }
  else
  {
    Serial.println("Status     : ACTIVE");
    Serial.println("Security   : MAC NOT FOUND");
  }

  Serial.println("----------------------------------------------");
}

// =====================================================
// FUNCTION: NETWORK SCAN
// =====================================================

void scanNetwork()
{
  Serial.println();
  Serial.println("==============================================");
  Serial.println("     ESP32 UNAUTHORIZED DEVICE DETECTOR");
  Serial.println("==============================================");

  localIP = WiFi.localIP();
  gatewayIP = WiFi.gatewayIP();
  subnetMask = WiFi.subnetMask();

  Serial.print("ESP32 IP     : ");
  Serial.println(localIP);

  Serial.print("Gateway      : ");
  Serial.println(gatewayIP);

  Serial.print("Subnet Mask  : ");
  Serial.println(subnetMask);

  Serial.println();
  Serial.println("Starting Network Scan...");
  Serial.println();

  int deviceCount = 0;

  // ---------------------------------------------------
  // Scan .1 - .254
  // ---------------------------------------------------

  for (int i = 1; i <= 254; i++)
  {
    IPAddress targetIP(
      localIP[0],
      localIP[1],
      localIP[2],
      i
    );

    // -----------------------------------------------
    // Skip ESP32 itself
    // -----------------------------------------------

    if (targetIP == localIP)
    {
      Serial.print("Skipping ESP32 IP: ");
      Serial.println(targetIP);

      continue;
    }

    // -----------------------------------------------
    // Ping host
    // -----------------------------------------------

    bool active = Ping.ping(
      targetIP,
      1
    );

    if (active)
    {
      deviceCount++;

      Serial.println();
      Serial.print("Active Host Found: ");
      Serial.println(targetIP);

      // Give ARP a moment
      delay(50);

      // ---------------------------------------------
      // Get MAC
      // ---------------------------------------------

      String mac = getMACFromARP(targetIP);

      // ---------------------------------------------
      // Print device
      // ---------------------------------------------

      printDevice(
        deviceCount,
        targetIP,
        mac
      );
    }
  }

  // =================================================
  // SCAN SUMMARY
  // =================================================

  Serial.println();
  Serial.println("==============================================");
  Serial.println("              SCAN COMPLETE");
  Serial.println("==============================================");

  Serial.print("Active Devices Found : ");
  Serial.println(deviceCount);

  Serial.println("==============================================");
  Serial.println();
}

// =====================================================
// SETUP
// =====================================================

void setup()
{
  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("==============================================");
  Serial.println(" ESP32 NETWORK SECURITY MONITOR");
  Serial.println(" Project 03 - Unauthorized Device Detector");
  Serial.println("==============================================");

  // ---------------------------------------------------
  // Wi-Fi mode
  // ---------------------------------------------------

  WiFi.mode(WIFI_STA);

  WiFi.begin(
    SSID,
    PASSWORD
  );

  Serial.println();
  Serial.print("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);

    Serial.print(".");
  }

  Serial.println();
  Serial.println();
  Serial.println("Wi-Fi Connected ✅");

  // ---------------------------------------------------
  // Network information
  // ---------------------------------------------------

  localIP = WiFi.localIP();
  gatewayIP = WiFi.gatewayIP();
  subnetMask = WiFi.subnetMask();

  Serial.println();
  Serial.println("----------------------------------------------");

  Serial.print("ESP32 IP     : ");
  Serial.println(localIP);

  Serial.print("Gateway      : ");
  Serial.println(gatewayIP);

  Serial.print("Subnet Mask  : ");
  Serial.println(subnetMask);

  Serial.print("ESP32 MAC    : ");
  Serial.println(WiFi.macAddress());

  Serial.println("----------------------------------------------");

  Serial.println();
  Serial.println("Authorized Device:");
  Serial.println(AUTHORIZED_MAC);

  Serial.println();
  Serial.println("Network Scanner Ready!");
  Serial.println("First scan starting...");
  Serial.println();

  delay(1000);

  // First scan
  scanNetwork();

  lastScan = millis();
}

// =====================================================
// LOOP
// =====================================================

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.println("Wi-Fi disconnected!");

    WiFi.reconnect();

    delay(3000);

    return;
  }

  // Scan every 10 seconds
  if (millis() - lastScan >= SCAN_INTERVAL)
  {
    lastScan = millis();

    scanNetwork();
  }
}

#include <Arduino.h>
#include <WiFi.h>

const char* MODEM_SSID     = "YOUR_MODEM_SSID";
const char* MODEM_PASSWORD = "YOUR_MODEM_PASSWORD";

const char* AP_SSID     = "Modem_Repeater";
const char* AP_PASSWORD = "YOUR_NEW_PASSWORD";

IPAddress AP_IP(192, 168, 50, 1);
IPAddress AP_GATEWAY(192, 168, 50, 1);
IPAddress AP_SUBNET(255, 255, 255, 0);
IPAddress AP_LEASE_START(192, 168, 50, 2);
IPAddress AP_DNS(8, 8, 8, 8);


void onEvent(arduino_event_id_t event, arduino_event_info_t info)
{
    switch (event)
    {
        case ARDUINO_EVENT_WIFI_AP_START:
            Serial.println("AP Started");
            break;

        case ARDUINO_EVENT_WIFI_AP_STACONNECTED:
            Serial.println(">>> PHONE CONNECTED TO AP");
            break;

        case ARDUINO_EVENT_WIFI_AP_STADISCONNECTED:
            Serial.println(">>> PHONE DISCONNECTED FROM AP");
            break;

        case ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED:

            Serial.print(">>> PHONE GOT IP: ");

            Serial.println(
                IPAddress(info.wifi_ap_staipassigned.ip.addr)
            );

            break;

        case ARDUINO_EVENT_WIFI_STA_CONNECTED:
            Serial.println("STA connected to modem");
            break;

        case ARDUINO_EVENT_WIFI_STA_GOT_IP:

            Serial.println("STA got IP");

            Serial.print("STA IP: ");
            Serial.println(WiFi.STA.localIP());

            Serial.print("Gateway: ");
            Serial.println(WiFi.STA.gatewayIP());

            Serial.print("DNS: ");
            Serial.println(WiFi.STA.dnsIP());

            // -------------------------
            // Enable NAT
            // -------------------------
            WiFi.AP.enableNAPT(true);

            Serial.println("NAPT enabled!");

            break;

        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("STA disconnected");
            break;

        default:
            break;
    }
}


void setup()
{
    Serial.begin(115200);
    delay(1000);

    Serial.println();
    Serial.println("=================================");
    Serial.println("ESP32 AP TEST");
    Serial.println("=================================");

    Network.onEvent(onEvent);

    // -------------------------
    // Start AP
    // -------------------------

    WiFi.AP.begin();

    WiFi.AP.config(
        AP_IP,
        AP_GATEWAY,
        AP_SUBNET,
        AP_LEASE_START,
        AP_DNS
    );

    bool apOK = WiFi.AP.create(
        AP_SSID,
        AP_PASSWORD
    );

    if (!apOK)
    {
        Serial.println("ERROR: AP creation failed!");
        return;
    }

    Serial.println("AP created successfully.");

    Serial.print("SSID: ");
    Serial.println(AP_SSID);

    Serial.print("Password: ");
    Serial.println(AP_PASSWORD);

    Serial.print("AP IP: ");
    Serial.println(WiFi.AP.localIP());


    // -------------------------
    // Connect to modem
    // -------------------------

    Serial.println();
    Serial.println("Connecting to modem...");

    WiFi.begin(
        MODEM_SSID,
        MODEM_PASSWORD
    );
}


void loop()
{
    delay(1000);
}
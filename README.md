# ESP32 Wi-Fi Repeater

A simple Wi-Fi repeater / NAT router example for ESP32 using the Arduino framework (version 3 of course).

This project demonstrates how an ESP32 can simultaneously:

1. Connect to an existing Wi-Fi network as a **Station (STA)**.
2. Create its own Wi-Fi network as an **Access Point (AP)**.
3. Enable **NAPT (Network Address and Port Translation)** so devices connected to the ESP32 AP can access the Internet through the upstream Wi-Fi network.

## Architecture

```text
                    Wi-Fi
              ┌────────────────┐
              │     Router     │
              └───────┬────────┘
                      │
                      │ STA
                      ▼
               ┌──────────────┐
               │    ESP32     │
               │              │
               │  STA + AP    │
               │     +        │
               │    NAPT      │
               └───────┬──────┘
                       │
                       │ AP
                       ▼
               ┌──────────────┐
               │ Phone/Laptop │
               └──────────────┘
```

The ESP32 acts as a small NAT router between the upstream Wi-Fi network and its own Access Point.

## Features

- Wi-Fi Station mode
- Wi-Fi Access Point mode
- STA + AP operation
- DHCP for connected clients
- NAPT/NAT for Internet sharing
- Serial Monitor status information
- Simple configuration
- Arduino framework compatible

## Requirements

### Hardware

- ESP32 development board
- Existing 2.4 GHz Wi-Fi router
- Phone, laptop, or another Wi-Fi device for testing

### Software

- Arduino IDE or PlatformIO
- ESP32 Arduino Core

The ESP32 must support 2.4 GHz Wi-Fi. Standard ESP32 boards such as ESP32-WROOM-32 can be used.

## Configuration

Before uploading the firmware, change the following values:

```cpp
const char* MODEM_SSID     = "YOUR_MODEM_SSID";
const char* MODEM_PASSWORD = "YOUR_MODEM_PASSWORD";

const char* AP_SSID     = "ESP32-Repeater";
const char* AP_PASSWORD = "YOUR_AP_PASSWORD";
```

`MODEM_SSID` and `MODEM_PASSWORD` are the credentials of the existing Wi-Fi network.

`AP_SSID` and `AP_PASSWORD` define the Wi-Fi network created by the ESP32.

The AP network uses the following configuration:

```text
Network: 192.168.50.0/24
ESP32:   192.168.50.1
```

## How It Works

### 1. Start the Access Point

The ESP32 creates a new Wi-Fi network:

```cpp
WiFi.AP.begin();

WiFi.AP.config(
    AP_IP,
    AP_GATEWAY,
    AP_SUBNET,
    AP_LEASE_START,
    AP_DNS
);

WiFi.AP.create(
    AP_SSID,
    AP_PASSWORD
);
```

Devices can then connect to the ESP32 using the configured SSID and password.

### 2. Connect to the upstream router

The ESP32 connects to the existing Wi-Fi network using Station mode:

```cpp
WiFi.begin(
    MODEM_SSID,
    MODEM_PASSWORD
);
```

After successfully connecting, the ESP32 receives an IP address from the router.

### 3. Enable NAPT

Once the ESP32 has received an IP address from the upstream router, NAPT is enabled:

```cpp
WiFi.AP.enableNAPT(true);
```

This allows devices connected to the ESP32 Access Point to access the upstream network and the Internet.

## Example Serial Output

A successful startup should look similar to:

```text
=================================
ESP32 AP TEST
=================================

AP Started
AP created successfully.

SSID: ESP32-Repeater
Password: ********

AP IP: 192.168.50.1

Connecting to modem...

STA connected to modem
STA got IP

STA IP: 192.168.1.35
Gateway: 192.168.1.1
DNS: 192.168.1.1

NAPT enabled!

>>> PHONE CONNECTED TO AP
>>> PHONE GOT IP: 192.168.50.2
```

The exact IP addresses depend on the configuration of the upstream router.

## Troubleshooting

### ESP32 cannot connect to the router

Check:

- SSID and password
- Router availability
- Wi-Fi signal strength
- Router security configuration
- Make sure the network is available on **2.4 GHz**

Standard ESP32 devices do not support 5 GHz Wi-Fi.

If the router uses the same SSID for both 2.4 GHz and 5 GHz networks, temporarily separating the SSIDs can make troubleshooting easier.

### Phone can connect to ESP32 but has no Internet

Check the Serial Monitor.

The ESP32 must first successfully connect to the upstream router:

```text
STA got IP
```

and NAPT must be enabled:

```text
NAPT enabled!
```

The phone should also receive an address from the ESP32 network, for example:

```text
Phone IP: 192.168.50.2
Gateway: 192.168.50.1
```

If the ESP32 AP works but the STA connection is repeatedly reported as:

```text
STA disconnected
```

the problem is on the ESP32-to-router link rather than the ESP32 AP.

### Weak signal

For best results, place the ESP32 somewhere between the router and the client:

```text
Router
   │
   │ Strong Wi-Fi
   ▼
 ESP32
   │
   │ Strong Wi-Fi
   ▼
Client
```

Placing the ESP32 in an area where the original router signal is already extremely weak will not provide a reliable repeater.

## Important Limitations

This project uses a single Wi-Fi radio for both the upstream and downstream connections.

Therefore, the achievable throughput of the repeater will generally be lower than the theoretical Wi-Fi data rate of the ESP32.

The actual performance depends on:

- ESP32 model
- Wi-Fi signal strength
- Distance
- Antenna
- Router configuration
- Wi-Fi channel congestion
- Number of connected clients
- TCP/IP and NAT overhead
- Interference from other 2.4 GHz devices

The theoretical Wi-Fi rate should therefore not be interpreted as the expected Internet throughput through the repeater.

## Performance Testing

For meaningful performance measurements, compare:

### Direct connection

```text
Router ─────────────► Client
```

with:

### Repeater connection

```text
Router ─────► ESP32 ─────► Client
```

Tools such as `iperf3` can be used to measure network throughput independently of the Internet connection.

This makes it possible to distinguish between:

- Router/Internet bandwidth limitations
- Wi-Fi link limitations
- ESP32 processing/NAT limitations

## Project Status

This project is intended primarily as an **educational and experimental example** for understanding:

- ESP32 Wi-Fi networking
- Station + Access Point operation
- NAT/NAPT
- DHCP
- Network routing
- Wi-Fi repeater architecture

It is not intended to replace a dedicated commercial Wi-Fi range extender or mesh access point.

## References

- [Arduino ESP32](https://github.com/espressif/arduino-esp32)
- [ESP-IDF](https://github.com/espressif/esp-idf)
- [ESP32 Wi-Fi documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-guides/wifi.html)

# IOT: AWS Hexagon

## Description

AWS re:Invent ESP32 Lanyard also known as AWS Hexagon is the development board created by Tekt Industries that mounts on-board the official WROOM32 module. The Hexagon device features built-in USB-to-Serial converter, automatic bootloader reset, Lithium Ion/Polymer charger, 2 Groove connectors, 2 user buttons, 6 APA102-2020 Super LED, and all the GPIO brought.

AWS Hexagon v1 contains a dual-core ESP32 chip, 4 MB of SPI Flash, tuned antenna, and The ESP32 microcontroller has both WiFi and Bluetooth Classic/LE support.

## Requirements

In order to program the Hexagon you need [Mongoose OS](https://mongoose-os.com/docs/quickstart/setup.md) binary.

The code will:

* Register the device using DNS-SD under the name `hexagon.local`
* Host a webserver that sensors the Temperature and Humidity using a
  DHT22 sensor.

## Build and Flash

Connect the device to your USB port and execute:

```bash
mos build --platform esp32
mos flash
```

# SI4735 Based RX ESP8266

FM/AM/SSB radio receiver project based on the ESP8266 and the SI4735/SI4732 chip, developed with PlatformIO and the PU2CLR SI4735 library.

This repository contains two main Arduino/ESP8266 sketches:

- `AM_FM_SERIAL_MONITOR.ino`: a simplified, minimal version for testing and verifying the receiver through the serial monitor.
- `src/OLED_ALL_IN_ONE.ino`: a complete radio with an OLED display, rotary encoder, EEPROM management, band switching, SSB/AM/FM, and menu commands.

## Project goals

The goal is to provide an ESP8266-based radio receiver capable of:

- receiving commercial FM broadcasts;
- tuning AM (LW/MW/SW);
- handling SSB (LSB/USB);
- controlling volume, band filters, AGC, BFO, frequency steps, and station seeking;
- saving the receiver state to EEPROM and restoring the configuration at startup;
- displaying frequency, RSSI level, and demodulator status on an OLED display.

## Main features

### Radio

- support for `SI4735` and `SI4732` chips;
- FM, AM, and SSB reception;
- automatic station seeking;
- multiple band support: FM, MW, SW, and LW;
- dynamic configuration of AM/SSB/FM filters;
- BFO control for SSB reception;
- volume and AGC parameter control.

### Hardware

- microcontroller: ESP8266 (ESP12E/ESP12F recommended);
- radio front end: SI4735/SI4732;
- display: SSD1306 OLED 128x32;
- rotary encoder with push button;
- standard I2C connections for the SI4735 chip and display.

### Software

- PlatformIO as the build environment;
- included libraries:
  - `PU2CLR SI4735` for receiver control;
  - `Adafruit SSD1306` for the display;
  - `Rotary` for the encoder;
  - `EEPROM` for local persistence.

## Wiring diagram

### KiCad hardware project

The `hardware/` folder contains the KiCad project `schematic_esp8266_basic.*`, which provides a schematic reference for the basic ESP8266 + SI4735/SI4732 + OLED + encoder assembly.

The design is consistent with the firmware in this repository:

- the radio module is connected over the I2C bus;
- `GPIO2` is used as the SI4735/SI4732 chip reset line;
- `GPIO4` and `GPIO5` are used as `SDA` and `SCL` for the I2C bus;
- `GPIO12`, `GPIO13`, and `GPIO14` are assigned to the rotary encoder and its push button;
- the layout follows the standard PU2CLR project reference design.

### ESP8266 + SI4735/SI4732

| SI4735/SI4732 chip | Function | ESP8266 |
|---|---|---|
| pin 15 / pin 9 | RESET | GPIO2 |
| pin 18 / pin 12 | SDIO | GPIO4 (SDA) |
| pin 17 / pin 11 | SCLK | GPIO5 (SCL) |

### Encoder + OLED

| Component | GPIO |
|---|---|
| Encoder A | GPIO13 |
| Encoder B | GPIO14 |
| Encoder push button | GPIO12 |
| OLED SDA | GPIO4 |
| OLED SCL | GPIO5 |

> Note: The KiCad schematics are an architectural reference for the project. The software configuration is the source of truth for the pins defined in the firmware and should be checked against the specifications of the radio module in use.

## Project files

### `AM_FM_SERIAL_MONITOR.ino`

A minimal sketch for quickly verifying that the SI4735 module works through the serial interface. It is useful for initial testing and does not require a display or buttons.

Main functions:

- radio module configuration;
- FM/AM tuning;
- band and filter bandwidth switching;
- up/down seeking;
- volume control;
- displaying the current signal through the serial monitor.

### `src/OLED_ALL_IN_ONE.ino`

The complete, advanced version of the receiver. It includes:

- an OLED display control menu;
- band management;
- saving the state to EEPROM;
- AGC/attenuation settings;
- frequency step selection;
- AM/SSB/FM filters;
- automatic station seeking;
- SSB reception with BFO.

## Requirements

- PlatformIO Core;
- ESP8266 support;
- the `PU2CLR SI4735` library, automatically downloaded through `lib_deps`;
- Adafruit SSD1306 for the OLED;
- an ESP8266 board with a correctly connected SI4735/SI4732 module.

## Initial setup

1. Open the project folder in PlatformIO.
2. Check that the board is configured as `esp12e` in `platformio.ini`.
3. Download the required libraries.
4. Connect the pins according to the wiring diagram above.
5. Build and upload the firmware to the ESP8266.

## Quick start

### Serial sketch

- `F`: tune to FM
- `A`: tune to AM/MW
- `L`: LW mode
- `1`: SW band
- `U` / `D`: increase/decrease frequency
- `S` / `s`: seek up/down
- `+` / `-`: volume up/down
- `B`: change filter bandwidth
- `0`: show status
- `?`: display help

### OLED radio

The complete version uses the encoder and push button to:

- select the band;
- change the frequency;
- open the control menu;
- adjust volume, step, AGC, BFO, filter, and soft mute;
- seek stations automatically.

## Repository structure

```text
.
├── AM_FM_SERIAL_MONITOR.ino
├── README.md
├── README.en.md
├── platformio.ini
├── include/
├── lib/
├── src/
│   └── OLED_ALL_IN_ONE.ino
├── hardware/
├── test/
└── .pio/
```

## KiCad hardware verification

The `hardware/schematic_esp8266_basic.kicad_sch` file is an "imported schematic" and does not represent a fully customized PCB design for the complete radio interface. However, the imported symbols confirm the essential connections used by the firmware:

- `ESP8266` symbol: `GPIO2`, `GPIO4`, `GPIO5`, `GPIO12`, `GPIO13`, and `GPIO14` are present as standard module pins;
- `SI4735-D60-GU` symbol: the `SCLK` and `SDIO` pins are those specified in the chip reference design;
- the software configuration therefore matches the project's basic hardware pin mapping.

From a design perspective, the file is valid as a conceptual schematic, but the complete system should be checked before producing a final PCB:

- power supply decoupling and filtering for the SI4735 module;
- a clean shared ground between the ESP8266, radio, and display;
- the antenna/RF path and input matching, if using an external antenna or the module's tuning network;
- EMI/EMC considerations for the I2C bus and RF area.

## Important notes

- For reliable operation, check the I2C connections and the voltage levels of the radio module.
- `src/OLED_ALL_IN_ONE.ino` includes EEPROM management. To reset saved data, press the encoder button during startup.
- The SI4735 radio is very sensitive to connections and PCB layout; a good ground and clean wiring are essential.
- Band and filter bandwidth parameters can be adjusted to suit your reception area and personal requirements.

## Troubleshooting

- No signal received: check the power supply, reset, and I2C connections.
- Frequency does not change: check the reset pin and device configuration.
- Blank OLED: check the 0x3C address and that the display is powered correctly.
- EEPROM not initialized: perform a manual reset by holding the encoder button during startup.

## Acknowledgments

- Library and documentation: `PU2CLR SI4735`;
- hardware support and examples that inspired this project.

## License

This repository is distributed for educational and experimental use. Check the licenses of dependent libraries before commercial use or public distribution.

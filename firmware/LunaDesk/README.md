# Luna Desk firmware

This sketch adapts Waveshare's `10_lvgl_arduino_v9` display/touch example, adding serial statistics, a dashboard and clock, a focus timer, navigation and character animation. It compiled in the development setup; hardware operation remains untested.

## Install dependencies

Development used Arduino IDE 2.3.10, Espressif Arduino-ESP32 3.3.10, ArduinoJson 7.4.2, and Waveshare's bundled libraries including LVGL 9.2.2.

The manufacturer snapshot available when these instructions were prepared is [`c7dd63c70258da7e3b6dae0c621f30ca62b1e1c9`](https://github.com/waveshareteam/ESP32-S3-Touch-LCD-3.5B/tree/c7dd63c70258da7e3b6dae0c621f30ca62b1e1c9). This provides a fixed download reference; the earlier development installation was not recorded at an exact upstream commit.

1. Download that repository snapshot, or clone it and check out the commit above.
2. Use a dedicated Arduino sketchbook, for example `Documents/Arduino-LunaDesk`, and restart the IDE after changing it.
3. Copy the manufacturer's supplied folders from `examples/arduino/libraries/` into that sketchbook's `libraries/` directory. Retain the bundled `lvgl`, `GFX_Library_for_Arduino`, `TCA9554`, and `esp_lcd_touch_axs15231b` rather than substituting generic display drivers.
4. Copy this project's `lv_conf.h` into the sketchbook's `libraries/` directory, beside the `lvgl` folder. Keep the sketch's copy too. It enables 16-bit color and the fonts used here. Avoid duplicate LVGL versions in the sketchbook.
5. Install ArduinoJson 7.4.2 through Library Manager and esp32 by Espressif Systems 3.3.10 through Boards Manager.
6. Open `LunaDesk.ino`, keeping `luna_character.h`, `luna_sprites.h` and `lv_conf.h` alongside it.

## Arduino board options

| Option | Setting |
| --- | --- |
| Board | ESP32S3 Dev Module |
| Flash Size | 16 MB |
| Flash Mode | QIO |
| PSRAM | OPI PSRAM |
| USB Mode | Hardware CDC and JTAG |
| USB CDC On Boot | Enabled |
| Partition Scheme | 16 MB flash, 3 MB application / approximately 9 MB FATFS |
| Port | Actual board USB port when available |

Verify, then upload when the board is available. Animation data are compiled into the sketch; no SD assets or filesystem image need uploading. Display and touch use rotation 1 and logical landscape dimensions of 480 × 320. Their alignment needs a physical test.

## Run

Close Serial Monitor and run the [PC sender](../../desktop/main.py) using the correct port at 115200 baud. Valid newline-delimited stats messages update the screen. CPU below 30% selects idle; 30% and above selects working. After five seconds without a valid message, the firmware clears readings and shows disconnected, using idle character frames. Before the first packet it shows waiting for the PC.

The navigation button opens the 25-minute focus timer. Start/Pause controls it and Reset restores 25:00. It stops at 00:00; celebration and sound are not implemented. The timer runs on the device without a continuing PC connection, but resets on device restart.

If the display stays blank, check the supplied libraries, OPI PSRAM setting, power and startup serial messages. If the sender cannot open the port, close competing serial applications. Restart the sender after connection failure. A compile or software loopback does not establish display or hardware USB operation.

The adjacent `README_ZH.md` is retained from the original manufacturer example, not a translation of Luna Desk's features. See the [root README](../../README.md) and [third-party notices](../../THIRD_PARTY_NOTICES.md).

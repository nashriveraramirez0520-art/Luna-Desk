# Third-party notices

The firmware adapts display and touch initialization from Waveshare's `10_lvgl_arduino_v9` example for the ESP32-S3-Touch-LCD-3.5B:

- Source: https://github.com/waveshareteam/ESP32-S3-Touch-LCD-3.5B
- Dependency/documentation reference snapshot: `c7dd63c70258da7e3b6dae0c621f30ca62b1e1c9`.
- The manufacturer's repository states that first-party code is Apache-2.0. A copy is in [licenses/Waveshare-Apache-2.0.txt](licenses/Waveshare-Apache-2.0.txt). Bundled third-party components retain their original licenses.
- Luna Desk changes add the statistics protocol, dashboard, clock, timer, navigation and character animations; these are not part of the unmodified vendor example.

Existing manufacturer comments and the original Chinese example README are retained. `lv_conf.h` originates from the LVGL/vendor setup. LVGL, Arduino_GFX, TCA9554, the AXS15231B touch driver, ArduinoJson and Espressif's platform are external dependencies. Preserve their supplied license and attribution files when downloading or redistributing them; this repository does not relicense those components.

Manufacturer hardware geometry was used as a CAD reference. The included assembly has only the three designed solids; screenshots also show the reference hardware.

Luna's original video was generated for the author's custom ChatGPT Work mode companion. GIFs and RGB565 arrays are cropped/resized excerpts, not hand-drawn artwork. Extraction parameters and the source checksum are in `assets/character/source.json`. These notices do not assert a blanket open-source license over the generated artwork or all project materials.

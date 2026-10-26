# Luna Desk

I wanted to make something for my desk that could show what my PC is doing, but also have a bit more personality than just a screen with numbers. That is where Luna Desk started: a small touchscreen with an animated character, live PC stats, a clock, and a focus timer.

This is my project for **Hack Club Half Life, Week 4: Displays**. Right now I have chosen the main board and worked out the first features and a starting budget. The firmware, Windows app, animations, and enclosure still need to be made, so this is the plan for the project, not a finished device.

## The idea

Luna will be a little robot character with purple hair, white armor, and a pink heart. I want her to react to what is happening on the computer instead of playing the same animation all the time.

When the PC is quiet, she could rest or look around. When it is working harder, she could switch to a running or working animation. Finishing a study session could trigger a small celebration. If the computer stops sending data, the display should show that it is disconnected instead of leaving old numbers on the screen.

The character will use prepared 2D animation frames with a rendered 3D look. I am not planning to render a full 3D model on the ESP32 or add a chatbot in the first version.

## What I want it to do

I am keeping the first version to three main pages:

| Page | What I want to show |
| --- | --- |
| PC monitor | CPU and RAM usage, connection status, and Luna reacting beside the readings |
| Focus timer | A timer I can start, pause, and reset, with a different animation when a session ends |
| Character and clock | A larger view of Luna with the time and a few touch interactions |

I would like the interface to feel like one device rather than several unrelated examples. Touch controls will switch between the pages, so the first version does not need an extra encoder or buttons.

GPU readings and temperatures would be nice later, but they depend on what information I can reliably get from Windows. I want to get CPU, RAM, and the connection working first.

## The hardware

The selected board is the **Waveshare ESP32-S3-Touch-LCD-3.5B**, SKU **31137**, without a case or camera.

It combines a 3.5-inch IPS capacitive touchscreen with the ESP32-S3, so I do not need a separate controller board and screen wiring. It has a **320 × 480** display, **8 MB PSRAM**, **16 MB Flash**, and a microSD slot.

The extra storage could be useful for the animations. I still need to decide how to store and load the frames, and test the animation speed with the rest of the interface running.

- [Board and purchasing page](https://www.waveshare.com/esp32-s3-touch-lcd-3.5b.htm)
- [Manufacturer documentation and examples](https://docs.waveshare.com/ESP32-S3-Touch-LCD-3.5B)

## Connecting it to the PC

The plan is to have a small Windows program read the PC statistics and send them to the ESP32 over USB. The device will draw the interface and choose the character animation from those readings.

For the first version, USB will provide power and the data connection. A battery is not part of the current plan. I still need to implement and test the USB communication, including what happens when the cable is unplugged or the Windows program closes.

## The enclosure

I will design the enclosure in Onshape around the actual board dimensions. I want a case that sits comfortably on the desk, keeps the touchscreen accessible, and can be opened again if something needs fixing.

The mounting points, USB opening, microSD access, and viewing angle still need to be worked out. Once the design is ready, I will add the editable Onshape link, STEP exports, and individual STL files here.

## Starting BOM and budget

These are the prices I checked on **October 26, 2026**, not purchases already made. I am planning around a **$65 budget target**, subject to the program's tier requirements and approval.

| Item | Quantity | Quoted cost (USD) | Notes |
| --- | --- | --- | --- |
| Waveshare ESP32-S3-Touch-LCD-3.5B | 1 | $25.99 | Board with touchscreen; no case or camera |
| Waveshare Registered Post Air Mail | 1 shipment | $11.20 | Quoted delivery estimate: 12–28 days |
| microSD card | To confirm | Not quoted | Check whether I already have a suitable card and whether it is needed for the final asset format |
| USB data cable | 1 if needed | Not quoted | Check available cables before buying another |
| Printed enclosure and mounting hardware | To confirm | Not quoted | Quantities and cost depend on the enclosure design |
| Taxes or other checkout charges | To confirm | Not quoted | Check the final order summary |

**Board plus quoted shipping: $37.19.** This leaves **$27.81** within the $65 target for the remaining items and any additional charges. It is not the final project total.

## What comes next

- Get the manufacturer's display and touch example running when the board is available.
- Work out the screen layout and prepare the first four character states: idle, working, celebration, and disconnected.
- Build the Windows stats sender and define the USB messages.
- Implement the PC page, clock, and focus timer.
- Design the enclosure and check access to the connectors.
- Test everything together and update the BOM, build instructions, and journal as the project develops.

While waiting for the hardware, I can work on the Windows app, animation assets, interface mockups, and enclosure planning. Anything tested only on the computer will be documented separately from tests on the actual board.

## References

[Turing Smart Screen Python](https://github.com/mathoudebine/turing-smart-screen-python) was a reference for the idea of displaying PC information on a small desktop screen. It supports specific USB displays; I am not assuming its display driver will work with this Waveshare board.

The Waveshare documentation will be the starting point for the board setup. Any libraries or assets used as development continues will be credited here.

Project by **Nash Andrés Rivera Ramírez**.

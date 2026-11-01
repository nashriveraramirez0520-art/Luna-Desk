# Luna Desk BOM and budget notes

Prepared November 1, 2026. All numeric costs are **USD**, and no purchases are recorded as completed. [BOM.csv](../BOM.csv) is the planning bill of materials.

## Purchase quantities versus installed quantities

The CSV's Qty field counts purchase packages for bought components. Separate columns record the required units and units per purchase. The budget charges the **whole package**, not just the small fraction installed.

| Item | Needed in one build | Purchase quantity | Budgeted amount |
| --- | --- | --- | --- |
| Waveshare ESP32-S3-Touch-LCD-3.5B, SKU 31137 | 1 board | 1 board | $25.99 |
| M2 × 10 mm button-head machine screws | 4 | 1 pack of 10 | $2.79 |
| M2 × 6 mm button-head machine screws | 2 | 1 pack of 10 | $2.79 |
| Ruthex RX-M2x4 M2 inserts | 2 | 1 pack of 70 | $9.99 estimate |
| USB-C data cable | 1 | Already owned | $0 additional |
| Printed front shell, rear cover, stand | 1 each | University printing planned | Shared $5 allowance below |

Machine screws are required for the existing metal threads and heat-set inserts; self-tapping screws are not a substitute. The insert geometry matches the CAD: 4 mm long and 3.6 mm outside diameter. Confirm the actual hardware and printed holes before assembly.

## Budget

| Category | Amount |
| --- | --- |
| Board and full fastener/insert packs | $41.56 |
| Previously quoted Waveshare shipping | $11.20 |
| Subtotal before printing allowance | **$52.76** |
| Provisional shared printing-material allowance | $5.00 |
| Planning subtotal including that allowance | **$57.76** |
| Additional hardware shipping | Pending checkout |
| Taxes and other checkout charges | Pending checkout |
| Final delivered build cost | **Not yet confirmed** |

The provisional funding target is $65. That leaves **$7.24** beyond the $57.76 planning subtotal for additional shipping, taxes, and any material cost above the allowance. This is a budget comparison, not a promise that the complete order will fit under $65 or that funding is approved.

The owner confirmed having a USB data cable and plans to print at the university, but has not confirmed the printing/material fee. The **$5 allowance is an author's planning reserve, not a university quote or a price for a full spool**. It covers all three printed parts together, so they are not charged a second time in the subtotal. Replace the allowance with the actual printing fee when known. If filament is bought instead, record the real spool price and any allocation to other projects; do not present a partial contribution as the full cash needed to buy a spool.

ODU describes the M-Lab as a free-to-use environment; that does not confirm that material for this particular project will be free. University permission, availability, material requirements and fees still need checking.

## Sources and price confidence

- **Board:** [Waveshare product page](https://www.waveshare.com/esp32-s3-touch-lcd-3.5b.htm), standard bare-board option. The $25.99 product price matches the earlier project quote; the public listing shows a $25.99–$32.99 range across versions. Do not select the -C case/camera version by mistake.
- **Screws:** [WREKD M2 button-head machine screws](https://wrekd.com/products/copy-of-metric-m2-hex-socket-button-head-grade-12-9-steel-screws-10-pack-choose-length). The listing and variant data show $2.79 per 10-pack for both 10 mm and 6 mm lengths. Shipping is calculated at checkout and is not included in these pack prices.
- **Inserts:** [Amazon ASIN B088QJG676](https://www.amazon.com/dp/B088QJG676), Ruthex RX-M2x4 70-pack. The direct marketplace price could not be read. **$9.99 is an indirect estimate**, based on [this US price-history reference](https://pricehistory.app/p/ruthex-m2-threaded-inserts-70-pieces-rx-lBU7Sy2h), whose displayed dated entry is September 21, 2026. It is not a verified November checkout offer. Check the live price, seller, availability and shipping before purchase.
- **Insert specification:** [Ruthex manufacturer product page](https://www.ruthex.de/en/products/ruthex-gewindeeinsatz-m2-70-stuck-rx-m2x4-messing-gewindebuchsen). Do not swap in an arbitrary M2 insert without checking outer diameter and length.
- **Printing location:** [ODU M-Lab](https://www.odu.edu/mlab). No project-specific material quote has been obtained.
- **Board shipping:** $11.20 Registered Post Air Mail from the project's October 26 quote. Reconfirm destination and rate at checkout; it does not cover WREKD or Amazon shipments.

No microSD card, battery, camera or custom PCB is required for the current firmware. Existing tools are not purchased through this budget; arrange access to a suitable screwdriver/hex driver and heat-set insertion tool or soldering iron for the build.

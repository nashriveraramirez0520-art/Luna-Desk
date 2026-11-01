/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

#include <lvgl.h>

/*To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 *You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 *Note that the `lv_examples` library is for LVGL v7 and you shouldn't install it for this version (since LVGL v8)
 *as the examples and demos are now part of the main LVGL library. */

// #include <examples/lv_examples.h>
// #include <demos/lv_demos.h>

#define DIRECT_RENDER_MODE // Uncomment to enable full frame buffer


#include <Arduino_GFX_Library.h>

#include "TCA9554.h"

#include "esp_lcd_touch_axs15231b.h"
#include <Wire.h>

#include <ArduinoJson.h>
#include <math.h>

#include "luna_character.h"

#define GFX_BL 6  // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

#define LCD_QSPI_CS   12
#define LCD_QSPI_CLK  5
#define LCD_QSPI_D0   1
#define LCD_QSPI_D1   2
#define LCD_QSPI_D2   3
#define LCD_QSPI_D3   4

#define I2C_SDA       8
#define I2C_SCL       7

TCA9554 TCA(0x20);

Arduino_DataBus *bus = new Arduino_ESP32QSPI(LCD_QSPI_CS, LCD_QSPI_CLK, LCD_QSPI_D0, LCD_QSPI_D1, LCD_QSPI_D2, LCD_QSPI_D3);

Arduino_GFX *gfx = new Arduino_AXS15231B(
  bus, -1 /* RST */, 1 /* rotation */, false, 320, 480
);



uint32_t screenWidth;
uint32_t screenHeight;
uint32_t bufSize;
lv_display_t *disp;
lv_color_t *disp_draw_buf1;
lv_color_t *disp_draw_buf2;

#if LV_USE_LOG != 0
void my_print(lv_log_level_t level, const char *buf)
{
  LV_UNUSED(level);
  Serial.println(buf);
  Serial.flush();
}
#endif

uint32_t millis_cb(void)
{
  return millis();
}

/* LVGL calls it when a rendered image needs to copied to the display*/
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);

  gfx->draw16bitRGBBitmap(area->x1, area->y1, (uint16_t *)px_map, w, h);

  /*Call it to tell LVGL you are ready*/
  lv_disp_flush_ready(disp);
}

/*Read the touchpad*/
void my_touchpad_read(lv_indev_t *indev, lv_indev_data_t *data)
{
  touch_data_t touch_data;
  bsp_touch_read();

  if (bsp_touch_get_coordinates(&touch_data)) {
    data->state = LV_INDEV_STATE_PR;
    /*Set the coordinates*/
    data->point.x = touch_data.coords[0].x;
    data->point.y = touch_data.coords[0].y;
  } else {
    data->state = LV_INDEV_STATE_REL;
  }
}

// Variables Globales //
lv_obj_t *status = nullptr;
lv_obj_t *cpu = nullptr;
lv_obj_t *ram = nullptr;
lv_obj_t *stateLabel = nullptr;
lv_obj_t *cpuBar = nullptr;
lv_obj_t *ramBar = nullptr;
lv_obj_t *focusPanel = nullptr;
lv_obj_t *navigationLabel = nullptr;
bool focusVisible = false;
lv_obj_t *clockLabel = nullptr;

bool lunaWorking = false;

const uint32_t FOCUS_DURATION = 25UL * 60UL * 1000UL;

uint32_t focusRemaining = FOCUS_DURATION;
uint32_t focusLastTick = 0;
bool focusRunning = false;

lv_obj_t *focusLabel = nullptr;

uint32_t lastStatsAt = 0;
bool pcConnected = false;

void processStats(const String &message)
{
  if (!status || !cpu || !ram) return;

  JsonDocument doc;
  if (deserializeJson(doc, message)) return;

  if (doc["type"] != "stats" || doc["version"] != 1) return;
  if (!doc["cpu_percent"].is<float>()) return;
  if (!doc["ram_percent"].is<float>()) return;

  float cpuValue = doc["cpu_percent"].as<float>();
  float ramValue = doc["ram_percent"].as<float>();

  if (!isfinite(cpuValue) || !isfinite(ramValue)) return;
  if (cpuValue < 0 || cpuValue > 100) return;
  if (ramValue < 0 || ramValue > 100) return;
  lunaWorking = cpuValue >= 30;

  char text[32];

  snprintf(text, sizeof(text), "CPU: %.1f %%", cpuValue);
  lv_label_set_text(cpu, text);

  snprintf(text, sizeof(text), "RAM: %.1f %%", ramValue);
  lv_label_set_text(ram, text);

  lv_label_set_text(status, "PC connected");
  if (cpuValue >= 30)
  {
     lv_label_set_text(stateLabel, "Luna: working");
     lv_obj_set_style_text_color(
       stateLabel, lv_color_hex(0xFFD166), 0
      );
  }
  else
  {
    lv_label_set_text(stateLabel, "Luna: idle");
    lv_obj_set_style_text_color(
      stateLabel, lv_color_hex(0xD9B8FF), 0
    );
  }
  lv_bar_set_value(cpuBar, (int)cpuValue, LV_ANIM_OFF);
  lv_bar_set_value(ramBar, (int)ramValue, LV_ANIM_OFF);

  const char *timestamp = doc["time"] | "";

  if (strlen(timestamp) >= 19 && timestamp[10] == 'T')
  {
   char clockText[9];
   snprintf(clockText, sizeof(clockText), "%.8s", timestamp + 11);
   lv_label_set_text(clockLabel, clockText);
  }

  lastStatsAt = millis();
  pcConnected = true;
}

void readPcMessages()
{
  static String message;
  static bool discardLine = false;
  unsigned int budget = 256;

  while (Serial.available() && budget > 0)
  {
    --budget;
    char character = Serial.read();

    if (character == '\n')
    {
      if (!discardLine && message.length() > 0)
      {
        processStats(message);
      }

      message = "";
      discardLine = false;
    }
    else if (character != '\r' && !discardLine)
    {
      if (message.length() < 511)
      {
        message += character;
      }
      else
      {
        message = "";
        discardLine = true;
      }
    }
  }
}

lv_obj_t *createStatsBar(lv_obj_t *parent, int y)
{
  lv_obj_t *bar = lv_bar_create(parent);
  lv_obj_set_size(bar, 210, 10);
  lv_obj_align(bar, LV_ALIGN_CENTER, 85, y);
  lv_bar_set_range(bar, 0, 100);
  lv_bar_set_value(bar, 0, LV_ANIM_OFF);

  lv_obj_set_style_bg_color(
    bar, lv_color_hex(0x342845), LV_PART_MAIN
  );
  lv_obj_set_style_bg_color(
    bar, lv_color_hex(0xD9B8FF), LV_PART_INDICATOR
  );

  return bar;
}

void refreshFocusLabel()
{
  if (!focusLabel) return;

  uint32_t seconds = (focusRemaining + 999) / 1000;
  uint32_t minutes = seconds / 60;
  seconds = seconds % 60;

  char text[16];
  snprintf(
    text, sizeof(text), "%02lu:%02lu",
    (unsigned long)minutes,
    (unsigned long)seconds
  );

  lv_label_set_text(focusLabel, text);
}

void updateFocusTimer()
{
  if (!focusRunning) return;

  uint32_t now = millis();
  uint32_t elapsed = now - focusLastTick;
  focusLastTick = now;

  if (elapsed >= focusRemaining)
  {
    focusRemaining = 0;
    focusRunning = false;
  }
  else
  {
    focusRemaining -= elapsed;
  }

  refreshFocusLabel();
}

void toggleFocusTimer(lv_event_t *event)
{
  updateFocusTimer();

  if (focusRunning)
  {
    focusRunning = false;
  }
  else if (focusRemaining > 0)
  {
    focusLastTick = millis();
    focusRunning = true;
  }
}

void resetFocusTimer(lv_event_t *event)
{
  focusRunning = false;
  focusRemaining = FOCUS_DURATION;
  refreshFocusLabel();
}

lv_obj_t *createButton(
  lv_obj_t *parent, const char *text,
  int x, int y, lv_event_cb_t callback
)
{
  lv_obj_t *button = lv_button_create(parent);
  lv_obj_set_size(button, 115, 40);
  lv_obj_set_pos(button, x, y);
  lv_obj_set_style_bg_color(
    button, lv_color_hex(0x342845), 0
  );
  lv_obj_add_event_cb(
    button, callback, LV_EVENT_CLICKED, nullptr
  );

  lv_obj_t *label = lv_label_create(button);
  lv_label_set_text(label, text);
  lv_obj_center(label);

  return label;
}

void switchPage(lv_event_t *event)
{
  focusVisible = !focusVisible;

  lv_obj_t *statsObjects[] = {
    status, cpu, ram, cpuBar, ramBar
  };

  for (lv_obj_t *object : statsObjects)
  {
    if (focusVisible)
      lv_obj_add_flag(object, LV_OBJ_FLAG_HIDDEN);
    else
      lv_obj_remove_flag(object, LV_OBJ_FLAG_HIDDEN);
  }

  if (focusVisible)
    lv_obj_remove_flag(focusPanel, LV_OBJ_FLAG_HIDDEN);
  else
    lv_obj_add_flag(focusPanel, LV_OBJ_FLAG_HIDDEN);

  lv_label_set_text(
    navigationLabel,
    focusVisible ? "PC status" : "Focus timer"
  );
}

void setup()
{
#ifdef DEV_DEVICE_INIT
  DEV_DEVICE_INIT();
#endif
  Wire.begin(I2C_SDA, I2C_SCL);
  TCA.begin();
  TCA.pinMode1(1, OUTPUT);
  TCA.write1(1, 1);
  delay(10);
  TCA.write1(1, 0);
  delay(10);
  TCA.write1(1, 1);
  delay(200);
  bsp_touch_init(&Wire, -1, 1, 480, 320);
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  // while(!Serial);
  Serial.println("Arduino_GFX LVGL_Arduino_v9 example ");
  String LVGL_Arduino = String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
  Serial.println(LVGL_Arduino);

  // Init Display
  if (!gfx->begin())
  {
    Serial.println("gfx->begin() failed!");
  }
  gfx->fillScreen(RGB565_BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  lv_init();

  /*Set a tick source so that LVGL will know how much time elapsed. */
  lv_tick_set_cb(millis_cb);

  /* register print function for debugging */
#if LV_USE_LOG != 0
  lv_log_register_print_cb(my_print);
#endif

  screenWidth = gfx->width();
  screenHeight = gfx->height();

  

#ifdef DIRECT_RENDER_MODE
  bufSize = screenWidth * screenHeight;
  disp_draw_buf1 = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  disp_draw_buf2 = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
#else
  bufSize = screenWidth * 40;
  disp_draw_buf1 = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  disp_draw_buf2 = (lv_color_t *)heap_caps_malloc(bufSize * 2, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
#endif
  if (!disp_draw_buf1 || !disp_draw_buf2)
  {
    Serial.println("LVGL disp_draw_buf allocate failed!");
  }
  else
  {
    disp = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(disp, my_disp_flush);
#ifdef DIRECT_RENDER_MODE
    lv_display_set_buffers(disp, disp_draw_buf1, disp_draw_buf2, bufSize * 2, LV_DISPLAY_RENDER_MODE_FULL);
#else
    lv_display_set_buffers(disp, disp_draw_buf1, disp_draw_buf2, bufSize * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif

    /*Initialize the (dummy) input device driver*/
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER); /*Touchpad should have POINTER type*/
    lv_indev_set_read_cb(indev, my_touchpad_read);

    /* Option 1: Create a simple label
     * ---------------------
     */
    lv_obj_t *screen = lv_screen_active();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x171322), 0);
    lv_obj_set_style_text_color(screen, lv_color_hex(0xD9B8FF), 0);

    lv_obj_t *title = lv_label_create(screen);
    lv_label_set_text(title, "LUNA DESK");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 85, 20);

    clockLabel = lv_label_create(screen);
    lv_label_set_text(clockLabel, "--:--:--");
    lv_obj_align(clockLabel, LV_ALIGN_TOP_MID, 85, 50);

    status = lv_label_create(screen);
    lv_label_set_text(status, "Waiting for PC");
    lv_obj_align(status, LV_ALIGN_CENTER, 85, -70);

    cpu = lv_label_create(screen);
    lv_label_set_text(cpu, "CPU: -- %");
    lv_obj_align(cpu, LV_ALIGN_CENTER, 85, -25);

    ram = lv_label_create(screen);
    lv_label_set_text(ram, "RAM: -- %");
    lv_obj_align(ram, LV_ALIGN_CENTER, 85, 20);

    stateLabel = lv_label_create(screen);
    lv_label_set_text(stateLabel, "Luna: waiting");
    lv_obj_align(stateLabel, LV_ALIGN_CENTER, 85, 65);
    cpuBar = createStatsBar(screen, -5);
    ramBar = createStatsBar(screen, 40);
    focusPanel = lv_obj_create(screen);
    lv_obj_remove_style_all(focusPanel);
    lv_obj_set_pos(focusPanel, 200, 65);
    lv_obj_set_size(focusPanel, 270, 145);
    lv_obj_remove_flag(focusPanel, LV_OBJ_FLAG_SCROLLABLE);

    focusLabel = lv_label_create(focusPanel);
    lv_label_set_text(focusLabel, "25:00");
    lv_obj_set_style_text_color(
      focusLabel, lv_color_hex(0xFFD166), 0
    );
    lv_obj_set_style_transform_scale(focusLabel, 512, 0);
    lv_obj_align(focusLabel, LV_ALIGN_TOP_MID, 0, 20);

    createButton(
      focusPanel, "Start/Pause",
      10, 95, toggleFocusTimer
    );

    createButton(
      focusPanel, "Reset",
      145, 95, resetFocusTimer
    );

    lv_obj_add_flag(focusPanel, LV_OBJ_FLAG_HIDDEN);

    navigationLabel = createButton(
      screen, "Focus timer",
      270, 265, switchPage
    );

    navigationLabel = createButton(
      screen, "Focus timer",
      270, 265, switchPage
    );

    LunaCharacter::create(screen);

    /* Option 2: Try an example. See all the examples
     *  - Online: https://docs.lvgl.io/master/examples.html
     *  - Source codes: https://github.com/lvgl/lvgl/tree/master/examples
     * ----------------------------------------------------------------
     */
    // lv_example_btn_1();

    /* Option 3: Or try out a demo. Don't forget to enable the demos in lv_conf.h. E.g. LV_USE_DEMOS_WIDGETS
     * -------------------------------------------------------------------------------------------
     */
    // lv_demo_widgets();
    // lv_demo_benchmark();
    // lv_demo_keypad_encoder();
    // lv_demo_music();
    // lv_demo_stress();
  }

  Serial.println("Setup done");
}

void loop()
{
  readPcMessages();
  updateFocusTimer();

  if (pcConnected && millis() - lastStatsAt >= 5000)
  {
    pcConnected = false;
    lv_label_set_text(status, "Disconnected");
    lv_label_set_text(cpu, "CPU: -- %");
    lv_label_set_text(ram, "RAM: -- %");
    lv_label_set_text(stateLabel, "Luna: disconnected");
    lv_obj_set_style_text_color(
      stateLabel, lv_color_hex(0xB9A9CD), 0
    );
    lv_bar_set_value(cpuBar, 0, LV_ANIM_OFF);
    lv_bar_set_value(ramBar, 0, LV_ANIM_OFF);
    lv_label_set_text(clockLabel, "--:--:--");
  }

  LunaCharacter::update(pcConnected && lunaWorking);

  lv_timer_handler();
  delay(5);
}

#pragma once
#include "luna_sprites.h"

namespace LunaCharacter {
static lv_obj_t *image = nullptr;
static lv_image_dsc_t idleImages[LunaSprites::IDLE_COUNT] = {};
static lv_image_dsc_t workingImages[LunaSprites::WORKING_COUNT] = {};
static bool working = false;
static int frameIndex = 0;
static uint32_t lastFrameAt = 0;

inline void initializeFrame(lv_image_dsc_t &frame, const uint8_t *data)
{
  frame.header.magic = LV_IMAGE_HEADER_MAGIC;
  frame.header.cf = LV_COLOR_FORMAT_RGB565;
  frame.header.w = LunaSprites::WIDTH;
  frame.header.h = LunaSprites::HEIGHT;
  frame.header.stride = LunaSprites::WIDTH * 2;
  frame.data_size = LunaSprites::WIDTH * LunaSprites::HEIGHT * 2;
  frame.data = data;
}

inline void create(lv_obj_t *parent)
{
  for (int i = 0; i < LunaSprites::IDLE_COUNT; ++i)
    initializeFrame(idleImages[i], LunaSprites::idleFrames[i]);
  for (int i = 0; i < LunaSprites::WORKING_COUNT; ++i)
    initializeFrame(workingImages[i], LunaSprites::workingFrames[i]);

  working = false;
  frameIndex = 0;
  lastFrameAt = millis();
  image = lv_image_create(parent);
  lv_obj_set_pos(image, 20, 65);
  lv_image_set_src(image, &idleImages[0]);
}

inline void update(bool workingNow)
{
  if (!image) return;
  uint32_t now = millis();

  if (workingNow != working)
  {
    working = workingNow;
    frameIndex = 0;
    lastFrameAt = now;
  }
  else
  {
    uint32_t steps = (now - lastFrameAt) / LunaSprites::FRAME_MS;
    if (steps == 0) return;
    int count = working ? LunaSprites::WORKING_COUNT : LunaSprites::IDLE_COUNT;
    frameIndex = (frameIndex + steps % count) % count;
    lastFrameAt += steps * LunaSprites::FRAME_MS;
  }

  lv_image_set_src(image, working ? &workingImages[frameIndex] : &idleImages[frameIndex]);
}
} // namespace LunaCharacter

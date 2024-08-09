#include <FastLED.h>

#define NUM_LEDS 30
#define DATA_PIN 2
#define BUTTON_PIN 3
#define GROUP_SIZE 3  // 每组亮起的LED数量
#define COLOR_COUNT 6 // 颜色数量
#define MAX_EFFECTS 5 // 最大同时运行的效果数

CRGB leds[NUM_LEDS];
bool buttonPressed = false;
int colorIndex = 0;
unsigned long previousMillis = 0;
const long interval = 50; // 流水速度，可以调整

// 定义多个颜色
CRGB colors[COLOR_COUNT] = {
  CRGB::Blue,
  CRGB::Red,
  CRGB::Green,
  CRGB::Yellow,
  CRGB::Purple,
  CRGB::Orange
};

struct Effect {
  int currentLed;
  int color;
  bool active;
};

Effect effects[MAX_EFFECTS];
int activeEffects = 0;

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // 初始化效果数组
  for (int i = 0; i < MAX_EFFECTS; i++) {
    effects[i].active = false;
  }
}

void loop() {
  unsigned long currentMillis = millis();

  // 检查按钮状态
  if (digitalRead(BUTTON_PIN) == LOW && !buttonPressed) {
    buttonPressed = true;
    addNewEffect();
  } else if (digitalRead(BUTTON_PIN) == HIGH) {
    buttonPressed = false;
  }

  // 非阻塞的流水灯效果
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    updateEffects();
  }

  FastLED.show();
}

void addNewEffect() {
  if (activeEffects < MAX_EFFECTS) {
    for (int i = 0; i < MAX_EFFECTS; i++) {
      if (!effects[i].active) {
        effects[i].currentLed = 0;
        effects[i].color = colorIndex;
        effects[i].active = true;
        activeEffects++;
        colorIndex = (colorIndex + 1) % COLOR_COUNT; // 切换到下一个颜色
        break;
      }
    }
  }
}

void updateEffects() {
  FastLED.clear();

  for (int i = 0; i < MAX_EFFECTS; i++) {
    if (effects[i].active) {
      // 点亮当前位置及后两个LED
      for (int j = 0; j < GROUP_SIZE; j++) {
        if (effects[i].currentLed + j < NUM_LEDS) {
          leds[effects[i].currentLed + j] = colors[effects[i].color];
        }
      }

      // 移动到下一个LED
      effects[i].currentLed++;
      
      // 如果效果结束，重置效果
      if (effects[i].currentLed >= NUM_LEDS) {
        effects[i].active = false;
        activeEffects--;
      }
    }
  }
}

#include <FastLED.h>

#define NUM_STRIPS 8   // LED灯条的数量

// 定义每个灯条的LED灯数量
#define NUM_LEDS_1 7
#define NUM_LEDS_2 35
#define NUM_LEDS_3 7
#define NUM_LEDS_4 9
#define NUM_LEDS_5 7
#define NUM_LEDS_6 39
#define NUM_LEDS_7 7
#define NUM_LEDS_8 10

// 定义每个灯条的数据引脚
#define DATA_PIN_1 2
#define DATA_PIN_2 3
#define DATA_PIN_3 4
#define DATA_PIN_4 5
#define DATA_PIN_5 6
#define DATA_PIN_6 7
#define DATA_PIN_7 8
#define DATA_PIN_8 9

#define TRIGGER_PIN4 A2  // 触发引脚
#define TRIGGER_PIN3 A3  // 触发引脚
#define TRIGGER_PIN2 A1  // 触发引脚
#define TRIGGER_PIN1 A0  // 触发引脚

// 创建LED灯条对象数组
CRGB leds[NUM_STRIPS][NUM_LEDS_6];

bool ledsOn4 = false;  // 灯条状态
bool ledsOn3 = false;  // 灯条状态
bool ledsOn2 = false;  // 灯条状态
bool ledsOn1 = false;  // 灯条状态

int Mode = 0;

// 为每个逐个点亮的灯条创建单独的ledIndex变量
int ledIndex2 = 0;
int ledIndex4 = 0;
int ledIndex6 = 0;
int ledIndex8 = 0;

unsigned long lastUpdateTime2 = 0;
unsigned long lastUpdateTime4 = 0;
unsigned long lastUpdateTime6 = 0;
unsigned long lastUpdateTime8 = 0;
unsigned long sequenceStartTime = 0;

int currentStep = 1;


int pos1 = 0;  // 灯条1的当前位置
int pos2 = 0;  // 灯条2的当前位置
int pos3 = 0;  // 灯条3的当前位置
int pos4 = 0;  // 灯条4的当前位置
int pos5 = 0;  // 灯条5的当前位置
int pos6 = 0;  // 灯条6的当前位置
int pos7 = 0;  // 灯条7的当前位置
int pos8 = 0;  // 灯条8的当前位置

// 定义每个灯条的RGB颜色值
const byte ledColors[NUM_STRIPS][3] = {
  {255, 255, 255},  // 灯条1的RGB颜色值
  {255, 0, 0},      // 灯条2的RGB颜色值
  {255, 255, 255},  // 灯条3的RGB颜色值
  {0, 255, 0},      // 灯条4的RGB颜色值
  {255, 255, 255},  // 灯条5的RGB颜色值
  {0, 0, 255},      // 灯条6的RGB颜色值
  {255, 255, 255},  // 灯条7的RGB颜色值
  {255, 255, 0}     // 灯条8的RGB颜色值
};

int counter = 0;  // 计数器
const int DELAY_COUNTS = 90;  // 计数器阈值,控制流水灯速度

void setup() {
  // 初始化每个WS2811 LED灯条
  FastLED.addLeds<WS2811, DATA_PIN_1, RGB>(leds[0], NUM_LEDS_1);
  FastLED.addLeds<WS2811, DATA_PIN_2, RGB>(leds[1], NUM_LEDS_2);
  FastLED.addLeds<WS2811, DATA_PIN_3, RGB>(leds[2], NUM_LEDS_3);
  FastLED.addLeds<WS2811, DATA_PIN_4, RGB>(leds[3], NUM_LEDS_4);
  FastLED.addLeds<WS2811, DATA_PIN_5, RGB>(leds[4], NUM_LEDS_5);
  FastLED.addLeds<WS2811, DATA_PIN_6, RGB>(leds[5], NUM_LEDS_6);
  FastLED.addLeds<WS2811, DATA_PIN_7, RGB>(leds[6], NUM_LEDS_7);
  FastLED.addLeds<WS2811, DATA_PIN_8, RGB>(leds[7], NUM_LEDS_8);


  pinMode(TRIGGER_PIN1, INPUT);  // 设置触发引脚为输入模式
  pinMode(TRIGGER_PIN2, INPUT);  // 设置触发引脚为输入模式
  pinMode(TRIGGER_PIN3, INPUT);  // 设置触发引脚为输入模式
  pinMode(TRIGGER_PIN4, INPUT);  // 设置触发引脚为输入模式
}

void loop() {
  if (digitalRead(TRIGGER_PIN4) == HIGH) {  // 检测触发引脚是否为高电平
    delay(20);
    if (digitalRead(TRIGGER_PIN4) == HIGH)
    {
      Mode = 4;
      ledsOn4 = !ledsOn4;  // 切换灯条状态
    }
    while (digitalRead(TRIGGER_PIN4) == HIGH);
  }

  if (digitalRead(TRIGGER_PIN3) == HIGH) {  // 检测触发引脚是否为高电平
    delay(20);
    if (digitalRead(TRIGGER_PIN3) == HIGH)
    {
      Mode = 3;
      ledsOn3 = !ledsOn3;  // 切换灯条状态
    }
    while (digitalRead(TRIGGER_PIN3) == HIGH);
  }

  if (digitalRead(TRIGGER_PIN2) == HIGH) { // 检测触发引脚是否为高电平
    delay(20);
    if (digitalRead(TRIGGER_PIN2) == HIGH)
    {
      Mode = 2;
      ledsOn2 = !ledsOn2; // 切换灯条状态
    }
    while (digitalRead(TRIGGER_PIN2) == HIGH);
  }

  if (digitalRead(TRIGGER_PIN1) == HIGH) { // 检测触发引脚是否为高电平
    delay(20);
    if (digitalRead(TRIGGER_PIN1) == HIGH)
    {
      Mode = 1;
      ledsOn1 = !ledsOn1; // 切换灯条状态
    }
    while (digitalRead(TRIGGER_PIN1) == HIGH);
  }

  if (Mode == 1)
  {
    if (ledsOn1)
    {
      switch (currentStep) {
        case 1:
          // 灯条1流水亮
          leds[0][pos1] = CRGB(ledColors[0][0], ledColors[0][1], ledColors[0][2]);
          FastLED.show();
          counter++;
          if (counter >= DELAY_COUNTS) {
            counter = 0;
            pos1++;
            if (pos1 >= NUM_LEDS_1) {
              currentStep++;
            }
          }
          break;

        case 2:
          // 灯条1关闭
          for (int i = 0; i < NUM_LEDS_1; i++) {
            leds[0][i] = CRGB(0, 0, 0);
          }
          FastLED.show();
          pos1 = 0;
          currentStep++;
          break;

        case 3:
          // 灯条2流水亮
          leds[1][pos2] = CRGB(ledColors[1][0], ledColors[1][1], ledColors[1][2]);
          FastLED.show();
          counter++;
          if (counter >= DELAY_COUNTS) {
            counter = 0;
            pos2++;
            if (pos2 >= NUM_LEDS_2) {
              currentStep++;
            }
          }
          break;

        case 4:
          // 灯条2关闭
          for (int i = 0; i < NUM_LEDS_2; i++) {
            leds[1][i] = CRGB(0, 0, 0);
          }
          FastLED.show();
          pos2 = 0;
          currentStep++;
          break;

        case 5:
          // 灯条3流水亮
          leds[2][pos3] = CRGB(ledColors[2][0], ledColors[2][1], ledColors[2][2]);
          FastLED.show();
          counter++;
          if (counter >= DELAY_COUNTS) {
            counter = 0;
            pos3++;
            if (pos3 >= NUM_LEDS_3) {
              currentStep++;
            }
          }
          break;

        case 6:
          // 灯条3关闭
          for (int i = 0; i < NUM_LEDS_3; i++) {
            leds[2][i] = CRGB(0, 0, 0);
          }
          FastLED.show();
          pos3 = 0;
          currentStep++;
          break;

        case 7:
          // 灯条4流水亮
          leds[3][pos4] = CRGB(ledColors[3][0], ledColors[3][1], ledColors[3][2]);
          FastLED.show();
          counter++;
          if (counter >= DELAY_COUNTS) {
            counter = 0;
            pos4++;
            if (pos4 >= NUM_LEDS_4) {
              currentStep++;
            }
          }
          break;

        case 8:
          // 灯条4关闭
          for (int i = 0; i < NUM_LEDS_4; i++) {
            leds[3][i] = CRGB(0, 0, 0);
          }
          FastLED.show();
          pos4 = 0;
          currentStep++;
          break;

        case 9:
          // 灯条5流水亮
          leds[4][pos5] = CRGB(ledColors[4][0], ledColors[4][1], ledColors[4][2]);
          FastLED.show();
          counter++;
          if (counter >= DELAY_COUNTS) {
            counter = 0;
            pos5++;
            if (pos5 >= NUM_LEDS_5) {
              currentStep++;
            }
          }
          break;

        case 10:
          // 灯条5关闭
          for (int i = 0; i < NUM_LEDS_5; i++) {
            leds[4][i] = CRGB(0, 0, 0);
          }
          FastLED.show();
          pos5 = 0;
          currentStep++;
          break;

        case 11:
          // 灯条6流水亮
          leds[5][pos6] = CRGB(ledColors[5][0], ledColors[5][1], ledColors[5][2]);
          FastLED.show();
          counter++;
          if (counter >= DELAY_COUNTS) {
            counter = 0;
            pos6++;
            if (pos6 >= NUM_LEDS_6) {
              currentStep++;
            }
          }
          break;

        case 12:
          // 灯条6关闭
          for (int i = 0; i < NUM_LEDS_6; i++) {
            leds[5][i] = CRGB(0, 0, 0);
          }
          FastLED.show();
          pos6 = 0;
          currentStep++;
          break;

        case 13:
          // 灯条7流水亮
          leds[6][pos7] = CRGB(ledColors[6][0], ledColors[6][1], ledColors[6][2]);
          FastLED.show();
          counter++;
          if (counter >= DELAY_COUNTS) {
            counter = 0;
            pos7++;
            if (pos7 >= NUM_LEDS_7) {
              currentStep++;
            }
          }
          break;

        case 14:
          // 灯条7关闭
          for (int i = 0; i < NUM_LEDS_7; i++) {
            leds[6][i] = CRGB(0, 0, 0);
          }
          FastLED.show();
          pos7 = 0;
          currentStep++;
          break;

        case 15:
          // 灯条8流水亮
          leds[7][pos8] = CRGB(ledColors[7][0], ledColors[7][1], ledColors[7][2]);
          FastLED.show();
          counter++;
          if (counter >= DELAY_COUNTS) {
            counter = 0;
            pos8++;
            if (pos8 >= NUM_LEDS_8) {
              // 灯条8关闭
              for (int i = 0; i < NUM_LEDS_8; i++) {
                leds[7][i] = CRGB(0, 0, 0);
              }
              FastLED.show();
              pos8 = 0;
              currentStep = 1;  // 重新开始sequence
            }
          }
          break;

        case 16:
          // 这个case现在可以删除,因为我们在case 15中处理了灯条8的关闭
          break;
          FastLED.show();
          pos8 = 0;
          currentStep++;
          break;
      }
    }
    else
    {
      // 全灭
      for (int i = 0; i < NUM_STRIPS; i++) {
        for (int j = 0; j < NUM_LEDS_6; j++) {
          leds[i][j] = CRGB(0, 0, 0);
        }
      }
      FastLED.show();  // 更新灯条显示
    }
  }




  if (Mode == 2)
  {
    if (ledsOn2) {
      // 全亮
      for (int i = 0; i < NUM_LEDS_1; i++) {
        leds[0][i] = CRGB(255, 255, 255);
      }
      for (int i = 0; i < NUM_LEDS_3; i++) {
        leds[2][i] = CRGB(255, 255, 255);
      }
      for (int i = 0; i < NUM_LEDS_5; i++) {
        leds[4][i] = CRGB(255, 255, 255);
      }
      for (int i = 0; i < NUM_LEDS_7; i++) {
        leds[6][i] = CRGB(255, 255, 255);
      }
      // 全灭
      for (int i = 0; i < NUM_LEDS_2; i++) {
        leds[1][i] = CRGB(0, 0, 0);
      }
      for (int i = 0; i < NUM_LEDS_4; i++) {
        leds[3][i] = CRGB(0, 0, 0);
      }
      for (int i = 0; i < NUM_LEDS_6; i++) {
        leds[5][i] = CRGB(0, 0, 0);
      }
      for (int i = 0; i < NUM_LEDS_8; i++) {
        leds[7][i] = CRGB(0, 0, 0);
      }
    } else {
      // 全灭
      for (int i = 0; i < NUM_LEDS_1; i++) {
        leds[0][i] = CRGB(0, 0, 0);
      }
      for (int i = 0; i < NUM_LEDS_3; i++) {
        leds[2][i] = CRGB(0, 0, 0);
      }
      for (int i = 0; i < NUM_LEDS_5; i++) {
        leds[4][i] = CRGB(0, 0, 0);
      }
      for (int i = 0; i < NUM_LEDS_7; i++) {
        leds[6][i] = CRGB(0, 0, 0);
      }
    }

    FastLED.show();  // 更新灯条显示
  }





  if (Mode == 3)
  {
    if (ledsOn3) {
      // 全亮
      for (int i = 0; i < NUM_LEDS_2; i++) {
        leds[1][i] = CRGB(255, 255, 255);
      }
      for (int i = 0; i < NUM_LEDS_4; i++) {
        leds[3][i] = CRGB(255, 255, 255);
      }
      for (int i = 0; i < NUM_LEDS_6; i++) {
        leds[5][i] = CRGB(255, 255, 255);
      }
      for (int i = 0; i < NUM_LEDS_8; i++) {
        leds[7][i] = CRGB(255, 255, 255);
      }
      for (int i = 0; i < NUM_LEDS_1; i++) {
        leds[0][i] = CRGB(0, 0, 0);
      }
      for (int i = 0; i < NUM_LEDS_3; i++) {
        leds[2][i] = CRGB(0, 0, 0);
      }
      for (int i = 0; i < NUM_LEDS_5; i++) {
        leds[4][i] = CRGB(0, 0, 0);
      }
      for (int i = 0; i < NUM_LEDS_7; i++) {
        leds[6][i] = CRGB(0, 0, 0);
      }
    } else {
      // 全灭
      for (int i = 0; i < NUM_LEDS_2; i++) {
        leds[1][i] = CRGB(0, 0, 0);
      }
      for (int i = 0; i < NUM_LEDS_4; i++) {
        leds[3][i] = CRGB(0, 0, 0);
      }
      for (int i = 0; i < NUM_LEDS_6; i++) {
        leds[5][i] = CRGB(0, 0, 0);
      }
      for (int i = 0; i < NUM_LEDS_8; i++) {
        leds[7][i] = CRGB(0, 0, 0);
      }
    }

    FastLED.show();  // 更新灯条显示
  }


  if (Mode == 4)
  {
    if (ledsOn4) {
      // 全亮
      for (int i = 0; i < NUM_STRIPS; i++) {
        for (int j = 0; j < NUM_LEDS_6; j++) {
          leds[i][j] = CRGB(255, 255, 255);
        }
      }
    } else {
      // 全灭
      for (int i = 0; i < NUM_STRIPS; i++) {
        for (int j = 0; j < NUM_LEDS_6; j++) {
          leds[i][j] = CRGB(0, 0, 0);
        }
      }
    }

    FastLED.show();  // 更新灯条显示
  }


}

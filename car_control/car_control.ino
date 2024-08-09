#include <AccelStepper.h>

// 引脚定义
#define EN_PIN PB1
#define DIR_PIN PB11
#define STEP_PIN PB10

#define OUTPUT1_PIN PA6
#define OUTPUT2_PIN PA1
#define INPUT_PIN_1 PB12
#define INPUT_PIN_2 PB13
#define INPUT_PIN_3 PB14
#define INPUT_PIN_4 PB15

#define SWITCH_PIN PB8

#define LIMIT_SWITCH1_PIN PA4
#define LIMIT_SWITCH2_PIN PA5

// 创建AccelStepper对象
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

unsigned long lastPrintTime = millis();
unsigned long currentTime;

enum MotorState {
  STOPPED,
  FORWARD,
  BACKWARD
};

MotorState currentState = STOPPED;
int currentSpeed = 0;
bool lastInputPin4State = LOW;
bool output2State = HIGH;  // 初始设置为高电平
bool lastSwitchPinState = HIGH;  // 新增：上一次SWITCH_PIN的状态

void setEnablePin(MotorState state) {
  if (state == STOPPED) {
    digitalWrite(EN_PIN, HIGH);  // 停止状态，EN_PIN输出高电平
  } else {
    digitalWrite(EN_PIN, LOW);   // 前进或后退状态，EN_PIN输出低电平
  }
}

void setup() {
  Serial1.begin(9600);
  Serial2.begin(9600);
  pinMode(EN_PIN, OUTPUT);
  pinMode(LIMIT_SWITCH1_PIN, INPUT_PULLUP);
  pinMode(LIMIT_SWITCH2_PIN, INPUT_PULLUP);
  pinMode(OUTPUT1_PIN, OUTPUT);
  pinMode(OUTPUT2_PIN, OUTPUT);
  pinMode(INPUT_PIN_1, INPUT);
  pinMode(INPUT_PIN_2, INPUT);
  pinMode(INPUT_PIN_3, INPUT);
  pinMode(INPUT_PIN_4, INPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP);  // 新增：设置SWITCH_PIN为上拉输入

  setEnablePin(STOPPED);  // 初始化为停止状态
  digitalWrite(OUTPUT1_PIN, LOW);  // 初始化OUTPUT1_PIN为低电平
  
  stepper.setMaxSpeed(50000);
  stepper.setAcceleration(1000);

  // 运动到第一个限位开关
  while (digitalRead(LIMIT_SWITCH1_PIN)) {
    currentTime = millis();
    if (currentTime - lastPrintTime >= 100) {
      Serial1.print(digitalRead(LIMIT_SWITCH1_PIN));
      Serial1.println(digitalRead(LIMIT_SWITCH2_PIN));
      lastPrintTime = currentTime;
    }
    setEnablePin(BACKWARD);  // 设置为后退状态
    stepper.setSpeed(-5000);
    stepper.runSpeed();
  }
  digitalWrite(OUTPUT2_PIN, output2State);  // 初始化OUTPUT2_PIN为高电平

  stepper.setSpeed(0);
  stepper.runSpeed();
  setEnablePin(STOPPED);  // 回到停止状态
}

void loop() {
  currentTime = millis();
  if (currentTime - lastPrintTime >= 100) {
    Serial1.print(digitalRead(LIMIT_SWITCH1_PIN));
    Serial1.println(digitalRead(LIMIT_SWITCH2_PIN));
    lastPrintTime = currentTime;
  }

//  // 检查INPUT_PIN_4状态
//  bool currentInputPin4State = digitalRead(INPUT_PIN_4);
//  if (currentInputPin4State == HIGH && lastInputPin4State == LOW) {
//    Serial2.print("D");  // 触发INPUT_PIN_4时，串口2输出D
//    setEnablePin(BACKWARD);
//    currentState = BACKWARD;
//      currentSpeed = -10000;
//    output2State = !output2State;
//    digitalWrite(OUTPUT2_PIN, output2State);  // 切换灯状态
//  }
//  lastInputPin4State = currentInputPin4State;

  // 检查SWITCH_PIN状态
  bool currentSwitchPinState = digitalRead(SWITCH_PIN);
  if (currentSwitchPinState == LOW && lastSwitchPinState == HIGH) {
    if (currentState == STOPPED) {
      currentState = FORWARD;
      currentSpeed = 10000;
      Serial2.print("C");
      setEnablePin(FORWARD);
    }
  }
  lastSwitchPinState = currentSwitchPinState;

  // 检查输入引脚状态
  if (currentState == STOPPED) {
    if (digitalRead(INPUT_PIN_1) == HIGH) {
      currentState = FORWARD;
      currentSpeed = 6000;
      Serial2.print("A");
      setEnablePin(FORWARD);
    } else if (digitalRead(INPUT_PIN_2) == HIGH) {
      currentState = FORWARD;
      currentSpeed = 10000;
      Serial2.print("B");
      setEnablePin(FORWARD);
    } else if (digitalRead(INPUT_PIN_3) == HIGH) {
      currentState = FORWARD;
      currentSpeed = 15000;
      Serial2.print("C");
      setEnablePin(FORWARD);
    }else if (digitalRead(INPUT_PIN_4) == HIGH) {
      currentState = BACKWARD;
      currentSpeed = -10000;
      Serial2.print("D");
      setEnablePin(BACKWARD);
    }
  }

  // 检查串口输入
  if (Serial2.available()) {
    char input = Serial2.read();
    if (currentState == STOPPED) {
      switch (input) {
        case 'A':
          currentState = FORWARD;
          currentSpeed = 6000;
          setEnablePin(FORWARD);
          break;
        case 'B':
          currentState = FORWARD;
          currentSpeed = 10000;
          setEnablePin(FORWARD);
          break;
        case 'C':
          currentState = FORWARD;
          currentSpeed = 15000;
          setEnablePin(FORWARD);
          break;
        case 'D':
          currentState = BACKWARD;
          currentSpeed = -10000;
          setEnablePin(BACKWARD);
          break;
      }
    }
  }

  // 检查限位开关状态
  if (digitalRead(LIMIT_SWITCH2_PIN) == LOW && currentState == FORWARD) {
    currentState = STOPPED;
    currentSpeed = 0;
    digitalWrite(OUTPUT1_PIN, HIGH);  // 触发LIMIT_SWITCH2_PIN时，OUTPUT1_PIN输出高电平
    setEnablePin(STOPPED);
  }

  if (digitalRead(LIMIT_SWITCH1_PIN) == LOW && currentState == BACKWARD) {
    currentState = STOPPED;
    currentSpeed = 0;
    digitalWrite(OUTPUT1_PIN, LOW);  // 触发LIMIT_SWITCH1_PIN时，OUTPUT1_PIN输出低电平
    setEnablePin(STOPPED);
  }

  // 设置和运行电机
  stepper.setSpeed(currentSpeed);
  stepper.runSpeed();
}

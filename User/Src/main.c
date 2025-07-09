//
// Created by hiifong on 2025/7/1.
//

#include "delay.h"
#include "stm32f10x.h"
#include "OLED.h"

#define BUTTON_Pin GPIO_Pin_1
#define BUTTON_Port GPIOB

uint8_t i;
uint8_t angle;

void PWM_Init(void) {
    // 时钟使能
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // GPIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 内部时钟, 可以省略（上电后定时器默认使用内部时钟）
    TIM_InternalClockConfig(TIM2);

    // 时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数模式
    TIM_TimeBaseStructure.TIM_Period = 20000 - 1; // 周期ARR，在10K的计数频率下记10000个数就是1s
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1; // 预分频PSC， 这里是对72M进行7200分频，得到10K的计数频率
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; // 重复计数器，高级定时器才有
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 输出比较单元
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure); // 初始化OCInitStructure
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // PWM模式1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0; // 脉冲，用来设置CCR寄存器值
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);

    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare2(uint16_t compare) {
    TIM_SetCompare2(TIM2, compare);
}

void Servo_Init(void) {
    PWM_Init();
}

// 0    500
// 180  2500
void Servo_SetAngle(float angle) {
    PWM_SetCompare2(angle / 180 * 2000 + 500);
}

void Button_Init(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = BUTTON_Pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUTTON_Port, &GPIO_InitStructure);
}

uint8_t Button_Pressed(void) {
    uint8_t pressed = 0;
    if (GPIO_ReadInputDataBit(BUTTON_Port, BUTTON_Pin) == 0) {
        Delay_ms(50);
        if (GPIO_ReadInputDataBit(BUTTON_Port, BUTTON_Pin) == 0) {
            pressed = 1;
        }
    }
    return pressed;
}

void Main() {
    OLED_Init();
    Button_Init();
    Servo_Init();
    OLED_ShowString(1, 1, "Angle:");
    while (1) {
        if (Button_Pressed()) {
            angle += 30;
            if (angle > 180) {
                angle = 0;
            }
        }
        Servo_SetAngle(angle);
        OLED_ShowNum(1, 7, angle, 3);
    }
}

//
// Created by hiifong on 2025/7/1.
//

#include "stm32f10x.h"
#include "OLED.h"

uint16_t num;

// Tim2
void Timer_Init(void) {
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 内部时钟, 可以省略（上电后定时器默认使用内部时钟）
    TIM_InternalClockConfig(TIM2);

    // 时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; // 时钟分频
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数模式
    TIM_TimeBaseStructure.TIM_Period = 10000 - 1;       // 周期，在10K的计数频率下记10000个数就是1s
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;     // 预分频， 这里是对72M进行7200分频，得到10K的计数频率
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;    // 重复计数器，高级定时器才有
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 使能计数中断
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);   // 避免初始化完就进入中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // NVIC
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

void Main() {
    OLED_Init();
    Timer_Init();
    OLED_ShowString(1,1, "Num:");
    while (1)
    {
        OLED_ShowNum(1, 5, num, 5);
        OLED_ShowNum(2,5, TIM_GetCounter(TIM2), 5); // 计数器值
    }
}

void TIM2_IRQHandler(void) {
    // 获取中断标志位
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
        num++;
        // 清除标志位
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

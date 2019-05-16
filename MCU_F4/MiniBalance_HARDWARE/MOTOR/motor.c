#include "motor.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
void MiniBalance_Motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOB, ENABLE);//使能GPIOB时钟
	
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4|GPIO_Pin_5;//对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIO
	GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5);//GPIOB4 5设置高
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;//对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_0);//GPIOB0设置设置高
}
void MiniBalance_PWM_Init(u16 arr,u16 psc)
{		 		
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);  	  //TIM8时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8); //GPIOC6复用为定时器8 CH1
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8); //GPIOC7复用为定时器8 CH2
  GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8); //GPIOC8复用为定时器8 CH3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;   //GPIO
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOC,&GPIO_InitStructure);              //初始化PA
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure);//初始化定时器1
	
	//初始化TIM8   PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 
  TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM8 
	TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE 主输出使能	
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM8在CCR1上的预装载寄存器
 	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM8在CCR2上的预装载寄存器
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //使能TIM8在CCR3上的预装载寄存器
  TIM_ARRPreloadConfig(TIM8,ENABLE);//ARPE使能 
	TIM_Cmd(TIM8, ENABLE);  //使能TIM8
	MiniBalance_Motor_Init();
} 


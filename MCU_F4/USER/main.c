#include "stm32f4xx.h"
#include "sys.h"  
u8 Flag_Left,Flag_Right,Flag_Direction=0;   //蓝牙遥控相关的变量
u8 Flag_Stop=1,Flag_Show=0;                 //停止标志位和 显示标志位 默认停止 显示打开
int Encoder_A,Encoder_B,Encoder_C;          //编码器的脉冲计数
long int Position_A=0,Position_B=0,Position_C=0,Rate_A,Rate_B,Rate_C; //PID控制相关变量                   
long int Motor_A=0,Motor_B=0,Motor_C=0;        //电机PWM变量
long int Target_A=0,Target_B=0,Target_C=0;     //电机目标值
int Voltage;                             //电池电压采样相关的变量
float Show_Data_Mb;                      //全局显示变量，用于显示需要查看的数据                         
u8 delay_50,delay_flag;                          //延时相关变量
u8 Run_Flag=0;  //蓝牙遥控相关变量和运行状态标志位
u8 rxbuf[8]={0},Urxbuf[8]={0},CAN_ON_Flag=0,PS2_ON_Flag=0,Usart_ON_Flag=0,Usart_Flag,PID_Send,Flash_Send;  //CAN和串口控制相关变量
u8 txbuf[8]={0},txbuf2[8]={0},Turn_Flag;             //CAN发送相关变量
float Pitch,Roll,Yaw,Move_X,Move_Y,Move_Z;   //三轴角度和XYZ轴目标速度
u32 PID_Parameter[10],Flash_Parameter[10];  //Flash相关数组
// float	Position_KP=40,Position_KI=0,Position_KD=40;  //位置控制PID参数
float	Position_KP=4,Position_KI=0,Position_KD=4;
// float Velocity_KP=10,Velocity_KI=10;	          //速度控制PID参数
float Velocity_KP=1,Velocity_KI=1;
int RC_Velocity=45,RC_Position=3000;         //设置遥控的速度和位置值
int PS2_LX,PS2_LY,PS2_RX,PS2_RY,PS2_KEY;     //
int Gryo_Z;
int main(void)
{
	delay_init(168);                //=====主频168M
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//=====设置系统中断优先级分组2
	LED_Init();                     //=====LED初始化
	KEY_Init();                     //=====按键初始化
	if(MODE==0)Run_Flag=1;          //=====启动的过程中，根据模式选择开关确定进入位置模式还是速度模式
	else Run_Flag=0;                //=====启动的过程中，根据模式选择开关确定进入位置模式还是速度模式
  OLED_Init();                    //=====OLED初始化
  uart_init(115200);              //=====延时初始化
	// uart2_init(9600);               //=====串口2初始化
	uart2_init(115200);  // this is for printf
	uart3_init(115200);             //=====串口3初始化
	Encoder_Init_TIM2();            //=====编码器初始化
	Encoder_Init_TIM3();            //=====编码器初始化
	Encoder_Init_TIM4();            //=====编码器初始化
	Adc_Init();                     //=====模拟量采集初始化
	delay_ms(500);                  //=====延时等待系统稳定
	IIC_Init();                     //=====IIC初始化
  MPU6050_initialize();           //=====MPU6050初始化	
  DMP_Init();                     //=====初始化DMP 
	// PS2_Init();									    //=====ps2驱动端口初始化
	// PS2_SetInit();		 					    //=====ps2配置初始化,配置“红绿灯模式”，并选择是否可以修改
	// MiniBalance_EXTI_Init();       				  //=====外部中断初始化
	// CAN1_Mode_Init(1,7,6,3,CAN_Mode_Normal);//=====CAN初始化  
	MiniBalance_PWM_Init(8400,1);   				//=====PWM初始化
	MiniBalance_EXTI_Init();
	delay_ms(500);
Usart_ON_Flag = 1;  // enable uart at boot
  while(1)
		{
				if(Flag_Show==0)           //使用MiniBalance APP和OLED显示屏
				{
					// APP_Show();	     // disable app show         
 				  oled_show();             //===显示屏打开
				}
				// CAN1_SEND();                //CAN发送	
				// PS2_Receive();             //PS2接收
				USART_TX();                //串口发送
				delay_flag=1;	
				delay_50=0;
				while(delay_flag);	       //通过MPU6050的INT中断实现的50ms精准延时				
	}
}

#include "sys.h"
#include "usart.h"	
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif


//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
		while((USART2->SR&0X40)==0);
		USART2->DR = (u8) ch;    	
  return ch;
}
#endif

int Usart_Receive;
/**************************实现函数**********************************************
*功    能:		usart1发送一个字节
*********************************************************************************/
void usart1_send(u8 data)
{
	USART1->DR = data;
	while((USART1->SR&0x40)==0);	
} 

//串口1中断服务程序
//bound:波特率
void uart_init(u32 bound){
   //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
	//串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); //GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); //GPIOA10复用为USART1
	//USART1端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA9，PA10
   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
  USART_Cmd(USART1, ENABLE);  //使能串口1 
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断
	//Usart NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}


/**************************************************************************
函数功能：串口1接收中断
入口参数：无
返回  值：无
**************************************************************************/
int USART1_IRQHandler(void)
{	
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) //接收到数据
	{	       
				u8 temp;
					static u8 count,last_data,last_last_data,Usart_ON_Count;
					if(Usart_ON_Flag==0)
					{	
						if(++Usart_ON_Count>10)Usart_ON_Flag=1;
					}
					temp=USART1->DR;
				   if(Usart_Flag==0)
						{	
						if(last_data==0xfe&&last_last_data==0xff) 
						Usart_Flag=1,count=0;	
						}
					 if(Usart_Flag==1)
						{	
							Urxbuf[count]=temp;     
							count++;                
							if(count==8){
								Usart_Flag=0;
								CAN_N_Usart_Control();  // no preemption thus no risk
							}
						}
						last_last_data=last_data;
						last_data=temp;
   }
return 0;	
}
/**************************实现函数**********************************************
*功    能:		usart2发送一个字节
*********************************************************************************/
void usart2_send(u8 data)
{
	USART2->DR = data;
	while((USART2->SR&0x40)==0);	
}

//串口2中断服务程序
//bound:波特率
void uart2_init(u32 bound){
 	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟
 
	//串口3对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2); //GPIOA3复用为USART2
	
	//端口配置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //GPIOA2 A3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); //初始化PA2 A3

   //USART2 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART2, &USART_InitStructure); //初始化串口
	
  USART_Cmd(USART2, ENABLE);  //使能串口
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
}

/**************************************************************************
函数功能：串口2接收中断
入口参数：无
返回  值：无
**************************************************************************/
int USART2_IRQHandler(void)
{	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET) //接收到数据
	{	      
		static u8 Flag_PID,i,j,Receive[50];
		static float Data;
		Usart_Receive=USART2->DR;
	
	  if(Usart_Receive==0x4B) Turn_Flag=1;  //进入转向控制界面
	  else	if(Usart_Receive==0x49||Usart_Receive==0x4A) 	 Turn_Flag=0;	//方向控制界面
		
		if(Run_Flag==0)//速度控制模式
		{			
				if(Turn_Flag==0)//速度控制模式
				{
						if(Usart_Receive>=0x41&&Usart_Receive<=0x48)  
						{	
							Flag_Direction=Usart_Receive-0x40;
						}
						else	if(Usart_Receive<=8)   
						{			
							Flag_Direction=Usart_Receive;
						}	
						else  Flag_Direction=0;
				}
				else	 if(Turn_Flag==1)//如果进入了转向控制界面
				 {
				 if(Usart_Receive==0x43) Flag_Left=0,Flag_Right=1;    
				 else if(Usart_Receive==0x47) Flag_Left=1,Flag_Right=0;
				 else Flag_Left=0,Flag_Right=0;
				 if(Usart_Receive==0x41||Usart_Receive==0x45)Flag_Direction=Usart_Receive-0x40;
				 else  Flag_Direction=0;
				 }
	  }	
		//以下是与APP调试界面通讯
		if(Usart_Receive==0x7B) Flag_PID=1;   //APP参数指令起始位
		if(Usart_Receive==0x7D) Flag_PID=2;   //APP参数指令停止位

		 if(Flag_PID==1)  //采集数据
		 {
			Receive[i]=Usart_Receive;
			i++;
		 }
		 if(Flag_PID==2)  //分析数据
		 {
			     if(Receive[3]==0x50) 	 PID_Send=1;
					 else  if(Receive[3]==0x57) 	 Flash_Send=1;
					 else  if(Receive[1]!=0x23) 
					 {								
						for(j=i;j>=4;j--)
						{
						  Data+=(Receive[j-1]-48)*pow(10,i-j);
						}
						switch(Receive[1])
						 {
							 case 0x30:  RC_Velocity=Data;break;
							 case 0x31:  RC_Position=Data;break;
							 case 0x32:  Position_KP=Data;break;
							 case 0x33:  Position_KI=Data;break;
							 case 0x34:  Position_KD=Data;break;
							 case 0x35:  Velocity_KP=Data;break;
							 case 0x36:  Velocity_KI=Data;break;
							 case 0x37:  break; //预留
							 case 0x38:  break; //预留
						 }
					 }				 
					 Flag_PID=0;//相关标志位清零
					 i=0;
					 j=0;
					 Data=0;
					 memset(Receive, 0, sizeof(u8)*50);//数组清零
		 } 	 
   }
return 0;	
}



/**************************************************************************
函数功能：串口扫描
**************************************************************************/
u8 click_RC (void)
{
			static u8 flag_key=1;//按键按松开标志
	    u8 temp;
			if(flag_key&&Usart_Receive!=0x5A)
			{
			flag_key=0;
		  if(Usart_Receive>=0x01&&Usart_Receive<=0x08)temp=Usart_Receive;
		  else	if(Usart_Receive>=0x41&&Usart_Receive<=0x48)temp=Usart_Receive-0x40;	
		//	else 	temp=0;
			return temp;	// 按键按下
			}
			else if(Usart_Receive==0x5A)			flag_key=1;
			return 0;//无按键按下
}
/**************************************************************************
函数功能：通过串口把自身的传感器发送出去
**************************************************************************/
void USART_TX(void)
{
        u8 Direction_A,Direction_B,Direction_C;
	      u16 Temp_GZ;
	           if(Encoder_A>0) Direction_A=0;
        else if(Encoder_A<0) Direction_A=2;
	      else                 Direction_A=1;
		         if(Encoder_B>0) Direction_B=0;
        else if(Encoder_B<0) Direction_B=2;
	      else                 Direction_B=1;     
		         if(Encoder_C>0) Direction_C=0;
        else if(Encoder_C<0) Direction_C=2;
	      else                 Direction_C=1;
      	Temp_GZ=Gryo_Z+32768;
	
				usart1_send(0xff);		
				usart1_send(0xfe);	
				usart1_send(abs(Encoder_A));		
				usart1_send(Direction_A);	
				usart1_send(abs(Encoder_B));		
				usart1_send(Direction_B);	
				usart1_send(abs(Encoder_C));		
				usart1_send(Direction_C);	
				usart1_send(Temp_GZ>>8);		
				usart1_send(Temp_GZ&0x00ff);	
				
				usart3_send(0xff);		
				usart3_send(0xfe);	
				usart3_send(abs(Encoder_A));		
				usart3_send(Direction_A);	
				usart3_send(abs(Encoder_B));		
				usart3_send(Direction_B);	
				usart3_send(abs(Encoder_C));		
				usart3_send(Direction_C);	
				usart3_send(Temp_GZ>>8);		
				usart3_send(Temp_GZ&0x00ff);	
}



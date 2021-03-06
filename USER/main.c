#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "hc05.h" 
#include "key.h"
#include "lcd.h"
#include "usart3.h" 	
#include "usart2.h"
#include "usart6.h"
#include "string.h"	
#include "common.h"


//ALIENTEK 探索者STM32F407开发板 实验13
//LCD显示实验-库函数版本
//技术支持：www.openedv.com
//淘宝店铺：http://eboard.taobao.com  
//广州市星翼电子科技有限公司  
//作者：正点原子 @ALIENTEK



int main(void){ 
		u16 t=999;		//加速第一次获取链接状态
		u8 constate=0;	//连接状态
		u8 is_wifi=1;
		u8 blood_tag=0;
		u8 key;
		u8 command;
		u8 p[32]={0};
		u8 temperature_len=0;
		u8 blood_len=0;
		u8 wifi_len=0;
		u8 *sendLen;
		u8 reclen=0;  
		//u8 lcd_id[12];				//存放LCD ID字符串
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
		delay_init(168);      //初始化延时函数
		uart_init(115200);		//初始化串口波特率为115200
		
		KEY_Init();             //初始化与按键连接的硬件接口
		LED_Init();					  //初始化LED
		LCD_Init();           //初始化LCD FSMC接口
		usart2_init(9600);    //初始化串口3波特率为9600
		usart6_init(9600);    //初始化串口3波特率为9600
	
		POINT_COLOR=RED;      //画笔颜色：红色
		delay_ms(1000);			//等待蓝牙模块上电稳定
	
		
	  LCD_ShowString(30,30,200,16,16,"KEY1 : measure blood pressure");	
	  LCD_ShowString(30,50,300,16,16,"KEY2 : up message to app");
		LCD_ShowString(30,90,300,16,16,"KEY_UP : change to blueteeth");
		
		delay_ms(100);
		USART3_RX_STA=0;
		sprintf(USART2_RX_BUF,"U1:155,125,83,S2\r\na");
mode:
		if(is_wifi){
			  atk_8266_test();
		}else{
				 while(HC05_Init()){
							LCD_ShowString(30,140,200,16,16,"ATK-HC05 Error!"); 
							delay_ms(500);
							LCD_ShowString(30,140,200,16,16,"Please Check!!!"); 
							delay_ms(100);
					}	
		}
		
		LCD_Fill(30,140,230,156,WHITE);
		printf("初始化完毕\r\n");


		while(1){		
				key=KEY_Scan(0);		//得到键值
				if(key==1){
						printf("KEY0修好了？\r\n");
				}else	if(key==2) {
						printf("开始测量血压\r\n");
						u2_printf("%s\r\n","AT+SP:200");
						delay_ms(100);							
				}else if(key==3||command){
						if(is_wifi){
									sprintf((char*)p,USART6_RX_BUF);//测试数据
									printf("温度数据的长度：%d\r\n",temperature_len);	
									atk_8266_send_cmd("AT+CIPSEND=0,34","OK",200);  //发送指定长度的数据
									delay_ms(200);
									atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
									printf("发送体温数据\r\n");		
									delay_ms(300);
									sprintf((char*)p,USART2_RX_BUF);//测试数据
									printf("血压数据的长度：%d\r\n",strlen(USART_RX_BUF));
									atk_8266_send_cmd("AT+CIPSEND=0,19","OK",200);  //发送指定长度的数据
									delay_ms(200);
									atk_8266_send_data(p,"OK",100);  //发送指定长度的数据
									
									printf("发送血压数据\r\n");						
						}else{
								u3_printf("%s\r\n",USART2_RX_BUF);
								printf("send bluetooth temperature data\r\n");
								u3_printf("%s\r\n",USART6_RX_BUF);
								printf("send bluetooth blood pressure data\r\n");
								delay_ms(100);
							
						}
						command=0;
				}else if(key==4){		
						if(is_wifi){
								is_wifi=0;
								LCD_ShowString(30,90,300,16,16,"KEY_UP : change to blueteeth");
								goto mode;
								
						}else{						
								is_wifi=1;
								LCD_ShowString(30,90,300,16,16,"KEY_UP : change to wifi");
								goto mode;
						}						
							
				}


				if(USART2_RX_STA&0X8000){					
						blood_len=USART2_RX_STA&0X7FFF;	
						USART2_RX_BUF[blood_len]='a';
						USART2_RX_BUF[blood_len+1]=0;			
						LCD_ShowString(30,200,209,119,16,USART2_RX_BUF);
						USART2_RX_STA=0;
						blood_len+=1;					
				}	 		

				if(USART6_RX_STA&0X8000){				
						temperature_len=USART6_RX_STA&0X7FFF;	
						USART6_RX_BUF[temperature_len]='q';
						USART6_RX_BUF[temperature_len+1]=0;	
						LCD_ShowString(30,280,209,119,16,USART6_RX_BUF);
						USART6_RX_STA=0;
						temperature_len+=1;
											
				}
				
				if(USART3_RX_STA&0X8000){ 						
							wifi_len=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
							USART3_RX_BUF[wifi_len]=0;		//添加结束符 
							if(!strcmp((const char*)USART3_RX_BUF,"COMMAND")){
									command=1;
									LED0=!LED0;
									LED1=!LED1;
							}
							
							printf("接收到的数据：%s\r\n",USART3_RX_BUF);	//发送到串口  
							USART3_RX_STA=0;
					
				}  

			
	
		} 
}

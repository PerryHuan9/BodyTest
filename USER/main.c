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


//ALIENTEK ̽����STM32F407������ ʵ��13
//LCD��ʾʵ��-�⺯���汾
//����֧�֣�www.openedv.com
//�Ա����̣�http://eboard.taobao.com  
//������������ӿƼ����޹�˾  
//���ߣ�����ԭ�� @ALIENTEK



int main(void){ 
		u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
		u8 constate=0;	//����״̬
		u8 is_wifi=1;
		u8 blood_tag=0;
		u8 key;
		u8 p[32]={0};
		u8 temperature_len=0;
		u8 blood_len=0;
		u8 wifi_len=0;
		u8 *sendLen;
		u8 reclen=0;  
		//u8 lcd_id[12];				//���LCD ID�ַ���
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
		delay_init(168);      //��ʼ����ʱ����
		uart_init(115200);		//��ʼ�����ڲ�����Ϊ115200
		
		KEY_Init();             //��ʼ���밴�����ӵ�Ӳ���ӿ�
		LED_Init();					  //��ʼ��LED
		LCD_Init();           //��ʼ��LCD FSMC�ӿ�
		usart2_init(9600);    //��ʼ������3������Ϊ9600
		usart6_init(9600);    //��ʼ������3������Ϊ9600
	
		POINT_COLOR=RED;      //������ɫ����ɫ
		delay_ms(1000);			//�ȴ�����ģ���ϵ��ȶ�
	
		
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
		
    
		printf("��ʼ�����\r\n");


		while(1){		
				key=KEY_Scan(0);		//�õ���ֵ
				if(key==1){
						printf("KEY0�޺��ˣ�\r\n");
				}else	if(key==2) {
						printf("��ʼ����Ѫѹ\r\n");
						u2_printf("%s\r\n","AT+SP:200");
						delay_ms(100);							
				}else if(key==3){
						if(is_wifi){
									sprintf((char*)p,USART6_RX_BUF);//��������
									printf("�¶����ݵĳ��ȣ�%d\r\n",temperature_len);	
									atk_8266_send_cmd("AT+CIPSEND=0,34","OK",200);  //����ָ�����ȵ�����
									delay_ms(200);
									atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
									printf("������������\r\n");		
									delay_ms(300);
									sprintf((char*)p,USART2_RX_BUF);//��������
									printf("Ѫѹ���ݵĳ��ȣ�%d\r\n",strlen(USART_RX_BUF));
									atk_8266_send_cmd("AT+CIPSEND=0,19","OK",200);  //����ָ�����ȵ�����
									delay_ms(200);
									atk_8266_send_data(p,"OK",100);  //����ָ�����ȵ�����
									printf("����Ѫѹ����\r\n");						
						}else{
								u3_printf("%s\r\n",USART2_RX_BUF);
								printf("send bluetooth temperature data\r\n");
								u3_printf("%s\r\n",USART6_RX_BUF);
								printf("send bluetooth blood pressure data\r\n");
								delay_ms(100);
						}
				}else if(key==4){		
						if(is_wifi){
//								 while(HC05_Init()){
//										LCD_ShowString(30,140,200,16,16,"ATK-HC05 Error!"); 
//										delay_ms(500);
//										LCD_ShowString(30,140,200,16,16,"Please Check!!!"); 
//										delay_ms(100);
//								}	
								is_wifi=0;
								LCD_ShowString(30,90,300,16,16,"KEY_UP : change to blueteeth");
								goto mode;
								
						}else{
						 // atk_8266_test();
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
							wifi_len=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
							USART3_RX_BUF[temperature_len]=0;		//��ӽ����� 
							printf("���յ������ݣ�%s\r\n",USART3_RX_BUF);	//���͵�����  
							USART3_RX_STA=0;
					
				}  

			
	
		} 
}

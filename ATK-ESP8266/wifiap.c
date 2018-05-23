#include "common.h"



//ATK-ESP8266 WIFI AP����
//���ڲ���TCP/UDP����
//����ֵ:0,����
//    ����,�������
u8 atk_8266_wifiap_test(void)
{
	
	u8 key;
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP����
	u8 p1[32]={0};
	u16 t=999;		//���ٵ�һ�λ�ȡ����״̬
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//����״̬
	//p=mymalloc(SRAMIN,32);							//����32�ֽ��ڴ�
	
//	printf("��ʼ����ΪAPģʽ\r\n");
//	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0�������ӣ�1��������		
//	sprintf((char*)p1,"AT+CIPSERVER=1,%s",(u8*)portnum);
//	atk_8266_send_cmd(p1,"OK",20);     //����Serverģʽ���˿ں�Ϊ8086		
//	delay_ms(200);
//	atk_8266_get_wanip(ipbuf);//������ģʽ,��ȡWAN IP
//	USART3_RX_STA=0;
	while(1)
	{
			key=KEY_Scan(0);
			if(key==WKUP_PRES)			//WK_UP �˳�����		 
			{  
				res=0;
				atk_8266_quit_trans();	//�˳�͸��
				atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //�ر�͸��ģʽ
				break;												 
			}
			else if(key==KEY1_PRES)	//KEY0 �������� 
			{
					sprintf((char*)p1,"ATK-8266%s����%02d\r\n",ATK_ESP8266_WORKMODE_TBL[0],t/10);//��������
					atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //����ָ�����ȵ�����
					delay_ms(200);
					atk_8266_send_data(p1,"OK",100);  //����ָ�����ȵ�����
					timex=100;
			}
		
//			if(timex)timex--;
//			if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
			t++;
			delay_ms(5);
			
			if(USART3_RX_STA&0X8000)		//���յ�һ��������
			{ 
				rlen=USART3_RX_STA&0X7FFF;	//�õ����ν��յ������ݳ���
				USART3_RX_BUF[rlen]=0;		//��ӽ����� 
				printf("�յ������ݣ�%s\r\n",USART3_RX_BUF);	//���͵�����   
				printf("���յ������ݳ���:%d\r\n",rlen);	
				
				USART3_RX_STA=0;
				if(constate!='+')t=1000;		//״̬Ϊ��δ����,������������״̬
				else t=0;                   //״̬Ϊ�Ѿ�������,10����ټ��
			}  

			if(t==1000)//����10����û���յ��κ�����,��������ǲ��ǻ�����.
			{
				constate=atk_8266_consta_check();//�õ�����״̬
				if(constate=='+')printf("���ӳɹ�\r\n");  //����״̬
				else printf("����ʧ��\r\n"); 	  	 
				t=0;
			}
			if((t%20)==0)LED0=!LED0;
		//	atk_8266_at_response(1);
	}
	
	return res;		
} 








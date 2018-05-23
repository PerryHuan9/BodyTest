#include "common.h"



//ATK-ESP8266 WIFI AP测试
//用于测试TCP/UDP连接
//返回值:0,正常
//    其他,错误代码
u8 atk_8266_wifiap_test(void)
{
	
	u8 key;
	u8 timex=0; 
	u8 ipbuf[16]; 	//IP缓存
	u8 p1[32]={0};
	u16 t=999;		//加速第一次获取链接状态
	u8 res=0;
	u16 rlen=0;
	u8 constate=0;	//连接状态
	//p=mymalloc(SRAMIN,32);							//申请32字节内存
	
//	printf("开始设置为AP模式\r\n");
//	atk_8266_send_cmd("AT+CIPMUX=1","OK",20);   //0：单连接，1：多连接		
//	sprintf((char*)p1,"AT+CIPSERVER=1,%s",(u8*)portnum);
//	atk_8266_send_cmd(p1,"OK",20);     //开启Server模式，端口号为8086		
//	delay_ms(200);
//	atk_8266_get_wanip(ipbuf);//服务器模式,获取WAN IP
//	USART3_RX_STA=0;
	while(1)
	{
			key=KEY_Scan(0);
			if(key==WKUP_PRES)			//WK_UP 退出测试		 
			{  
				res=0;
				atk_8266_quit_trans();	//退出透传
				atk_8266_send_cmd("AT+CIPMODE=0","OK",20);   //关闭透传模式
				break;												 
			}
			else if(key==KEY1_PRES)	//KEY0 发送数据 
			{
					sprintf((char*)p1,"ATK-8266%s测试%02d\r\n",ATK_ESP8266_WORKMODE_TBL[0],t/10);//测试数据
					atk_8266_send_cmd("AT+CIPSEND=0,25","OK",200);  //发送指定长度的数据
					delay_ms(200);
					atk_8266_send_data(p1,"OK",100);  //发送指定长度的数据
					timex=100;
			}
		
//			if(timex)timex--;
//			if(timex==1)LCD_Fill(30+54,100,239,112,WHITE);
			t++;
			delay_ms(5);
			
			if(USART3_RX_STA&0X8000)		//接收到一次数据了
			{ 
				rlen=USART3_RX_STA&0X7FFF;	//得到本次接收到的数据长度
				USART3_RX_BUF[rlen]=0;		//添加结束符 
				printf("收到的数据：%s\r\n",USART3_RX_BUF);	//发送到串口   
				printf("接收到的数据长度:%d\r\n",rlen);	
				
				USART3_RX_STA=0;
				if(constate!='+')t=1000;		//状态为还未连接,立即更新连接状态
				else t=0;                   //状态为已经连接了,10秒后再检查
			}  

			if(t==1000)//连续10秒钟没有收到任何数据,检查连接是不是还存在.
			{
				constate=atk_8266_consta_check();//得到连接状态
				if(constate=='+')printf("连接成功\r\n");  //连接状态
				else printf("连接失败\r\n"); 	  	 
				t=0;
			}
			if((t%20)==0)LED0=!LED0;
		//	atk_8266_at_response(1);
	}
	
	return res;		
} 








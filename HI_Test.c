/*        
  �������������
*/        
#include <rs232.h>
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>
#include "FiberSensor.h"
#include "variable.h"
static giCurrentRes = 1;
static step =0;

/********** mmsun add  ***************/
/* �����ͨ������ 
** intput: ͨ�����  */
int ChannlHiCurTest(int Channel);

/* �պϴ�������Լ̵��� 
** intput: ͨ�����   */
int CloseHIRelay(int RelayChannel);   

/* �Ͽ���������Լ̵�����
��Ϊû��ר�ŵĹر�ĳһͨ����ָ�
���ر����еļ̵��� 
** intput: ͨ�����   */
int OpenHIRelay();  

/* �������������ʼ���� 
** */
int StartCurrent();

/* ��������� 
** */
int ReadCurrent();

/* �����������ֹͣ���� 
** */
int StopCurrent(); 


//�˳�����
int CVICALLBACK PANEL_8_Exit (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_HITest);
			break;
	}
	return 0;
}

//������һ��������ֵ����
int CVICALLBACK PANEL_8_PreTest (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DisplayPanel(panelHandle);
			HidePanel(pH_HITest);
			break;
	}
	return 0;
}

//����һ������ѹ����
int CVICALLBACK PANEL_8_NestTest (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_HITest);      
			sprintf(gComBufT, "[M]!FW#");
			i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
			if(i<0) 
				return -1;
			InstallPopup(pH_HVTest);  
			
			break;
	}
	return 0;
}			  

 // ˫��ĳ�����������û�������
int CVICALLBACK PANEL_Table3 (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	Point MyPoint;
	int i;
	char sTxt[30];
	switch (event)
	{
		case EVENT_LEFT_DOUBLE_CLICK:
			GetActiveTableCell(panel, control, &MyPoint);
			if(MyPoint.x >=8)
			{
				MessagePopup("��ʾ","������������֧��7��������");
				return -1;
			}
			if((giChanResSel[MyPoint.x-1]!=1) && (giChanResSel[MyPoint.x-1]!=2) )   
				return -1;
			giCurrentRes = MyPoint.x; 
			
			sprintf(sTxt, "����%d������", giCurrentRes);
			SetCtrlAttribute (pH_HITest, PANEL_8_NUMERICSLIDE, ATTR_LABEL_TEXT, sTxt);
			sprintf(sTxt, "����%d�����", giCurrentRes);
			SetCtrlAttribute (pH_HITest, PANEL_8_TEXTMSG_5, ATTR_CTRL_VAL, sTxt);
			
			sprintf(gComBufT, "[M]>CU%d#", giCurrentRes-1);
			i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
			if(i<0)
				return -1;
			Delay(1.0);
			
			ResetTimer(pH_HITest, PANEL_8_TIMER);
			SetCtrlAttribute (pH_HITest, PANEL_8_TIMER, ATTR_ENABLED, 1);   
			break;
	}
	return 0;
}

//����������������� ��ʼ����
int CVICALLBACK PANEL_8_Start_Test (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	int i,iStatus;
	switch (event)
	{
		case EVENT_COMMIT:
			step = 0;   
			ResetTimer(pH_HITest, PANEL_8_TIMER);
			SetCtrlAttribute (pH_HITest, PANEL_8_TIMER, ATTR_ENABLED, 1);   
			
			/*
			gComBufT[0] = 0x05;
			gComBufT[1] = 0x16;
			gComBufT[2] = 0x00;
			gComBufT[3] = 0x01;
			gComBufT[4] = 0x00;
			gComBufT[5] = 0x00;
			gComBufT[6] = 0x18;
			gComBufT[7] = 0x4D;
			i = SendComCMD(gComHI, 8, gComBufT); //�����������
			if(i<0)
			{
				MessagePopup("��ʾ","������������ʧ�ܣ����飡");
				return -1;
			}
			
			i = ComRd (gComHI, gComBufR, 20); //�����������  
			if(i<8)
			{
				MessagePopup("��ʾ","������������ʧ�ܣ����飡");     
				return -1;
			}
			
			iStatus = (gComBufR[i-1]== gComBufT[7]) & (gComBufR[i-2]== gComBufT[6]) & (gComBufR[i-3]== gComBufT[5]) & (gComBufR[i-4]== gComBufT[4]) & (gComBufR[i-5]== gComBufT[3]) & (gComBufR[i-6]== gComBufT[2]) & (gComBufR[i-7]== gComBufT[1]) &(gComBufR[i-8]== gComBufT[0]);
			if(iStatus==0)
			{
				MessagePopup("��ʾ","������������ʧ�ܣ����飡");     
				return -1;
			}
			FlushInQ(gComHI);//�����������  
			FlushOutQ(gComHI); //�����������  
			MessagePopup("��ʾ","���������ɹ���");
			*/
			break;
	}
	return 0;
}

 //���õ���
int CVICALLBACK PANEL_8_Set_HI (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	int i,iStatus;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(pH_HITest, PANEL_8_NUMERICMETER, &i);
			if(i>200)
				return -1;
			gComBufT[0] = 0x05;
			gComBufT[1] = 0x16;
			gComBufT[2] = 0x00;
			gComBufT[3] = 0x03;
			gComBufT[4] = (i>255)?(i/256):0;
			gComBufT[5] = (i>255)?(i%256):i;
			gComBufT[6] = 0xB9;
			gComBufT[7] = 0x5F;
			i = SendComCMD(gComHI, 8, gComBufT); //�����������  
			if(i<0)
			{
				MessagePopup("��ʾ","���õ���ֵʧ�ܣ����飡");
				return -1;
			}
			
			i = ComRd (gComHI, gComBufR, 20); //�����������  
			if(i<8)
			{
				MessagePopup("��ʾ","���õ���ֵʧ�ܣ����飡");     
				return -1;
			}
			
			iStatus = (gComBufR[i-5]== gComBufT[3]) & (gComBufR[i-6]== gComBufT[2]) & (gComBufR[i-7]== gComBufT[1]) &(gComBufR[i-8]== gComBufT[0]);
			if(iStatus==0)
			{
				MessagePopup("��ʾ","���õ���ֵʧ�ܣ����飡");     
				return -1;
			}
			FlushInQ(gComHI);   //�����������  
			FlushOutQ(gComHI);  //�����������  
			MessagePopup("��ʾ","���õ���ֵ�ɹ���");
			break;
	}
	return 0;
}

 //�رյ���������
int CVICALLBACK PANEL_8_Stop_Test (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	int i,iStatus;
	switch (event)
	{
		case EVENT_COMMIT: 
			SetCtrlVal(pH_HITest, PANEL_8_NUMERICSLIDE, 0.0); 
			SetCtrlAttribute (pH_HITest, PANEL_8_TIMER, ATTR_ENABLED, 0); 
			 StopCurrent();
			 /*
			gComBufT[0] = 0x05;
			gComBufT[1] = 0x16;
			gComBufT[2] = 0x00;
			gComBufT[3] = 0x02;
			gComBufT[4] = 0x00;
			gComBufT[5] = 0x00;
			gComBufT[6] = 0xE8;
			gComBufT[7] = 0x4D;
			i = SendComCMD(gComHI, 8, gComBufT); //�����������  
			if(i<0)
			{
				MessagePopup("��ʾ","ֹͣ��������ʧ�ܣ����飡");
				return -1;
			}
			
			i = ComRd (gComHI, gComBufR, 20); //�����������  
			if(i<8)
			{
				MessagePopup("��ʾ","ֹͣ��������ʧ�ܣ����飡");     
				return -1;
			}
			
			iStatus = (gComBufR[i-1]== gComBufT[7]) & (gComBufR[i-2]== gComBufT[6]) & (gComBufR[i-3]== gComBufT[5]) & (gComBufR[i-4]== gComBufT[4]) & (gComBufR[i-5]== gComBufT[3]) & (gComBufR[i-6]== gComBufT[2]) & (gComBufR[i-7]== gComBufT[1]) &(gComBufR[i-8]== gComBufT[0]);
			if(iStatus==0)
			{
				MessagePopup("��ʾ","ֹͣ��������ʧ�ܣ����飡");     
				return -1;
			}
			FlushInQ(gComHI); //�����������  
			FlushOutQ(gComHI);
			MessagePopup("��ʾ","ֹͣ��������ɹ���");  */
			break;
	}
	return 0;
}

//�������Խ����ͨ��
int CVICALLBACK PANEL_8_PASS (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetTableCellAttribute (pH_HITest, PANEL_8_TABLE_4, MakePoint(giCurrentRes,1), ATTR_CTRL_VAL, "Y");    
			SetTableCellRangeAttribute (pH_HITest, PANEL_8_TABLE_4, MakeRect(1,giCurrentRes,1,1), ATTR_TEXT_BGCOLOR, VAL_GREEN);
			break;
	}
	return 0;
}

//�������Խ����δͨ��
int CVICALLBACK PANEL_8_UNPASS (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetTableCellAttribute (pH_HITest, PANEL_8_TABLE_4, MakePoint(giCurrentRes,1), ATTR_CTRL_VAL, "N");    
			SetTableCellRangeAttribute (pH_HITest, PANEL_8_TABLE_4, MakeRect(1,giCurrentRes,1,1), ATTR_TEXT_BGCOLOR, VAL_RED);
			break;
	}
	return 0;
}

//�����ÿ��״̬��ʾ
//int step =0;
int CVICALLBACK PANEL_8_Progress (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	int iColor;
	double fProgress,fTime;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			{
			  ChannlHiCurTest(1);	
			}
			/*
			GetTableCellAttribute (pH_HITest, PANEL_8_TABLE_3, MakePoint(giCurrentRes,1), ATTR_CMD_BUTTON_COLOR, &iColor);
			SetTableCellAttribute (pH_HITest, PANEL_8_TABLE_3, MakePoint(giCurrentRes,1), ATTR_CMD_BUTTON_COLOR, (iColor==VAL_YELLOW)?VAL_GREEN:VAL_YELLOW); 
		
			GetCtrlVal(pH_HITest, PANEL_8_NUMERIC, &fTime);  
			GetCtrlVal(pH_HITest, PANEL_8_NUMERICSLIDE, &fProgress);

			if(fProgress>=1)
			{
				SetTableCellAttribute (pH_HITest, PANEL_8_TABLE_3, MakePoint(giCurrentRes,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE);  
				SetCtrlVal(pH_HITest, PANEL_8_NUMERICSLIDE, 0.0); 
				SetCtrlAttribute (pH_HITest, PANEL_8_TIMER, ATTR_ENABLED, 0); 
				
				sprintf(gComBufT, "[M]!FW#");
				SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
				//�Ͽ����м̵���         
				return 0;
			}
			SetCtrlVal(pH_HITest, PANEL_8_NUMERICSLIDE, ((fProgress+1.0/fTime)>1)?1:(fProgress+1.0/fTime) ); 
			*/
			break;
	}
	return 0;
}

int ChannlHiCurTest(int Channel)
{
	double fProgress,fTime;
    int iCurrent;  
	
	switch(step)
	{
		case 0: //close relay
			{
			  CloseHIRelay(Channel-1);
			  step =1;
			}
			break;
		case 1: //start current
			{
			  StartCurrent();
			  step = 2;
			}
			break;
		case 2:// testing
			{
			  iCurrent = ReadCurrent();
			  SetCtrlVal(pH_HITest, PANEL_8_NUMERICMETER, iCurrent); 
			  GetCtrlVal(pH_HITest, PANEL_8_NUMERIC, &fTime);  
			  GetCtrlVal(pH_HITest, PANEL_8_NUMERICSLIDE, &fProgress);
			  SetCtrlVal(pH_HITest, PANEL_8_NUMERICSLIDE, ((fProgress+1.0/fTime)>1)?1:(fProgress+1.0/fTime) );
			  if(fProgress>=1)
			    {
				  SetTableCellAttribute (pH_HITest, PANEL_8_TABLE_3, MakePoint(giCurrentRes,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE);  
				  SetCtrlVal(pH_HITest, PANEL_8_NUMERICSLIDE, 0.0); 
				  //SetCtrlAttribute (pH_HITest, PANEL_8_TIMER, ATTR_ENABLED, 0); 
				
				  //sprintf(gComBufT, "[M]!FW#");
				  //SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
				  //�Ͽ����м̵���  
				  	step = 3;  
				  return 0;
			    }
			  
				
			}
			break;
		case 3:  // stop current
			{
				StopCurrent();
				step = 4;
			}
			break;
		case 4: // open relay,update stauts
			{
			   OpenHIRelay();
			   //step = 0;
			}
			break;
	}
	
	return 0;
}

int CloseHIRelay(int RelayChannel )
{
	int i;
	sprintf(gComBufT, "[M]>CU%d#", RelayChannel);
	i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
    if(i<0)
	  return -1;
	return 0;
}

int OpenHIRelay()
{
	sprintf(gComBufT, "[M]!FW#");
	SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
	return 0;
	
}

int StartCurrent()
{	 	
	int i,iStatus; 
			gComBufT[0] = 0x05;
			gComBufT[1] = 0x16;
			gComBufT[2] = 0x00;
			gComBufT[3] = 0x01;
			gComBufT[4] = 0x00;
			gComBufT[5] = 0x00;
			gComBufT[6] = 0x18;
			gComBufT[7] = 0x4D;
			i = SendComCMD(gComHI, 8, gComBufT); //�����������
			if(i<0)
			{
				MessagePopup("��ʾ","������������ʧ�ܣ����飡");
				return -1;
			}
			
			i = ComRd (gComHI, gComBufR, 20); //�����������  
			if(i<8)
			{
				MessagePopup("��ʾ","������������ʧ�ܣ����飡");     
				return -1;
			}
			
			iStatus = (gComBufR[i-1]== gComBufT[7]) & (gComBufR[i-2]== gComBufT[6]) & (gComBufR[i-3]== gComBufT[5]) & (gComBufR[i-4]== gComBufT[4]) & (gComBufR[i-5]== gComBufT[3]) & (gComBufR[i-6]== gComBufT[2]) & (gComBufR[i-7]== gComBufT[1]) &(gComBufR[i-8]== gComBufT[0]);
			if(iStatus==0)
			{
				MessagePopup("��ʾ","������������ʧ�ܣ����飡");     
				return -1;
			}
			FlushInQ(gComHI);//�����������  
			FlushOutQ(gComHI); //�����������  
			MessagePopup("��ʾ","�����������ɹ���");
  return 0;
}

int  ReadCurrent()
{
	int OutCurrent=0;
	
	int i,iStatus; 
			gComBufT[0] = 0x05;
			gComBufT[1] = 0x03;
			gComBufT[2] = 0x00;
			gComBufT[3] = 0x03;
			gComBufT[4] = 0x00;
			gComBufT[5] = 0x00;
			gComBufT[6] = 0XB4;
			gComBufT[7] = 0x4E;
			i = SendComCMD(gComHI, 8, gComBufT); //�����������
			if(i<0)
			{
				MessagePopup("��ʾ","��������������ʧ��1�����飡");
				return -1;
			}
			
			Delay(0.5);
			
			i = ComRd (gComHI, gComBufR, 20); //�����������  
			if(i<8)
			{  
				FlushInQ(gComHI);//�����������  
			    FlushOutQ(gComHI); //����������� 
				MessagePopup("��ʾ","��������������ʧ��2�����飡");     
				return -1;
			}
			
			iStatus = /*(gComBufR[i-1]== gComBufT[7]) & (gComBufR[i-2]== gComBufT[6]) & (gComBufR[i-3]== gComBufT[5]) & (gComBufR[i-4]== gComBufT[4]) & (gComBufR[i-5]== gComBufT[3]) & (gComBufR[i-6]== gComBufT[2]) & */(gComBufR[i-7]== gComBufT[1]) &(gComBufR[i-8]== gComBufT[0]);
			if(iStatus==0)
			{
				MessagePopup("��ʾ","��������������ʧ��3�����飡");     
				return -1;
			}
			else
			{
			 OutCurrent = 	(gComBufR[i-4]*256 +gComBufR[i-3])/10;  
			}
			FlushInQ(gComHI);//�����������  
			FlushOutQ(gComHI); //�����������  
		//	MessagePopup("��ʾ","�����������ɹ���");
	
	return OutCurrent;
}

int StopCurrent()
{
	int i,iStatus;
	gComBufT[0] = 0x05;
	gComBufT[1] = 0x16;
	gComBufT[2] = 0x00;
	gComBufT[3] = 0x02;
	gComBufT[4] = 0x00;
	gComBufT[5] = 0x00;
	gComBufT[6] = 0xE8;
	gComBufT[7] = 0x4D;
	i = SendComCMD(gComHI, 8, gComBufT); //�����������  
	if(i<0)
	   {
		 MessagePopup("��ʾ","ֹͣ��������ʧ�ܣ����飡");
			return -1;
	   }
			
	i = ComRd (gComHI, gComBufR, 20); //�����������  
	if(i<8)
	  {
		MessagePopup("��ʾ","ֹͣ��������ʧ�ܣ����飡");     
		  return -1;
	   }
			
	iStatus = (gComBufR[i-1]== gComBufT[7]) & (gComBufR[i-2]== gComBufT[6]) & (gComBufR[i-3]== gComBufT[5]) & (gComBufR[i-4]== gComBufT[4]) & (gComBufR[i-5]== gComBufT[3]) & (gComBufR[i-6]== gComBufT[2]) & (gComBufR[i-7]== gComBufT[1]) &(gComBufR[i-8]== gComBufT[0]);
	if(iStatus==0)
	   {
			MessagePopup("��ʾ","ֹͣ��������ʧ�ܣ����飡");     
			return -1;
	   }
	FlushInQ(gComHI); //�����������  
	FlushOutQ(gComHI);
    MessagePopup("��ʾ","ֹͣ��������ɹ���");
	
	return 0;
	
}

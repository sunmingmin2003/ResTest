/*        
  ������ѹ����
*/        
#include <utility.h>
#include <rs232.h>
#include <ansi_c.h>
#include <userint.h>
#include "FiberSensor.h"
#include "variable.h"
static int giCurrentHVChan = -1;   
int giChanTmp;
int gHVGNDSel;

//�˳�����
int CVICALLBACK PANEL_7_Exit (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_HVTest);
			break;
	}
	return 0;
}

//�л�����һ������Ե����
int CVICALLBACK PANEL_7_HI_Test (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_HVTest);   
			sprintf(gComBufT, "[M]!FW#");
			i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);			
			if(i<0)
				return -1;
			InstallPopup(pH_InsulationTest); //��Ե����
			for(i=0;i<20;i++)
			{
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_4, MakePoint(i,1), ATTR_CTRL_VAL, 0.0);    
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_4, MakePoint(i,2), ATTR_CTRL_VAL, 0.0);  
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_5, MakePoint(i,1), ATTR_CTRL_VAL, 0.0);    
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_5, MakePoint(i,2), ATTR_CTRL_VAL, 0.0);  
			}
			
			break;
	}
	return 0;
}

//������һ��
int CVICALLBACK PANEL_7_PreTest (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_HVTest); 
			InstallPopup(pH_HITest);  //���������
			break;
	}
	return 0;
}

//������ѹ������
int CVICALLBACK PANEL_7_Start_Device (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf(gComBufT, "#G");
			i = SendComCMD(gComHV, 2, gComBufT); 
			if(i<0)
			{
				MessagePopup("��ʾ","��ѹ��������ʧ�ܣ����飡");
				return -1;
			}
			
			i = ComRd (gComHV, gComBufR, 20);
			if(i<4)
			{
				MessagePopup("��ʾ","��ѹ��������ʧ�ܣ����飡");     
				return -1;
			}
			
			FlushInQ(gComHV);
			FlushOutQ(gComHV);
			MessagePopup("��ʾ","��ѹ�����ɹ���");
			break;
	}
	return 0;
}

//��ʾ�������Խ���
int CVICALLBACK PANEL_7_Progrocess (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	int iColor,i;
	double fProgress;
	int iChan;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			iChan = giCurrentHVChan+1;
			GetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_3, MakePoint(iChan,1), ATTR_CMD_BUTTON_COLOR, &iColor);
			SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_3, MakePoint(iChan,1), ATTR_CMD_BUTTON_COLOR, (iColor==VAL_YELLOW)?VAL_GREEN:VAL_YELLOW); 
			GetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE, &fProgress);
			GetCtrlVal(pH_HVTest, PANEL_7_NUMERIC, &i);   //ÿ����������ʱ��     
			SetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE, ((fProgress+1.0/i)>1)?1:(fProgress+1.0/i) ); 
			break;
	}
	return 0;
}

//�رո�ѹ������
int CVICALLBACK PANEL_7_Close_Device(int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf(gComBufT, "#U");
			i = SendComCMD(gComHV, 2, gComBufT); 
			if(i<0)
			{
				MessagePopup("��ʾ","��ѹֹͣ����ʧ�ܣ����飡");
				return -1;
			}
			
			i = ComRd (gComHV, gComBufR, 20);
			if(i<4)
			{
				MessagePopup("��ʾ","��ѹֹͣ����ʧ�ܣ����飡");     
				return -1;
			}
			
			FlushInQ(gComHV);
			FlushOutQ(gComHV);
			MessagePopup("��ʾ","��ѹֹͣ�ɹ���");			
			break;
	}
	return 0;
}

 //������ѹ���Ե�ѹֵ
int CVICALLBACK PANEL_7_Set_HV (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{		
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(pH_HVTest, PANEL_7_NUMERICMETER, &i);
			i = i/4;
			sprintf(gComBufT, "%s%04d", "#S0500",i);
			i = SendComCMD(gComHV, strlen(gComBufT), gComBufT); 
			i = ComRd (gComHV, gComBufR, 20);
			if(i<4)
			{
				MessagePopup("��ʾ","��ѹ��������ʧ�ܣ����飡");     
				return -1;
			}	
			MessagePopup("��ʾ","��ѹ��������ɹ�");  			
			break;
	}						   
	return 0;
}
 //�Զ����Ի�����ѹ
int CVICALLBACK PANEL_7_Auto_Start (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_INTERVAL, 1.0);//����ִ��
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_ENABLED, 1); 
			gHVGNDSel = 0;
			break;
	}
	return 0;
}

 //�Զ�����,��ʱ��PANEL_7�Ļص�����
int CVICALLBACK PANEL_7_Auto_Test (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{  	int i,j;
	char sTxt[50];
	switch (event)
	{
		case EVENT_TIMER_TICK:
			GetCtrlVal(pH_HVTest, PANEL_7_NUMERIC, &i);  //�õ�ÿ����������ʱ��
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_INTERVAL, 1.0*i);
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_ENABLED, 0); //disable timer	
			ResetTimer(pH_HVTest, PANEL_7_TIMER_2); 
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER_2, ATTR_ENABLED, 0);  //disable ��ʾ���� timer
			if(giCurrentHVChan!=-1)		//�ָ���һ�β���Ϊ��ɫ
				SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_3, MakePoint(giChanTmp+1,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE); 
			
			for(i=0;i<40;i++)
			{
             //����������ƣ���ֹͣ����
				if(giCurrentHVChan>=40)
				{
					giCurrentHVChan = -1;
					ResetTimer (pH_HVTest, PANEL_7_TIMER);
					ResetTimer (pH_HVTest, PANEL_7_TIMER_2);  
					SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER,   ATTR_ENABLED, 0);  
					SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER_2, ATTR_ENABLED, 0);
					MessagePopup("��ʾ","��ѹ��ѹ������ϣ�");
					for(j=0;j<40;j++)
					{
						if( (giChanResSel[j]==1) || (giChanResSel[j]==2) )
							SetTableCellRangeAttribute (pH_HITest, PANEL_8_TABLE_4, MakeRect(1,j+1,1,1), ATTR_TEXT_BGCOLOR, VAL_WHITE);   
					}
					//�Ͽ��̵���
					sprintf(gComBufT, "[M]!FW#");
					i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
					if(i<0)
						return -1;
					return 0;
				}			
				giCurrentHVChan++;
				if(giChanResSel[giCurrentHVChan]==0)
					continue;
				else
					break;
			}

			if(gHVGNDSel==0)
			{
				sprintf(sTxt, "����%d������ѹ������", giCurrentHVChan+1);
				sprintf(gComBufT, "[M]>VL%d#", giCurrentHVChan);
				i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
				if(i<0)
				return -1;
			}
			else
			{
				sprintf(sTxt, "����%d�Ե���ѹ������", giCurrentHVChan +1);  
				sprintf(gComBufT, "[M]>PR%d#",  giCurrentHVChan );
				i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT); 
				if(i<0)
					return -1;
			}
						  
			Delay(1.0); 
			SetCtrlAttribute (pH_HVTest, PANEL_7_NUMERICSLIDE, ATTR_LABEL_TEXT, sTxt);  
			
			ResetTimer(pH_HVTest, PANEL_7_TIMER); 
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_ENABLED, 1);     			
			ResetTimer(pH_HVTest, PANEL_7_TIMER_2);
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER_2, ATTR_ENABLED, 1); //������ʾ���ȼ�ʱ��
			SetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE, 0.0 ); 
			giChanTmp = giCurrentHVChan; 
			break;
	}
	return 0;
}

//ֹͣ��ѹ����
int CVICALLBACK PANEL_7_Stop_Test (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	int j;
	switch (event)
	{
		case EVENT_COMMIT:
			giCurrentHVChan = -1;         
			ResetTimer(pH_HVTest, PANEL_7_TIMER);//�Զ����Զ�ʱ�� 
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_ENABLED, 0); //ֹͣ�Զ����Զ�ʱ��   			
			ResetTimer(pH_HVTest, PANEL_7_TIMER_2);
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER_2, ATTR_ENABLED, 0); //������ʾ���ȼ�ʱ��
			SetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE, 0.0 ); 		
			for(j=0;j<40;j++)
			{
				if( (giChanResSel[j]==1) || (giChanResSel[j]==2) )
					SetTableCellRangeAttribute (pH_HITest, PANEL_8_TABLE_4, MakeRect(1,j+1,1,1), ATTR_TEXT_BGCOLOR, VAL_WHITE); //��������Խ��  
			}
			sprintf(gComBufT, "[M]!FW#");
			SendComCMD(gComRLY, strlen(gComBufT), gComBufT);			
			break;
	}
	return 0;
}
 //��ѹ���Խ��
int CVICALLBACK PANEL_7_SetResult (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	char sTmp[10];
	Point MyPoint;
	int i;
	switch (event)
	{
		case EVENT_LEFT_DOUBLE_CLICK:
			GetActiveTableCell (pH_HVTest, PANEL_7_TABLE_4, &MyPoint);
			if( (giChanResSel[MyPoint.x-1]!=1) && (giChanResSel[MyPoint.x-1]!=2) )
				return -1;
			GetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_4, MyPoint, ATTR_CTRL_VAL, sTmp);
			if(sTmp[0]=='Y')
			{
				SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_4, MyPoint, ATTR_CTRL_VAL, "N");  
				SetTableCellRangeAttribute (pH_HVTest, PANEL_7_TABLE_4, MakeRect(MyPoint.y ,MyPoint.x,1,1), ATTR_TEXT_BGCOLOR, VAL_RED);  
			}
			else
			{
				SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_4, MyPoint, ATTR_CTRL_VAL, "Y"); 
				SetTableCellRangeAttribute (pH_HVTest, PANEL_7_TABLE_4, MakeRect(MyPoint.y ,MyPoint.x,1,1), ATTR_TEXT_BGCOLOR, VAL_WHITE);  				
			}
			break;
	}
	return 0;
}

//�Զ����Ի����Ե���ѹ
int CVICALLBACK PANEL_7_Auto_GND_Start (int panel, int control, int event,
										void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_INTERVAL, 1.0);//����ִ��
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_ENABLED, 1); 
			gHVGNDSel = 1;
			break;
	}
	return 0;
}

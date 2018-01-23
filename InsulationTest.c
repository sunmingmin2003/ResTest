/*
  绝缘测试
*/

#include <rs232.h>
#include <formatio.h>
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>
#include "FiberSensor.h"
#include "variable.h"
static int giInsulationThreshold = 10;
static int giCurrentInsulationChan=-1;  

int CVICALLBACK PANEL_13_PreTest (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_InsulationTest);
			InstallPopup(pH_HVTest);
			break;
	}
	return 0;
}

int CVICALLBACK PANEL_13_Exit (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		   	HidePanel(pH_InsulationTest);     
			break;
	}
	return 0;
}

int CVICALLBACK PANEL_13_NextTest (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			InstallPopup(panelHandle);
			sprintf(gComBufT, "[M]!FW#");
			i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);	
			if(i<0)
				return -1;
			HidePanel(pH_InsulationTest);     
			break;
	}
	return 0;
}
//绝缘阻值，开始自动测试
int CVICALLBACK PANEL_13_Start_Test (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER, ATTR_ENABLED, 1); // 启动下一滑道测试定时器
	}
	return 0;
}

//停止测试
int CVICALLBACK PANEL_13_Stop_Test (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf(gComBufT, "tzcl8888");
			i = SendComCMD(gComInsulation, strlen(gComBufT), gComBufT); 
			
			giCurrentInsulationChan = -1;
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER, ATTR_ENABLED, 0);  
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_2, ATTR_ENABLED, 0);
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_3, ATTR_ENABLED, 0); 
			SetCtrlVal(pH_InsulationTest, PANEL_13_NUMERICSLIDE, 0.0);  			
			ResetTimer (pH_InsulationTest, PANEL_13_TIMER);
			ResetTimer (pH_InsulationTest, PANEL_13_TIMER_2);  
			ResetTimer (pH_InsulationTest, PANEL_13_TIMER_3);  
			sprintf(gComBufT, "[M]!FW#");
			i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);			
			break;
	}
	return 0;
}

//20S后开始读取绝缘值
int CVICALLBACK PANEL_13_Insulation_ReadBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int i,j,iLen;
	char sTmp[50];
	double fRValue;
	switch (event)
	{
		case EVENT_TIMER_TICK:
//停止测量			
		
			iLen =	GetInQLen (gComInsulation); //绝缘测试仪表的端口号
			if(iLen<100)
			{
				MessagePopup("警告","当前绝缘仪串口未收到数据！");
				return -1;												    
			}
			i = ComRd (gComInsulation, gComBufR, iLen);
			i = FindPattern (gComBufR, 0, iLen, "s=010", 0, 0);
			if(i<20)
				return -1;
			sprintf(sTmp, "");
			memcpy(sTmp, &gComBufR[i-11], 9);
			fRValue = atof(sTmp);
			if(giCurrentInsulationChan<=39)
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_4, MakePoint((int)(0.5*giCurrentInsulationChan)+1,((giCurrentInsulationChan%2)==1)?2:1), ATTR_CTRL_VAL, fRValue);
			else
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_5, MakePoint((int)(0.5*(giCurrentInsulationChan-40))+1,((giCurrentInsulationChan%2)==1)?2:1), ATTR_CTRL_VAL, fRValue);
			sprintf(gComBufT, "tzcl8888");
			i = SendComCMD(gComInsulation, strlen(gComBufT), gComBufT); 
			Delay(3.0);
//下一次测量	
			ResetTimer(pH_InsulationTest, PANEL_13_TIMER); 
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER, ATTR_ENABLED, 1); 	 //2秒后开始下一滑道测试
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_2, ATTR_ENABLED, 0); //20秒后开始读取绝缘值
			ResetTimer(pH_InsulationTest, PANEL_13_TIMER_3); //显示进度
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_3, ATTR_ENABLED, 0); 
			SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_3, MakePoint((int)(0.5*giCurrentInsulationChan)+1,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE);  
			SetCtrlVal(pH_InsulationTest, PANEL_13_NUMERICSLIDE, 0.0); 
			break;
	}
	return 0;
}

 //2秒钟后下一滑道测试
int CVICALLBACK PANEL_13_Insulation_Test1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int i;
	char sTxt[50];
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			for(i=0;i<80;i++)
			{
//如果超过限制，则停止测量
				if(giCurrentInsulationChan>=80)
				{
					giCurrentInsulationChan = -1;
					ResetTimer (pH_InsulationTest, PANEL_13_TIMER);
					ResetTimer (pH_InsulationTest, PANEL_13_TIMER_2);  
					ResetTimer (pH_InsulationTest, PANEL_13_TIMER_3);  	
					SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER,   ATTR_ENABLED, 0);  
					SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_2, ATTR_ENABLED, 0);
					SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_3, ATTR_ENABLED, 0); 					
					MessagePopup("提示","绝缘电阻测试完毕！");
					
					sprintf(gComBufT, "[M]!FW#");
					i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
					if(i<0)
						return -1;					
					return 0;
				}			
				giCurrentInsulationChan++; //下一滑道
				if(giChanResSel[(int)(0.5*giCurrentInsulationChan)]==0)
					continue;
				else
					break;
			}

			if((giCurrentInsulationChan%2)==0)
			{
				sprintf(sTxt, "滑道%d环间绝缘电阻测试中", (int)(0.5*giCurrentInsulationChan)+1);
				sprintf(gComBufT, "[M]>GP%d#", (int)(0.5*giCurrentInsulationChan));
				i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT); 
				if(i<0)
					return -1;
			}
			else
			{
				sprintf(sTxt, "滑道%d对地绝缘电阻测试中", (int)(0.5*giCurrentInsulationChan)+1);  
				sprintf(gComBufT, "[M]>IS%d#", (int)(0.5*giCurrentInsulationChan));
				i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT); 
				if(i<0)
					return -1;
			}
			//清空缓冲区			
			FlushInQ(gComInsulation);
			FlushOutQ(gComInsulation);    
//先停止设备
			sprintf(gComBufT, "tzcl8888");
			i = SendComCMD(gComInsulation, strlen(gComBufT), gComBufT); 			
			//	if(i<0)
			//		return -1;		
			Delay(1);
			
//恢复到开始测试状态
			sprintf(gComBufT, "qxcc3456");
			i = SendComCMD(gComInsulation, strlen(gComBufT), gComBufT); 			
			//	if(i<0)
			//		return -1;		
			Delay(1);
			
//启动绝缘仪串口
			sprintf(gComBufT, "cl780050");
			i = SendComCMD(gComInsulation, strlen(gComBufT), gComBufT); 			
			//	if(i<0)
			//		return -1;			
			
						  
			Delay(1.0);  
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_NUMERICSLIDE, ATTR_LABEL_TEXT, sTxt);
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER, ATTR_ENABLED, 0); //禁止本定时器   
			ResetTimer(pH_InsulationTest, PANEL_13_TIMER_2);
			ResetTimer(pH_InsulationTest, PANEL_13_TIMER_3); 			
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_2, ATTR_ENABLED, 1); //启动20S读数据计时器
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_3, ATTR_ENABLED, 1); //显示进度
			
			break;
	}
	return 0;
}

int CVICALLBACK PANEL_13_Progrocess (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	int iColor;
	double fProgress;
	int iChan;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			iChan = (int)(0.5*giCurrentInsulationChan)+1;	 
			GetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_3, MakePoint(iChan,1), ATTR_CMD_BUTTON_COLOR, &iColor);
			SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_3, MakePoint(iChan,1), ATTR_CMD_BUTTON_COLOR, (iColor==VAL_YELLOW)?VAL_GREEN:VAL_YELLOW); 
			GetCtrlVal(pH_InsulationTest, PANEL_13_NUMERICSLIDE, &fProgress);

			if(fProgress>=0.95)
			{
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_3, MakePoint(iChan,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE);  
				SetCtrlVal(pH_InsulationTest, PANEL_13_NUMERICSLIDE, 0.0); 
				SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_3, ATTR_ENABLED, 0);
				ResetTimer(pH_InsulationTest, PANEL_13_TIMER_3);
				return 0;
			}
			SetCtrlVal(pH_InsulationTest, PANEL_13_NUMERICSLIDE, ((fProgress+0.05)>1)?1:(fProgress+0.05) ); 
			break;
	}
	return 0;
}

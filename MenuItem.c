#include <userint.h>
#include <ansi_c.h>
#include "IPctrl.h"
#include <tcpsupp.h>
#include "FiberSensor.h"
#include <variable.h>

void CVICALLBACK ITEM_NetConnect (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	if ((pH_NetConnect = LoadPanel (0, "FiberSensor.uir", PANEL_2)) < 0)
	  return;
	InstallPopup (pH_NetConnect); 
	IPCtrl_ConvertFromString (pH_NetConnect, PANEL_2_IP); 
	SetCtrlVal(pH_NetConnect, PANEL_2_IP,gNetIP);   //显示默认IP
	SetCtrlVal(pH_NetConnect, PANEL_2_PORT, gNetPort); 	//显示默认端口号
	
}

void CVICALLBACK Item_DisconnectNet (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	//断开网络连接，并关闭自动重连定时器
	gNetConnectStatus = 0;
	DisconnectFromTCPServer (pH_NetHandle);
	SetCtrlAttribute (panelHandle, PANEL_TIMER_AutoConnect, ATTR_ENABLED, 0);
}


void CVICALLBACK ITEM_DisplaySet (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	InstallPopup  (pH_DisplaySet);
	SetCtrlVal(pH_DisplaySet, PANEL_3_CHK1, gDisplayAnnotation); 
	SetCtrlVal(pH_DisplaySet, PANEL_3_CHK2, giEnableAlarm);
	SetCtrlVal(pH_DisplaySet, PANEL_3_NUMERIC,   gYAxisMin);				
	SetCtrlVal(pH_DisplaySet, PANEL_3_NUMERIC_2, gYAxisMax);
	SetCtrlVal(pH_DisplaySet, PANEL_3_NUMERIC_3, giBreakCriteria); 	
}

int CVICALLBACK PANEL_2_ButtonQuit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (pH_NetConnect);  
			break;
	}
	return 0;
}


void CVICALLBACK ITEM_Login (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	SetCtrlVal(pH_Login, PANEL_4_PASSWORD, "");
	SetCtrlVal(pH_Login, PANEL_4_PASSWORD_2, "");
	SetCtrlVal(pH_Login, PANEL_4_PASSWORD_3, "");
	InstallPopup (pH_Login);  
}


void CVICALLBACK ITEM_RecordSet (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	InstallPopup  (pH_RecordSet); 
	SetCtrlVal(pH_RecordSet, PANEL_9_STRING, gsFileSaveDir);
	SetCtrlVal(pH_RecordSet, PANEL_9_CHK_SaveData, gSaveData);
	SetCtrlVal(pH_RecordSet, PANEL_9_NUMERIC, gFileSavePeriod);    			
	SetCtrlVal(pH_RecordSet, PANEL_9_CHK_SaveAlarm, gSaveAlarm);    			
	SetCtrlVal(pH_RecordSet, PANEL_9_CHK_SaveDeviceInfo, gSaveDeviceInfo);	
}

void CVICALLBACK ITEM_History (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	if( 0>system ("History.exe") )
		MessagePopup("错误", "打开数据分析程序错误");
		return;
}

void CVICALLBACK Item_Alarm (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	if( 0>system ("Alarm.exe") )
		MessagePopup("错误", "打开数据分析程序错误");
		return;
}


void CVICALLBACK ITEM_Quit (int menuBar, int menuItem, void *callbackData,
		int panel)
{
	QuitUserInterface(0); 	//退出用户界面
}


void CVICALLBACK ITEM_Support (int menuBar, int menuItem, void *callbackData,
							   int panel)
{
	//
}


void CVICALLBACK Item_ChanSel (int menuBar, int menuItem, void *callbackData,
							   int panel)
{
	InstallPopup (pH_ChanSel);   
}

void CVICALLBACK Item_ComSet (int menuBar, int menuItem, void *callbackData,
							   int panel)
{
	InstallPopup (pH_ComSet);  
	SetCtrlVal(pH_ComSet, PANEL_6_NUMERIC1, gComRLY);
	SetCtrlVal(pH_ComSet, PANEL_6_NUMERIC2, gComHV);
	SetCtrlVal(pH_ComSet, PANEL_6_NUMERIC3, gComHI); //大电流发生器  
	SetCtrlVal(pH_ComSet, PANEL_6_NUMERIC4, gComInsulation);   
}
							  

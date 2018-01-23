#include <ansi_c.h>
#include <userint.h>
#include "FiberSensor.h"
#include "Command.h"
#include "variable.h"    
static int iDataTypeUpload = 0x1;  //默认上传温度数据


void ProcessDeviceCMDDebug(void)
{
	return ;
}


int CVICALLBACK PANEL_10_OnBTNQuit (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_CMD);
			break;
	}
	return 0;
}

int CVICALLBACK PANEL_11_Quit (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (pH_DeviceIP);  
			break;
	}
	return 0;
}



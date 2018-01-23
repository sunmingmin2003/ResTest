/*
  通道选择
*/
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "FiberSensor.h"
#include "variable.h"

//测试通道选择，退出
int CVICALLBACK PANEL_16_QUIT (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_ChanSel);
			break;
	}
	return 0;
}

//通道选择，table1
int CVICALLBACK OnTable (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	Point MyPoint;
	int iColor, i, iTmp;
	switch (event)
	{
		case EVENT_LEFT_CLICK_UP:
			 GetActiveTableCell(panel, control, &MyPoint);
			 GetTableCellAttribute (panel, control, MyPoint, ATTR_CMD_BUTTON_COLOR, &iColor);
			 iTmp = MyPoint.y; 
			 for(i=0;i<4;i++)
			 {
				MyPoint.y = i+1; 
				SetTableCellAttribute (panel, control, MyPoint, ATTR_CMD_BUTTON_COLOR, VAL_WHITE);
			 }
			 MyPoint.y=iTmp;   			 
			 if(iColor==VAL_GREEN)
				 SetTableCellAttribute (panel, control, MyPoint, ATTR_CMD_BUTTON_COLOR, VAL_WHITE);
			 else
				 SetTableCellAttribute (panel, control, MyPoint, ATTR_CMD_BUTTON_COLOR, VAL_GREEN);
			break;
	}
	return 0;
}

 //通道选择，表格1清除
int CVICALLBACK PANEL_16_Clear1 (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	Point MyPoint;
	int i,j;
	switch (event)
	{
		case EVENT_COMMIT:
			for(i=0;i<30;i++)
			{
				for(j=0;j<4;j++)
				{
					MyPoint.x = i+1;
					MyPoint.y = j+1;																		
					SetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE, MyPoint, ATTR_CMD_BUTTON_COLOR, VAL_WHITE);  					
				}
			}
			break;
	}
	return 0;
}

//通道选择，表格2清除
int CVICALLBACK PANEL_16_Clear2 (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	Point MyPoint;
	int i,j;
	switch (event)
	{
		case EVENT_COMMIT:
			for(i=0;i<30;i++)
			{
				for(j=0;j<4;j++)
				{
					MyPoint.x = i+1;
					MyPoint.y = j+1;																		
					SetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE_2, MyPoint, ATTR_CMD_BUTTON_COLOR, VAL_WHITE);  					
				}
			}
			break;
	}
	return 0;
}

//测量通道选择,保存配置
int CVICALLBACK PANEL_16_SaveCFG (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	char strFilePath[260];
	int i,j, iColor;
	Point MyPoint;
	/*double*/int iTmp;
	switch (event)
	{
		case EVENT_COMMIT:
			i = FileSelectPopup ("", "*.res", "", "文件保存名称", VAL_SAVE_BUTTON, 0, 0, 1, 1, strFilePath);
			if(i==0)
				return 0;
//保存前30组数据--------------------------------------			
			for(i=0;i<30;i++)
			{
				MyPoint.x = i+1;
				for(j=0;j<4;j++)
				{
					MyPoint.y = j+1;
					GetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE, MyPoint, ATTR_CMD_BUTTON_COLOR, &iColor); 
					if(iColor==VAL_GREEN)
					{
						giChanResSel[i] = j+1;
						break;
					}
					else 
						giChanResSel[i] = 0;
				}
			}
//保存后30组数据--------------------------------------
			for(i=0;i<30;i++)
			{
				MyPoint.x = i+1;
				for(j=0;j<4;j++)
				{
					MyPoint.y = j+1;
					GetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE_2, MyPoint, ATTR_CMD_BUTTON_COLOR, &iColor); 
					if(iColor==VAL_GREEN)
					{
						giChanResSel[30+i] = j+1;
						break;
					}
					else 
						giChanResSel[30+i] = 0;
				}
			}
//保存4个阈值--------------------------------------  
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC, &iTmp);
			giChanResSel[60] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_2, &iTmp);
			giChanResSel[61] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_3, &iTmp);
			giChanResSel[62] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_4, &iTmp);
			giChanResSel[63] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_5, &iTmp);
			giChanResSel[64] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_6, &iTmp);
			giChanResSel[65] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_7, &iTmp);
			giChanResSel[66] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_8, &iTmp);
			giChanResSel[67] = iTmp;
			
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_9, &iTmp);
			giChanResSel[68] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_10, &iTmp);
			giChanResSel[69] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_11, &iTmp);
			giChanResSel[70] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_12, &iTmp);
			giChanResSel[71] = iTmp;			
			
			i = ArrayToFile (strFilePath, giChanResSel, VAL_INTEGER, 72, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 10, VAL_ASCII, VAL_TRUNCATE);
			
			if(i==0)
				MessagePopup("信息提示","配置保存成功！");

			break;
	}
	return 0;
}

 //通道选择，加载配置
int CVICALLBACK PANEL_16_LoadCFG (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	char strFilePath[260];
	int i,j, iColor;
	Point MyPoint;
	switch (event)
	{
		case EVENT_COMMIT:
			i = FileSelectPopup ("", "*.res", "", "文件保存名称", VAL_LOAD_BUTTON, 0, 0, 1, 0, strFilePath);
			if(i==0)
				return 0;
			FileToArray (strFilePath, giChanResSel, VAL_INTEGER, 72, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
//设置前30组数据--------------------------------------			
			for(i=0;i<30;i++)
			{
				MyPoint.x = i+1;
				for(j=0;j<4;j++)
				{
					MyPoint.y = j+1;
					SetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE, MyPoint, ATTR_CMD_BUTTON_COLOR, VAL_WHITE); 
				}
				MyPoint.y = giChanResSel[i]; 
				SetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE, MyPoint, ATTR_CMD_BUTTON_COLOR, VAL_GREEN);   
			}
//设置后30组数据--------------------------------------
			for(i=0;i<30;i++)
			{
				MyPoint.x = i+1;
				for(j=0;j<4;j++)
				{
					MyPoint.y = j+1;
					SetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE_2, MyPoint, ATTR_CMD_BUTTON_COLOR, VAL_WHITE); 
				}
				MyPoint.y = giChanResSel[i+30]; 
				SetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE_2, MyPoint, ATTR_CMD_BUTTON_COLOR, VAL_GREEN);   
			}
//保存4个阈值--------------------------------------  
			SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC,  giChanResSel[60]);
			SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_2,giChanResSel[61]);
			SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_3,giChanResSel[62]);
			SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_4,giChanResSel[63]);
			SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_5,giChanResSel[64]);
			SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_6,giChanResSel[65]);
			SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_7,giChanResSel[66]);
			SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_8,giChanResSel[67]);
			SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_9,giChanResSel[68]);
			SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_10,giChanResSel[69]);
			SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_11,giChanResSel[70]);
			SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_12,giChanResSel[71]);				
			break;
	}
	
	InitialDefaultCtrl();
	return 0;
}
//通道选择，确认
int CVICALLBACK PANEL_16_Set (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int i,j, iColor;
	Point MyPoint;
	int iTmp;
	switch (event)
	{
		case EVENT_COMMIT:
//设定前30组数据--------------------------------------			
			for(i=0;i<30;i++)
			{
				MyPoint.x = i+1;
				for(j=0;j<4;j++)
				{
					MyPoint.y = j+1;
					GetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE, MyPoint, ATTR_CMD_BUTTON_COLOR, &iColor); 
					if(iColor==VAL_GREEN)
					{
						giChanResSel[i] = j+1;
						break;
					}
					else 
						giChanResSel[i] = 0;
				}
			}
//设定后30组数据--------------------------------------
			for(i=0;i<30;i++)
			{
				MyPoint.x = i+1;
				for(j=0;j<4;j++)
				{
					MyPoint.y = j+1;
					GetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE_2, MyPoint, ATTR_CMD_BUTTON_COLOR, &iColor); 
					if(iColor==VAL_GREEN)
					{
						giChanResSel[30+i] = j+1;
						break;
					}
					else 
						giChanResSel[30+i] = 0;
				}
			}
//保存4个阈值--------------------------------------  
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC, &iTmp);
			giChanResSel[60] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_2, &iTmp);
			giChanResSel[61] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_3, &iTmp);
			giChanResSel[62] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_4, &iTmp);
			giChanResSel[63] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_5, &iTmp);
			giChanResSel[64] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_6, &iTmp);
			giChanResSel[65] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_7, &iTmp);
			giChanResSel[66] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_8, &iTmp);
			giChanResSel[67] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_9, &iTmp);
			giChanResSel[68] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_10, &iTmp);
			giChanResSel[69] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_11, &iTmp);
			giChanResSel[70] = iTmp;
			GetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_12, &iTmp);
			giChanResSel[71] = iTmp;			
			break;
	}
	
	InitialDefaultCtrl	();
	return 0;
}

void LoadChannelCfg(void)
{
	int i,j, iColor;
	Point MyPoint;
	
	FileToArray ("DefaultChanCfg.res", giChanResSel, VAL_INTEGER, 72, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_ASCII);
			
//设置前30组数据--------------------------------------			
	for(i=0;i<30;i++)
	{
		MyPoint.x = i+1;
		for(j=0;j<4;j++)
		{
			MyPoint.y = j+1;
			SetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE, MyPoint, ATTR_CMD_BUTTON_COLOR, VAL_WHITE); 
		}
		MyPoint.y = giChanResSel[i]; 
		SetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE, MyPoint, ATTR_CMD_BUTTON_COLOR, VAL_GREEN);   
	}
//设置后30组数据--------------------------------------
	for(i=0;i<30;i++)
	{
		MyPoint.x = i+1;
		for(j=0;j<4;j++)
		{
			MyPoint.y = j+1;
			SetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE_2, MyPoint, ATTR_CMD_BUTTON_COLOR, VAL_WHITE); 
		}
		MyPoint.y = giChanResSel[i+30]; 
		SetTableCellAttribute (pH_ChanSel, PANEL_16_TABLE_2, MyPoint, ATTR_CMD_BUTTON_COLOR, VAL_GREEN);   
	}
//保存4个阈值--------------------------------------  
	SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC,   giChanResSel[60]);
	SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_2, giChanResSel[61]);
	SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_3, giChanResSel[62]);
	SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_4, giChanResSel[63]);
	SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_5, giChanResSel[64]);
	SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_6, giChanResSel[65]);
	SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_7, giChanResSel[66]);
	SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_8, giChanResSel[67]);
	SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_9, giChanResSel[68]);
	SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_10, giChanResSel[69]);
	SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_11, giChanResSel[70]);
	SetCtrlVal(pH_ChanSel, PANEL_16_NUMERIC_12, giChanResSel[71]);	
}

void SaveChannelCfg(void)
{
	char strProjectDir[260];
	GetProjectDir (strProjectDir); 			                                               
	sprintf(strProjectDir, "%s\\DefaultChanCfg.res", strProjectDir); 
	ArrayToFile (strProjectDir, giChanResSel, VAL_INTEGER, 72, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 10, VAL_ASCII, VAL_TRUNCATE);	
}

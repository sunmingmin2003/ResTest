/*
   主程序，线阻测试
*/

#include <windows.h>
#include <rs232.h>
#include <analysis.h>
#include "toolbox.h"
#include "color_picker.h"
#include <ansi_c.h>
#include <tcpsupp.h>
#include <formatio.h>
#include "pwctrl.h"
#include "IPctrl.h"
#include <utility.h>
#include <cvirte.h>
#include <userint.h>
#include "FiberSensor.h"
#include "variable.h"
#include "Config.h"
#include "SaveData.h"
#include "ChanGraphView.h"
#include "Command.h"

static int giTCPDisable = 0;  //接收到命令后，要关闭TCP的接收缓冲部分
static int giMsgRowCurrent =0;
static int giTCPReadCNT=0;
static int gRevStatus = 0;
static int giNumRead  = 0;	  //已经收到的数据数量
static int giCurrentGraph = 1;
static int giEnableDataRev = 1;

static unsigned short giDataOrder=0;



int CVICALLBACK ClientTCPCB (unsigned handle, int event, int error,
							 void *callbackData)
{
	int i, iTmp;
	int iStatus = 0;
	double fMax, fMin;
	int iMaxIndex, iMinIndex;
	char strTmp[2048];
	char strTmp_1[2048];
	char Text[2048];
	int j;
	double fTmp=0;
	Point MyPoint;
	double fTmp1[54000];

	switch (event)
	{
		case TCP_DATAREADY:
//重置定时检测网络定时器
			ResetTimer(panelHandle, PANEL_TIMER_AutoConnect);//自动重连timer
			gNetConnectStatus = 1;

			if(giTCPDisable)  //=1禁止接收
				return 0;

			if(giTCPReadCNT==0)
			{
				i = ClientTCPRead (pH_NetHandle, gBufRev, 13, _TCP_TIME_OUT) ;   //第一次先读取13个字节
				CopyBytes (gDataBuf, 0, gBufRev, 0, i);
				giTCPReadCNT++;
				giNumRead = i;
				iStatus = (gBufRev[1]==0x7B)&&(gBufRev[2]==0x07)&&(gBufRev[3]==0xAF)&&(gBufRev[4]==0xEC)
						  &&(gBufRev[5]==0x66)&&(gBufRev[6]==0x48)&&(gBufRev[7]==0xC5)&&(gBufRev[8]==0x73);
				if( (i!=13) || (iStatus ==0) ) //恢复初始状态
					TCPReadReset();//清空缓冲区
				return 0;
			}

			iTmp = gDataBuf[9]+(gDataBuf[10]<<8) + (gDataBuf[11]<<16) + (gDataBuf[12]<<24);  //计算要接收的数据
			if( ((iTmp - giNumRead)>0) && ( (iTmp - giNumRead) <2*gDataMaxLen))              //防止因为下位机数据出错而造成的数组溢出
			{
				i = ClientTCPRead (pH_NetHandle, gBufRev, iTmp - giNumRead, _TCP_TIME_OUT) ;   //
				CopyBytes (gDataBuf, giNumRead, gBufRev, 0, i);
				giNumRead = giNumRead + i;
			}
			giTCPReadCNT++;
            //如果超过100次数据仍然没有读完毕
			if(giTCPReadCNT==100)
			{
				TCPReadReset(); //清空缓冲区   
				return 0;
			}
            //如果数据读取完毕并且数据正确
			if( (0==(iTmp - giNumRead)) && (gDataBuf[giNumRead-1]==0x55) )
			{
				giNumRead  = 0;
				giTCPReadCNT = 0;
			}
			else 
				return 0;
//---------------------------------------------
			switch (gDataBuf[0])
			{
				case _DATA_ID_DATA_PACKET:
				
					if((gDataBuf[13]<0) || (gDataBuf[13]>1))// 0 or 1
						return -1;
                    //-----如果出现报警，则终止数据传输					
					if(giEnableDataRev ==0)
						return -1; 

			        giDataOrder = giDataOrder|(1<<gDataBuf[13]);
					for(i=0; i<60; i++)
					{
						for(j=0; j<900; j++)
						{
							gfChanResVal[i][gDataBuf[13]*900+j] = gDataBuf[i*1800+22+2*j] + (gDataBuf[i*1800+23+2*j]<<8);//gDataBuf[13]表示第几次传输的数据
							if(gfChanResVal[i][gDataBuf[13]*900+j] >=32768)
								gfChanResVal[i][gDataBuf[13]*900+j] =(0x7fff & (gDataBuf[i*1800+22+2*j] + (gDataBuf[i*1800+23+2*j]<<8)) ) * 16;
						}
					}
					//设置LED的显示状态
					GetCtrlVal(panelHandle, PANEL_LED, &i);//运行指示
					if(i==0)	//关闭的时候
					{
						GetCtrlAttribute (panelHandle, PANEL_LED, ATTR_OFF_COLOR, &iTmp);
						if(iTmp == VAL_GREEN)
							SetCtrlAttribute (panelHandle, PANEL_LED, ATTR_OFF_COLOR, VAL_DK_GREEN);
						else
							SetCtrlAttribute (panelHandle, PANEL_LED, ATTR_OFF_COLOR, VAL_GREEN); 
					}
					
					if(gDataBuf[13]==1)   //从0到1，数据传输完毕
					{
						if(giDataOrder!=3)	 // 11
						{
							giDataOrder = 0;
							return -1;
						}
						giDataOrder = 0; 
						
					}
					if(gDataBuf[13]==0)
						return 0;
					
					for(i=0;i<60;i++)
					{
						for(j=0;j<1800;j++)
							gfDataSaveBuf[i*1800+j] = gfChanResVal[i][j];
					}
					
					if(gSaveData==1)
						SaveData(gfDataSaveBuf, 108000, 0);					
					
					//画图，插入标签
					DeleteGraphPlot (pH_CMD, PANEL_10_GRAPH1, -1, VAL_IMMEDIATE_DRAW);
					PlotY (pH_CMD, PANEL_10_GRAPH1, gfChanResVal[giCurrentGraph-1], 1800, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
					InsertGraphLabel(giCurrentGraph);

					//插入时间
					sprintf(strTmp, "%s %s", DateStr(), TimeStr());
					SetCtrlVal(panelHandle, PANEL_STRING, strTmp);//设置测试时间
					
					SetAlarmDisplay(); //结果显示处理
					
					break;

//下面的是上位机需要拿定时器来处理的数据
				default:
					gNetCMDRev   = _DATA_CMD_READY; //只要接收到了命令，都表示当前是ready状态，接下来需要“命令定时器”来处理
					giTCPDisable = 1;//禁止接收	 //经过0.2s后才开始继续接收数据，以防止：接收到命令数据以后，定时器还没来及得处理命令，下面的数据又上来了，把数据给冲乱了
					SetCtrlAttribute (panelHandle, PANEL_TIMER_EnableRXD, ATTR_ENABLED, 1);
					break;
			}
			break;
		case TCP_DISCONNECT:
			gNetConnectStatus = 0;
			break;
	}
	return 0;
}

//网络状态Timer 0.2S internel，发送完网络命令后，启动该定时器，处理接收到得数据
int CVICALLBACK PANEL_Timer_CMD_Process (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int mmsun;
	int i;

	switch (event)
	{
		case EVENT_TIMER_TICK:
			gTimerCNT ++;
			//没收到数据，还没有超时
			if( (gTimerCNT<_TCP_CMD_TIME_OUT_CNT) && (gNetCMDRev ==_DATA_CMD_IDLE) )	   //继续累加一个定时器的0.2s，判读是否成功
				return 0;
//超时都没收到数据
			else if((gTimerCNT>=_TCP_CMD_TIME_OUT_CNT) && (gNetCMDRev ==_DATA_CMD_IDLE) )  //如果始终等于IDLE，通信中断
				ProcessTimeOutErr(gNetCMDCode);

//收到了数据 ,处理命令码
			else if(gNetCMDRev == _DATA_CMD_READY)  //gNetCMDRev 在 ClientTCPCB()里面，判断置位
				switch (gNetCMDCode)
				{
					case  _NET_CONNECT_TEST:
						if(gDataBuf[13] == 'Y')
							gNetConnectStatus = 1;
						break;
//消音、复位、自检----------------------------------------------------
					case  _NET_DAQ_START:
						if(gDataBuf[13] == 'Y')
						{
							InsertTableMsg("命令","启动测量命令发送成功");
							SetMenuBarAttribute (MENUBAR, MENUBAR, ATTR_DIMMED, 1);
						}
						else
							InsertTableMsg("命令","启动测量命令发送失败");
						break;
					case   _NET_DAQ_STOP:
						if(gDataBuf[13] == 'Y')
							InsertTableMsg("命令","停止测量命令发送成功");
						else
							InsertTableMsg("命令","停止测量命令发送失败");
						break;

					case   _NET_CLEAR_ALARM:
						if(gDataBuf[13] == 'Y')
							InsertTableMsg("命令","清除报警信号命令发送成功");
						else
							InsertTableMsg("命令","清除报警信号命令发送失败");
						break;
						
					case   _NET_SINGLE_DAQ:
						if(gDataBuf[13] == 'Y')
							InsertTableMsg("命令","单侧测量启动！");
						else
							InsertTableMsg("命令","单次测量失败！");
						break;						
						
//--------------------------------------------------------
					default:
						break ;
				}
//关闭当前定时器------------------------------------------------------------------------------------
			EnableCtrlCMD();
			SetCtrlAttribute (panelHandle, PANEL_TIMER_CMD_Process, ATTR_ENABLED, 0);
																 // InitCVIRTE (0, argv, 0);
			gNetCMDRev = _DATA_CMD_IDLE;
			gTimerCNT = 0;
//--------------------------------------------------------------------------------------------------
			break;
	}
	return 0;
}



int main (int argc, char *argv[])
{
	int i;

	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */

	//load default value for software config
	LoadConfig();

	DisableBreakOnLibraryErrors();
	//自动连接IP
	i = ConnectToTCPServerEx (&pH_NetHandle, gNetPort, gNetIP, ClientTCPCB, NULL, _TCP_TIME_OUT, TCP_ANY_LOCAL_PORT);
	if(i < 0)
		MessagePopup("提示","不能打开网络连接!");
	
	gNetCMDRev = _DATA_CMD_IDLE;

	if(i <0)
		gNetConnectStatus = 0;
	else
		gNetConnectStatus = 1;

	DiscardPanel (pH_Start);
//---------------------------------
	if ((pH_DisplaySet = LoadPanel (0, "FiberSensor.uir", PANEL_3)) < 0)
		return -1;
	
	if ((pH_SuperPassword = LoadPanel (0, "FiberSensor.uir", PANEL_5)) < 0)
		return -1;
	PasswordCtrl_ConvertFromString (pH_SuperPassword, PANEL_5_PASSWORD);
	
	if ((pH_ComSet = LoadPanel (0, "FiberSensor.uir", PANEL_6)) < 0)
		return -1;
	
	if ((pH_HVTest = LoadPanel (0, "FiberSensor.uir", PANEL_7)) < 0)
		return -1;	
	
	if ((pH_RecordSet = LoadPanel (0, "FiberSensor.uir", PANEL_9)) < 0)
		return -1;

	if ((pH_CMD = LoadPanel (0, "FiberSensor.uir", PANEL_10)) < 0)
		return -1;

	if ((pH_DeviceIP = LoadPanel (0, "FiberSensor.uir", PANEL_11)) < 0)
		return -1;
												  
	if ((pH_ShortKeyMSG = LoadPanel (0, "FiberSensor.uir", PANEL_12)) < 0)
		return -1;
	
	if ((pH_InsulationTest = LoadPanel (0, "FiberSensor.uir", PANEL_13)) < 0)
		return -1;	

	if ((pH_ChanSel = LoadPanel (0, "FiberSensor.uir", PANEL_16)) < 0)
		return -1;
//---------------------------------
	if ((panelHandle = LoadPanel (0, "FiberSensor.uir", PANEL)) < 0)
		return -1;
	
	if ((pH_HITest = LoadPanel (0, "FiberSensor.uir", PANEL_8)) < 0)
		return -1;	

	
	pHMenu = GetPanelMenuBar(panelHandle);
	DisplayPanel (panelHandle);
	//显示区域窗口大小设置
	SetPanelAttribute (panelHandle, ATTR_WINDOW_ZOOM, VAL_MAXIMIZE);
	GetScreenSize (&giScreenHight, &giScreenWidth);
	giGraphAreaWidth = giScreenWidth - _GRAPH_AREA_LEFT;
	giGraphAreaHight = giScreenHight - _GRAPH_AREA_TOP - _MSG_AREA_HIGHT;

	ctrlArrayID = GetCtrlArrayFromResourceID (panelHandle, CTRLARRAY_Scale);
	MoveCtrlArray (ctrlArrayID, giGraphAreaWidth/2, 0);

//设置消息窗口的坐标
	SetCtrlAttribute (panelHandle, PANEL_TABLE_MSG, ATTR_TOP,   giScreenHight - _MSG_AREA_HIGHT );
	SetCtrlAttribute (panelHandle, PANEL_TABLE_MSG, ATTR_WIDTH,  giGraphAreaWidth);

	SetTableColumnAttribute (panelHandle, PANEL_TABLE_MSG, 3, ATTR_COLUMN_WIDTH, giGraphAreaWidth -200 -100- 30);

	//SetCtrlAttribute (panelHandle, PANEL_DECORATION_2, ATTR_HEIGHT, giScreenHight - 665);    //665是根据上面的所有控件的位置计算得来
	//--------------------------------------------------------
	SetAxisScalingMode (pH_CMD, PANEL_10_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL, gYAxisMin,gYAxisMax);
	SetAxisScalingMode (pH_DeviceIP, PANEL_11_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL, gYAxisMin, gYAxisMax);  
	SetAxisScalingMode (pH_DeviceIP, PANEL_11_GRAPH2, VAL_LEFT_YAXIS, VAL_MANUAL, gYAxisMin, gYAxisMax);  	

	//UnAuthrity();
	LoadPassword();
	LoadChannelCfg();
	InitialDefaultCtrl();
	//串口配置
	CloseCom(gComRLY);
	CloseCom(gComHI); //大电流发生器
	CloseCom(gComHV);
	CloseCom(gComInsulation);
	
	 i= OpenComConfig (gComRLY, "", 9600, 0, 8, 1, 512, 512);
	 SetComTime (gComRLY, 2.0);
				
	 if(i<0)
		 InsertTableMsg("错误提示","继电器控制模块串口无法打开，请填写正确串口号!");
	 
	 i= OpenComConfig (gComHI, "", 9600, 0, 8, 1, 512, 512); //大电流发生器	
	 SetComTime (gComHI, 1.0);    
	 if(i<0)
		 InsertTableMsg("错误提示","大电流发生器串口无法打开，请填写正确串口号!");
	 
	 i= OpenComConfig (gComHV, "", 9600, 0, 8, 1, 512, 512);
	 SetComTime (gComHV, 1.0);    
	 if(i<0)
		 InsertTableMsg("错误提示","高压发生器串口无法打开，请填写正确串口号!");
	 
	 i= OpenComConfig (gComInsulation, "", 9600, 0, 8, 1, 4096, 4096);
	 SetComTime (gComInsulation, 1.0);   	 
	 if(i<0)
		 InsertTableMsg("错误提示","绝缘发生器串口无法打开，请填写正确串口号!");
	 
	  //发送停止命令，防止一直有数据传上来
	 sprintf(gBufTxd, "%s", "[E]>MA0#"); 
	 SendNetCMD(_NET_DAQ_STOP,gBufTxd);
	
	//初始化LED灯
	SetCtrlAttribute (panelHandle, PANEL_LED, ATTR_OFF_COLOR, VAL_GREEN);     	
	RunUserInterface ();  //调用该函数后，运行用户界面
	DiscardPanel (panelHandle);  
	return 0;
}

//软件设置，确认
int CVICALLBACK PANEL_3_ButtonQuit (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (pH_DisplaySet);
			GetCtrlVal(pH_DisplaySet, PANEL_3_CHK1, &gDisplayAnnotation);
			GetCtrlVal(pH_DisplaySet, PANEL_3_NUMERIC, &gYAxisMin);
			GetCtrlVal(pH_DisplaySet, PANEL_3_NUMERIC_2, &gYAxisMax);
			GetCtrlVal(pH_DisplaySet, PANEL_3_CHK2, &giEnableAlarm);  
			GetCtrlVal(pH_DisplaySet, PANEL_3_NUMERIC_3, &giBreakCriteria);
			break;
	}
	return 0;
}

 //管理员登录，退出
int CVICALLBACK PANEL_4_ButtomQuit (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (pH_Login);
			break;
	}
	return 0;
}

//退出
int CVICALLBACK PANEL_Quit (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SaveConfig();
			SaveChannelCfg();
			//断开网络连接
			DisconnectFromTCPServer (pH_NetHandle);
			SavePassword();
			CloseCom (gComRLY);
			QuitUserInterface(0); //退出用户界面 
			break;
	}
	return 0;
}

//运行记录设置，确认
int CVICALLBACK PANEL_9_ButtomQuit (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(pH_RecordSet,PANEL_9_CHK_SaveData, &gSaveData);
			GetCtrlVal(pH_RecordSet,PANEL_9_NUMERIC, &gFileSavePeriod);
			GetCtrlVal(pH_RecordSet,PANEL_9_CHK_SaveAlarm, &gSaveAlarm);
			GetCtrlVal(pH_RecordSet,PANEL_9_CHK_SaveDeviceInfo, &gSaveDeviceInfo);
			HidePanel (pH_RecordSet);
			break;
	}
	return 0;
}

//网络连接面板，连接
int CVICALLBACK PANEL_2_Connect (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	int iNetStatus;
	switch (event)
	{
		case EVENT_COMMIT:
			SetWaitCursor(1);
			GetCtrlVal(pH_NetConnect, PANEL_2_IP,gNetIP);
			GetCtrlVal(pH_NetConnect, PANEL_2_PORT, &gNetPort);

//先断开原来开网络连接
			gNetConnectStatus = 0;
			DisconnectFromTCPServer (pH_NetHandle);

//重新打开自动重连定时器
			SetCtrlAttribute (panelHandle, PANEL_TIMER_AutoConnect, ATTR_ENABLED, 1);

			iNetStatus = ConnectToTCPServerEx (&pH_NetHandle, gNetPort, gNetIP, ClientTCPCB, NULL, 6000, TCP_ANY_LOCAL_PORT);
			SetWaitCursor(0);
			if(iNetStatus <0)
				SetCtrlVal(pH_NetConnect, PANEL_2_STR_NetStatus, "设备连接失败！");
			else
			{
				SetCtrlVal(pH_NetConnect, PANEL_2_STR_NetStatus, "设备连接成功！");
				gNetConnectStatus = 1;
			}
			break;
	}
	return 0;
}

//本定时器负责随时检测网络状态,根据gNetConnectStatus状态，更新控件状态 ，tick is 2S
int CVICALLBACK PANEL_NetStatus (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	int x,y;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			if(gNetConnectStatus ==1)
			{
				SetCtrlVal(panelHandle, PANEL_TXT_NetStatus, "设备连接正常！");
				SetCtrlAttribute (panelHandle, PANEL_PIC_Unconnect, ATTR_VISIBLE, 0);
				GetCtrlAttribute (panelHandle, PANEL_PIC_Connect, ATTR_VISIBLE, &x);
				if(x==1)
				{
					SetCtrlAttribute (panelHandle, PANEL_PIC_Connect, ATTR_VISIBLE, 0);
					SetCtrlAttribute (panelHandle, PANEL_PIC_ConnectGrey, ATTR_VISIBLE, 1);
				}
				else
				{
					SetCtrlAttribute (panelHandle, PANEL_PIC_Connect, ATTR_VISIBLE, 1);
					SetCtrlAttribute (panelHandle, PANEL_PIC_ConnectGrey, ATTR_VISIBLE, 0);
				}
			}

			else if(gNetConnectStatus ==0)
			{
				SetCtrlVal(panelHandle, PANEL_TXT_NetStatus, "设备连接中断！");
				SetCtrlAttribute (panelHandle, PANEL_PIC_Connect, ATTR_VISIBLE, 0);
				SetCtrlAttribute (panelHandle, PANEL_PIC_ConnectGrey, ATTR_VISIBLE, 0);
				SetCtrlAttribute (panelHandle, PANEL_PIC_Unconnect, ATTR_VISIBLE, 1);
			}

			break;
	}
	return 0;
}


//(停用)本定时器负责自动连接网络，每隔一分钟检测网络状态，如果网络断开，则自动重连
int CVICALLBACK PANEL_AutoConnect (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			sprintf(gBufTxd, "%s" ,"[E]?ET#");
			SendNetCMD(_NET_CONNECT_TEST, gBufTxd);
			break;
	}
	return 0;
}


int CVICALLBACK OnTABLE_MSG (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_DOUBLE_CLICK:
			DeleteTableRows (panelHandle, PANEL_TABLE_MSG, 1, -1);
			InsertTableRows (panelHandle, PANEL_TABLE_MSG, 1, 20, VAL_CELL_STRING); //the cell holds text data
			giMsgRowCurrent = 0;
			break;
	}
	return 0;
}

//向网络发送命令，由0.2S后的 定时器PANEL_Timer_CMD_Process，处理接收到得数据
void SendNetCMD (int iCMDCode, char*str)
{
	int i;
	int count;
	DisableCtrlCMD();
	gNetCMDCode = iCMDCode;

	gNetCMDRev = _DATA_CMD_IDLE;
	ResetTimer(panelHandle, PANEL_TIMER_CMD_Process);
	SetCtrlAttribute (panelHandle, PANEL_TIMER_CMD_Process, ATTR_ENABLED, 1); //使能处理命令timer
	count =  StringLength(str);
	i = ClientTCPWrite (pH_NetHandle, str, StringLength(str), _TCP_TIME_OUT);
	gTimerCNT = 0;
	EnableCtrlCMD();
}

int SendComCMD(int iPortNo, int iLen, char*str)
{
	char sComRD[50];
	int i;
	FlushInQ(iPortNo);
	FlushOutQ(iPortNo);
	i = ComWrt (iPortNo, str, iLen);
	if(i!=iLen)
	{
		MessagePopup("警告","串口命令未能正确执行，请重试！");
		return -1;
	}
	
	if(iPortNo == gComRLY)
	{
		i = ComRd(iPortNo, sComRD, 10);
		if(i<10)
			return -2;
		if((sComRD[1]!=0x7B) && (sComRD[2]!=0x07) &&(sComRD[3]!=0xAF) &&(sComRD[4]!=0xEC) &&(sComRD[5]!=0x66) &&(sComRD[6]!=0x48) &&(sComRD[7]!=0xC5) &&(sComRD[8]!=0x73) )
			return -2;
	}
	return 0;
}

void DisableCtrlCMD(void)
{

	return;
}
void EnableCtrlCMD(void)
{

	return;
}

//曲线显示
int CVICALLBACK OnBTNShortKey (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			InstallPopup (pH_ShortKeyMSG);
			break;
	}
	return 0;
}

//图形操作说明，**
int CVICALLBACK PANEL_12_Quit (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_ShortKeyMSG);
			break;
	}
	return 0;
}

void TCPReadReset(void)
{
	ClientTCPRead (pH_NetHandle, gBufRev, 2*gDataMaxLen, _TCP_TIME_OUT) ;   //清空缓冲区
	giNumRead  = 0;
	giTCPReadCNT = 0;
}

void ProcessTimeOutErr(int iCMD)
{
	int i;
	switch (iCMD)
	{
		case  _NET_CONNECT_TEST:
			gNetConnectStatus = 0;
			InsertTableMsg("网络中断","网络中断，自动重连中...");
			if(0>ConnectToTCPServerEx (&pH_NetHandle, gNetPort, gNetIP, ClientTCPCB, NULL, 2000, TCP_ANY_LOCAL_PORT))
			{
				InsertTableMsg("命令","网络自动重连成功");
				gNetConnectStatus = 1;
			}

			break;
		case  _NET_DAQ_START:
			InsertTableMsg("信息提示","通讯异常，请重试！");
			break;
		case   _NET_DAQ_STOP:
			InsertTableMsg("信息提示","通讯异常，请重试！");
			break;
		case  _NET_CLEAR_ALARM:
			InsertTableMsg("信息提示","通讯异常，请重试！");
			break;
		case   _NET_SET_ALARM:
		//	InsertTableMsg("信息提示","通讯异常，请重试！");
			break;
		case   _NET_SINGLE_DAQ:
			InsertTableMsg("信息提示","通讯异常，请重试！");
			break;			
		default:
			break ;
	}
}

void InsertTableMsg(char *str1, char *str2)
{
	Point MyPoint;
	char strTime[20];
	char strTmp[20];
	sprintf(strTmp, "%s", DateStr() );
	CopyString (strTime, 0, strTmp, 0, 5);
	sprintf(strTime, "%s  %s", strTime, TimeStr()) ;

	if(giMsgRowCurrent <20)
	{
		giMsgRowCurrent ++;
	}
	else
	{
		giMsgRowCurrent = 20;
		DeleteTableRows (panelHandle, PANEL_TABLE_MSG, 1, 1);
		InsertTableRows (panelHandle, PANEL_TABLE_MSG, -1, 1, VAL_CELL_STRING);
	}
	MyPoint.y = giMsgRowCurrent;

	MyPoint.x =1;
	SetTableCellAttribute (panelHandle, PANEL_TABLE_MSG, MyPoint, ATTR_CTRL_VAL, strTime);

	MyPoint.x =2;
	SetTableCellAttribute (panelHandle, PANEL_TABLE_MSG, MyPoint, ATTR_CTRL_VAL, str1);

	MyPoint.x =3;
	SetTableCellAttribute (panelHandle, PANEL_TABLE_MSG, MyPoint, ATTR_CTRL_VAL, str2);

	SetCtrlAttribute (panelHandle, PANEL_TABLE_MSG, ATTR_FIRST_VISIBLE_ROW, (giMsgRowCurrent>3)?(giMsgRowCurrent-3):1 );
}

void InsertGraphLabel(int channel)
{
	char *strTime ;
	char strText[100];
	strTime = TimeStr ();
	sprintf(strText, "通道%d数据:%s", channel ,strTime);

	SetCtrlAttribute (pH_CMD, PANEL_10_GRAPH1, ATTR_LABEL_TEXT, strText);
	SetCtrlAttribute (pH_CMD, PANEL_10_GRAPH1, ATTR_LABEL_LEFT, VAL_AUTO_CENTER);
}

//使能网络数据接收,tick is 0.2S
int CVICALLBACK PANEL_EnableRXD (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			giTCPDisable = 0;   //使能接收
			SetCtrlAttribute (panelHandle, PANEL_TIMER_EnableRXD, ATTR_ENABLED, 0);
			break;
	}
	return 0;
}

//开始线阻测试
int CVICALLBACK PANEL_DAQ_START (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf(gComBufT, "[M]>RL1#");  //打开线阻继电器
			i = SendComCMD(gComRLY,  strlen(gComBufT), gComBufT);
			if(-2 ==i)
				return -1;
			Delay(1);   /*
			SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_TEXT, "开始测量等"); 
			Delay(1);
			SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_TEXT, "开始测量等");
			Delay(1);
			SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_TEXT, "开始测量等"); 
			Delay(1);  
				*/
			giEnableDataRev = 1;    //启动数据刷新
			SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_TEXT, "开始测量");
			SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_COLOR, VAL_BLACK);
			sprintf(gBufTxd, "%s", "[E]>MA1#");
			SendNetCMD(_NET_DAQ_START, gBufTxd);
			break;
	}
	return 0;
}

//停止测量
int CVICALLBACK PANEL_DAQ_STOP (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf(gBufTxd, "%s", "[E]>MA0#");
			SendNetCMD(_NET_DAQ_STOP, gBufTxd);

			sprintf(gComBufT, "[M]!FW#");
		    SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
			break;
	}
	return 0;
}

//输出报表
int CVICALLBACK PANEL_Print (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	int i,iTmp;
	int iValueTmp;
	Point MyPoint;
	char *strTime ;
	char strText[100];
	int iColor;
	
	switch (event)
	{
		case EVENT_COMMIT:
			InstallPopup (pH_DeviceIP);  
			SetPanelAttribute (pH_DeviceIP, ATTR_WINDOW_ZOOM, VAL_MAXIMIZE); 
			GetNumTableColumns(panelHandle, PANEL_TABLE, &iTmp);
		//	DuplicateCtrl (panelHandle, PANEL_TABLE, pH_DeviceIP, "线阻测试结果", 50, 20); 
			
			for(i=0;i<iTmp;i++)
			{
				MyPoint.x = i+1;
				MyPoint.y = 1;
				GetTableCellAttribute (panelHandle, PANEL_TABLE, MyPoint, ATTR_CTRL_VAL, &iValueTmp);
				SetTableCellAttribute (pH_DeviceIP, PANEL_11_TABLE, MyPoint, ATTR_CTRL_VAL, iValueTmp);  				
				MyPoint.y = 2;
				GetTableCellAttribute (panelHandle, PANEL_TABLE, MyPoint, ATTR_CTRL_VAL, &iValueTmp);
				SetTableCellAttribute (pH_DeviceIP, PANEL_11_TABLE, MyPoint, ATTR_CTRL_VAL, iValueTmp);  				
			}
			GetNumTableColumns(panelHandle, PANEL_TABLE_2, &iTmp);
			for(i=0;i<iTmp;i++)
			{
				MyPoint.x = i+1;
				MyPoint.y = 1;
				GetTableCellAttribute (panelHandle, PANEL_TABLE_2, MyPoint, ATTR_CTRL_VAL, &iValueTmp);
				SetTableCellAttribute (pH_DeviceIP, PANEL_11_TABLE_2, MyPoint, ATTR_CTRL_VAL, iValueTmp);  				
				MyPoint.y = 2;
				GetTableCellAttribute (panelHandle, PANEL_TABLE_2, MyPoint, ATTR_CTRL_VAL, &iValueTmp);
				SetTableCellAttribute (pH_DeviceIP, PANEL_11_TABLE_2, MyPoint, ATTR_CTRL_VAL, iValueTmp);  				
			}
			
/*			
			
			//设置图1的曲线显示
			DeleteGraphPlot (pH_DeviceIP, PANEL_11_GRAPH1, -1, VAL_IMMEDIATE_DRAW);
			DeleteGraphPlot (pH_DeviceIP, PANEL_11_GRAPH2, -1, VAL_IMMEDIATE_DRAW);  
			for(i=0; i<60; i++)
			{
				if(giChanResSel[i]==1)
				{
					GetTableCellAttribute (panelHandle, PANEL_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, &iColor);
					PlotY (pH_DeviceIP, PANEL_11_GRAPH1, gfChanResVal[i], 1800, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, iColor);
				}
			} 
			strTime = TimeStr ();
			sprintf(strText, "信号通道数据:%s", strTime);
			SetCtrlAttribute (pH_DeviceIP, PANEL_11_GRAPH1, ATTR_LABEL_TEXT, strText);
			SetCtrlAttribute (pH_DeviceIP, PANEL_11_GRAPH1, ATTR_LABEL_LEFT, VAL_AUTO_CENTER);
			
			//设置图2的曲线显示
			for(i=0; i<60; i++)
			{
				if(giChanResSel[i]==2)
				{
					GetTableCellAttribute (panelHandle, PANEL_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, &iColor);    
					PlotY (pH_DeviceIP, PANEL_11_GRAPH2, gfChanResVal[i], 1800, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, iColor);
				}
			} 
			strTime = TimeStr ();
			sprintf(strText, "动力通道数据:%s", strTime);
			SetCtrlAttribute (pH_DeviceIP, PANEL_11_GRAPH2, ATTR_LABEL_TEXT, strText);
			SetCtrlAttribute (pH_DeviceIP, PANEL_11_GRAPH2, ATTR_LABEL_LEFT, VAL_AUTO_CENTER);	
*/
			
			//设置时间显示
			GetCtrlVal(panelHandle, PANEL_STRING, strText);
			SetCtrlVal(pH_DeviceIP, PANEL_11_STRING_2, strText);
			
			GetTableCellVal (panelHandle, PANEL_TABLE_5, MakePoint(1,1), strText);
			i = strlen (strText);															  
			SetTableCellValFromIndex (pH_DeviceIP, PANEL_11_TABLE_5, MakePoint(1,1), (i<5)?0:1);  //设置合格与否 	
//复制电流测试控件
			DuplicateCtrl (pH_HITest, PANEL_8_TABLE_4, pH_DeviceIP, "大电流测试结果", 350, 20);
			DuplicateCtrl (pH_HVTest, PANEL_7_TABLE_4, pH_DeviceIP, "耐压测试结果", 480, 20);  	
			DuplicateCtrl (pH_InsulationTest, PANEL_13_TABLE_6, pH_DeviceIP, "绝缘仪测试结果", 660, 20);  				
		//	DuplicateCtrl (pH_InsulationTest, PANEL_13_TABLE_5, pH_DeviceIP, "", 740, 20);  				
			
			break;
	}
	return 0;
}

//保存结果
int CVICALLBACK PANEL_SaveScreen (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	char strFileName[260];
	char strYMDName[10];
	char strHMSName[10];
	
	switch (event)
	{
		case EVENT_COMMIT: 
			GetYMDNameStr(strYMDName);
			GetHMSNameStr(strHMSName);	
			sprintf(strFileName,"%s%s-%s.bmp", "D:\\Host1\\", strYMDName, strHMSName); 			
			SavePanelDisplayToFile (panelHandle, 0, VAL_ENTIRE_OBJECT, -1, -1, strFileName);
			InsertTableMsg("数据保存完毕！", strFileName);
			break;
	}
	return 0;
}

//打印结果，打印报表
int CVICALLBACK PANEL_11_Print (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

		    //SetPrintAttribute(); 
			PrintPanel (panel, "", 0, VAL_FULL_PANEL, 1);
			break;
	}
	return 0;
}

//初始化控件
void InitialDefaultCtrl(void)
{
	int i,iTmp;
	char sTmp[10];
	DeleteTableColumns (panelHandle, PANEL_TABLE,  1, -1);	  //主界面上，线阻测试table
	DeleteTableColumns (panelHandle, PANEL_TABLE_2, 1, -1); 
	DeleteTableColumns (pH_DeviceIP, PANEL_11_TABLE,  1, -1); //打印结果上的table
	DeleteTableColumns (pH_DeviceIP, PANEL_11_TABLE_2, 1, -1);  	
	//设置主面板的列数
	for(i=0; i<60; i++)
	{
		if((i < 7)&&((giChanResSel[i]==1)||(giChanResSel[i]==3))) // 动力通道
		{   //PANEL_TABLE,主界面上，线阻测试table1 
			InsertTableColumns (panelHandle, PANEL_TABLE, -1, 1, VAL_CELL_NUMERIC);//在表的末尾,插入1列 
			GetNumTableColumns (panelHandle, PANEL_TABLE, &iTmp);  //获取列数
			sprintf(sTmp, "%d", i+1);
			SetTableColumnAttribute (panelHandle, PANEL_TABLE, iTmp, ATTR_USE_LABEL_TEXT, 1); // 1 = Use custom text as label   
			SetTableColumnAttribute (panelHandle, PANEL_TABLE, iTmp, ATTR_LABEL_TEXT, sTmp);
			SetTableColumnAttribute (panelHandle, PANEL_TABLE, iTmp, ATTR_COLUMN_WIDTH, 38);
			
			InsertTableColumns (pH_DeviceIP, PANEL_11_TABLE, -1, 1, VAL_CELL_NUMERIC);//在表的末尾,插入1列 
			GetNumTableColumns (pH_DeviceIP, PANEL_11_TABLE, &iTmp);  //获取列数
			sprintf(sTmp, "%d", i+1);
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE, iTmp, ATTR_USE_LABEL_TEXT, 1); // 1 = Use custom text as label   
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE, iTmp, ATTR_LABEL_TEXT, sTmp);
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE, iTmp, ATTR_COLUMN_WIDTH, 38);
			
			
			
		}
		else if((giChanResSel[i]==2)||(giChanResSel[i]==3)) // 信号通道
		{    //PANEL_TABLE_2,主界面上，线阻测试table2
			InsertTableColumns (panelHandle, PANEL_TABLE_2, -1, 1, VAL_CELL_NUMERIC);	 //主界面上，线阻测试table2 
			GetNumTableColumns (panelHandle, PANEL_TABLE_2, &iTmp);
			sprintf(sTmp, "%d", i+1);
			SetTableColumnAttribute (panelHandle, PANEL_TABLE_2, iTmp, ATTR_USE_LABEL_TEXT, 1);// 1 = Use custom text as label   			
			SetTableColumnAttribute (panelHandle, PANEL_TABLE_2, iTmp, ATTR_LABEL_TEXT, sTmp);
			SetTableColumnAttribute (panelHandle, PANEL_TABLE_2, iTmp, ATTR_COLUMN_WIDTH, 38);  		
			
			InsertTableColumns (pH_DeviceIP, PANEL_11_TABLE_2, -1, 1, VAL_CELL_NUMERIC);	 //主界面上，线阻测试table2 
			GetNumTableColumns (pH_DeviceIP, PANEL_11_TABLE_2, &iTmp);
			sprintf(sTmp, "%d", i+1);
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE_2, iTmp, ATTR_USE_LABEL_TEXT, 1);// 1 = Use custom text as label   			
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE_2, iTmp, ATTR_LABEL_TEXT, sTmp);
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE_2, iTmp, ATTR_COLUMN_WIDTH, 38);  			
			
		}		
	}
//设置打印面板的列数	
/*	for(i=0; i<60; i++)
	{
		if(giChanResSel[i]==1)
		{
			InsertTableColumns (pH_DeviceIP, PANEL_11_TABLE, -1, 1, VAL_CELL_NUMERIC);
			GetNumTableColumns (pH_DeviceIP, PANEL_11_TABLE, &iTmp);
			sprintf(sTmp, "%d", i+1);
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE, iTmp, ATTR_USE_LABEL_TEXT, 1);
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE, iTmp, ATTR_LABEL_TEXT, sTmp);
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE, iTmp, ATTR_COLUMN_WIDTH, 40);
		}
		if(giChanResSel[i]==2)
		{
			InsertTableColumns (pH_DeviceIP, PANEL_11_TABLE_2, -1, 1, VAL_CELL_NUMERIC);
			GetNumTableColumns (pH_DeviceIP, PANEL_11_TABLE_2, &iTmp);
			sprintf(sTmp, "%d", i+1);
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE_2, iTmp, ATTR_USE_LABEL_TEXT, 1);   			
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE_2, iTmp, ATTR_LABEL_TEXT, sTmp);
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE_2, iTmp, ATTR_COLUMN_WIDTH, 40);  			
		}		
	}	*/
	
	for(i=0;i<60;i++)
	{   //显示滑道阻值
		if( (giChanResSel[i]!=1) && (giChanResSel[i]!=2) )
			SetTableCellAttribute (panelHandle, PANEL_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_LT_GRAY);
		else 			
			SetTableCellAttribute (panelHandle, PANEL_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE);
	}
	for(i=0;i<60;i++)
	{   //合格判定，通断，噪声，闪断
		if( (giChanResSel[i]!=1) && (giChanResSel[i]!=2) )
			SetTableCellRangeAttribute (panelHandle, PANEL_TABLE_4, MakeRect(1,i+1,3,1), ATTR_TEXT_BGCOLOR, 0x00F0F0F0);
		else 
			SetTableCellRangeAttribute (panelHandle, PANEL_TABLE_4, MakeRect(1,i+1,3,1), ATTR_TEXT_BGCOLOR, VAL_WHITE);   
	}
//设置四个阈值显示	
	SetTableCellAttribute (panelHandle, PANEL_TABLE_8, MakePoint(1,2), ATTR_CTRL_VAL, giChanResSel[60]);//设定噪声阈值最大值
	SetTableCellAttribute (panelHandle, PANEL_TABLE_8, MakePoint(1,1), ATTR_CTRL_VAL, giChanResSel[64]);//设定噪声阈值最小值
	SetTableCellAttribute (panelHandle, PANEL_TABLE_9, MakePoint(1,2), ATTR_CTRL_VAL, giChanResSel[61]);//设定噪声阈值最大值 
	SetTableCellAttribute (panelHandle, PANEL_TABLE_9, MakePoint(1,1), ATTR_CTRL_VAL, giChanResSel[65]);//设定噪声阈值最小值
	SetTableCellAttribute (pH_DeviceIP, PANEL_TABLE_8, MakePoint(1,2), ATTR_CTRL_VAL, giChanResSel[62]);//设定噪声阈值最大值 
	SetTableCellAttribute (pH_DeviceIP, PANEL_TABLE_8, MakePoint(1,1), ATTR_CTRL_VAL, giChanResSel[66]);//设定噪声阈值最小值
	SetTableCellAttribute (pH_DeviceIP, PANEL_TABLE_9, MakePoint(1,2), ATTR_CTRL_VAL, giChanResSel[63]);//设定噪声阈值最大值 
	SetTableCellAttribute (pH_DeviceIP, PANEL_TABLE_9, MakePoint(1,1), ATTR_CTRL_VAL, giChanResSel[67]);//设定噪声阈值最小值
//设置大电流HI面板滑环的显示
	for(i=0;i<7;i++)  // mmsun
	{   // 7路动力滑道，双击某一滑道，启动该滑道测量 
		if(/* (giChanResSel[i]!=1) && */(giChanResSel[i]==1) )
			SetTableCellAttribute (pH_HITest, PANEL_8_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE);
		else 			
			SetTableCellAttribute (pH_HITest, PANEL_8_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_LT_GRAY);
	}
	for(i=0;i<7;i++)   //mmsun
	{   // 7路动力滑道，测试结果 
		if( /*(giChanResSel[i]!=1) &&*/ (giChanResSel[i]==1) )
			SetTableCellRangeAttribute (pH_HITest, PANEL_8_TABLE_4, MakeRect(1,i+1,1,1), ATTR_TEXT_BGCOLOR, VAL_WHITE);
		else 
			SetTableCellRangeAttribute (pH_HITest, PANEL_8_TABLE_4, MakeRect(1,i+1,1,1), ATTR_TEXT_BGCOLOR, 0x00F0F0F0);   
	}	
	
//设置HV面板滑环的显示
	for(i=0;i<40;i++)
	{   //40路
		if( (giChanResSel[i]==1) || (giChanResSel[i]==2) || (giChanResSel[i]==3) )
		{	
		   SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE);
		   
		   SetTableCellRangeAttribute (pH_HVTest, PANEL_7_TABLE_4, MakeRect(1,i+1,1,2), ATTR_TEXT_BGCOLOR, VAL_WHITE);   
		   SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_4, MakePoint(i+1,1 ), ATTR_CTRL_VAL, "-");
		   SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_4, MakePoint(i+1,2 ), ATTR_CTRL_VAL, "-");
		   
		   if(i<20)
		   {
			 SetTableCellRangeAttribute (pH_HVTest, PANEL_7_TABLE_5, MakeRect(1,i+1,1,2), ATTR_TEXT_BGCOLOR, VAL_WHITE);
		     SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_5, MakePoint(i+1,1 ), ATTR_CTRL_VAL, "-"); 
			 SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_5, MakePoint(i+1,2 ), ATTR_CTRL_VAL, "-");  
		   }
		   else
		   {
			 SetTableCellRangeAttribute (pH_HVTest, PANEL_7_TABLE_6, MakeRect(1,i+1-20,1,2), ATTR_TEXT_BGCOLOR, VAL_WHITE);
		     SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_6, MakePoint(i+1-20,1 ), ATTR_CTRL_VAL, "-"); 
			 SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_6, MakePoint(i+1-20,2 ), ATTR_CTRL_VAL, "-");  
			   
		   }
		}
		else 			
		{	
		   SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR,VAL_LT_GRAY );
		   SetTableCellRangeAttribute (pH_HVTest, PANEL_7_TABLE_4, MakeRect(1,i+1,2,1), ATTR_TEXT_BGCOLOR, VAL_LT_GRAY);   
		   
		   if(i<20)
		   {
			  SetTableCellRangeAttribute (pH_HVTest, PANEL_7_TABLE_5, MakeRect(1,i+1,1,2), ATTR_TEXT_BGCOLOR, VAL_LT_GRAY);  
		   }
		   else
		   {
			  SetTableCellRangeAttribute (pH_HVTest, PANEL_7_TABLE_6, MakeRect(1,i+1-20,1,2), ATTR_TEXT_BGCOLOR, VAL_LT_GRAY); 
		   }
		}
	}
	/*
	for(i=0;i<40;i++)
	{
		if( (giChanResSel[i]==1) || (giChanResSel[i]==2)|| (giChanResSel[i]==3))
		{	
		   SetTableCellRangeAttribute (pH_HVTest, PANEL_7_TABLE_4, MakeRect(1,i+1,1,2), ATTR_TEXT_BGCOLOR, VAL_WHITE);   
		   SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_4, MakePoint(i+1,1 ), ATTR_CTRL_VAL, "-");
		   SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_4, MakePoint(i+1,2 ), ATTR_CTRL_VAL, "-");
		}
		else 
		{
		   SetTableCellRangeAttribute (pH_HVTest, PANEL_7_TABLE_4, MakeRect(1,i+1,2,1), ATTR_TEXT_BGCOLOR, VAL_LT_GRAY);
		}
	}
	*/
//设置Insulation面板	
	for(i=0;i<40;i++)
	{
		if( (giChanResSel[i]!=1) && (giChanResSel[i]!=2) )
			SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_LT_GRAY);
		else 			
			SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE);
	}

	/*
	for(i=0;i<20;i++)
	{
		if( (giChanResSel[i]!=1) && (giChanResSel[i]!=2) )
			SetTableCellRangeAttribute (pH_InsulationTest, PANEL_13_TABLE_4, MakeRect(1,i+1,2,1), ATTR_TEXT_BGCOLOR, 0x00F0F0F0);
		else 
			SetTableCellRangeAttribute (pH_InsulationTest, PANEL_13_TABLE_4, MakeRect(1,i+1,2,1), ATTR_TEXT_BGCOLOR, VAL_WHITE);   
	}
	for(i=0;i<20;i++)
	{
		if( (giChanResSel[i+20]!=1) && (giChanResSel[i+20]!=2) )
			SetTableCellRangeAttribute (pH_InsulationTest, PANEL_13_TABLE_5, MakeRect(1,i+1,2,1), ATTR_TEXT_BGCOLOR, 0x00F0F0F0);
		else 
			SetTableCellRangeAttribute (pH_InsulationTest, PANEL_13_TABLE_5, MakeRect(1,i+1,2,1), ATTR_TEXT_BGCOLOR, VAL_WHITE);   
	}	
*/	
}

void SetAlarmDisplay(void)  //结果显示处理
{
	double fMax, fMin;
	int i,j;
	int iTmp=0;
	int iAlalrmNum;
	int iMaxIdx, iMinIdx;
	int iAlarmFlag=0;
	char strTmp[256];
	
	SetTableCellValFromIndex (panelHandle, PANEL_TABLE_5, MakePoint(1,1), 0);  //设置合格与否
	for(i=0;i<7;i++)
	{
		if((giChanResSel[i]==1)||(giChanResSel[i]==3))   //设置信号通道的上限值和下限值
		{
			iTmp++;
			MaxMin1D (gfChanResVal[i], gChanDataLen, &fMax, &iMaxIdx, &fMin, &iMinIdx);
			SetTableCellAttribute (panelHandle, PANEL_TABLE, MakePoint(iTmp,1), ATTR_CTRL_VAL, (int)fMax);
			SetTableCellAttribute (panelHandle, PANEL_TABLE, MakePoint(iTmp,2), ATTR_CTRL_VAL, (int)fMin);
		}
	}
	iTmp = 0;
	for(i=7;i<60;i++)
	{
		if((giChanResSel[i]==2)||(giChanResSel[i]==3))   //设置动力通道的上限值和下限值
		{
			iTmp++;
			MaxMin1D (gfChanResVal[i], gChanDataLen, &fMax, &iMaxIdx, &fMin, &iMinIdx);
			SetTableCellAttribute (panelHandle, PANEL_TABLE_2, MakePoint(iTmp,1), ATTR_CTRL_VAL, (int)fMax);
			SetTableCellAttribute (panelHandle, PANEL_TABLE_2, MakePoint(iTmp,2), ATTR_CTRL_VAL, (int)fMin);
		}
	}
	
	//根据总的数据进行报警处理
	for(i=0;i<60;i++)
	{
		SetTableCellAttribute (panelHandle, PANEL_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_LT_GRAY); 
		SetTableCellRangeAttribute (panelHandle, PANEL_TABLE_4, MakeRect(1,i+1,3,1), ATTR_TEXT_BGCOLOR, 0x00F0F0F0);  		
	}
	for(i=0;i<60;i++)
	{
		iAlalrmNum = 0;
		iTmp = giChanResSel[i];
		if((iTmp==1)|| (iTmp==2)||(iTmp==3))
		{
			SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,1), ATTR_TEXT_BGCOLOR, 0x00F0F0F0);   			
			SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,2), ATTR_TEXT_BGCOLOR, 0x00F0F0F0);  			
			SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,3), ATTR_TEXT_BGCOLOR, 0x00F0F0F0);  
			
			for(j=0; j<gChanDataLen; j++)
			{
				if(gfChanResVal[i][j] >(iTmp==1? giChanResSel[68]:giChanResSel[69]) )//通断判断
					iAlalrmNum++;
			}
			
			SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,2), ATTR_TEXT_BGCOLOR, VAL_GREEN);  			//先把噪声设为绿色
			if(iAlalrmNum==0) 
			{
				SetTableCellAttribute (panelHandle, PANEL_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_GREEN);		//如果无通断报警，电阻以及三色灯设置为绿色
				SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,1), ATTR_TEXT_BGCOLOR, VAL_GREEN);  
				SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,3), ATTR_TEXT_BGCOLOR, VAL_GREEN);  
			}
			if(iAlalrmNum>=1) 
			{
				SetTableCellAttribute (panelHandle, PANEL_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_RED);	 //如果有通断报警，电阻设置为红色
				iAlarmFlag = 1;
				sprintf(strTmp,"%s%d%s","滑环通道",i+1,"报警");
				InsertTableMsg("超出阈值提示：",strTmp);   
			}
			if((iAlalrmNum>=1) && (iAlalrmNum < (0.01*giBreakCriteria*gChanDataLen)))
			{
				SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,3), ATTR_TEXT_BGCOLOR, VAL_RED);  //闪断  
			}
			if(iAlalrmNum >= (0.01*giBreakCriteria*gChanDataLen))
				SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,1), ATTR_TEXT_BGCOLOR, VAL_RED);  //通断 
		}
	}
	
	for(i=0;i<60;i++) //噪声阈值是否超限
	{
		MaxMin1D (gfChanResVal[i], gChanDataLen, &fMax, &iMaxIdx, &fMin, &iMinIdx);  		
		if((i<7)&&((giChanResSel[i]==1)||(giChanResSel[i]==3))) //动力环
		{
			if( (fMax-fMin)>(giChanResSel[60] -giChanResSel[64]))
				SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,2), ATTR_TEXT_BGCOLOR, VAL_RED);  	//噪声设置为红色				
		}
		else if((giChanResSel[i]==2)||(giChanResSel[i]==3))//信号环
		{
			if( (fMax-fMin)>(giChanResSel[61] -giChanResSel[65]))  
				SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,2), ATTR_TEXT_BGCOLOR, VAL_RED);  	//				
		}			
   
	}
	
	if(iAlarmFlag ==1)
	{
		SetTableCellValFromIndex (panelHandle, PANEL_TABLE_5, MakePoint(1,1), 1);  //设置合格与否     
		SetCtrlAttribute (panelHandle, PANEL_LED_1, ATTR_OFF_COLOR, VAL_RED);   		
		iAlarmFlag = 0;
		giEnableDataRev = 0;    //终止数据刷新
		SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_TEXT, "已经暂停");
		SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_COLOR, VAL_RED);
		if(giEnableAlarm)
		{
			sprintf(gBufTxd, "%s", "[E]>AL1#");  				
			SendNetCMD(_NET_SET_ALARM, gBufTxd); 
		}
	}
}

//消除报警
int CVICALLBACK PANEL_DEL_ALARM (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (panelHandle, PANEL_LED_1, ATTR_OFF_COLOR, VAL_TRANSPARENT);
			InsertTableMsg("操作说明","报警提示已清除！");
			sprintf(gBufTxd, "%s", "[E]>AL0#");  				
			SendNetCMD(_NET_CLEAR_ALARM, gBufTxd);  
			break;
	}
	return 0;
}

//单次测量
int CVICALLBACK PANEL_Single_DAQ (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	int i,length;
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf(gComBufT, "[M]>RL1#");  //打开线组继电器
			length = strlen(gComBufT);
			if(-2==SendComCMD(gComRLY, strlen(gComBufT), gComBufT) )
				return -1;
			Delay(1);
			giEnableDataRev = 1;    //启动数据刷新 	
			sprintf(gBufTxd, "%s", "[E]>MS1#");  				
			SendNetCMD(_NET_SINGLE_DAQ, gBufTxd);  	
			break;
	}
	return 0;
}

//切换到下一步，大电流测试
int CVICALLBACK PANEL_HV_Test (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
	//		HidePanel(panelHandle);
			sprintf(gComBufT, "[M]!FW#");
			i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
			if(i<0)
				return -1;
			InstallPopup(pH_HITest);									
			SetPanelAttribute (pH_HITest, ATTR_WINDOW_ZOOM, VAL_MAXIMIZE);    			

			break;
	}
	return 0;
}

//设备串口设置，确认
int CVICALLBACK PABEL_6_SetCom (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	int i,i1,i2,i3,i4;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(pH_ComSet, PANEL_6_NUMERIC1, &i1);
			GetCtrlVal(pH_ComSet, PANEL_6_NUMERIC2, &i2);
			GetCtrlVal(pH_ComSet, PANEL_6_NUMERIC3, &i3);
			GetCtrlVal(pH_ComSet, PANEL_6_NUMERIC4, &i4);  
			
			if( (i1==i2) || (i1==i3) || (i1==i4) || (i2==i3) || (i2==i4) || (i3==i4)  )
			{
				MessagePopup("提示","串口号不能有相同，请修改串口号！");
				return -1;
			}
			gComRLY = i1;
			gComHV = i2;
			gComHI = i3;  //大电流发生器  
			gComInsulation = i4;
			
			
			//串口配置
			CloseCom(gComRLY);
			CloseCom(gComHI); //大电流发生器  	
			CloseCom(gComHV);
			CloseCom(gComInsulation);
	
			i= OpenComConfig (gComRLY, "", 9600, 0, 8, 1, 512, 512);
			if(i<0)
				 InsertTableMsg("错误提示","继电器控制模块串口无法打开，请填写正确串口号!");
		
			i= OpenComConfig (gComHI, "", 9600, 0, 8, 1, 512, 512);//大电流发生器  
			if(i<0)
				 InsertTableMsg("错误提示","大电流发生器串口无法打开，请填写正确串口号!");

			i= OpenComConfig (gComHV, "", 9600, 0, 8, 1, 512, 512);
			if(i<0)
				 InsertTableMsg("错误提示","高压发生器串口无法打开，请填写正确串口号!");
			SetComTime (gComHV, 1.0);   

			i= OpenComConfig (gComInsulation, "", 9600, 0, 8, 1, 4096, 4096);
			if(i<0)
				 InsertTableMsg("错误提示","绝缘发生器串口无法打开，请填写正确串口号!");
			
			break;
	}
	return 0;
}

// 设备串口设置，退出
int CVICALLBACK PANEL_6_Exit (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_ComSet);
			break;
	}
	return 0;
}

//打印结果，保存文件
int CVICALLBACK PANEL_11_Save_Result (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	char strFileName[260];
	char strYMDName[10];
	char strHMSName[10];
	
	switch (event)
	{
		case EVENT_COMMIT: 
			GetYMDNameStr(strYMDName);
			GetHMSNameStr(strHMSName);	
			sprintf(strFileName,"%s%s-%s.bmp", "D:\\Host1\\", strYMDName, strHMSName); 			
			SavePanelDisplayToFile (panel, 0, VAL_ENTIRE_OBJECT, -1, -1, strFileName);
			InsertTableMsg("数据保存完毕！", strFileName);
			break;
	}
	return 0;
}

int CVICALLBACK PANEL_DAQ_CloseRelay (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf(gComBufT, "[M]>RL1#");  //打开线阻继电器
			i = SendComCMD(gComRLY,  strlen(gComBufT), gComBufT);
			if(-2 ==i)
				return -1;
			

			break;
	}
	return 0;
}

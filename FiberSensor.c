/*
   �������������
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

static int giTCPDisable = 0;  //���յ������Ҫ�ر�TCP�Ľ��ջ��岿��
static int giMsgRowCurrent =0;
static int giTCPReadCNT=0;
static int gRevStatus = 0;
static int giNumRead  = 0;	  //�Ѿ��յ�����������
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
//���ö�ʱ������綨ʱ��
			ResetTimer(panelHandle, PANEL_TIMER_AutoConnect);//�Զ�����timer
			gNetConnectStatus = 1;

			if(giTCPDisable)  //=1��ֹ����
				return 0;

			if(giTCPReadCNT==0)
			{
				i = ClientTCPRead (pH_NetHandle, gBufRev, 13, _TCP_TIME_OUT) ;   //��һ���ȶ�ȡ13���ֽ�
				CopyBytes (gDataBuf, 0, gBufRev, 0, i);
				giTCPReadCNT++;
				giNumRead = i;
				iStatus = (gBufRev[1]==0x7B)&&(gBufRev[2]==0x07)&&(gBufRev[3]==0xAF)&&(gBufRev[4]==0xEC)
						  &&(gBufRev[5]==0x66)&&(gBufRev[6]==0x48)&&(gBufRev[7]==0xC5)&&(gBufRev[8]==0x73);
				if( (i!=13) || (iStatus ==0) ) //�ָ���ʼ״̬
					TCPReadReset();//��ջ�����
				return 0;
			}

			iTmp = gDataBuf[9]+(gDataBuf[10]<<8) + (gDataBuf[11]<<16) + (gDataBuf[12]<<24);  //����Ҫ���յ�����
			if( ((iTmp - giNumRead)>0) && ( (iTmp - giNumRead) <2*gDataMaxLen))              //��ֹ��Ϊ��λ�����ݳ������ɵ��������
			{
				i = ClientTCPRead (pH_NetHandle, gBufRev, iTmp - giNumRead, _TCP_TIME_OUT) ;   //
				CopyBytes (gDataBuf, giNumRead, gBufRev, 0, i);
				giNumRead = giNumRead + i;
			}
			giTCPReadCNT++;
            //�������100��������Ȼû�ж����
			if(giTCPReadCNT==100)
			{
				TCPReadReset(); //��ջ�����   
				return 0;
			}
            //������ݶ�ȡ��ϲ���������ȷ
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
                    //-----������ֱ���������ֹ���ݴ���					
					if(giEnableDataRev ==0)
						return -1; 

			        giDataOrder = giDataOrder|(1<<gDataBuf[13]);
					for(i=0; i<60; i++)
					{
						for(j=0; j<900; j++)
						{
							gfChanResVal[i][gDataBuf[13]*900+j] = gDataBuf[i*1800+22+2*j] + (gDataBuf[i*1800+23+2*j]<<8);//gDataBuf[13]��ʾ�ڼ��δ��������
							if(gfChanResVal[i][gDataBuf[13]*900+j] >=32768)
								gfChanResVal[i][gDataBuf[13]*900+j] =(0x7fff & (gDataBuf[i*1800+22+2*j] + (gDataBuf[i*1800+23+2*j]<<8)) ) * 16;
						}
					}
					//����LED����ʾ״̬
					GetCtrlVal(panelHandle, PANEL_LED, &i);//����ָʾ
					if(i==0)	//�رյ�ʱ��
					{
						GetCtrlAttribute (panelHandle, PANEL_LED, ATTR_OFF_COLOR, &iTmp);
						if(iTmp == VAL_GREEN)
							SetCtrlAttribute (panelHandle, PANEL_LED, ATTR_OFF_COLOR, VAL_DK_GREEN);
						else
							SetCtrlAttribute (panelHandle, PANEL_LED, ATTR_OFF_COLOR, VAL_GREEN); 
					}
					
					if(gDataBuf[13]==1)   //��0��1�����ݴ������
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
					
					//��ͼ�������ǩ
					DeleteGraphPlot (pH_CMD, PANEL_10_GRAPH1, -1, VAL_IMMEDIATE_DRAW);
					PlotY (pH_CMD, PANEL_10_GRAPH1, gfChanResVal[giCurrentGraph-1], 1800, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_RED);
					InsertGraphLabel(giCurrentGraph);

					//����ʱ��
					sprintf(strTmp, "%s %s", DateStr(), TimeStr());
					SetCtrlVal(panelHandle, PANEL_STRING, strTmp);//���ò���ʱ��
					
					SetAlarmDisplay(); //�����ʾ����
					
					break;

//���������λ����Ҫ�ö�ʱ�������������
				default:
					gNetCMDRev   = _DATA_CMD_READY; //ֻҪ���յ����������ʾ��ǰ��ready״̬����������Ҫ�����ʱ����������
					giTCPDisable = 1;//��ֹ����	 //����0.2s��ſ�ʼ�����������ݣ��Է�ֹ�����յ����������Ժ󣬶�ʱ����û�����ô����������������������ˣ������ݸ�������
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

//����״̬Timer 0.2S internel����������������������ö�ʱ����������յ�������
int CVICALLBACK PANEL_Timer_CMD_Process (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int mmsun;
	int i;

	switch (event)
	{
		case EVENT_TIMER_TICK:
			gTimerCNT ++;
			//û�յ����ݣ���û�г�ʱ
			if( (gTimerCNT<_TCP_CMD_TIME_OUT_CNT) && (gNetCMDRev ==_DATA_CMD_IDLE) )	   //�����ۼ�һ����ʱ����0.2s���ж��Ƿ�ɹ�
				return 0;
//��ʱ��û�յ�����
			else if((gTimerCNT>=_TCP_CMD_TIME_OUT_CNT) && (gNetCMDRev ==_DATA_CMD_IDLE) )  //���ʼ�յ���IDLE��ͨ���ж�
				ProcessTimeOutErr(gNetCMDCode);

//�յ������� ,����������
			else if(gNetCMDRev == _DATA_CMD_READY)  //gNetCMDRev �� ClientTCPCB()���棬�ж���λ
				switch (gNetCMDCode)
				{
					case  _NET_CONNECT_TEST:
						if(gDataBuf[13] == 'Y')
							gNetConnectStatus = 1;
						break;
//��������λ���Լ�----------------------------------------------------
					case  _NET_DAQ_START:
						if(gDataBuf[13] == 'Y')
						{
							InsertTableMsg("����","������������ͳɹ�");
							SetMenuBarAttribute (MENUBAR, MENUBAR, ATTR_DIMMED, 1);
						}
						else
							InsertTableMsg("����","�������������ʧ��");
						break;
					case   _NET_DAQ_STOP:
						if(gDataBuf[13] == 'Y')
							InsertTableMsg("����","ֹͣ��������ͳɹ�");
						else
							InsertTableMsg("����","ֹͣ���������ʧ��");
						break;

					case   _NET_CLEAR_ALARM:
						if(gDataBuf[13] == 'Y')
							InsertTableMsg("����","��������ź�����ͳɹ�");
						else
							InsertTableMsg("����","��������ź������ʧ��");
						break;
						
					case   _NET_SINGLE_DAQ:
						if(gDataBuf[13] == 'Y')
							InsertTableMsg("����","�������������");
						else
							InsertTableMsg("����","���β���ʧ�ܣ�");
						break;						
						
//--------------------------------------------------------
					default:
						break ;
				}
//�رյ�ǰ��ʱ��------------------------------------------------------------------------------------
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
	//�Զ�����IP
	i = ConnectToTCPServerEx (&pH_NetHandle, gNetPort, gNetIP, ClientTCPCB, NULL, _TCP_TIME_OUT, TCP_ANY_LOCAL_PORT);
	if(i < 0)
		MessagePopup("��ʾ","���ܴ���������!");
	
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
	//��ʾ���򴰿ڴ�С����
	SetPanelAttribute (panelHandle, ATTR_WINDOW_ZOOM, VAL_MAXIMIZE);
	GetScreenSize (&giScreenHight, &giScreenWidth);
	giGraphAreaWidth = giScreenWidth - _GRAPH_AREA_LEFT;
	giGraphAreaHight = giScreenHight - _GRAPH_AREA_TOP - _MSG_AREA_HIGHT;

	ctrlArrayID = GetCtrlArrayFromResourceID (panelHandle, CTRLARRAY_Scale);
	MoveCtrlArray (ctrlArrayID, giGraphAreaWidth/2, 0);

//������Ϣ���ڵ�����
	SetCtrlAttribute (panelHandle, PANEL_TABLE_MSG, ATTR_TOP,   giScreenHight - _MSG_AREA_HIGHT );
	SetCtrlAttribute (panelHandle, PANEL_TABLE_MSG, ATTR_WIDTH,  giGraphAreaWidth);

	SetTableColumnAttribute (panelHandle, PANEL_TABLE_MSG, 3, ATTR_COLUMN_WIDTH, giGraphAreaWidth -200 -100- 30);

	//SetCtrlAttribute (panelHandle, PANEL_DECORATION_2, ATTR_HEIGHT, giScreenHight - 665);    //665�Ǹ�����������пؼ���λ�ü������
	//--------------------------------------------------------
	SetAxisScalingMode (pH_CMD, PANEL_10_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL, gYAxisMin,gYAxisMax);
	SetAxisScalingMode (pH_DeviceIP, PANEL_11_GRAPH1, VAL_LEFT_YAXIS, VAL_MANUAL, gYAxisMin, gYAxisMax);  
	SetAxisScalingMode (pH_DeviceIP, PANEL_11_GRAPH2, VAL_LEFT_YAXIS, VAL_MANUAL, gYAxisMin, gYAxisMax);  	

	//UnAuthrity();
	LoadPassword();
	LoadChannelCfg();
	InitialDefaultCtrl();
	//��������
	CloseCom(gComRLY);
	CloseCom(gComHI); //�����������
	CloseCom(gComHV);
	CloseCom(gComInsulation);
	
	 i= OpenComConfig (gComRLY, "", 9600, 0, 8, 1, 512, 512);
	 SetComTime (gComRLY, 2.0);
				
	 if(i<0)
		 InsertTableMsg("������ʾ","�̵�������ģ�鴮���޷��򿪣�����д��ȷ���ں�!");
	 
	 i= OpenComConfig (gComHI, "", 9600, 0, 8, 1, 512, 512); //�����������	
	 SetComTime (gComHI, 1.0);    
	 if(i<0)
		 InsertTableMsg("������ʾ","����������������޷��򿪣�����д��ȷ���ں�!");
	 
	 i= OpenComConfig (gComHV, "", 9600, 0, 8, 1, 512, 512);
	 SetComTime (gComHV, 1.0);    
	 if(i<0)
		 InsertTableMsg("������ʾ","��ѹ�����������޷��򿪣�����д��ȷ���ں�!");
	 
	 i= OpenComConfig (gComInsulation, "", 9600, 0, 8, 1, 4096, 4096);
	 SetComTime (gComInsulation, 1.0);   	 
	 if(i<0)
		 InsertTableMsg("������ʾ","��Ե�����������޷��򿪣�����д��ȷ���ں�!");
	 
	  //����ֹͣ�����ֹһֱ�����ݴ�����
	 sprintf(gBufTxd, "%s", "[E]>MA0#"); 
	 SendNetCMD(_NET_DAQ_STOP,gBufTxd);
	
	//��ʼ��LED��
	SetCtrlAttribute (panelHandle, PANEL_LED, ATTR_OFF_COLOR, VAL_GREEN);     	
	RunUserInterface ();  //���øú����������û�����
	DiscardPanel (panelHandle);  
	return 0;
}

//������ã�ȷ��
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

 //����Ա��¼���˳�
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

//�˳�
int CVICALLBACK PANEL_Quit (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SaveConfig();
			SaveChannelCfg();
			//�Ͽ���������
			DisconnectFromTCPServer (pH_NetHandle);
			SavePassword();
			CloseCom (gComRLY);
			QuitUserInterface(0); //�˳��û����� 
			break;
	}
	return 0;
}

//���м�¼���ã�ȷ��
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

//����������壬����
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

//�ȶϿ�ԭ������������
			gNetConnectStatus = 0;
			DisconnectFromTCPServer (pH_NetHandle);

//���´��Զ�������ʱ��
			SetCtrlAttribute (panelHandle, PANEL_TIMER_AutoConnect, ATTR_ENABLED, 1);

			iNetStatus = ConnectToTCPServerEx (&pH_NetHandle, gNetPort, gNetIP, ClientTCPCB, NULL, 6000, TCP_ANY_LOCAL_PORT);
			SetWaitCursor(0);
			if(iNetStatus <0)
				SetCtrlVal(pH_NetConnect, PANEL_2_STR_NetStatus, "�豸����ʧ�ܣ�");
			else
			{
				SetCtrlVal(pH_NetConnect, PANEL_2_STR_NetStatus, "�豸���ӳɹ���");
				gNetConnectStatus = 1;
			}
			break;
	}
	return 0;
}

//����ʱ��������ʱ�������״̬,����gNetConnectStatus״̬�����¿ؼ�״̬ ��tick is 2S
int CVICALLBACK PANEL_NetStatus (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	int x,y;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			if(gNetConnectStatus ==1)
			{
				SetCtrlVal(panelHandle, PANEL_TXT_NetStatus, "�豸����������");
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
				SetCtrlVal(panelHandle, PANEL_TXT_NetStatus, "�豸�����жϣ�");
				SetCtrlAttribute (panelHandle, PANEL_PIC_Connect, ATTR_VISIBLE, 0);
				SetCtrlAttribute (panelHandle, PANEL_PIC_ConnectGrey, ATTR_VISIBLE, 0);
				SetCtrlAttribute (panelHandle, PANEL_PIC_Unconnect, ATTR_VISIBLE, 1);
			}

			break;
	}
	return 0;
}


//(ͣ��)����ʱ�������Զ��������磬ÿ��һ���Ӽ������״̬���������Ͽ������Զ�����
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

//�����緢�������0.2S��� ��ʱ��PANEL_Timer_CMD_Process��������յ�������
void SendNetCMD (int iCMDCode, char*str)
{
	int i;
	int count;
	DisableCtrlCMD();
	gNetCMDCode = iCMDCode;

	gNetCMDRev = _DATA_CMD_IDLE;
	ResetTimer(panelHandle, PANEL_TIMER_CMD_Process);
	SetCtrlAttribute (panelHandle, PANEL_TIMER_CMD_Process, ATTR_ENABLED, 1); //ʹ�ܴ�������timer
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
		MessagePopup("����","��������δ����ȷִ�У������ԣ�");
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

//������ʾ
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

//ͼ�β���˵����**
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
	ClientTCPRead (pH_NetHandle, gBufRev, 2*gDataMaxLen, _TCP_TIME_OUT) ;   //��ջ�����
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
			InsertTableMsg("�����ж�","�����жϣ��Զ�������...");
			if(0>ConnectToTCPServerEx (&pH_NetHandle, gNetPort, gNetIP, ClientTCPCB, NULL, 2000, TCP_ANY_LOCAL_PORT))
			{
				InsertTableMsg("����","�����Զ������ɹ�");
				gNetConnectStatus = 1;
			}

			break;
		case  _NET_DAQ_START:
			InsertTableMsg("��Ϣ��ʾ","ͨѶ�쳣�������ԣ�");
			break;
		case   _NET_DAQ_STOP:
			InsertTableMsg("��Ϣ��ʾ","ͨѶ�쳣�������ԣ�");
			break;
		case  _NET_CLEAR_ALARM:
			InsertTableMsg("��Ϣ��ʾ","ͨѶ�쳣�������ԣ�");
			break;
		case   _NET_SET_ALARM:
		//	InsertTableMsg("��Ϣ��ʾ","ͨѶ�쳣�������ԣ�");
			break;
		case   _NET_SINGLE_DAQ:
			InsertTableMsg("��Ϣ��ʾ","ͨѶ�쳣�������ԣ�");
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
	sprintf(strText, "ͨ��%d����:%s", channel ,strTime);

	SetCtrlAttribute (pH_CMD, PANEL_10_GRAPH1, ATTR_LABEL_TEXT, strText);
	SetCtrlAttribute (pH_CMD, PANEL_10_GRAPH1, ATTR_LABEL_LEFT, VAL_AUTO_CENTER);
}

//ʹ���������ݽ���,tick is 0.2S
int CVICALLBACK PANEL_EnableRXD (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			giTCPDisable = 0;   //ʹ�ܽ���
			SetCtrlAttribute (panelHandle, PANEL_TIMER_EnableRXD, ATTR_ENABLED, 0);
			break;
	}
	return 0;
}

//��ʼ�������
int CVICALLBACK PANEL_DAQ_START (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf(gComBufT, "[M]>RL1#");  //������̵���
			i = SendComCMD(gComRLY,  strlen(gComBufT), gComBufT);
			if(-2 ==i)
				return -1;
			Delay(1);   /*
			SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_TEXT, "��ʼ������"); 
			Delay(1);
			SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_TEXT, "��ʼ������");
			Delay(1);
			SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_TEXT, "��ʼ������"); 
			Delay(1);  
				*/
			giEnableDataRev = 1;    //��������ˢ��
			SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_TEXT, "��ʼ����");
			SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_COLOR, VAL_BLACK);
			sprintf(gBufTxd, "%s", "[E]>MA1#");
			SendNetCMD(_NET_DAQ_START, gBufTxd);
			break;
	}
	return 0;
}

//ֹͣ����
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

//�������
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
		//	DuplicateCtrl (panelHandle, PANEL_TABLE, pH_DeviceIP, "������Խ��", 50, 20); 
			
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
			
			//����ͼ1��������ʾ
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
			sprintf(strText, "�ź�ͨ������:%s", strTime);
			SetCtrlAttribute (pH_DeviceIP, PANEL_11_GRAPH1, ATTR_LABEL_TEXT, strText);
			SetCtrlAttribute (pH_DeviceIP, PANEL_11_GRAPH1, ATTR_LABEL_LEFT, VAL_AUTO_CENTER);
			
			//����ͼ2��������ʾ
			for(i=0; i<60; i++)
			{
				if(giChanResSel[i]==2)
				{
					GetTableCellAttribute (panelHandle, PANEL_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, &iColor);    
					PlotY (pH_DeviceIP, PANEL_11_GRAPH2, gfChanResVal[i], 1800, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, iColor);
				}
			} 
			strTime = TimeStr ();
			sprintf(strText, "����ͨ������:%s", strTime);
			SetCtrlAttribute (pH_DeviceIP, PANEL_11_GRAPH2, ATTR_LABEL_TEXT, strText);
			SetCtrlAttribute (pH_DeviceIP, PANEL_11_GRAPH2, ATTR_LABEL_LEFT, VAL_AUTO_CENTER);	
*/
			
			//����ʱ����ʾ
			GetCtrlVal(panelHandle, PANEL_STRING, strText);
			SetCtrlVal(pH_DeviceIP, PANEL_11_STRING_2, strText);
			
			GetTableCellVal (panelHandle, PANEL_TABLE_5, MakePoint(1,1), strText);
			i = strlen (strText);															  
			SetTableCellValFromIndex (pH_DeviceIP, PANEL_11_TABLE_5, MakePoint(1,1), (i<5)?0:1);  //���úϸ���� 	
//���Ƶ������Կؼ�
			DuplicateCtrl (pH_HITest, PANEL_8_TABLE_4, pH_DeviceIP, "��������Խ��", 350, 20);
			DuplicateCtrl (pH_HVTest, PANEL_7_TABLE_4, pH_DeviceIP, "��ѹ���Խ��", 480, 20);  	
			DuplicateCtrl (pH_InsulationTest, PANEL_13_TABLE_6, pH_DeviceIP, "��Ե�ǲ��Խ��", 660, 20);  				
		//	DuplicateCtrl (pH_InsulationTest, PANEL_13_TABLE_5, pH_DeviceIP, "", 740, 20);  				
			
			break;
	}
	return 0;
}

//������
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
			InsertTableMsg("���ݱ�����ϣ�", strFileName);
			break;
	}
	return 0;
}

//��ӡ�������ӡ����
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

//��ʼ���ؼ�
void InitialDefaultCtrl(void)
{
	int i,iTmp;
	char sTmp[10];
	DeleteTableColumns (panelHandle, PANEL_TABLE,  1, -1);	  //�������ϣ��������table
	DeleteTableColumns (panelHandle, PANEL_TABLE_2, 1, -1); 
	DeleteTableColumns (pH_DeviceIP, PANEL_11_TABLE,  1, -1); //��ӡ����ϵ�table
	DeleteTableColumns (pH_DeviceIP, PANEL_11_TABLE_2, 1, -1);  	
	//��������������
	for(i=0; i<60; i++)
	{
		if((i < 7)&&((giChanResSel[i]==1)||(giChanResSel[i]==3))) // ����ͨ��
		{   //PANEL_TABLE,�������ϣ��������table1 
			InsertTableColumns (panelHandle, PANEL_TABLE, -1, 1, VAL_CELL_NUMERIC);//�ڱ��ĩβ,����1�� 
			GetNumTableColumns (panelHandle, PANEL_TABLE, &iTmp);  //��ȡ����
			sprintf(sTmp, "%d", i+1);
			SetTableColumnAttribute (panelHandle, PANEL_TABLE, iTmp, ATTR_USE_LABEL_TEXT, 1); // 1 = Use custom text as label   
			SetTableColumnAttribute (panelHandle, PANEL_TABLE, iTmp, ATTR_LABEL_TEXT, sTmp);
			SetTableColumnAttribute (panelHandle, PANEL_TABLE, iTmp, ATTR_COLUMN_WIDTH, 38);
			
			InsertTableColumns (pH_DeviceIP, PANEL_11_TABLE, -1, 1, VAL_CELL_NUMERIC);//�ڱ��ĩβ,����1�� 
			GetNumTableColumns (pH_DeviceIP, PANEL_11_TABLE, &iTmp);  //��ȡ����
			sprintf(sTmp, "%d", i+1);
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE, iTmp, ATTR_USE_LABEL_TEXT, 1); // 1 = Use custom text as label   
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE, iTmp, ATTR_LABEL_TEXT, sTmp);
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE, iTmp, ATTR_COLUMN_WIDTH, 38);
			
			
			
		}
		else if((giChanResSel[i]==2)||(giChanResSel[i]==3)) // �ź�ͨ��
		{    //PANEL_TABLE_2,�������ϣ��������table2
			InsertTableColumns (panelHandle, PANEL_TABLE_2, -1, 1, VAL_CELL_NUMERIC);	 //�������ϣ��������table2 
			GetNumTableColumns (panelHandle, PANEL_TABLE_2, &iTmp);
			sprintf(sTmp, "%d", i+1);
			SetTableColumnAttribute (panelHandle, PANEL_TABLE_2, iTmp, ATTR_USE_LABEL_TEXT, 1);// 1 = Use custom text as label   			
			SetTableColumnAttribute (panelHandle, PANEL_TABLE_2, iTmp, ATTR_LABEL_TEXT, sTmp);
			SetTableColumnAttribute (panelHandle, PANEL_TABLE_2, iTmp, ATTR_COLUMN_WIDTH, 38);  		
			
			InsertTableColumns (pH_DeviceIP, PANEL_11_TABLE_2, -1, 1, VAL_CELL_NUMERIC);	 //�������ϣ��������table2 
			GetNumTableColumns (pH_DeviceIP, PANEL_11_TABLE_2, &iTmp);
			sprintf(sTmp, "%d", i+1);
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE_2, iTmp, ATTR_USE_LABEL_TEXT, 1);// 1 = Use custom text as label   			
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE_2, iTmp, ATTR_LABEL_TEXT, sTmp);
			SetTableColumnAttribute (pH_DeviceIP, PANEL_11_TABLE_2, iTmp, ATTR_COLUMN_WIDTH, 38);  			
			
		}		
	}
//���ô�ӡ��������	
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
	{   //��ʾ������ֵ
		if( (giChanResSel[i]!=1) && (giChanResSel[i]!=2) )
			SetTableCellAttribute (panelHandle, PANEL_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_LT_GRAY);
		else 			
			SetTableCellAttribute (panelHandle, PANEL_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE);
	}
	for(i=0;i<60;i++)
	{   //�ϸ��ж���ͨ�ϣ�����������
		if( (giChanResSel[i]!=1) && (giChanResSel[i]!=2) )
			SetTableCellRangeAttribute (panelHandle, PANEL_TABLE_4, MakeRect(1,i+1,3,1), ATTR_TEXT_BGCOLOR, 0x00F0F0F0);
		else 
			SetTableCellRangeAttribute (panelHandle, PANEL_TABLE_4, MakeRect(1,i+1,3,1), ATTR_TEXT_BGCOLOR, VAL_WHITE);   
	}
//�����ĸ���ֵ��ʾ	
	SetTableCellAttribute (panelHandle, PANEL_TABLE_8, MakePoint(1,2), ATTR_CTRL_VAL, giChanResSel[60]);//�趨������ֵ���ֵ
	SetTableCellAttribute (panelHandle, PANEL_TABLE_8, MakePoint(1,1), ATTR_CTRL_VAL, giChanResSel[64]);//�趨������ֵ��Сֵ
	SetTableCellAttribute (panelHandle, PANEL_TABLE_9, MakePoint(1,2), ATTR_CTRL_VAL, giChanResSel[61]);//�趨������ֵ���ֵ 
	SetTableCellAttribute (panelHandle, PANEL_TABLE_9, MakePoint(1,1), ATTR_CTRL_VAL, giChanResSel[65]);//�趨������ֵ��Сֵ
	SetTableCellAttribute (pH_DeviceIP, PANEL_TABLE_8, MakePoint(1,2), ATTR_CTRL_VAL, giChanResSel[62]);//�趨������ֵ���ֵ 
	SetTableCellAttribute (pH_DeviceIP, PANEL_TABLE_8, MakePoint(1,1), ATTR_CTRL_VAL, giChanResSel[66]);//�趨������ֵ��Сֵ
	SetTableCellAttribute (pH_DeviceIP, PANEL_TABLE_9, MakePoint(1,2), ATTR_CTRL_VAL, giChanResSel[63]);//�趨������ֵ���ֵ 
	SetTableCellAttribute (pH_DeviceIP, PANEL_TABLE_9, MakePoint(1,1), ATTR_CTRL_VAL, giChanResSel[67]);//�趨������ֵ��Сֵ
//���ô����HI��廬������ʾ
	for(i=0;i<7;i++)  // mmsun
	{   // 7·����������˫��ĳһ�����������û������� 
		if(/* (giChanResSel[i]!=1) && */(giChanResSel[i]==1) )
			SetTableCellAttribute (pH_HITest, PANEL_8_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE);
		else 			
			SetTableCellAttribute (pH_HITest, PANEL_8_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_LT_GRAY);
	}
	for(i=0;i<7;i++)   //mmsun
	{   // 7·�������������Խ�� 
		if( /*(giChanResSel[i]!=1) &&*/ (giChanResSel[i]==1) )
			SetTableCellRangeAttribute (pH_HITest, PANEL_8_TABLE_4, MakeRect(1,i+1,1,1), ATTR_TEXT_BGCOLOR, VAL_WHITE);
		else 
			SetTableCellRangeAttribute (pH_HITest, PANEL_8_TABLE_4, MakeRect(1,i+1,1,1), ATTR_TEXT_BGCOLOR, 0x00F0F0F0);   
	}	
	
//����HV��廬������ʾ
	for(i=0;i<40;i++)
	{   //40·
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
//����Insulation���	
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

void SetAlarmDisplay(void)  //�����ʾ����
{
	double fMax, fMin;
	int i,j;
	int iTmp=0;
	int iAlalrmNum;
	int iMaxIdx, iMinIdx;
	int iAlarmFlag=0;
	char strTmp[256];
	
	SetTableCellValFromIndex (panelHandle, PANEL_TABLE_5, MakePoint(1,1), 0);  //���úϸ����
	for(i=0;i<7;i++)
	{
		if((giChanResSel[i]==1)||(giChanResSel[i]==3))   //�����ź�ͨ��������ֵ������ֵ
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
		if((giChanResSel[i]==2)||(giChanResSel[i]==3))   //���ö���ͨ��������ֵ������ֵ
		{
			iTmp++;
			MaxMin1D (gfChanResVal[i], gChanDataLen, &fMax, &iMaxIdx, &fMin, &iMinIdx);
			SetTableCellAttribute (panelHandle, PANEL_TABLE_2, MakePoint(iTmp,1), ATTR_CTRL_VAL, (int)fMax);
			SetTableCellAttribute (panelHandle, PANEL_TABLE_2, MakePoint(iTmp,2), ATTR_CTRL_VAL, (int)fMin);
		}
	}
	
	//�����ܵ����ݽ��б�������
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
				if(gfChanResVal[i][j] >(iTmp==1? giChanResSel[68]:giChanResSel[69]) )//ͨ���ж�
					iAlalrmNum++;
			}
			
			SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,2), ATTR_TEXT_BGCOLOR, VAL_GREEN);  			//�Ȱ�������Ϊ��ɫ
			if(iAlalrmNum==0) 
			{
				SetTableCellAttribute (panelHandle, PANEL_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_GREEN);		//�����ͨ�ϱ����������Լ���ɫ������Ϊ��ɫ
				SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,1), ATTR_TEXT_BGCOLOR, VAL_GREEN);  
				SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,3), ATTR_TEXT_BGCOLOR, VAL_GREEN);  
			}
			if(iAlalrmNum>=1) 
			{
				SetTableCellAttribute (panelHandle, PANEL_TABLE_3, MakePoint(i+1,1), ATTR_CMD_BUTTON_COLOR, VAL_RED);	 //�����ͨ�ϱ�������������Ϊ��ɫ
				iAlarmFlag = 1;
				sprintf(strTmp,"%s%d%s","����ͨ��",i+1,"����");
				InsertTableMsg("������ֵ��ʾ��",strTmp);   
			}
			if((iAlalrmNum>=1) && (iAlalrmNum < (0.01*giBreakCriteria*gChanDataLen)))
			{
				SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,3), ATTR_TEXT_BGCOLOR, VAL_RED);  //����  
			}
			if(iAlalrmNum >= (0.01*giBreakCriteria*gChanDataLen))
				SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,1), ATTR_TEXT_BGCOLOR, VAL_RED);  //ͨ�� 
		}
	}
	
	for(i=0;i<60;i++) //������ֵ�Ƿ���
	{
		MaxMin1D (gfChanResVal[i], gChanDataLen, &fMax, &iMaxIdx, &fMin, &iMinIdx);  		
		if((i<7)&&((giChanResSel[i]==1)||(giChanResSel[i]==3))) //������
		{
			if( (fMax-fMin)>(giChanResSel[60] -giChanResSel[64]))
				SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,2), ATTR_TEXT_BGCOLOR, VAL_RED);  	//��������Ϊ��ɫ				
		}
		else if((giChanResSel[i]==2)||(giChanResSel[i]==3))//�źŻ�
		{
			if( (fMax-fMin)>(giChanResSel[61] -giChanResSel[65]))  
				SetTableCellAttribute (panelHandle, PANEL_TABLE_4, MakePoint(i+1,2), ATTR_TEXT_BGCOLOR, VAL_RED);  	//				
		}			
   
	}
	
	if(iAlarmFlag ==1)
	{
		SetTableCellValFromIndex (panelHandle, PANEL_TABLE_5, MakePoint(1,1), 1);  //���úϸ����     
		SetCtrlAttribute (panelHandle, PANEL_LED_1, ATTR_OFF_COLOR, VAL_RED);   		
		iAlarmFlag = 0;
		giEnableDataRev = 0;    //��ֹ����ˢ��
		SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_TEXT, "�Ѿ���ͣ");
		SetCtrlAttribute (panelHandle, PANEL_cButtonStart, ATTR_LABEL_COLOR, VAL_RED);
		if(giEnableAlarm)
		{
			sprintf(gBufTxd, "%s", "[E]>AL1#");  				
			SendNetCMD(_NET_SET_ALARM, gBufTxd); 
		}
	}
}

//��������
int CVICALLBACK PANEL_DEL_ALARM (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (panelHandle, PANEL_LED_1, ATTR_OFF_COLOR, VAL_TRANSPARENT);
			InsertTableMsg("����˵��","������ʾ�������");
			sprintf(gBufTxd, "%s", "[E]>AL0#");  				
			SendNetCMD(_NET_CLEAR_ALARM, gBufTxd);  
			break;
	}
	return 0;
}

//���β���
int CVICALLBACK PANEL_Single_DAQ (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	int i,length;
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf(gComBufT, "[M]>RL1#");  //������̵���
			length = strlen(gComBufT);
			if(-2==SendComCMD(gComRLY, strlen(gComBufT), gComBufT) )
				return -1;
			Delay(1);
			giEnableDataRev = 1;    //��������ˢ�� 	
			sprintf(gBufTxd, "%s", "[E]>MS1#");  				
			SendNetCMD(_NET_SINGLE_DAQ, gBufTxd);  	
			break;
	}
	return 0;
}

//�л�����һ�������������
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

//�豸�������ã�ȷ��
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
				MessagePopup("��ʾ","���ںŲ�������ͬ�����޸Ĵ��ںţ�");
				return -1;
			}
			gComRLY = i1;
			gComHV = i2;
			gComHI = i3;  //�����������  
			gComInsulation = i4;
			
			
			//��������
			CloseCom(gComRLY);
			CloseCom(gComHI); //�����������  	
			CloseCom(gComHV);
			CloseCom(gComInsulation);
	
			i= OpenComConfig (gComRLY, "", 9600, 0, 8, 1, 512, 512);
			if(i<0)
				 InsertTableMsg("������ʾ","�̵�������ģ�鴮���޷��򿪣�����д��ȷ���ں�!");
		
			i= OpenComConfig (gComHI, "", 9600, 0, 8, 1, 512, 512);//�����������  
			if(i<0)
				 InsertTableMsg("������ʾ","����������������޷��򿪣�����д��ȷ���ں�!");

			i= OpenComConfig (gComHV, "", 9600, 0, 8, 1, 512, 512);
			if(i<0)
				 InsertTableMsg("������ʾ","��ѹ�����������޷��򿪣�����д��ȷ���ں�!");
			SetComTime (gComHV, 1.0);   

			i= OpenComConfig (gComInsulation, "", 9600, 0, 8, 1, 4096, 4096);
			if(i<0)
				 InsertTableMsg("������ʾ","��Ե�����������޷��򿪣�����д��ȷ���ں�!");
			
			break;
	}
	return 0;
}

// �豸�������ã��˳�
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

//��ӡ����������ļ�
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
			InsertTableMsg("���ݱ�����ϣ�", strFileName);
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
			sprintf(gComBufT, "[M]>RL1#");  //������̵���
			i = SendComCMD(gComRLY,  strlen(gComBufT), gComBufT);
			if(-2 ==i)
				return -1;
			

			break;
	}
	return 0;
}

#define _GRAPH_SCALE_FACTOR   1.1   //������ͼ��������     
#define _TCP_TIME_OUT         500
#define _DAQ_SAMPLE_RATE      0.5  //��λΪM/��
#define _LIGHT_SPEED          100.0  //��λΪM/us �������ٶ�
#define _TCP_CMD_TIME_OUT_CNT 30  
#define gDataMaxLen  110000		   //����������ݳ��ȣ�1800����*60 = 108000

#define gChanDataLen  1800		   //����ÿ��ͨ����������ݳ���    
#define gChanNum  60		       //����ͨ������
double gfChanResVal[gChanNum][gChanDataLen];

int giChanResSel[72];	 //0~60ȡֵ0����ѡ����1���źţ���2����������3��A����4����B��  60~63��������ֵ���ֵ��64~68��������ֵ��Сֵ��69~71��ͨ�����ֵ


#define MAX_ALARM_REGION 127
#define MAX_DEL_FIBER_REGION 8  

//gNetCMD
#define _NET_CONNECT_TEST     1
#define _NET_DAQ_START        2
#define _NET_DAQ_STOP         3
#define _NET_CLEAR_ALARM      4 
#define _NET_SET_ALARM        5
#define _NET_SINGLE_DAQ       6 

#define _NET_SET_FIBER_CAL_PARA   7   

#define _NET_SET_ALARM_REGION     8
#define _NET_DELETE_ALARM_REGION  9
#define _NET_READ_ALARM_REGION   10
#define _NET_EDIT_ALARM_REGION	 11

#define _DATA_ID_DATA_PACKET      0xAA
#define _DATA_DEVICE_INFO         0xA9  
#define _READ_CAL_PARA_SUCCESS    0xA7 
#define _DATA_ALARM_FIX_VALUE     0xA6
#define _DATA_ALARM_DIFF_VALUE    0xA5   

#define _READ_ALARM_SUCCESS       0xA4  
#define _READ_RELAY_SUCCESS       0xA3
#define _READ_CAL_RATIO_SUCCESS   0xBB
#define _READ_DEL_FIBER_REGION_SUCCESS 0xBD

#define _FIBER_BROKEN 0xBC

#define _DATA_CMD_IDLE        0x00 
#define _DATA_CMD_READY       0x01 
#define _DATA_CMD_FAIL        0x02

#define _DATA_READ_SUCCESS    0xAF
#define _DATA_READ_FAIL       0xA0


//������ҳ���ͷ�ĳ�ʼ�߶�
#define _TreeArrow_TOP        55

int panelHandle;	   //PANEL
int pH_Start;          //PANEL_1
int pH_NetConnect;     //PANEL_2
int pH_DisplaySet;     //PANEL_3
int pH_Login;	      //PANEL_4 
int pH_SuperPassword; //PANEL_5
int pH_ComSet;        //PANEL_6
int pH_HVTest;        //PANEL_7 
int pH_HITest;        //PANEL_8 
int pH_RecordSet;   //PANEL_9 
int pH_CMD;         //PANEL_10 
int pH_DeviceIP;	//PANEL_11
int pH_ShortKeyMSG; //PANEL_12
int pH_InsulationTest; //PANEL_13  
int pH_ChanSel; //PANEL_16
int pH_SMSModerm;      //PANEL_17
int pHMenu;         //handle of Menu
int ctrlArrayID;    

int gComRLY, gComHV, gComHI, gComInsulation;

int giHVThreshold, giHVTestTime;
int giInsulationThreshold, giInsulationTestTime;
								
unsigned char gBufRev[2*gDataMaxLen];
unsigned char gDataBuf[2*gDataMaxLen];		//������������Ļ�����
char gBufTxd[2048];
char gComBufT[2048];   
char gComBufR[4096];        

double gfDisplayData[gDataMaxLen];		   //���浱ǰ���ݵ�����
double gfDataSaveBuf[gDataMaxLen];

//�����������
double gfAlarmValue[2048];  
double gfAlarmPos[2048]; 

//��ͼ��صı���
int gChannelCurrent;			   //��¼�ǵ�ǰ���Ǹ�ͨ��������λ����ȡ������
int gCurrentChannelPoints;
//���ڳ����С
#define _GRAPH_AREA_TOP   400
#define _GRAPH_AREA_LEFT  600
#define _GRAPH_AREA_BOTTOM  600

#define _MSG_AREA_HIGHT     250   //��Ϣ���ڵĳߴ�̶�Ϊ150

int giScreenHight, giScreenWidth;    //��Ļ�ֱ���
int giGraphAreaHight, giGraphAreaWidth;

//������У׼ϵ����ȫ�ֱ���
int giFiberCalRatioActiveChannel;      	//��ǰѡ�õĹ���У׼ͨ��
int giFiberCalRatioCurrentItem;

//�����쳣����ɾ����ȫ�ֱ���
int giDelFiberRegionActiveChannel;	   //��ǰѡ�õĹ����޳�ͨ��   
int giDelFiberRegionItemNum;
int giDelFiberRegionNewNum;
int giDelFiberRegionCurrentRow;

//���屨�������ȫ�ֱ���
int giAlarmRegionCurrentRow;	  		//��������ɾ������Ŀ��ʱ��ʹ��      
int giAlarmRegionItemNum;   			//��¼��ǰһ���ж��ٸ���Ŀ
int giAlarmRegionActiveChannel;      	//��ǰѡ�õı���ͨ��
int giAlarmRegionCurrentItem;			//��ǰ�༭�ı�����Ŀ

//����̵�����ȫ�ֱ���
int giRelayCurrentRow;	  		//��������ɾ������Ŀ��ʱ��ʹ��      
int giRelayItemNum;   			//��¼��ǰһ���ж��ٸ���Ŀ
int giRelayActiveChannel;      	//��ǰѡ�õļ̵������
int giRelayNewNum;               //�½���һ���̵����������
 
//global
char *gNetIP;
int  gNetPort ;
int  gDisplayAlarmThreshold ; 
int  gDisplayAnnotation;
int  gDisplayTemperaturePeak ;   
int  gDisplayAlarmPoint ;
int  gYAxisMin;
int  gYAxisMax;
int giEnableAlarm;
int giBreakCriteria;
int giSingleMeasureMode;
//�ļ�������� 
int  gSaveData          ; 
char gsFileSaveDir[260] ;
int gFileSavePeriod     ;
int  gSaveAlarm         ;
int  gSaveDeviceInfo    ;

int pH_NetHandle;
int gNetConnectStatus;
int gNetCMDCode;            //���͵���������
int gNetCMDRev;				//���յ�����������
int gTimerCNT;   //ÿ�����涼��һ�������ʱ����ÿ0.2�봦��һ�Σ���ദ��10��   

int gKeyOriginal;
char gPassword[260];

//����è����
int  gEnableSMS;
char gSMSSendNum[12];
char gSMSRevNum[12];
			
char gsRegionName[16][MAX_ALARM_REGION][33];

//user defined function
void InsertTableMsg(char *str1, char *str2);
void InsertGraphLabel(int channel);  
void InsertCalPara(void) ;

void InsertAlarmRegionItem(void);
void InsertChannelAlarmRegion(void);
void ReplaceAlarmRegionItem(void);

void InsertFiberCalRatioItem(void); 
void ReplaceFiberCalRatioItem(void); 
void InsertChannelFiberCalRatio(void);  

void InsertDelFiberRegion(void);
void InsertDelFiberRegionItem(void);

void InsertSingleRelayItem(void);
void InsertRelayAllItem(void);
void ReviewRelay(int);

void TCPReadReset(void);

void SendNetCMD (int , char*);
void EnableCtrlCMD(void);
void DisableCtrlCMD(void);

void LoadPassword(void);
void SavePassword(void);
void ProcessTimeOutErr(int);

int SMS_Send(char *); 

int CVICALLBACK ClientTCPCB (unsigned handle, int event, int error, void *callbackData);

void LoadChannelCfg(void);
void SaveChannelCfg(void);

void InitialDefaultCtrl(void);
void SetAlarmDisplay(void);

int SendComCMD(int, int, char*);

//������������ߣ�˫ĸͷ
//��ѹ�������ߣ�˫ĸͷ
//�̵�����ֱ���ߣ���ĸͷ
//��Ե�ǣ�ֱ���ߣ���ĸͷ              

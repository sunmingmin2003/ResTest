#define _GRAPH_SCALE_FACTOR   1.1   //定义视图缩放因子     
#define _TCP_TIME_OUT         500
#define _DAQ_SAMPLE_RATE      0.5  //单位为M/点
#define _LIGHT_SPEED          100.0  //单位为M/us ，反射速度
#define _TCP_CMD_TIME_OUT_CNT 30  
#define gDataMaxLen  110000		   //定义最大数据长度，1800个点*60 = 108000

#define gChanDataLen  1800		   //定义每个通道传输的数据长度    
#define gChanNum  60		       //定义通道长度
double gfChanResVal[gChanNum][gChanDataLen];

int giChanResSel[72];	 //0~60取值0（不选），1（信号），2（动力），3（A），4（）B，  60~63：噪声阈值最大值，64~68：噪声阈值最小值，69~71：通断最大值


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


//定义主页面箭头的初始高度
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
unsigned char gDataBuf[2*gDataMaxLen];		//用来处理命令的缓冲区
char gBufTxd[2048];
char gComBufT[2048];   
char gComBufR[4096];        

double gfDisplayData[gDataMaxLen];		   //保存当前数据的数组
double gfDataSaveBuf[gDataMaxLen];

//报警数据相关
double gfAlarmValue[2048];  
double gfAlarmPos[2048]; 

//绘图相关的变量
int gChannelCurrent;			   //记录是当前是那个通道，从下位机读取上来的
int gCurrentChannelPoints;
//窗口程序大小
#define _GRAPH_AREA_TOP   400
#define _GRAPH_AREA_LEFT  600
#define _GRAPH_AREA_BOTTOM  600

#define _MSG_AREA_HIGHT     250   //消息窗口的尺寸固定为150

int giScreenHight, giScreenWidth;    //屏幕分辨率
int giGraphAreaHight, giGraphAreaWidth;

//定光纤校准系数的全局变量
int giFiberCalRatioActiveChannel;      	//当前选用的光纤校准通道
int giFiberCalRatioCurrentItem;

//定义异常区域删除的全局变量
int giDelFiberRegionActiveChannel;	   //当前选用的光纤剔除通道   
int giDelFiberRegionItemNum;
int giDelFiberRegionNewNum;
int giDelFiberRegionCurrentRow;

//定义报警区域的全局变量
int giAlarmRegionCurrentRow;	  		//仅仅留作删除该条目的时候使用      
int giAlarmRegionItemNum;   			//记录当前一共有多少个条目
int giAlarmRegionActiveChannel;      	//当前选用的报警通道
int giAlarmRegionCurrentItem;			//当前编辑的报警条目

//定义继电器的全局变量
int giRelayCurrentRow;	  		//仅仅留作删除该条目的时候使用      
int giRelayItemNum;   			//记录当前一共有多少个条目
int giRelayActiveChannel;      	//当前选用的继电器序号
int giRelayNewNum;               //新建的一个继电器的区域号
 
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
//文件保存变量 
int  gSaveData          ; 
char gsFileSaveDir[260] ;
int gFileSavePeriod     ;
int  gSaveAlarm         ;
int  gSaveDeviceInfo    ;

int pH_NetHandle;
int gNetConnectStatus;
int gNetCMDCode;            //发送的命令码字
int gNetCMDRev;				//接收到的命令码字
int gTimerCNT;   //每个界面都有一个命令处理定时器，每0.2秒处理一次，最多处理10次   

int gKeyOriginal;
char gPassword[260];

//短信猫设置
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

//大电流：交叉线，双母头
//高压：交叉线，双母头
//继电器：直连线，公母头
//绝缘仪：直连线，公母头              

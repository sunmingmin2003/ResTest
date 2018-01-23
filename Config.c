#include "toolbox.h"
#include <formatio.h>
#include <ansi_c.h>
#include <userint.h>
#include "FiberSensor.h"
#include "variable.h"
#include "Config.h"
int  gConfigFile; 

void LoadConfig()
{
	int iFileHandle;
	
	strBuf = (char *)malloc(100);    
	gConfigFile = OpenFile ("default.ini", VAL_READ_WRITE, VAL_OPEN_AS_IS, VAL_ASCII); 
	ReadFile (gConfigFile, strRead, 1000);
	 
	gNetIP = (char *)malloc(17); 
	AnalyseValue("[gNetIP]=")  ;
	CopyString (gNetIP, 0, strBuf, 0, -1);
	if(-1 == atoi(strBuf) )
		gNetIP = "192.168.100.123";
	
	AnalyseValue("[gNetPort]=")  ;
	gNetPort = atoi(strBuf);
	if( (gNetPort <0) || (gNetPort>65535) )
		gNetPort = 5000;
//---------------------------------------------	
	AnalyseValue("[gDisplayAlarmThreshold]=")  ;
	gDisplayAlarmThreshold = atoi(strBuf);
	if( (gDisplayAlarmThreshold !=0) && (gDisplayAlarmThreshold !=1) )
		gDisplayAlarmThreshold = 1;

	AnalyseValue("[gDisplayTemperaturePeak]=")  ;
	gDisplayTemperaturePeak = atoi(strBuf);
	if( (gDisplayTemperaturePeak !=0) && (gDisplayTemperaturePeak !=1) )
		gDisplayTemperaturePeak = 1;
	
	AnalyseValue("[gDisplayAlarmPoint]=")  ;
	gDisplayAlarmPoint = atoi(strBuf);
 	if( (gDisplayAlarmPoint !=0) && (gDisplayAlarmPoint !=1) )
		gDisplayAlarmPoint = 1;

	AnalyseValue("[gDisplayAnnotation]=")  ;
	gDisplayAnnotation = atoi(strBuf);
 	if( (gDisplayAnnotation !=0) && (gDisplayAnnotation !=1) )
		gDisplayAnnotation = 1;
	
	AnalyseValue("[giEnableAlarm]=")  ;
	giEnableAlarm = atoi(strBuf);
 	if( (giEnableAlarm !=0) && (giEnableAlarm !=1) )
		giEnableAlarm = 1;	
	
	AnalyseValue("[giSingleMeasureMode]=")  ;
	giSingleMeasureMode = atoi(strBuf);
 	if( (giSingleMeasureMode !=0) && (giSingleMeasureMode !=1) )
		giSingleMeasureMode = 0;		
	
	AnalyseValue("[giBreakCriteria]=")  ;
	giBreakCriteria = atoi(strBuf);
 	if( (giBreakCriteria <=0) || (giBreakCriteria >100) )
		giBreakCriteria = 50;		
	
	AnalyseValue("[gYAxisMin]=")  ;
	gYAxisMin = atoi(strBuf);
 	if( (gYAxisMin <-100000) || (gYAxisMin >1000000) )
		gYAxisMin = 0;	
	
	AnalyseValue("[gYAxisMax]=")  ;
	gYAxisMax = atoi(strBuf);
 	if( (gYAxisMax <-100000) || (gYAxisMax >1000000) )
		gYAxisMax = 1000;	
	if( gYAxisMax<=gYAxisMin )
	{
		gYAxisMin = 0;
		gYAxisMax = 1000;	  		
	}
//---------------------------------------------
	AnalyseValue("[gsFileSaveDir]=")  ;
	CopyString (gsFileSaveDir, 0, strBuf, 0, -1);
	if(!FileExists (strBuf, 0))
		sprintf(gsFileSaveDir, "%s" ,"D:\\");		

	AnalyseValue("[gFileSavePeriod]=")  ;
	gFileSavePeriod = atoi(strBuf);
	if( (gFileSavePeriod <0) || (gFileSavePeriod>65535) )
		gFileSavePeriod = 1;
		
	AnalyseValue("[gSaveData]=")  ;
	gSaveData = atoi(strBuf);
	if( (gSaveData !=0) && (gSaveData !=1) )
		gSaveData = 1;
	
	AnalyseValue("[gSaveAlarm]=")  ;
	gSaveAlarm = atoi(strBuf);
	if( (gSaveAlarm !=0) && (gSaveAlarm !=1) )
		gSaveAlarm = 1;
	
	AnalyseValue("[gSaveDeviceInfo]=")  ;
	gSaveDeviceInfo = atoi(strBuf);
	if( (gSaveDeviceInfo !=0) && (gSaveDeviceInfo !=1) )
		gSaveDeviceInfo = 0;	
	

	AnalyseValue("[gEnableSMS]=")  ;
	gEnableSMS = atoi(strBuf);
	if( (gEnableSMS <0) || (gEnableSMS >1) )
		gEnableSMS = 0;	
	
	AnalyseValue("[gSMSSendNum]=")  ;
	CopyString (gSMSSendNum, 0, strBuf, 0, -1); 
	if(11 != StringLength (gSMSSendNum))
		sprintf(gSMSSendNum,"%s","13800574500");
	
	AnalyseValue("[gSMSRevNum]=")  ;
	CopyString (gSMSRevNum, 0, strBuf, 0, -1);  
	if(11 != StringLength (gSMSRevNum))    	
		sprintf(gSMSRevNum,"%s","13800574500");   
	
//-------------------------------------------     	
	AnalyseValue("[gComRLY]=")  ;
	gComRLY = atoi(strBuf);
	if( (gComRLY <1) || (gComRLY>128) )
		gComRLY = 1;
	
	AnalyseValue("[gComHV]=")  ;
	gComHV = atoi(strBuf);
	if( (gComHV <1) || (gComHV>128) )
		gComHV = 2;
										   
	AnalyseValue("[gComHI]=")  ; //大电流发生器		
	gComHI = atoi(strBuf);    
	if( (gComHI <1) || (gComHI>128) )
		gComHI = 3;	
	
	AnalyseValue("[gComInsulation]=")  ;
	gComInsulation = atoi(strBuf);    
	if( (gComInsulation <1) || (gComInsulation>128) )
		gComInsulation = 4;		
							 
//程序初始化时把目录写到文件中，以方便数据查询	
	iFileHandle = OpenFile ("path.cfg", VAL_WRITE_ONLY, VAL_TRUNCATE, VAL_BINARY);
	if(iFileHandle<0)
		return;
	WriteLine (iFileHandle, gsFileSaveDir, -1);  
	CloseFile(iFileHandle);
	
}

void SaveConfig()
{
	char *strWrite;
	strWrite = (char *)malloc(100);
	SetFilePtr (gConfigFile, 0, 0);
	
	sprintf(strWrite,"[gNetIP]={%s};",gNetIP);   
	WriteLine (gConfigFile, strWrite, -1);

	sprintf(strWrite,"[gNetPort]={%d};",gNetPort);
	WriteLine (gConfigFile, strWrite, -1);   
	
	sprintf(strWrite,"[gDisplayAlarmThreshold]={%d};",gDisplayAlarmThreshold);
	WriteLine (gConfigFile, strWrite, -1);  
	
	sprintf(strWrite,"[gDisplayAnnotation]={%d};",gDisplayAnnotation);
	WriteLine (gConfigFile, strWrite, -1);  	
	
	sprintf(strWrite,"[gDisplayTemperaturePeak]={%d};",gDisplayTemperaturePeak);
	WriteLine (gConfigFile, strWrite, -1);   
	
	sprintf(strWrite,"[gDisplayAlarmPoint]={%d};",gDisplayAlarmPoint);
	WriteLine (gConfigFile, strWrite, -1);   
	
	sprintf(strWrite,"[gYAxisMin]={%d};", gYAxisMin);
	WriteLine (gConfigFile, strWrite, -1);  
	
	sprintf(strWrite,"[gYAxisMax]={%d};", gYAxisMax);
	WriteLine (gConfigFile, strWrite, -1);  	
//-------------------------------------------
	sprintf(strWrite,"[gSaveData]={%d};",gSaveData);
	WriteLine (gConfigFile, strWrite, -1);   
	
	sprintf(strWrite,"[gsFileSaveDir]={%s};",gsFileSaveDir);   
	WriteLine (gConfigFile, strWrite, -1);	

	sprintf(strWrite,"[gFileSavePeriod]={%d};",gFileSavePeriod);
	WriteLine (gConfigFile, strWrite, -1);   

	sprintf(strWrite,"[gSaveAlarm]={%d};",gSaveAlarm);
	WriteLine (gConfigFile, strWrite, -1); 
	
	sprintf(strWrite,"[gSaveDeviceInfo]={%d};",gSaveDeviceInfo);
	WriteLine (gConfigFile, strWrite, -1); 
//-------------------------------------------
	
	sprintf(strWrite,"[gComRLY]={%d};",gComRLY);
	WriteLine (gConfigFile, strWrite, -1); 
	
	sprintf(strWrite,"[gComHV]={%d};",gComHV);
	WriteLine (gConfigFile, strWrite, -1); 
	
	sprintf(strWrite,"[gComHI]={%d};",gComHI); //大电流发生器	
	WriteLine (gConfigFile, strWrite, -1);
	
	sprintf(strWrite,"[gComInsulation]={%d};",gComInsulation);
	WriteLine (gConfigFile, strWrite, -1); 	
//-------------------------------------------		
	sprintf(strWrite,"[gEnableSMS]={%d};",gEnableSMS);
	WriteLine (gConfigFile, strWrite, -1); 	
	
	sprintf(strWrite,"[gSMSSendNum]={%s};",gSMSSendNum);
	WriteLine (gConfigFile, strWrite, -1); 

	sprintf(strWrite,"[gSMSRevNum]={%s};",gSMSRevNum);
	WriteLine (gConfigFile, strWrite, -1); 
	
	sprintf(strWrite,"[giEnableAlarm]={%d};",giEnableAlarm);
	WriteLine (gConfigFile, strWrite, -1); 
	
	sprintf(strWrite,"[giSingleMeasureMode]={%d};",giSingleMeasureMode);
	WriteLine (gConfigFile, strWrite, -1); 	
	
	sprintf(strWrite,"[giBreakCriteria]={%d};",giBreakCriteria);
	WriteLine (gConfigFile, strWrite, -1); 	
	
	strWrite = "------------请勿任意修改本文内容---------------------" ;
	WriteLine (gConfigFile, strWrite, -1);  	
	
	CloseFile (gConfigFile);
}

void AnalyseValue(char *Text)
{
	int i,j,k;
	
	i = FindPattern (strRead, 0, -1, Text, 0, 0);
	j = FindPattern (strRead, i, -1, "{", 0, 0);
	k = FindPattern (strRead, i, -1, "}", 0, 0);
	if((0>i) || (0>j) || (0>k) || (j>=k) )
	{
		strBuf = "-1";
		return;
	}
	strBuf = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
	CopyString (strBuf, 0, strRead, j+1, k-j-1);
	if(	0>= StringLength (strBuf) )
	{
		strBuf = "-1"; 
		return;
	}
}


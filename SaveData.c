#include "toolbox.h"
#include <ansi_c.h>
#include <formatio.h>
#include <SaveData.h>
#include "variable.h"

static int iFileHandle = -1;
static char strCurrentFileName[20];

int SaveData(double *strDataBuf, int Len, int iDataType)
{
	char strTmp[260];
	char strFileName[260];
	char strYMDName[10];
	char strHMSName[10];
	
	int i;
	char sDriverName[5];
	
	GetYMDNameStr(strYMDName);
	GetHMSNameStr(strHMSName); 
	
	sprintf(strFileName, "%s\\%s\\%s.dat", "D:\\Host1", strYMDName, strHMSName);
	i = ArrayToFile (strFileName, strDataBuf, VAL_DOUBLE, Len, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 1, VAL_BINARY, VAL_TRUNCATE);
	if(-1 == i)  //文件打开失败
	{
		SplitPath (gsFileSaveDir, sDriverName, strTmp, NULL);
		i = ((int)sDriverName[0]>=97) ? ((int)sDriverName[0]-97) : ((int)sDriverName[0]-65);
		if( 0>SetDrive(i) )
			return -1;		
		if( 0>SetDir(strTmp) )
			return -1;
		if( 0>SetDir("Host1") )
		{
			if(0>MakeDir("Host1"))
				return -1;
			if(0>SetDir("Host1"))
				return -1;			
		}
		
		if( 0>SetDir(strYMDName) )
		{
			if(0>MakeDir(strYMDName))
				return -1;
			if(0>SetDir(strYMDName))
				return -1;
		}		
			 
	  i = ArrayToFile (strFileName, strDataBuf, VAL_DOUBLE, Len, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 1, VAL_BINARY, VAL_TRUNCATE);
	
	  if(0>i)
		  return -1;
	}
	return 0;  	  
}
void SaveDeviceInfo(char *strDataBuf, int iLen)
{
	int i;
	char strFisrRow[260];
	char strTmp[20]; 
	char strProjectDir[260];
	char strFileFullName[260]; 	
//系统打开时重新打开文件	
	if(iFileHandle<0)
	{
		GetYMDNameStr(strCurrentFileName);  
		GetProjectDir (strProjectDir); 			
		sprintf(strFileFullName, "%s\\DeviceInfo\\%s.csv", strProjectDir, strCurrentFileName); 
		i = FileExists (strFileFullName, 0);
		iFileHandle = OpenFile (strFileFullName, VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
		if(iFileHandle<0)
			return;
		if(i!=1)  //如果是首次创建文件
		{
			sprintf(strFisrRow,"时间,参考温度,EDFA电流,EDFA温度,EDFA功率,'-2V5,3V3,1V2,1V8,sys_mon,APD1偏压,APD2偏压,APD温度,APD1设置,APD2设置");  			
			WriteLine(iFileHandle, strFisrRow, strlen(strFisrRow)); 			
		}
	}
	
	GetYMDNameStr(strTmp);
	if( FindPattern (strTmp, 0, -1, strCurrentFileName, 0, 0) <0)
	{
		CloseFile(iFileHandle);
		GetYMDNameStr (strCurrentFileName);  
		GetProjectDir (strProjectDir); 			
		sprintf(strFileFullName, "%s\\DeviceInfo\\%s.csv", strProjectDir, strCurrentFileName); 
		iFileHandle = OpenFile (strFileFullName, VAL_WRITE_ONLY, VAL_APPEND, VAL_ASCII);
		if(iFileHandle<0)
			return;
		sprintf(strFisrRow,"时间,参考温度,EDFA电流,EDFA温度,EDFA功率,'-2V5,3V3,1V2,1V8,sys_mon,APD1偏压,APD2偏压,APD温度,APD1设置,APD2设置");
		WriteLine(iFileHandle, strFisrRow, strlen(strFisrRow)); 
	}
	else
	{
//根据格式重新写文件
		WriteLine(iFileHandle, strDataBuf, iLen);
	}
	return;
}

void GetYMDNameStr(char *str)
{
	char * strTmp;
	strTmp = malloc(16);
	strTmp = DateStr ();
	CopyString (str, 0, strTmp, 6, 4);   //year
	CopyString (str, 4, strTmp, 0, 2);   //month
	CopyString (str, 6, strTmp, 3, 2);   //day
}

void GetHMSNameStr(char *str)
{
	char * strTmp;
	strTmp = malloc(16);
	strTmp = TimeStr ();
	CopyString (str, 0, strTmp, 0, 2);   //year
	CopyString (str, 2, strTmp, 3, 2);   //month
	CopyString (str, 4, strTmp, 6, 2);   //day
}
void GetMonitorStr(char *str)
{
	sprintf(str,"%s,",TimeStr());	                                      //时间10:05:08
	sprintf(str,"%s%2f," , str, 0.01*(gDataBuf[13]+(gDataBuf[14]<<8)) -20.0);  //参考温度，25.00℃
	sprintf(str,"%s%d,"  , str, gDataBuf[15]+(gDataBuf[16]<<8)  );        //EDFA电流  200
	sprintf(str,"%s%d," , str, gDataBuf[17]+(gDataBuf[18]<<8)  );         //EDFA温度，25.00℃   
	sprintf(str,"%s%d,"  , str, gDataBuf[19]+(gDataBuf[20]<<8)  );        //EDFA光功率  50mV
	sprintf(str,"%s%d,"  , str, gDataBuf[25]+(gDataBuf[26]<<8)  );        //-2V5电压  50mV  	
	sprintf(str,"%s%d,"  , str, gDataBuf[27]+(gDataBuf[28]<<8)  );        //3V3电压  50mV 		
	sprintf(str,"%s%d,"  , str, gDataBuf[29]+(gDataBuf[30]<<8)  );        //1V2电压  50mV 
	sprintf(str,"%s%d,"  , str, gDataBuf[31]+(gDataBuf[32]<<8)  );        //1V8电压  50mV  		
	sprintf(str,"%s%d,"  , str, gDataBuf[33]+(gDataBuf[34]<<8)  );        //光敏电压 50mV 
	sprintf(str,"%s%d,"  , str, gDataBuf[21]+(gDataBuf[22]<<8)  );        //APD1高压  50mV 	
	sprintf(str,"%s%d,"  , str, gDataBuf[35]+(gDataBuf[36]<<8)  );        //APD2高压  50mV  
	sprintf(str,"%s%2f," , str, 0.01*(gDataBuf[23]+(gDataBuf[24]<<8)) -20.0);  //APD温度，25.00℃  		
	sprintf(str,"%s%d,"  , str, gDataBuf[39]+(gDataBuf[40]<<8)  );        //APD1设置的偏压，即DAC输出的数字量 
	sprintf(str,"%s%d,"  , str, gDataBuf[41]+(gDataBuf[42]<<8)  );        //APD2设置的偏压 
	//注：本来电路中还有2V5D的监控
}

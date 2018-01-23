#define _DATA_TEMPERATURE 0
#define _DATA_STOKES      1
#define _DATA_ANSTOKES    2

//char *strSaveBuf;    

//数据保存相关函数
int  SaveData(double *, int, int);
void SaveDeviceInfo(char *, int);
void GetYMDNameStr(char *);
void GetHMSNameStr(char *);
void GetMonitorStr(char *);   


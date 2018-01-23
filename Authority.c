#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "FiberSensor.h"
#include "variable.h"

int CVICALLBACK OnSuperAdminLogin (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	int i;
	char str[260];
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panel, PANEL_5_PASSWORD, str);
			if( 0 !=CompareStrings (str, 0, "gxcwsb", 0, 1)
					|| (-1 == FindPattern (str, 0, -1, "gxcwsb", 0, 0)) )
			{
				MessagePopup("提示","密码不正确！");
				return 0;
			}
			
			HidePanel(pH_SuperPassword);   			
			InstallPopup (pH_CMD);
			break;
	}
	return 0;
}

int CVICALLBACK OnCancel (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_SuperPassword);
			break;
	}
	return 0;
}

void LoadPassword()
{
	int fileHandle;
	int i,iTmp;
	char str[260];
	
	GetProjectDir (str); 			
	sprintf(str, "%s\\Test.vof", str); 
	fileHandle = OpenFile (str, VAL_READ_ONLY, VAL_OPEN_AS_IS, VAL_BINARY);
	
	iTmp = ReadLine (fileHandle, str, 259);
	if(iTmp<3)
	{
		sprintf(gPassword,"%s", "superadmin123");
		return;
	}
	for(i=0; i<iTmp;i++)
		gPassword[i] = str[i]^0xFF;
	CloseFile(fileHandle);
}
void SavePassword()
{
	int fileHandle;
	int i,iTmp;
	char str[260];
	iTmp = StringLength(gPassword);
	for(i=0; i<iTmp;i++)
		gPassword[i] = gPassword[i]^0xFF;
	
	GetProjectDir (str); 			
	sprintf(str, "%s\\Test.vof", str); 	
	fileHandle = OpenFile (str, VAL_WRITE_ONLY, VAL_TRUNCATE, VAL_BINARY);
	WriteLine (fileHandle, gPassword, iTmp); 
	CloseFile(fileHandle); 
}

int CVICALLBACK OnModifyPassword (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	char str1[260],str2[260];
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(pH_Login, PANEL_4_PASSWORD_2, str1);
			GetCtrlVal(pH_Login, PANEL_4_PASSWORD_3, str2);
			if(StringLength(str1)<3)
			{
				MessagePopup("提示","密码至少3位！");
				return 0;				
			}
			
			if( (0 !=CompareStrings (str1, 0, str2, 0, 1)) 
				||( -1 == FindPattern (str1, 0, -1, str2, 0, 0)))
			{
				MessagePopup("提示","两次输入密码不一致！");
				return 0;
			}
			else 
				MessagePopup("提示","密码修改成功！");  
				sprintf(gPassword, "%s", str1);
			break;
	}
	return 0;
}


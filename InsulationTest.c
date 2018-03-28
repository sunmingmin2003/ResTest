/*
  绝缘测试
*/

#include <rs232.h>
#include <formatio.h>
#include <utility.h>
#include <ansi_c.h>
#include <userint.h>
#include "FiberSensor.h"
#include "variable.h"
static int giInsulationThreshold = 10;
static int giCurrentInsulationChan=1;  

float fHiVolt;
float iHiRes, iLiRes;
float  fTestTime;
/**************************************
** 单环绝缘阻抗测试                      **
** step1:open all relay,set mode,set current voltage para
** step2:close resp relay
** step3:start test
** step4:testing for setted time,juduge test wether finished
** step5:open resp relay
** setp6:judement reusult

** flag 0:滑间;1:对地
***************************************/
int irstep=1;
int irflag = 0;
int IRSlipRelationTest(int Channel,int flag);

int CVICALLBACK PANEL_13_PreTest (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_InsulationTest);
			InstallPopup(pH_HVTest);
			break;
	}
	return 0;
}

int CVICALLBACK PANEL_13_Exit (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		   	HidePanel(pH_InsulationTest);     
			break;
	}
	return 0;
}

int CVICALLBACK PANEL_13_NextTest (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			InstallPopup(panelHandle);
			sprintf(gComBufT, "[M]!FW#");
			i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);	
			if(i<0)
				return -1;
			HidePanel(pH_InsulationTest);     
			break;
	}
	return 0;
}
//绝缘阻值，开始自动测试
int CVICALLBACK PANEL_13_Start_Test (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			{
			GetCtrlVal(pH_InsulationTest,PANEL_13_NUMERIC_HV,&fHiVolt);
	        GetCtrlVal(pH_InsulationTest,PANEL_13_NUMERIC_HRES,&iHiRes); 
	        GetCtrlVal(pH_InsulationTest,PANEL_13_NUMERIC_LRES,&iLiRes); 
	        GetCtrlVal(pH_InsulationTest,PANEL_13_NUMERIC_TIME,&fTestTime);
			irflag = 0; 
			
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER, ATTR_ENABLED, 1); // 启动下一滑道测试定时器
			}
			break;
	}
	return 0;
}

int CVICALLBACK PANEL_13_StartTestSec (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		{
			GetCtrlVal(pH_InsulationTest,PANEL_13_NUMERIC_HV,&fHiVolt);
	        GetCtrlVal(pH_InsulationTest,PANEL_13_NUMERIC_HRES,&iHiRes); 
	        GetCtrlVal(pH_InsulationTest,PANEL_13_NUMERIC_LRES,&iLiRes); 
	        GetCtrlVal(pH_InsulationTest,PANEL_13_NUMERIC_TIME,&fTestTime);
	
			irflag = 1; 
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER, ATTR_ENABLED, 1); // 启动下一滑道测试定时器
		}
			
			break;
	}
	return 0;
}

/*
int CVICALLBACK PANEL_13_Start_Test (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}
*/

//停止测试
int CVICALLBACK PANEL_13_Stop_Test (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf(gComBufT, "tzcl8888");
			i = SendComCMD(gComInsulation, strlen(gComBufT), gComBufT); 
			
			giCurrentInsulationChan = -1;
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER, ATTR_ENABLED, 0);  
		//	SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_2, ATTR_ENABLED, 0);
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_3, ATTR_ENABLED, 0); 
			SetCtrlVal(pH_InsulationTest, PANEL_13_NUMERICSLIDE, 0.0);  			
			ResetTimer (pH_InsulationTest, PANEL_13_TIMER);
		//	ResetTimer (pH_InsulationTest, PANEL_13_TIMER_2);  
			ResetTimer (pH_InsulationTest, PANEL_13_TIMER_3);  
			sprintf(gComBufT, "[M]!FW#");
			i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);			
			break;
	}
	return 0;
}

//20S后开始读取绝缘值
int CVICALLBACK PANEL_13_Insulation_ReadBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int i,j,iLen;
	char sTmp[50];
	double fRValue;
	switch (event)
	{
		case EVENT_TIMER_TICK:
//停止测量			
		
			iLen =	GetInQLen (gComInsulation); //绝缘测试仪表的端口号
			if(iLen<100)
			{
				MessagePopup("警告","当前绝缘仪串口未收到数据！");
				return -1;												    
			}
			i = ComRd (gComInsulation, gComBufR, iLen);
			i = FindPattern (gComBufR, 0, iLen, "s=010", 0, 0);
			if(i<20)
				return -1;
			sprintf(sTmp, "");
			memcpy(sTmp, &gComBufR[i-11], 9);
			fRValue = atof(sTmp);
			if(giCurrentInsulationChan<=39)
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_4, MakePoint((int)(0.5*giCurrentInsulationChan)+1,((giCurrentInsulationChan%2)==1)?2:1), ATTR_CTRL_VAL, fRValue);
			else
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_5, MakePoint((int)(0.5*(giCurrentInsulationChan-40))+1,((giCurrentInsulationChan%2)==1)?2:1), ATTR_CTRL_VAL, fRValue);
			sprintf(gComBufT, "tzcl8888");
			i = SendComCMD(gComInsulation, strlen(gComBufT), gComBufT); 
			Delay(3.0);
//下一次测量	
			ResetTimer(pH_InsulationTest, PANEL_13_TIMER); 
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER, ATTR_ENABLED, 1); 	 //2秒后开始下一滑道测试
		//	SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_2, ATTR_ENABLED, 0); //20秒后开始读取绝缘值
			ResetTimer(pH_InsulationTest, PANEL_13_TIMER_3); //显示进度
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_3, ATTR_ENABLED, 0); 
			SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_3, MakePoint((int)(0.5*giCurrentInsulationChan)+1,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE);  
			SetCtrlVal(pH_InsulationTest, PANEL_13_NUMERICSLIDE, 0.0); 
			break;
	}
	return 0;
}

int CVICALLBACK PANEL_13_Res_Test (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{   
	int i;    
	switch (event)
	{
		case EVENT_COMMIT:
			 InstallPopup(panelHandle);
			 sprintf(gComBufT, "[M]!FW#");
			 i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);	
			 if(i<0)
				return -1;
			 HidePanel(pH_InsulationTest);     
			break;
	}
	return 0;
}

 //2秒钟后下一滑道测试
int CVICALLBACK PANEL_13_Insulation_Test1 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int i;
	char sTxt[50];
	switch (event)
	{
		case EVENT_TIMER_TICK:
			{
				if(irflag==0)
				{
				   if(giCurrentInsulationChan<38)
				   {
				        sprintf(sTxt,"正在测试第%d滑道间绝缘电阻",giCurrentInsulationChan);
				        SetCtrlAttribute (pH_InsulationTest, PANEL_13_cButtonPrint_3, ATTR_LABEL_TEXT, sTxt); 
						
				        if( ((giChanResSel[giCurrentInsulationChan-1]==1) || (giChanResSel[giCurrentInsulationChan-1]==2))&&(irstep<=5)  )
				        {
			                IRSlipRelationTest(giCurrentInsulationChan,0);
				        }
				        else
				        {
					        irstep=1;
				            giCurrentInsulationChan++;	 
				        }
				   }
				   else
				   {
				       giCurrentInsulationChan=1;
				       SetCtrlAttribute (pH_InsulationTest, PANEL_13_cButtonPrint_3, ATTR_LABEL_TEXT, "滑道间耐压测试完毕，可重新启动");
			           SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER, ATTR_INTERVAL, 1.0);
			           SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER, ATTR_ENABLED, 0); //disable timer	
					   
				   }
				}
				else
				{
					if(giCurrentInsulationChan<38)
				   {
				        sprintf(sTxt,"正在测试第%d滑道间绝缘电阻",giCurrentInsulationChan);
				        SetCtrlAttribute (pH_InsulationTest, PANEL_13_cButtonPrint_5, ATTR_LABEL_TEXT, sTxt); 
						
				        if( ((giChanResSel[giCurrentInsulationChan-1]==1) || (giChanResSel[giCurrentInsulationChan-1]==2))&&(irstep<=5)  )
				        {
			                IRSlipRelationTest(giCurrentInsulationChan,1);
				        }
				        else
				        {
					        irstep=1;
				            giCurrentInsulationChan++;	 
				        }
				   }
				   else
				   {
				       giCurrentInsulationChan=1;
				       SetCtrlAttribute (pH_InsulationTest, PANEL_13_cButtonPrint_3, ATTR_LABEL_TEXT, "滑道间耐压测试完毕，可重新启动");
			           SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER, ATTR_INTERVAL, 1.0);
			           SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER, ATTR_ENABLED, 0); //disable timer						   
				   }

				}
			}
			/*
			for(i=0;i<80;i++)
			{
//如果超过限制，则停止测量
				if(giCurrentInsulationChan>=80)
				{
					giCurrentInsulationChan = -1;
					ResetTimer (pH_InsulationTest, PANEL_13_TIMER);
					ResetTimer (pH_InsulationTest, PANEL_13_TIMER_2);  
					ResetTimer (pH_InsulationTest, PANEL_13_TIMER_3);  	
					SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER,   ATTR_ENABLED, 0);  
					SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_2, ATTR_ENABLED, 0);
					SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_3, ATTR_ENABLED, 0); 					
					MessagePopup("提示","绝缘电阻测试完毕！");
					
					sprintf(gComBufT, "[M]!FW#");
					i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
					if(i<0)
						return -1;					
					return 0;
				}			
				giCurrentInsulationChan++; //下一滑道
				if(giChanResSel[(int)(0.5*giCurrentInsulationChan)]==0)
					continue;
				else
					break;
			}

			if((giCurrentInsulationChan%2)==0)
			{
				sprintf(sTxt, "滑道%d环间绝缘电阻测试中", (int)(0.5*giCurrentInsulationChan)+1);
				sprintf(gComBufT, "[M]>GP%d#", (int)(0.5*giCurrentInsulationChan));
				i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT); 
				if(i<0)
					return -1;
			}
			else
			{
				sprintf(sTxt, "滑道%d对地绝缘电阻测试中", (int)(0.5*giCurrentInsulationChan)+1);  
				sprintf(gComBufT, "[M]>IS%d#", (int)(0.5*giCurrentInsulationChan));
				i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT); 
				if(i<0)
					return -1;
			}
			//清空缓冲区			
			FlushInQ(gComInsulation);
			FlushOutQ(gComInsulation);    
//先停止设备
			sprintf(gComBufT, "tzcl8888");
			i = SendComCMD(gComInsulation, strlen(gComBufT), gComBufT); 			
			//	if(i<0)
			//		return -1;		
			Delay(1);
			
//恢复到开始测试状态
			sprintf(gComBufT, "qxcc3456");
			i = SendComCMD(gComInsulation, strlen(gComBufT), gComBufT); 			
			//	if(i<0)
			//		return -1;		
			Delay(1);
			
//启动绝缘仪串口
			sprintf(gComBufT, "cl780050");
			i = SendComCMD(gComInsulation, strlen(gComBufT), gComBufT); 			
			//	if(i<0)
			//		return -1;			
			
						  
			Delay(1.0);  
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_NUMERICSLIDE, ATTR_LABEL_TEXT, sTxt);
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER, ATTR_ENABLED, 0); //禁止本定时器   
			ResetTimer(pH_InsulationTest, PANEL_13_TIMER_2);
			ResetTimer(pH_InsulationTest, PANEL_13_TIMER_3); 			
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_2, ATTR_ENABLED, 1); //启动20S读数据计时器
			SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_3, ATTR_ENABLED, 1); //显示进度
		    */	
			break;
	}
	return 0;
}

int CVICALLBACK PANEL_13_Progrocess (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	int iColor;
	double fProgress;
	int iChan;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			iChan = (int)(0.5*giCurrentInsulationChan)+1;	 
			GetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_3, MakePoint(iChan,1), ATTR_CMD_BUTTON_COLOR, &iColor);
			SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_3, MakePoint(iChan,1), ATTR_CMD_BUTTON_COLOR, (iColor==VAL_YELLOW)?VAL_GREEN:VAL_YELLOW); 
			GetCtrlVal(pH_InsulationTest, PANEL_13_NUMERICSLIDE, &fProgress);

			if(fProgress>=0.95)
			{
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_3, MakePoint(iChan,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE);  
				SetCtrlVal(pH_InsulationTest, PANEL_13_NUMERICSLIDE, 0.0); 
				SetCtrlAttribute (pH_InsulationTest, PANEL_13_TIMER_3, ATTR_ENABLED, 0);
				ResetTimer(pH_InsulationTest, PANEL_13_TIMER_3);
				return 0;
			}
			SetCtrlVal(pH_InsulationTest, PANEL_13_NUMERICSLIDE, ((fProgress+0.05)>1)?1:(fProgress+0.05) ); 
			break;
	}
	return 0;
}

/**************************************
** 单环耐压测试                      **
** step1:open all relay,set mode,set current voltage para
** step2:close resp relay
** step3:start test
** step4:testing for setted time,juduge test wether finished
** step5:open resp relay
** setp6:judement reusult

** flag 0:滑间;1:对地
*/
int IRSlipRelationTest(int Channel,int flag)   
{
   int i;
   char strTmp[20];
   switch(irstep)
	{
		case 1:
			{
			  OpenRelay();
			  SetInstruct("AIN:","FUNC ","MANU","");    //Switch to MANU Mode
			  SetInstruct("MANU:","EDIT:","MODE ","IR"); //Switch to MANU IR Mode 
			  sprintf(strTmp,"%g",fHiVolt); 
			  SetInstruct("MANU:","IR:","VOLT ",strTmp);  // kV
			  sprintf(strTmp,"%g",iHiRes); 
			  SetInstruct("MANU:","IR: ","RHIS ",strTmp);   // mA
			  sprintf(strTmp,"%g",iLiRes); 
			  SetInstruct("MANU:","IR:","RLOS ",strTmp);    // mA
			  sprintf(strTmp,"%g",fTestTime); 
			  SetInstruct("MANU:","IR:","TTIM ",strTmp);   // S 
			  SetInstruct("MANU:","IR:","FREQ ","50");   // Hz
			  
			  irstep=2;  
			  break;
			}
		case 2:
			{
			  CloseHiRelay(Channel,flag);
			  irstep=3;
              break;	
			}
		case 3:
			{
			  SetInstruct("FUNC:","TEST ","ON","");
			  irstep=4;  
			  break;
			}
		case 4:
			{
			  QueryInstruct("FUNC:","TEST");
			  i=FindPattern(gComBufR,0,-1,"OFF",0,0);
			  if(i>0)
			    irstep=5;  
			  break;
			}
		case 5:
			{
			  int j,k,l,m;
			  OpenRelay(); 
			  QueryInstruct("MEAS ",""); 
			  i=FindPattern(gComBufR,0,-1,"FAIL",0,0);
			  
			  j=FindPattern(gComBufR,0,-1,",",0,0);
			  k=FindPattern(gComBufR,j+1,-1,",",0,0);
			  l=FindPattern(gComBufR,k+1,-1,",",0,0); 
			  m=FindPattern(gComBufR,l+1,-1,",",0,0); 
			  CopyString(strTmp,0,gComBufR,l+1,(m-l)-5);
			  
			  Point CurPoint;//(Channel,1);
			  CurPoint.x= Channel;
			  if(flag==0)
			  {
                CurPoint.y=1;
			  }
              else
			  {
				 CurPoint.y=2;  
			  }
			  if(i>0)
			  {  
					
				 SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_6, CurPoint, ATTR_CTRL_VAL, "N");
			  }
			  else 
			  {
			     SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_6, CurPoint, ATTR_CTRL_VAL, "Y"); 
			  }
			  
			  if(Channel>20)
			  {
				CurPoint.x= Channel-20;
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_5, CurPoint, ATTR_CTRL_VAL, strTmp); 
			  }
			  else
			  {
				CurPoint.x= Channel;
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_4, CurPoint, ATTR_CTRL_VAL, strTmp); 
				
			  }
			  irstep=6;  
			  break;
			}
	}	
	return 0;
}


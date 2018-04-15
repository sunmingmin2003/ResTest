/*        
  ������ѹ����
*/        
#include <formatio.h>
#include <utility.h>
#include <rs232.h>
#include <ansi_c.h>
#include <userint.h>
#include "FiberSensor.h"
#include "variable.h"
static int giCurrentHVChan = -1;   
int giChanTmp;
int gHVGNDSel;

float fHiVolt1,fHiVolt2;
float fHiCurrt1,fHiCurrt2;
float fLiCurrt1,fLiCurrt2;
float fTestTime1,fTestTime2;

/**************************************
** ������ѹ����                      **
** step1:open all relay,set mode,set current voltage para
** step2:close resp relay
** step3:start test
** step4:testing for setted time,juduge test wether finished
** step5:open resp relay
** setp6:judement reusult

** flag 0:����;1:�Ե�
***************************************/
int acwstep=1;
int flag = 0;
int ACWSlipRelationTest(int Channel,int flag);

/**************************************
** ����ָ��                          **
***************************************/
int SetInstruct(char* mode,char*testmode,char* cmd,char* Value);

/**************************************
** ��ѯָ��                          **
***************************************/
int QueryInstruct(char* mode,char*testmode);


/*****************************
open relay 
*****************************/
int OpenRelay();

/*******************
�պ�һ·��ѹ�̵���
flag:0������֮��
flag:1, �����Ե�
**/
int CloseHiRelay(int Channel,int flag );


//�˳�����
int CVICALLBACK PANEL_7_Exit (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_HVTest);
			break;
	}
	return 0;
}

//�л�����һ������Ե����
int CVICALLBACK PANEL_7_HI_Test (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_HVTest);   
			sprintf(gComBufT, "[M]!FW#");
			i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);			
			if(i<0)
				return -1;
			InstallPopup(pH_InsulationTest); //��Ե����
			for(i=0;i<20;i++)
			{   /*
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_4, MakePoint(i,1), ATTR_CTRL_VAL, 0.0);    
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_4, MakePoint(i,2), ATTR_CTRL_VAL, 0.0);  
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_5, MakePoint(i,1), ATTR_CTRL_VAL, 0.0);    
				SetTableCellAttribute (pH_InsulationTest, PANEL_13_TABLE_5, MakePoint(i,2), ATTR_CTRL_VAL, 0.0);  
			*/
			}
			
			break;
	}
	return 0;
}

//������һ��
int CVICALLBACK PANEL_7_PreTest (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_HVTest); 
			InstallPopup(pH_HITest);  //���������
			break;
	}
	return 0;
}

//������ѹ������
int CVICALLBACK PANEL_7_Start_Device (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf(gComBufT, "#G");
			i = SendComCMD(gComHV, 2, gComBufT); 
			if(i<0)
			{
				MessagePopup("��ʾ","��ѹ��������ʧ�ܣ����飡");
				return -1;
			}
			
			i = ComRd (gComHV, gComBufR, 20);
			if(i<4)
			{
				MessagePopup("��ʾ","��ѹ��������ʧ�ܣ����飡");     
				return -1;
			}
			
			FlushInQ(gComHV);
			FlushOutQ(gComHV);
			MessagePopup("��ʾ","��ѹ�����ɹ���");
			break;
	}
	return 0;
}

//��ʾ�������Խ���
int CVICALLBACK PANEL_7_Progrocess (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	int iColor,i=0;
	double fProgress;
	int iChan;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			iChan = giCurrentHVChan+1;
			GetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_3, MakePoint(iChan,1), ATTR_CMD_BUTTON_COLOR, &iColor);
			SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_3, MakePoint(iChan,1), ATTR_CMD_BUTTON_COLOR, (iColor==VAL_YELLOW)?VAL_GREEN:VAL_YELLOW); 
			GetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE, &fProgress);
		//	GetCtrlVal(pH_HVTest, PANEL_7_NUMERIC, &i);   //ÿ����������ʱ��     
			SetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE, ((fProgress+1.0/i)>1)?1:(fProgress+1.0/i) ); 
			break;
	}
	return 0;
}

//�رո�ѹ������
int CVICALLBACK PANEL_7_Close_Device(int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	int i;
	switch (event)
	{
		case EVENT_COMMIT:
			sprintf(gComBufT, "#U");
			i = SendComCMD(gComHV, 2, gComBufT); 
			if(i<0)
			{
				MessagePopup("��ʾ","��ѹֹͣ����ʧ�ܣ����飡");
				return -1;
			}
			
			i = ComRd (gComHV, gComBufR, 20);
			if(i<4)
			{
				MessagePopup("��ʾ","��ѹֹͣ����ʧ�ܣ����飡");     
				return -1;
			}
			
			FlushInQ(gComHV);
			FlushOutQ(gComHV);
			MessagePopup("��ʾ","��ѹֹͣ�ɹ���");			
			break;
	}
	return 0;
}

 //������ѹ���Ե�ѹֵ
int CVICALLBACK PANEL_7_Set_HV (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{		
	int i=0;
	switch (event)
	{
		case EVENT_COMMIT:
		//	GetCtrlVal(pH_HVTest, PANEL_7_NUMERICMETER, &i);
			i = i/4;
			sprintf(gComBufT, "%s%04d", "#S0500",i);
			i = SendComCMD(gComHV, strlen(gComBufT), gComBufT); 
			i = ComRd (gComHV, gComBufR, 20);
			if(i<4)
			{
				MessagePopup("��ʾ","��ѹ��������ʧ�ܣ����飡");     
				return -1;
			}	
			MessagePopup("��ʾ","��ѹ��������ɹ�");  			
			break;
	}						   
	return 0;
}
 //�Զ����Ի�����ѹ
int CVICALLBACK PANEL_7_Auto_Start (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{

	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_HV1_1,&fHiVolt1);
	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_HC1_1,&fHiCurrt1); 
	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_LC1_1,&fLiCurrt1); 
	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_TIME1_1,&fTestTime1);
	
  	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_HV1_2,&fHiVolt2);
	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_HC1_2,&fHiCurrt2); 
	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_LC1_2,&fLiCurrt2); 
	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_TIME1_2,&fTestTime2); 
	
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_INTERVAL, 1.0);//����ִ��
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_ENABLED, 1); 
			giCurrentHVChan=1;
			gHVGNDSel = 0;
			acwstep = 1;
			flag = 0;
			break;
	}
	return 0;
}

 //�Զ�����,��ʱ��PANEL_7�Ļص�����
int CVICALLBACK PANEL_7_Auto_Test (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{  	int i,j;
	char sTxt[50];
	switch (event)
	{
		case EVENT_TIMER_TICK:
			{  
				if(flag==0)
			    {
				    if(giCurrentHVChan<38)
			        {
				   
				        if( ((giChanResSel[giCurrentHVChan-1]==1) || (giChanResSel[giCurrentHVChan-1]==2)||(giChanResSel[giCurrentHVChan-1]==3))  )
				        {
				           sprintf(sTxt,"���ڲ��Ե�%d��������ѹ",giCurrentHVChan);
				           SetCtrlAttribute (pH_HVTest, PANEL_7_cButtonPrint_3, ATTR_LABEL_TEXT, sTxt); 
							
			                ACWSlipRelationTest(giCurrentHVChan,0);
							if(acwstep>=6)
							{
							    giCurrentHVChan++;
								acwstep=1;	
							}
				        }
				        else
				        {
				           sprintf(sTxt,"��%d�����䲻��Ҫ��ѹ����",giCurrentHVChan);
				           SetCtrlAttribute (pH_HVTest, PANEL_7_cButtonPrint_3, ATTR_LABEL_TEXT, sTxt); 
							
					        acwstep=1;
				            giCurrentHVChan++;	 
							break;
				        }
			        }
			        else
			        {
				       giCurrentHVChan=1;
				       SetCtrlAttribute (pH_HVTest, PANEL_7_cButtonPrint_3, ATTR_LABEL_TEXT, "��������ѹ������ϣ�����������");
			           SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_INTERVAL, 1.0);
			           SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_ENABLED, 0); //disable timer	
				    }
				}
			    else
			    {
				    if(giCurrentHVChan<38)
			        {
				   
				        if( ((giChanResSel[giCurrentHVChan-1]==1) || (giChanResSel[giCurrentHVChan-1]==2))  )
				        {
				          sprintf(sTxt,"���ڲ��Ե�%d�����Ե���ѹ��",giCurrentHVChan);
				          SetCtrlAttribute (pH_HVTest, PANEL_7_cButtonPrint_5, ATTR_LABEL_TEXT, sTxt); 
							
			              ACWSlipRelationTest(giCurrentHVChan,1);
						  if(acwstep>=6)
						  {
							 acwstep=1;
							 giCurrentHVChan++;
						  }								
				        }
				        else if(giChanResSel[giCurrentHVChan-1]==3)
				        {
				          sprintf(sTxt,"��%d�����ӵػ�,����Ե���ѹ���ԣ�",giCurrentHVChan);
				          SetCtrlAttribute (pH_HVTest, PANEL_7_cButtonPrint_5, ATTR_LABEL_TEXT, sTxt); 
						 
					       acwstep=1;
				           giCurrentHVChan++;	 
				       }
					   else
					   {
				          sprintf(sTxt,"��%d��������Ե���ѹ���ԣ�",giCurrentHVChan);
				          SetCtrlAttribute (pH_HVTest, PANEL_7_cButtonPrint_5, ATTR_LABEL_TEXT, sTxt); 
						 
					       acwstep=1;
				           giCurrentHVChan++;	 
						   
					   }
			        }
			        else
			        {
				       giCurrentHVChan=1;
				       SetCtrlAttribute (pH_HVTest, PANEL_7_cButtonPrint_5, ATTR_LABEL_TEXT, "�����Եز�����ϣ�����������");
			           SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_INTERVAL, 1.0);
			           SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_ENABLED, 0); //disable timer	
				    }		   
			   }
			}
			/*
			GetCtrlVal(pH_HVTest, PANEL_7_NUMERIC, &i);  //�õ�ÿ����������ʱ��
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_INTERVAL, 1.0*i);
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_ENABLED, 0); //disable timer	
			ResetTimer(pH_HVTest, PANEL_7_TIMER_2); 
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER_2, ATTR_ENABLED, 0);  //disable ��ʾ���� timer
			if(giCurrentHVChan!=-1)		//�ָ���һ�β���Ϊ��ɫ
				SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_3, MakePoint(giChanTmp+1,1), ATTR_CMD_BUTTON_COLOR, VAL_WHITE); 
			
			for(i=0;i<40;i++)
			{
             //����������ƣ���ֹͣ����
				if(giCurrentHVChan>=40)
				{
					giCurrentHVChan = -1;
					ResetTimer (pH_HVTest, PANEL_7_TIMER);
					ResetTimer (pH_HVTest, PANEL_7_TIMER_2);  
					SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER,   ATTR_ENABLED, 0);  
					SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER_2, ATTR_ENABLED, 0);
					MessagePopup("��ʾ","��ѹ��ѹ������ϣ�");
					for(j=0;j<40;j++)
					{
						if( (giChanResSel[j]==1) || (giChanResSel[j]==2) )
							SetTableCellRangeAttribute (pH_HITest, PANEL_8_TABLE_4, MakeRect(1,j+1,1,1), ATTR_TEXT_BGCOLOR, VAL_WHITE);   
					}
					//�Ͽ��̵���
					sprintf(gComBufT, "[M]!FW#");
					i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
					if(i<0)
						return -1;
					return 0;
				}			
				giCurrentHVChan++;
				if(giChanResSel[giCurrentHVChan]==0)
					continue;
				else
			
					break;
		    	}
				
			if(gHVGNDSel==0)
			{
				sprintf(sTxt, "����%d������ѹ������", giCurrentHVChan+1);
				sprintf(gComBufT, "[M]>VL%d#", giCurrentHVChan);
				i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
				if(i<0)
				return -1;
			}
			else
			{
				sprintf(sTxt, "����%d�Ե���ѹ������", giCurrentHVChan +1);  
				sprintf(gComBufT, "[M]>PR%d#",  giCurrentHVChan );
				i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT); 
				if(i<0)
					return -1;
			}
						  
			Delay(1.0); 
			SetCtrlAttribute (pH_HVTest, PANEL_7_NUMERICSLIDE, ATTR_LABEL_TEXT, sTxt);  
			
			ResetTimer(pH_HVTest, PANEL_7_TIMER); 
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_ENABLED, 1);     			
			ResetTimer(pH_HVTest, PANEL_7_TIMER_2);
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER_2, ATTR_ENABLED, 1); //������ʾ���ȼ�ʱ��
			SetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE, 0.0 ); 
			giChanTmp = giCurrentHVChan;  */
			break;   
	}
	return 0;
}

//ֹͣ��ѹ����
int CVICALLBACK PANEL_7_Stop_Test (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	int j;
	switch (event)
	{
		case EVENT_COMMIT:
			giCurrentHVChan = -1;         
			ResetTimer(pH_HVTest, PANEL_7_TIMER);//�Զ����Զ�ʱ�� 
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_ENABLED, 0); //ֹͣ�Զ����Զ�ʱ��   			
			ResetTimer(pH_HVTest, PANEL_7_TIMER_2);
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER_2, ATTR_ENABLED, 0); //������ʾ���ȼ�ʱ��
			SetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE, 0.0 ); 		
			for(j=0;j<40;j++)
			{
				if( (giChanResSel[j]==1) || (giChanResSel[j]==2) )
					SetTableCellRangeAttribute (pH_HITest, PANEL_8_TABLE_4, MakeRect(1,j+1,1,1), ATTR_TEXT_BGCOLOR, VAL_WHITE); //��������Խ��  
			}
			sprintf(gComBufT, "[M]!FW#");
			SendComCMD(gComRLY, strlen(gComBufT), gComBufT);			
			break;
	}
	return 0;
}
 //��ѹ���Խ��
int CVICALLBACK PANEL_7_SetResult (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	char sTmp[10];
	Point MyPoint;
	int i;
	switch (event)
	{
		case EVENT_LEFT_DOUBLE_CLICK:
			GetActiveTableCell (pH_HVTest, PANEL_7_TABLE_4, &MyPoint);
			if( (giChanResSel[MyPoint.x-1]!=1) && (giChanResSel[MyPoint.x-1]!=2) )
				return -1;
			GetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_4, MyPoint, ATTR_CTRL_VAL, sTmp);
			if(sTmp[0]=='Y')
			{
				SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_4, MyPoint, ATTR_CTRL_VAL, "N");  
				SetTableCellRangeAttribute (pH_HVTest, PANEL_7_TABLE_4, MakeRect(MyPoint.y ,MyPoint.x,1,1), ATTR_TEXT_BGCOLOR, VAL_RED);  
			}
			else
			{
				SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_4, MyPoint, ATTR_CTRL_VAL, "Y"); 
				SetTableCellRangeAttribute (pH_HVTest, PANEL_7_TABLE_4, MakeRect(MyPoint.y ,MyPoint.x,1,1), ATTR_TEXT_BGCOLOR, VAL_WHITE);  				
			}
			break;
	}
	return 0;
}

//�Զ����Ի����Ե���ѹ
int CVICALLBACK PANEL_7_Auto_GND_Start (int panel, int control, int event,
										void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT: 
			GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_HV1_1,&fHiVolt1);
	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_HC1_1,&fHiCurrt1); 
	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_LC1_1,&fLiCurrt1); 
	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_TIME1_1,&fTestTime1);
	
   	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_HV1_2,&fHiVolt2);
	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_HC1_2,&fHiCurrt2); 
	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_LC1_2,&fLiCurrt2); 
	        GetCtrlVal(pH_HVTest,PANEL_7_NUMERIC_TIME1_2,&fTestTime2); 
	
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_INTERVAL, 1.0);//����ִ��
			SetCtrlAttribute (pH_HVTest, PANEL_7_TIMER, ATTR_ENABLED, 1); 
			giCurrentHVChan=1;  
			flag = 1;
			gHVGNDSel = 1;		 
		//	CloseHiRelay(19,1);
			
			break;
	}
	return 0;
}

	
/**************************************
** ������ѹ����                      **
** step1:open all relay,set mode,set current voltage para
** step2:close resp relay
** step3:start test
** step4:testing for setted time,juduge test wether finished
** step5:open resp relay
** setp6:judement reusult

** flag 0:����;1:�Ե�
***************************************/
int ACWSlipRelationTest(int Channel,int flag)
{   int i;
    char strTmp[10];
	double fProgress;
	switch(acwstep)
	{
		case 1:
			{
			  OpenRelay();
			 // SetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE,0);
			  if(Channel>7)
			  {
			    SetInstruct("MAIN:","FUNC ","MANU","");    //Switch to MANU Mode
			    SetInstruct("MANU:","EDIT:","MODE ","ACW"); //Switch to MANU ACW Mode  
				sprintf(strTmp,"%g",fHiVolt2);
			    SetInstruct("MANU:","ACW:","VOLT ",strTmp);     // kV
				sprintf(strTmp,"%g",fHiCurrt2); 
			    SetInstruct("MANU:","ACW: ","CHIS ",strTmp);   // mA
				sprintf(strTmp,"%g",fLiCurrt2);
			    SetInstruct("MANU:","ACW:","CLOS ",strTmp);    // mA
				sprintf(strTmp,"%g",fTestTime2);
			    SetInstruct("MANU:","ACW:","TTIM ",strTmp);   // S 
				
			    SetInstruct("MANU:","ACW:","FREQ ","50");   // Hz
			  }
			  else
			  {
			    SetInstruct("MAIN:","FUNC ","MANU","");    //Switch to MANU Mode
			    SetInstruct("MANU:","EDIT:","MODE ","ACW"); //Switch to MANU ACW Mode 
				sprintf(strTmp,"%g",fHiVolt1); 
			    SetInstruct("MANU:","ACW:","VOLT ",strTmp);  // kV
				sprintf(strTmp,"%g",fHiCurrt1); 
			    SetInstruct("MANU:","ACW: ","CHIS ",strTmp);   // mA
				sprintf(strTmp,"%g",fLiCurrt1); 
			    SetInstruct("MANU:","ACW:","CLOS ",strTmp);    // mA
				sprintf(strTmp,"%g",fTestTime1); 
			    SetInstruct("MANU:","ACW:","TTIM ",strTmp);   // S 
			    SetInstruct("MANU:","ACW:","FREQ ","50");   // Hz
			  }
			  
			  acwstep=2;  
			  break;
			}
		case 2:
			{
			  CloseHiRelay(Channel,flag);
			  acwstep=3;
              break;	
			}
		case 3:
			{
			  SetInstruct("FUNC:","TEST ","ON","");
			  acwstep=4;  
			  break;
			}
		case 4:
			{
			  if(Channel>7) 
			  {
			    GetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE, &fProgress);
			    GetCtrlVal(pH_HVTest, PANEL_7_NUMERIC_TIME1_2, &fTestTime2);   //ÿ����������ʱ��     
			    SetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE, ((fProgress+1.0/fTestTime2)>1)?1:(fProgress+1.0/fTestTime2) ); 
			  }
			  else
			  {
			    GetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE, &fProgress);
			    GetCtrlVal(pH_HVTest, PANEL_7_NUMERIC_TIME1_1, &fTestTime1);   //ÿ����������ʱ��     
			    SetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE, ((fProgress+1.0/fTestTime1)>1)?1:(fProgress+1.0/fTestTime1) ); 
			  }
			  QueryInstruct("FUNC:","TEST");
			  i=FindPattern(gComBufR,0,-1,"OFF",0,0);
			  if(i>0)
			  {
			    acwstep=5;
		//		SetCtrlVal(pH_HVTest, PANEL_7_NUMERICSLIDE,1);
				
			  }
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
					
				   SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_4, CurPoint, ATTR_CTRL_VAL, "N");
				}
			  else 
			    {
				   SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_4, CurPoint, ATTR_CTRL_VAL, "Y"); 	
				}
			  if(Channel>20)
			  {
				CurPoint.x= Channel-20;
				SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_6, CurPoint, ATTR_CTRL_VAL, strTmp); 
			  }
			  else
			  {
				CurPoint.x= Channel;
				SetTableCellAttribute (pH_HVTest, PANEL_7_TABLE_5, CurPoint, ATTR_CTRL_VAL, strTmp); 
				
			  }
				
			  acwstep=6;  
			  break;
			}
		
		
	
	}
return 0;
}

/**************************************
** ����ָ��                          **
***************************************/
int SetInstruct(char* mode,char*testmode,char* cmd,char* Value)
{
   int i;
   sprintf(gComBufT, "%s%s%s%s\r\n",mode,testmode,cmd,Value);
   i = SendComCMD(gComHV, strlen(gComBufT), gComBufT);
   Delay(0.1);
  // 	i = ComRd (gComHV, gComBufR, 40);  
   return 0;
}

/**************************************
** ��ѯָ��                          **
***************************************/
int QueryInstruct(char* mode,char*testmode)
{
	char sTmp[50];
	int i;
	sprintf(gComBufT,"%s%s ?\r\n",mode,testmode); 
    i = SendComCMD(gComHV, strlen(gComBufT), gComBufT); 
	Delay(0.5);
	i = ComRd (gComHV, gComBufR, 40);
	if(i<=0)
		return i;
	//sprintf(sTmp, ""); 
	return 0;   
}

/*****************************
open relay 
*****************************/
int OpenRelay()
{
	int i;
	sprintf(gComBufT, "[M]!FW#");
	i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT);
	if(i<0)
	  return -1;
    return 0;
}

/*******************
�պ�һ·��ѹ�̵���
flag:0������֮��
flag:1, �����Ե�
**/
int CloseHiRelay(int Channel,int flag )
{
	int i;
    if(flag==0)
	{
	   sprintf(gComBufT, "[M]>VL%d#",  Channel-1 );
	}
	else
	{
	   sprintf(gComBufT, "[M]>PR%d#",  Channel-1 );
	}
	i = SendComCMD(gComRLY, strlen(gComBufT), gComBufT); 
	if(i<0)
	  return -1;
	return 0;
}

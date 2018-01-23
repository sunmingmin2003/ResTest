#include <userint.h>
#include <ansi_c.h>
#include <formatio.h>
#include "variable.h"
#include "FiberSensor.h"        
#include "ChanGraphView.h"
static int gMouseOldX, gMouseOldY;
static int gMaxGraphID  = 16;  //用来记录哪一个窗口被最大化

int CVICALLBACK PANEL_BTNScale (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int iZOOMStyle = VAL_ZOOM_TO_RECT;
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (pH_CMD, PANEL_10_cButtonXScale, ATTR_CTRL_VAL, 0);
			SetCtrlAttribute (pH_CMD, PANEL_10_cButtonYScale, ATTR_CTRL_VAL, 0); 
			SetCtrlAttribute (pH_CMD, PANEL_10_cButtonRectScale, ATTR_CTRL_VAL, 0); 
			SetCtrlAttribute (panel, control, ATTR_CTRL_VAL, 1);	//让别的控件不使能
			
			if(control == PANEL_10_cButtonXScale)
				iZOOMStyle = VAL_ZOOM_XAXIS;
			
			if(control == PANEL_10_cButtonYScale)
				iZOOMStyle = VAL_ZOOM_YAXIS;
			
			if(control == PANEL_10_cButtonRectScale)
				iZOOMStyle = VAL_ZOOM_TO_RECT;
			
			break;
	}
	return 0;
}


int CVICALLBACK OnGraph (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int i;
	int iXScaleMode,iYScaleMode;
	double	fXScaleMin, fXScaleMax, fYScaleMin,fYScaleMax;
	
	double fScaleFactor;
	int iZOOMStyle, iXYScaleZOOMStyle;
	double fXCoordinate ,fYCoordinate;
	int x,y,iHight, iWidth;
	int iMouse;
	int iAnnotationIndex;

	char str1[50];
	switch (event)
	{
		case EVENT_MOUSE_POINTER_MOVE: 
			GetRelativeMouseState (panel, control, &x, &y, &iMouse, NULL, NULL);
			if(iMouse==1)
			{
				GetCtrlAttribute (panel, control, ATTR_HEIGHT, &iHight);
				GetCtrlAttribute (panel, control, ATTR_WIDTH, &iWidth);				
				GetAxisScalingMode (panel, control, VAL_BOTTOM_XAXIS, &iXScaleMode, &fXScaleMin, &fXScaleMax); 
				GetAxisScalingMode (panel, control, VAL_LEFT_YAXIS,   &iYScaleMode, &fYScaleMin, &fYScaleMax);   
				SetAxisScalingMode (panel, control, VAL_BOTTOM_XAXIS, VAL_MANUAL, fXScaleMin - (x-gMouseOldX)*(fXScaleMax-fXScaleMin)/iWidth, fXScaleMax - (x-gMouseOldX)*(fXScaleMax-fXScaleMin)/iWidth ); 
				SetAxisScalingMode (panel, control, VAL_LEFT_YAXIS,   VAL_MANUAL, fYScaleMin + (y-gMouseOldY)*(fYScaleMax-fYScaleMin)/iHight, fYScaleMax + (y-gMouseOldY)*(fYScaleMax-fYScaleMin)/iHight ); 				
			}
			gMouseOldX =x;
			gMouseOldY =y; 			
			break;
		case EVENT_LEFT_CLICK: 
 			GetRelativeMouseState (panel, control, &gMouseOldX, &gMouseOldY, NULL, NULL, NULL);   
	   	case EVENT_LEFT_CLICK_UP: 
			DeleteGraphAnnotation (panel, control, -1);
			if(gDisplayAnnotation)
			{
				GetGraphCursor (panel, control, 1, &fXCoordinate, &fYCoordinate);                               			
				sprintf(str1,"(%d-%.3fΩ)", (int)fXCoordinate, fYCoordinate );                                                                   			
				iAnnotationIndex = AddGraphAnnotation (panel, control, fXCoordinate, fYCoordinate, str1, 10, 10);
				SetAnnotationAttribute (panel, control, iAnnotationIndex, ATTR_ANNOTATION_GLYPH_STYLE, VAL_NO_POINT);
				SetAnnotationAttribute (panel, control, iAnnotationIndex, ATTR_ANNOTATION_CAPTION_COLOR, VAL_BLACK); 			
				SetAnnotationAttribute (panel, control, iAnnotationIndex, ATTR_ANNOTATION_CAPTION_COLOR, VAL_BLACK);
			}
			break;			
		case EVENT_LEFT_DOUBLE_CLICK:
			SetAxisScalingMode (panel, control, VAL_BOTTOM_XAXIS, VAL_AUTOSCALE, 0, 1);
			GetAxisScalingMode (panel, control, VAL_BOTTOM_XAXIS, &iXScaleMode, &fXScaleMin, &fXScaleMax);
			SetAxisScalingMode (panel, control, VAL_BOTTOM_XAXIS, VAL_MANUAL, fXScaleMin, fXScaleMax); 			
			SetAxisScalingMode (panel, control, VAL_LEFT_YAXIS, VAL_MANUAL, gYAxisMin, gYAxisMax);
			break;

		case EVENT_MOUSE_WHEEL_SCROLL:
			GetGraphCursor (panel, control, 1, &fXCoordinate, &fYCoordinate);
			GetRelativeMouseState (panel, control, &x, &y, NULL, NULL, &iMouse);
			fScaleFactor = (eventData1==2)?_GRAPH_SCALE_FACTOR:(1/_GRAPH_SCALE_FACTOR);
			
			GetCtrlVal(panel, PANEL_10_cButtonRectScale, &iXYScaleZOOMStyle);
			GetCtrlVal(panel, PANEL_10_cButtonXScale, &iZOOMStyle);  
			if( (iZOOMStyle ==1) || (iXYScaleZOOMStyle ==1) &&(iMouse!=VAL_MENUKEY_MODIFIER))		  //如果选择了X轴缩放或者选择了矩形缩放,按Ctrl也可以放大Y轴
			{
				GetAxisScalingMode (panel, control, VAL_BOTTOM_XAXIS, &iXScaleMode, &fXScaleMin, &fXScaleMax);
				SetAxisScalingMode (panel, control, VAL_BOTTOM_XAXIS, iXScaleMode, fXScaleMin + (fXCoordinate -fXScaleMin)*(fScaleFactor-1), fXScaleMax - (fXScaleMax -fXCoordinate)*(fScaleFactor-1));
			}
			GetCtrlVal(panel, PANEL_10_cButtonYScale, &iZOOMStyle);
			if( (iZOOMStyle ==1) || (iXYScaleZOOMStyle ==1)&&(iMouse!=VAL_SHIFT_MODIFIER))     	  //如果选择了Y轴缩放或者选择了矩形缩放
			{
				GetAxisScalingMode (panel, control, VAL_LEFT_YAXIS, &iYScaleMode, &fYScaleMin, &fYScaleMax);
				SetAxisScalingMode (panel, control, VAL_LEFT_YAXIS, iYScaleMode, fYScaleMin + (fYCoordinate -fYScaleMin)*(fScaleFactor-1), fYScaleMax - (fYScaleMax -fYCoordinate)*(fScaleFactor-1));        
			}	  	
			break;
	}
	return 0;
}

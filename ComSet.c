#include <userint.h>
#include "FiberSensor.h"
#include "variable.h"

int CVICALLBACK PABEL_6_SetCom (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(pH_ComSet, PANEL_6_NUMERIC1, &gComRLY);
			GetCtrlVal(pH_ComSet, PANEL_6_NUMERIC2, &gComHV);
			GetCtrlVal(pH_ComSet, PANEL_6_NUMERIC3, &gComHI);	//大电流发生器	
			GetCtrlVal(pH_ComSet, PANEL_6_NUMERIC4, &gComInsulation);  
			break;
	}
	return 0;
}

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

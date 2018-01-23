#include <userint.h>
#include "FiberSensor.h"
#include "variable.h"               

int CVICALLBACK PANEL_18_ButtomQuit (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(pH_MeanAlarm);
			break;
	}
	return 0;
}


int CVICALLBACK PANEL_18_DelFiberRegion (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

int CVICALLBACK PANEL_18_AddRegion (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}

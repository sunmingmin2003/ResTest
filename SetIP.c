#include "variable.h"

int CVICALLBACK PANEL_11_Quit (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel (pH_DeviceIP);  
			break;
	}
	return 0;
}

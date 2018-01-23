#include <formatio.h>
#include <ansi_c.h>
#include <userint.h>
#include "FiberSensor.h"
#include "variable.h"

int CVICALLBACK PANEL_9_SetSaveDir (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int iFileHandle;
	switch (event)
	{
		case EVENT_COMMIT:
			return 0;
		   DirSelectPopup ("d:\\DTS\\CVI", "Select Directory", 1, 1, gsFileSaveDir);
		   SetCtrlVal(panel, PANEL_9_STRING, gsFileSaveDir);
//±£´æÄ¿Â¼´«µÝ		   
			iFileHandle = OpenFile ("path.cfg", VAL_WRITE_ONLY, VAL_TRUNCATE, VAL_BINARY);
			if(iFileHandle<0)
				return 0;
			WriteLine (iFileHandle, gsFileSaveDir, -1);  
			CloseFile(iFileHandle);
			break;
	}
	return 0;
}

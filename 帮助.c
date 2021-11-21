#include <cvirte.h>		
#include <userint.h>
#include "°ïÖú.h"

static int helppanel;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((helppanel = LoadPanel (0, "°ïÖú.uir", HelpPanel)) < 0)
		return -1;
	// TODO: Get the panel handle associated with a tab page with GetPanelHandleFromTabPage
	DisplayPanel (helppanel);
	RunUserInterface ();
	DiscardPanel (helppanel);
	return 0;
}

int CVICALLBACK HelpPanel_Callback (int panel, int event, void *callbackData,
									int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

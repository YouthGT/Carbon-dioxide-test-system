/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1       /* callback function: Panel_Callback */
#define  PANEL_SystemTitle                2       /* control type: textMsg, callback function: (none) */
#define  PANEL_PASSWORD                   3       /* control type: string, callback function: (none) */
#define  PANEL_ENTERSYSTEM                4       /* control type: command, callback function: EnterSystem_Callback */
#define  PANEL_PICTURE                    5       /* control type: picture, callback function: (none) */
#define  PANEL_STRING                     6       /* control type: string, callback function: (none) */
#define  PANEL_PICTURE_2                  7       /* control type: picture, callback function: (none) */
#define  PANEL_PICTURE_Lock_2             8       /* control type: picture, callback function: (none) */
#define  PANEL_PICTURE_Lock               9       /* control type: picture, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK EnterSystem_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Panel_Callback(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
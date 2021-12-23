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

#define  SetPanel                         1       /* callback function: SetPanel_Callback */
#define  SetPanel_Set_TAB                 2       /* control type: tab, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_Co_UPPER_LIMIT          2       /* control type: scale, callback function: Co_SetAlarms_Callback */
#define  TABPANEL_Co_LOWER_LIMIT          3       /* control type: scale, callback function: Co_SetAlarms_Callback */
#define  TABPANEL_DECORATION_6            4       /* control type: deco, callback function: (none) */
#define  TABPANEL_DECORATION_4            5       /* control type: deco, callback function: (none) */
#define  TABPANEL_TEXTMSG_13              6       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_TEXTMSG_4               7       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_TEXTMSG_14              8       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_TEXTMSG_1               9       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_Co_AIrate               10      /* control type: numeric, callback function: (none) */
#define  TABPANEL_Co_AIcontrol            11      /* control type: string, callback function: (none) */
#define  TABPANEL_TEXTMSG_10              12      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_2_Te_UPPER_LIMIT        2       /* control type: scale, callback function: Te_SetAlarms_Callback */
#define  TABPANEL_2_Te_LOWER_LIMIT        3       /* control type: scale, callback function: Te_SetAlarms_Callback */
#define  TABPANEL_2_DECORATION_6          4       /* control type: deco, callback function: (none) */
#define  TABPANEL_2_DECORATION_4          5       /* control type: deco, callback function: (none) */
#define  TABPANEL_2_TEXTMSG               6       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_17            7       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_14            8       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_16            9       /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_Te_AIrate             10      /* control type: numeric, callback function: (none) */
#define  TABPANEL_2_Te_AIcontrol          11      /* control type: string, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_10            12      /* control type: textMsg, callback function: (none) */
#define  TABPANEL_2_TEXTMSG_11            13      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK Co_SetAlarms_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetPanel_Callback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Te_SetAlarms_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
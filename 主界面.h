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

#define  MAINPANEL                        1       /* callback function: MainPanel_Callback */
#define  MAINPANEL_Quit                   2       /* control type: command, callback function: Quit_Callback */
#define  MAINPANEL_CONCENTRATION          3       /* control type: scale, callback function: (none) */
#define  MAINPANEL_TEMP                   4       /* control type: scale, callback function: (none) */
#define  MAINPANEL_Te_ALARM_HIGH          5       /* control type: LED, callback function: (none) */
#define  MAINPANEL_Te_ALARM_LOW           6       /* control type: LED, callback function: (none) */
#define  MAINPANEL_Co_ALARM_HIGH          7       /* control type: LED, callback function: (none) */
#define  MAINPANEL_Co_ALARM_LOW           8       /* control type: LED, callback function: (none) */
#define  MAINPANEL_OffTiming              9       /* control type: command, callback function: OffTiming_Callback */
#define  MAINPANEL_AutoSave               10      /* control type: command, callback function: AutoSave_Callback */
#define  MAINPANEL_SaveData               11      /* control type: command, callback function: SaveData_Callback */
#define  MAINPANEL_ParameterSet           12      /* control type: command, callback function: ParameterSet_Callback */
#define  MAINPANEL_StopAcquire            13      /* control type: command, callback function: StopAcquire_Callback */
#define  MAINPANEL_Co_Te_Start            14      /* control type: command, callback function: Co_Te_Start_Callback */
#define  MAINPANEL_CleanChart             15      /* control type: command, callback function: CleanChart_Callback */
#define  MAINPANEL_DECORATION_8           16      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_3           17      /* control type: deco, callback function: (none) */
#define  MAINPANEL_Time                   18      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_7              19      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_Date                   20      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_7           21      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_2           22      /* control type: deco, callback function: (none) */
#define  MAINPANEL_Te_Chart               23      /* control type: strip, callback function: (none) */
#define  MAINPANEL_Co_Chart               24      /* control type: strip, callback function: (none) */
#define  MAINPANEL_DECORATION_5           25      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG_6              26      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_PICTURE                27      /* control type: picture, callback function: (none) */
#define  MAINPANEL_TEXTMSG_0              28      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_3              29      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TIMER_mm               30      /* control type: numeric, callback function: (none) */
#define  MAINPANEL_Co_SWITCH              31      /* control type: binary, callback function: Co_SWITCH_Callback */
#define  MAINPANEL_Te_SWITCH              32      /* control type: binary, callback function: Te_SWITCH_Callback */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

#define  MENUBAR                          1
#define  MENUBAR_Setting                  2
#define  MENUBAR_Setting_FilePath         3       /* callback function: FilePath_Callback */
#define  MENUBAR_Setting_ITEM1            4
#define  MENUBAR_Setting_ITEM1_SUBMENU    5
#define  MENUBAR_Setting_ITEM1_ASCII_O    6       /* callback function: ASCII_O_Callback */
#define  MENUBAR_Setting_ITEM1_Binary_O   7       /* callback function: Binary_O_Callback */
#define  MENUBAR_Setting_ITEM_2           8
#define  MENUBAR_Setting_ITEM_2_SUBMENU   9
#define  MENUBAR_Setting_ITEM_2_ASCII_I   10      /* callback function: ASCII_I_Callback */
#define  MENUBAR_Setting_ITEM_2_Binary_I  11      /* callback function: Binary_I_Callback */
#define  MENUBAR_DataAys                  12      /* callback function: DataAys_Callback */
#define  MENUBAR_ScreenShot               13      /* callback function: ScreenShot_Callback */
#define  MENUBAR_Help                     14      /* callback function: Help_Callback */
#define  MENUBAR_About                    15      /* callback function: About_Callback */


     /* Callback Prototypes: */

void CVICALLBACK About_Callback(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ASCII_I_Callback(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ASCII_O_Callback(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK AutoSave_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK Binary_I_Callback(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Binary_O_Callback(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK CleanChart_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Co_SWITCH_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Co_Te_Start_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK DataAys_Callback(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK FilePath_Callback(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Help_Callback(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK MainPanel_Callback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OffTiming_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ParameterSet_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SaveData_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK ScreenShot_Callback(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK StopAcquire_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Te_SWITCH_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
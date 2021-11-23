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
#define  MAINPANEL_CleanFile              2       /* control type: command, callback function: CleanFile_Callback */
#define  MAINPANEL_Quit                   3       /* control type: command, callback function: Quit_Callback */
#define  MAINPANEL_Stop                   4       /* control type: command, callback function: Stop_Callback */
#define  MAINPANEL_CONCENTRATION          5       /* control type: scale, callback function: (none) */
#define  MAINPANEL_ALARM_HIGH             6       /* control type: LED, callback function: (none) */
#define  MAINPANEL_ALARM_LOW              7       /* control type: LED, callback function: (none) */
#define  MAINPANEL_LoadFile               8       /* control type: command, callback function: LoadFile_Callback */
#define  MAINPANEL_AutoSave               9       /* control type: command, callback function: AutoSave_Callback */
#define  MAINPANEL_SaveData               10      /* control type: command, callback function: SaveData_Callback */
#define  MAINPANEL_CleanChart             11      /* control type: command, callback function: CleanChart_Callback */
#define  MAINPANEL_Acquire                12      /* control type: command, callback function: Acquire */
#define  MAINPANEL_UPPER_LIMIT            13      /* control type: scale, callback function: SetAlarms_Callback */
#define  MAINPANEL_LOWER_LIMIT            14      /* control type: scale, callback function: SetAlarms_Callback */
#define  MAINPANEL_DECORATION_3           15      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION_4           16      /* control type: deco, callback function: (none) */
#define  MAINPANEL_Time                   17      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_6           18      /* control type: deco, callback function: (none) */
#define  MAINPANEL_DECORATION             19      /* control type: deco, callback function: (none) */
#define  MAINPANEL_Date                   20      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_AIrate                 21      /* control type: numeric, callback function: (none) */
#define  MAINPANEL_AIcontrol              22      /* control type: string, callback function: (none) */
#define  MAINPANEL_TEXTMSG_2              23      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_DECORATION_2           24      /* control type: deco, callback function: (none) */
#define  MAINPANEL_STRIPCHART             25      /* control type: strip, callback function: (none) */
#define  MAINPANEL_DECORATION_5           26      /* control type: deco, callback function: (none) */
#define  MAINPANEL_TEXTMSG_6              27      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_LED                    28      /* control type: LED, callback function: (none) */
#define  MAINPANEL_NUMERIC_co             29      /* control type: numeric, callback function: (none) */
#define  MAINPANEL_NUMERIC_do             30      /* control type: numeric, callback function: (none) */
#define  MAINPANEL_NUMERIC_up             31      /* control type: numeric, callback function: (none) */
#define  MAINPANEL_PICTURE                32      /* control type: picture, callback function: (none) */
#define  MAINPANEL_TEXTMSG_0              33      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_3              34      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_5              35      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_4              36      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_1              37      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TIMER_mm               38      /* control type: numeric, callback function: (none) */
#define  MAINPANEL_FileAnalysis           39      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_7              40      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_8              41      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_9              42      /* control type: textMsg, callback function: (none) */
#define  MAINPANEL_TEXTMSG_10             43      /* control type: textMsg, callback function: (none) */


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
#define  MENUBAR_Help                     12      /* callback function: Help_Callback */
#define  MENUBAR_About                    13      /* callback function: About_Callback */


     /* Callback Prototypes: */

void CVICALLBACK About_Callback(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK Acquire(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK ASCII_I_Callback(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK ASCII_O_Callback(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK AutoSave_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK Binary_I_Callback(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Binary_O_Callback(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK CleanChart_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CleanFile_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
void CVICALLBACK FilePath_Callback(int menubar, int menuItem, void *callbackData, int panel);
void CVICALLBACK Help_Callback(int menubar, int menuItem, void *callbackData, int panel);
int  CVICALLBACK LoadFile_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MainPanel_Callback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Quit_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SaveData_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SetAlarms_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK Stop_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
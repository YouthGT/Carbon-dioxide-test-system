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

#define  HelpPanel                        1       /* callback function: HelpPanel_Callback */
#define  HelpPanel_TAB                    2       /* control type: tab, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_0_TEXTBOX               2       /* control type: textBox, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_1_TEXTBOX               2       /* control type: textBox, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_3_TEXTBOX               2       /* control type: textBox, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_4_TEXTBOX               2       /* control type: textBox, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_5_TEXTBOX               2       /* control type: textBox, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK HelpPanel_Callback(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
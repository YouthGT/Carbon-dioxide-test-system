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

#define  DataAnalys                       1       /* callback function: DataAnalysis_Callback */
#define  DataAnalys_DECORATION_7          2       /* control type: deco, callback function: (none) */
#define  DataAnalys_DECORATION_6          3       /* control type: deco, callback function: (none) */
#define  DataAnalys_LoadFile              4       /* control type: command, callback function: LoadFile_Callback */
#define  DataAnalys_History_Te_Chart      5       /* control type: strip, callback function: (none) */
#define  DataAnalys_Co_Chart              6       /* control type: graph, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK DataAnalysis_Callback(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK LoadFile_Callback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
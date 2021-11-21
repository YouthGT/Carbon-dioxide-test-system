#include <NIDAQmx.h>
#include <formatio.h>
#include "pwctrl.h"
#include <cvirte.h>		
#include <userint.h>
#include "二氧化碳测试系统.h"
#include "主界面.h"
#include "帮助.h"

static int PassWord;//
static int panelHandle;
static int MainPanelHandle;
static int HelpPanelHandle;
static TaskHandle AItaskhandle;
double aidata[3]={25.0,0,18.0};//默认高低限
double upper_limit =25,lower_limit =18;//默认高低限
int fileTypeI=1;//默认输入ASCII类型
int fileTypeO=1;//默认输出ASCII类型
char timeString[50],dateString[50];//时间、日期数组
static char NewFilePath[MAX_PATHNAME_LEN]={"f:\\"};//文件夹名称
static char file_name[MAX_PATHNAME_LEN];


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "二氧化碳测试系统.uir", PANEL)) < 0)
	    return -1;
	if ((MainPanelHandle = LoadPanel (0, "主界面.uir", MAINPANEL)) < 0)
	    return -1;
	PassWord = PasswordCtrl_ConvertFromString (panelHandle, PANEL_PASSWORD);//将输入的字符转换成密码
	PasswordCtrl_SetAttribute(panelHandle,PANEL_PASSWORD,ATTR_PASSWORD_MASK_CHARACTER,42);//让输入的密码显示为*
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (MainPanelHandle);
	return 0;
}
//输入密码界面回调
int CVICALLBACK Panel_Callback (int panel, int event, void *callbackData,
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
/*进入系统按钮*/
int CVICALLBACK EnterSystem_Callback (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	char pw[100];
	char TruePw[100]="youthgt";
	//strcmp(str1, str2);   strcmp比较字符串是否相同返回值：
    //如果返回值 < 0，则表示 str1 小于 str2。
    //如果返回值 > 0，则表示 str2 小于 str1。
    //如果返回值 = 0，则表示 str1 等于 str2。
	switch (event)
	{
		case EVENT_COMMIT:
			PasswordCtrl_GetAttribute (panelHandle, PANEL_PASSWORD, ATTR_PASSWORD_VAL, pw);
			if(!strcmp(TruePw, pw))

			{
			//SetCtrlAttribute (MainPanelHandle, MAINPANEL_SystemTime, ATTR_ENABLED, 1);//启动时间显示
				EnableDragAndDrop (MainPanelHandle);//让主面板使能
				DisplayPanel (MainPanelHandle); //显示主面板  
				DiscardPanel (panelHandle);//释放输入密码界面
				
			}
			else
			{
				MessagePopup("提示信息", pw);
			}

			
			break;
	}
	return 0;
}
/*退出按钮*/
int CVICALLBACK QuitSystem_Callback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int Waring;
	switch (event)
	{
		case EVENT_COMMIT:
			Waring = ConfirmPopup ("警告", "是否退出系统？");
			if(Waring)
			{
				QuitUserInterface (0);
			}
			else
				break;
			break;
	}
	return 0;
}


/*************************************************************************************/
/*                                                                                   */
/*                              下面回调都有关于主界面                                 */
/*                                                                                   */
/*************************************************************************************/

/*主界面回调*/
int CVICALLBACK MainPanel_Callback (int panel, int event, void *callbackData,
									int eventData1, int eventData2)
{
	char **addr;
	char aiin[3];
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
            QuitUserInterface (0);
			break;
		case EVENT_FILESDROPPED:
			addr=(char **)eventData1;
			memset (addr, 0, 100);
//			LoadFile_Callback (panelHandle,0, EVENT_COMMIT,0,0,0);		
			FileToArray (*addr, aiin, VAL_DOUBLE, 3, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, fileTypeI);
			SetCtrlVal (MainPanelHandle, MAINPANEL_FileAnalysis,*addr);//显示文件路径				
			SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_up, aiin[0]);//显示当时上限
			SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_co, aiin[1]);//显示当时浓度
			SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_do, aiin[2]);//显示当时下限
			if(aiin[2]<=aiin[1]&&aiin[1]<=aiin[0])//当时浓度正常
			   {
				   SetCtrlAttribute (MainPanelHandle, MAINPANEL_LED, ATTR_ON_COLOR, VAL_GREEN);
			   }
			else if(aiin[2]>aiin[1])//当时温度比温度下限低
			   {
				   SetCtrlAttribute (MainPanelHandle, MAINPANEL_LED, ATTR_ON_COLOR, VAL_BLUE);
			   }
			else                    //(aiin[1]>aiin[2])当时温度比温度上限高
			   {
				   SetCtrlAttribute (MainPanelHandle, MAINPANEL_LED, ATTR_ON_COLOR, VAL_RED);
			   }  
		  SetCtrlVal (MainPanelHandle, MAINPANEL_LED, 1);
	      MessagePopup ("载入文件", "载入成功！");
	
	}
	return 0;
}

/*AI数据Timer*/
int CVICALLBACK AItimer_Callback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
    float64 AIdata[1000];
	int32 sampsread;
	int i=0;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			DAQmxReadAnalogF64 (AItaskhandle, DAQmx_Val_Auto, 10.0, DAQmx_Val_GroupByChannel, AIdata, 1000, &sampsread, 0);	//读取数据
		    AIdata[i]=AIdata[i]*12.5+3; //标度变换
			aidata[1]=AIdata[i];//将采集数据传递给第二组Cursor,以便在示波器中显示
			PlotStripChart (MainPanelHandle, MAINPANEL_STRIPCHART, aidata, 3, 0, 0, VAL_DOUBLE);
			SetCtrlVal (MainPanelHandle, MAINPANEL_CONCENTRATION, aidata[1]);
			//温度警报灯亮灭控制
			if (AIdata[i] >= upper_limit) 
				SetCtrlVal (MainPanelHandle, MAINPANEL_ALARM_HIGH, 1);
   	   	    else
				SetCtrlVal (MainPanelHandle, MAINPANEL_ALARM_HIGH, 0);
			if (AIdata[i] <= lower_limit)
				SetCtrlVal (MainPanelHandle, MAINPANEL_ALARM_LOW, 1);
			else
				SetCtrlVal (MainPanelHandle, MAINPANEL_ALARM_LOW, 0);	
			break;
	}
	return 0;
}
/*获取系统时间Timer*/
int CVICALLBACK SystemTime_Callback (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
    int Seconds;
    int Minutes;
    int Hours;
	int Month;
	int Day;
	int Year;
	switch (event)
	{
		case EVENT_TIMER_TICK:
		GetSystemTime (&Hours, &Minutes, &Seconds);
		GetSystemDate (&Month, &Day, &Year);
        Fmt (dateString, "%d年%d月%d日",Year,Month,Day); 
		Fmt (timeString, "%d时%d分%d秒",Hours,Minutes,Seconds);
		SetCtrlVal (MainPanelHandle, MAINPANEL_Date, dateString);
		SetCtrlVal (MainPanelHandle, MAINPANEL_Time, timeString);
			break;
	}
	return 0;
}

/*清除文件显示按钮**/
int CVICALLBACK CleanFile_Callback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//将文件分析面板的值都恢复默认
			SetCtrlVal (MainPanelHandle, MAINPANEL_FileAnalysis, "文件分析");
			SetCtrlVal (MainPanelHandle, MAINPANEL_LED, 0);
			SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_up, 0.00);
			SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_co, 0.00);
			SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_do, 0.00);
			break;
	}
	return 0;
}
/*停止按钮**/
int CVICALLBACK Stop_Callback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 SetCtrlAttribute (MainPanelHandle, MAINPANEL_AItimer, ATTR_ENABLED, 0);//启用采集timer
			 SetCtrlAttribute (MainPanelHandle, MAINPANEL_Acquire, ATTR_DIMMED, 0);//让开始采集可用
			 DAQmxClearTask (AItaskhandle);
			 AItaskhandle=0;	 			
			break;
	}
	return 0;
}
/*载入文件按钮*/
int CVICALLBACK LoadFile_Callback (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
    double aiin[3];
	switch (event)
	{
		case EVENT_COMMIT:
			 if (FileSelectPopupEx (NewFilePath, "*.txt", "*.txt", "选择要打开的文件", VAL_OK_BUTTON, 0, 1, file_name) > 0)
				 {
					
						FileToArray (file_name, aiin, VAL_DOUBLE, 3, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, fileTypeI);
						SetCtrlVal (MainPanelHandle, MAINPANEL_FileAnalysis,file_name);//显示文件路径				
						SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_up, aiin[0]);//显示当时上限
						SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_co, aiin[1]);//显示当时浓度
						SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_do, aiin[2]);//显示当时下限
						if(aiin[2]<=aiin[1]&&aiin[1]<=aiin[0])//当时浓度正常
						   {
							   SetCtrlAttribute (MainPanelHandle, MAINPANEL_LED, ATTR_ON_COLOR, VAL_GREEN);
						   }
						else if(aiin[2]>aiin[1])//当时温度比温度下限低
						   {
							   SetCtrlAttribute (MainPanelHandle, MAINPANEL_LED, ATTR_ON_COLOR, VAL_BLUE);
						   }
   						else                    //(aiin[1]>aiin[2])当时温度比温度上限高
						   {
							   SetCtrlAttribute (MainPanelHandle, MAINPANEL_LED, ATTR_ON_COLOR, VAL_RED);
						   }  
					  SetCtrlVal (MainPanelHandle, MAINPANEL_LED, 1);
				      MessagePopup ("载入文件", "载入成功！");
				 }
	}
	return 0;
}
/*自动保存按钮*/
int CVICALLBACK AutoSave_Callback (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}
/*保存按钮*/
int CVICALLBACK SaveData_Callback (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	char TempFilePath[512];//定义临时路径
	
	switch (event)
	{
		case EVENT_COMMIT:
			strcpy(TempFilePath,NewFilePath);//将获得的新路径传递给定义临时路径
			strcat(TempFilePath,"//");//给路径加分隔线
            strcat(TempFilePath,strcat(strcat(dateString, timeString),".txt"));//拼接成完整路径
			ArrayToFile (TempFilePath, aidata, VAL_DOUBLE, 3, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 10, fileTypeO, VAL_TRUNCATE);
			MessagePopup ("保存文件", "保存成功！");  
			break;
	}
	return 0;
}
/*清除图表按钮*/
int CVICALLBACK CleanChart_Callback (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			ClearStripChart (MainPanelHandle, MAINPANEL_STRIPCHART);
			break;
	}
	return 0;
}
/*开始采集按钮*/
int CVICALLBACK Acquire (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	int AIrate;
    char AIchanel[256];
	switch (event)
	{
		case EVENT_COMMIT:
            GetCtrlVal (MainPanelHandle, MAINPANEL_AIcontrol, AIchanel);// 从界面得到模拟输入通道
			GetCtrlVal (MainPanelHandle, MAINPANEL_AIrate, &AIrate); // 从界面得到点数
			DAQmxCreateTask ("", &AItaskhandle); // 创建模拟输入任务
			//创建模拟输入通道
			DAQmxCreateAIVoltageChan (AItaskhandle, AIchanel, "Voltage", DAQmx_Val_RSE, -5.0, 5, DAQmx_Val_Volts, "");
			// 设置采样率
			DAQmxCfgSampClkTiming (AItaskhandle, "", AIrate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 100);
			//启用aitimer
			SetCtrlAttribute (MainPanelHandle, MAINPANEL_AItimer, ATTR_ENABLED, 1);
			//启用保存数据
			SetCtrlAttribute (MainPanelHandle, MAINPANEL_SaveData, ATTR_DIMMED, 0);
			//防止重复点击
			SetCtrlAttribute (MainPanelHandle, MAINPANEL_Acquire, ATTR_DIMMED, 1);
			break;
	}
	return 0;
}
/*设置温度限*/
int CVICALLBACK SetAlarms_Callback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_VAL_CHANGED)
    switch (control)
       {
        case MAINPANEL_UPPER_LIMIT :
            GetCtrlVal  (MainPanelHandle, MAINPANEL_UPPER_LIMIT, &upper_limit); 
			GetCtrlVal (MainPanelHandle, MAINPANEL_LOWER_LIMIT, &lower_limit);
			if (upper_limit<=lower_limit)
				{
					MessagePopup("警告！！！", "请重新设置上限，上限不能低于下限！！！");
					SetCtrlVal (MainPanelHandle, MAINPANEL_UPPER_LIMIT, 40.00);	
				}
			else
				{
	              aidata[0] = upper_limit;
				}	
            break;
        case MAINPANEL_LOWER_LIMIT :
			GetCtrlVal (MainPanelHandle, MAINPANEL_UPPER_LIMIT, &upper_limit);
            GetCtrlVal (MainPanelHandle, MAINPANEL_LOWER_LIMIT, &lower_limit);
			if (lower_limit>=upper_limit)
				{
					MessagePopup("警告！！！", "请重新设置下限，下限不能高于上限！！！");
					
					SetCtrlVal (MainPanelHandle, MAINPANEL_LOWER_LIMIT, 10.00);
					
				}
			else
				{
	              aidata[2] = lower_limit;
				}
            break;
        }
	    return 0;
}
/*退出按钮*/

int CVICALLBACK Quit_Callback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int Waring;
	switch (event)
	{
		case EVENT_COMMIT:
			Waring = ConfirmPopup ("警告", "是否关闭系统？");
			if(Waring)
			{
				DAQmxClearTask (AItaskhandle);
				QuitUserInterface (0);
			}
			else
				break;
			break;
	}
	return 0;
}
/*************************************************************************************/
/*                                                                                   */
/*                              下面回调都有关于菜单栏                                 */
/*                                                                                   */
/*************************************************************************************/
/*默认文件路径选择*/
void CVICALLBACK FilePath_Callback (int menuBar, int menuItem, void *callbackData,
									int panel)
{
	DirSelectPopupEx ("C:\\", "Select Directory", NewFilePath);
}
/*输出文件类型*/
void CVICALLBACK ASCII_O_Callback (int menuBar, int menuItem, void *callbackData,
								   int panel)
{
	fileTypeO = 1;
	MessagePopup("提示", "输出文件类型已改为ASCII");
}

void CVICALLBACK Binary_O_Callback (int menuBar, int menuItem, void *callbackData,
									int panel)
{
	fileTypeO = 0;
	MessagePopup("提示", "输出文件类型已改为Binary");
}
/*输入文件类型*/
void CVICALLBACK ASCII_I_Callback (int menuBar, int menuItem, void *callbackData,
								   int panel)
{
	fileTypeI = 1;
	MessagePopup("提示", "输入文件类型已改为ASCII");
	
}
void CVICALLBACK Binary_I_Callback (int menuBar, int menuItem, void *callbackData,
									int panel)
{
	fileTypeI = 0;
	MessagePopup("提示", "输入文件类型已改为Binary");
}
/*帮助*/
void CVICALLBACK Help_Callback (int menuBar, int menuItem, void *callbackData,
								int panel)
{
	HelpPanelHandle = LoadPanel (0, "帮助.uir", HelpPanel);
    DisplayPanel (HelpPanelHandle);
}
/*帮助界面的回调*/
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
			DiscardPanel (HelpPanelHandle);
			break;
	}
	return 0;
}
/*关于*/
void CVICALLBACK About_Callback (int menuBar, int menuItem, void *callbackData,
								 int panel)
{
     MessagePopup("关于", "Lab windows(CVI)课程设计-YouthGT");	
}






#include "asynctmr.h"
#include <NIDAQmx.h>
#include <formatio.h>
#include "pwctrl.h"
#include <cvirte.h>		
#include <userint.h>
#include "二氧化碳测试系统.h"
#include "主界面.h"
#include "参数设置.h"
#include "数据分析.h"
#include "帮助.h"

static int PassWord;
static int panelHandle;
static int MainPanelHandle;
static int HelpPanelHandle;
static int DataAnalysHandle;
static int SetPanelHandle;
static int Icon_Handle;

static int Co_TabHandle;
static int Te_TabHandle;

int CoAsyncTimer;//定义采集异步定时器
int TeAsyncTimer;//定义采集异步定时器
int DTAsyncTimer;//定义日期时间异步定时器
int TSAsyncTimer;//定义定时保存异步定时器
static TaskHandle Co_taskhandle;//创建
static TaskHandle Te_taskhandle;

double Co_aidata[3]={4000.0,0,2000.0};//默认高低限
double Co_upper_limit =4000,Co_lower_limit =2000;//默认高低限

double Te_aidata[3]={40.0,0,20};//默认高低限
double Te_upper_limit =40,Te_lower_limit =20;//默认高低限

float64 Co_data[1000];
float64 Te_data[1000];

int fileTypeI=1;//默认输入ASCII类型
int fileTypeO=1;//默认输出ASCII类型
char timeString[50],dateString[50];//时间、日期数组
static char NewFilePath[MAX_PATHNAME_LEN]={"f:\\"};//文件夹名称
static char file_name[MAX_PATHNAME_LEN];
int icon_index;//系统托盘目录
int TimingTime;//定时时间

int CVICALLBACK Icon_Callback (int iconHandle, int event, int eventData);
int CVICALLBACK CoAsyncTimer_Callback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2);
int CVICALLBACK TeAsyncTimer_Callback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2);
int CVICALLBACK DTAsyncTimer_Callback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2);
int CVICALLBACK TSAsyncTimer_Callback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2);
int CVICALLBACK Te_SWITCH_Callback (int panel, int control, int event,void *callbackData, int eventData1, int eventData2);
int CVICALLBACK Co_SWITCH_Callback (int panel, int control, int event,void *callbackData, int eventData1, int eventData2);
int CVICALLBACK SaveData_Callback (int panel, int control, int event,void *callbackData, int eventData1, int eventData2);


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "二氧化碳测试系统.uir", PANEL)) < 0)
	    return -1;
	if ((MainPanelHandle = LoadPanel (0, "主界面.uir", MAINPANEL)) < 0)
	    return -1;
	if ((DataAnalysHandle = LoadPanel (0, "数据分析.uir", PANEL)) < 0)
		return -1;
	if ((SetPanelHandle = LoadPanel (0, "参数设置.uir", PANEL)) < 0)
		return -1;
	if ((HelpPanelHandle = LoadPanel (0, "帮助.uir", PANEL)) < 0)
	    return -1;
	GetPanelHandleFromTabPage (SetPanelHandle, SetPanel_Set_TAB, 0, &Co_TabHandle);
	GetPanelHandleFromTabPage (SetPanelHandle, SetPanel_Set_TAB, 1, &Te_TabHandle); 
	PassWord = PasswordCtrl_ConvertFromString (panelHandle, PANEL_PASSWORD);//将输入的字符转换成密码
	PasswordCtrl_SetAttribute(panelHandle,PANEL_PASSWORD,ATTR_PASSWORD_MASK_CHARACTER,42);//让输入的密码显示为*
	DTAsyncTimer = NewAsyncTimer (0.1, -1, 0, DTAsyncTimer_Callback, 0);//DT异步定时器参数设置
	CoAsyncTimer = NewAsyncTimer (0.1, -1, 0, CoAsyncTimer_Callback, 0);//Co异步定时器参数设置
	TeAsyncTimer = NewAsyncTimer (0.1, -1, 0, TeAsyncTimer_Callback, 0);//Te异步定时器参数设置
	DisplayPanel (panelHandle);//显示输入密码面板
	RunUserInterface ();
	DiscardPanel (MainPanelHandle);
	return 0;
}
/*浓度异步定时器*/
int CVICALLBACK CoAsyncTimer_Callback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2)
{
	int32 sampsread;
	int i=0;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			DAQmxReadAnalogF64 (Co_taskhandle, DAQmx_Val_Auto, 10.0, DAQmx_Val_GroupByChannel, Co_data, 1000, &sampsread, 0);	//读取数据
		    Co_data[i]=Co_data[i]*1000; //标度变换
			Co_aidata[1]=Co_data[i];
			PlotStripChart (MainPanelHandle, MAINPANEL_Co_Chart, Co_aidata, 3, 0, 0, VAL_DOUBLE);
			SetCtrlVal (MainPanelHandle, MAINPANEL_CONCENTRATION, Co_aidata[1]);
			//温度警报灯亮灭控制
			if (Co_data[i] >= Co_upper_limit) 
				SetCtrlVal (MainPanelHandle, MAINPANEL_Co_ALARM_HIGH, 1);
   	   	    else
				SetCtrlVal (MainPanelHandle, MAINPANEL_Co_ALARM_HIGH, 0);
			if (Co_data[i] <= Co_lower_limit)
				SetCtrlVal (MainPanelHandle, MAINPANEL_Co_ALARM_LOW, 1);
			else
				SetCtrlVal (MainPanelHandle, MAINPANEL_Co_ALARM_LOW, 0);	
			break;
	}
	return 0;
	
}
/*温度异步定时器*/
int CVICALLBACK TeAsyncTimer_Callback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2)
{

	int32 sampsread;
	int i=0;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			DAQmxReadAnalogF64 (Te_taskhandle, DAQmx_Val_Auto, 10.0, DAQmx_Val_GroupByChannel, Te_data, 1000, &sampsread, 0);	//读取数据
		    Te_data[i]=Te_data[i]*12.5+3; //标度变换
			Te_aidata[1]=Te_data[i];
			PlotStripChart (MainPanelHandle, MAINPANEL_Te_Chart, Te_aidata, 3, 0, 0, VAL_DOUBLE);
			SetCtrlVal (MainPanelHandle, MAINPANEL_TEMP, Te_aidata[1]);
			//温度警报灯亮灭控制
			if (Te_data[i] >= Te_upper_limit) 
				SetCtrlVal (MainPanelHandle, MAINPANEL_Te_ALARM_HIGH, 1);
   	   	    else
				SetCtrlVal (MainPanelHandle, MAINPANEL_Te_ALARM_HIGH, 0);
			if (Te_data[i] <= Te_lower_limit)
				SetCtrlVal (MainPanelHandle, MAINPANEL_Te_ALARM_LOW, 1);
			else
				SetCtrlVal (MainPanelHandle, MAINPANEL_Te_ALARM_LOW, 0);	
			break;
	}
	return 0;
	
}
/*获取日期时间异步定时器*/
int CVICALLBACK DTAsyncTimer_Callback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2)
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
/*定时保存异步定时器*/
int CVICALLBACK TSAsyncTimer_Callback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2)
{

	switch (event)
	{
		case EVENT_TIMER_TICK:
            SaveData_Callback (MainPanelHandle, 0, EVENT_COMMIT,0,0,0);
			break;
	}
	return 0;
}
/*输入密码界面回调*/
int CVICALLBACK Panel_Callback (int panel, int event, void *callbackData,int eventData1, int eventData2)
{	int Waring;
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			Waring = ConfirmPopup ("警告", "是否退出系统？");
			if(Waring)
			{
				QuitUserInterface (0);
			}
			break;
	}
	return 0;
}
/*进入系统按钮*/
int CVICALLBACK EnterSystem_Callback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	static char Pw[100];
    static char TruePw[100]="youthgt";
	//strcmp(str1, str2);   strcmp比较字符串是否相同返回值：
    //如果返回值 < 0，则表示 str1 小于 str2。
    //如果返回值 > 0，则表示 str2 小于 str1。
    //如果返回值 = 0，则表示 str1 等于 str2。
	switch (event)
	{
		case EVENT_COMMIT:
			PasswordCtrl_GetAttribute (panelHandle, PANEL_PASSWORD, ATTR_PASSWORD_VAL, Pw);
			if(!strcmp(TruePw, Pw))

			{	
				SetAsyncTimerAttribute (DTAsyncTimer, ASYNC_ATTR_ENABLED,1);//启动时间显示
				InstallSysTrayIcon ("e:\\Download\\农场_25化肥.ico", "肥料发酵过程二氧化碳测试系统正在运行", Icon_Callback, &Icon_Handle);//安装系统托盘
				AttachTrayIconMenu (Icon_Handle);//添加系统托盘的菜单
				InsertTrayIconMenuItem (Icon_Handle, "退出系统", &icon_index);//添加菜单内容
				InsertTrayIconMenuItem (Icon_Handle, "显示主面板", &icon_index);
				EnableDragAndDrop (MainPanelHandle);//让主面板使能
				DisplayPanel (MainPanelHandle); //显示主面板  
				DiscardPanel (panelHandle);//释放输入密码界面
				
			}
			else
			{
				MessagePopup("密码错误,请检查密码！！！", Pw);
			}

			
			break;
	}
	return 0;
}
//系统托盘回调
int CVICALLBACK Icon_Callback (int iconHandle, int event, int eventData)
{
	switch (event)
		{
			case EVENT_LEFT_DOUBLE_CLICK:
                SetPanelAttribute (MainPanelHandle, ATTR_VISIBLE, 1);
		}
	if (eventData==1)//托盘 显示面板
		{
			SetPanelAttribute (MainPanelHandle, ATTR_VISIBLE, 1);
		}
	else if (eventData==2)//托盘 退出,真正退出系统
		{
                DAQmxClearTask (Co_taskhandle);
				DAQmxClearTask (Te_taskhandle);
				QuitUserInterface (0);
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
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			SetPanelAttribute (MainPanelHandle, ATTR_VISIBLE, 0);//让面板隐藏到托盘
			break;
	}
	return 0;
}

/*停止按钮**/
int CVICALLBACK StopAcquire_Callback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			break;
	}
	return 0;
}


/*定时保存按钮*/
int CVICALLBACK AutoSave_Callback (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		    GetCtrlVal (MainPanelHandle, MAINPANEL_TIMER_mm, &TimingTime);
		    TSAsyncTimer = NewAsyncTimer (TimingTime*10, -1, 0, TSAsyncTimer_Callback, 0);//TS异步定时器参数设置
            SetAsyncTimerAttribute (TSAsyncTimer, ASYNC_ATTR_ENABLED,1);
			MessagePopup ("提示", "定时成功");
			break;
	}
	return 0;
}
/*关闭定时按钮*/
int CVICALLBACK OffTiming_Callback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
            SetAsyncTimerAttribute (TSAsyncTimer, ASYNC_ATTR_ENABLED,0);
			DiscardAsyncTimer (TSAsyncTimer);
			MessagePopup ("提示", "关闭定时成功");	
			break;
	}
	return 0;
}

/*保存按钮*/
int CVICALLBACK SaveData_Callback (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	char TempFilePath[MAX_PATHNAME_LEN];//定义临时路径
	char CoTempFilePath[MAX_PATHNAME_LEN];
	char TeTempFilePath[MAX_PATHNAME_LEN];
	memset(TempFilePath, 0, sizeof TempFilePath);//清空临时路径
	switch (event)
	{
		case EVENT_COMMIT:
			strcpy(TempFilePath,NewFilePath);//将获得的新路径传递给定义临时路径
			strcat(TempFilePath,"//");//给路径加分隔线
			strcpy(CoTempFilePath,strcat(TempFilePath,"Co"));
			strcpy(TeTempFilePath,strcat(TempFilePath,"Te"));
            strcat(CoTempFilePath,strcat(strcat(dateString, timeString),".txt"));//拼接成完整路径
			strcat(TeTempFilePath,strcat(strcat(dateString, timeString),".txt"));//拼接成完整路径
			ArrayToFile (CoTempFilePath,Co_data, VAL_DOUBLE, 3, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 10, fileTypeO, VAL_TRUNCATE);
			ArrayToFile (TeTempFilePath,Te_data, VAL_DOUBLE, 3, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 10, fileTypeO, VAL_TRUNCATE);
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
			ClearStripChart (MainPanelHandle, MAINPANEL_Co_Chart );
			ClearStripChart (MainPanelHandle, MAINPANEL_Te_Chart );
			break;
	}
	return 0;
}
/*浓度采集switch*/
int CVICALLBACK Co_SWITCH_Callback (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	int Co_AIrate;
    char Co_AIchanel[256]="Dev1/ai0";
	int statues;
	switch (event)
	{
		case EVENT_COMMIT:
		GetCtrlVal (MainPanelHandle, MAINPANEL_Co_SWITCH, &statues);
		if(statues==1)
			{
				GetCtrlVal (Co_TabHandle, TABPANEL_Co_AIcontrol, Co_AIchanel);// 从界面得到模拟输入通道
				GetCtrlVal (Co_TabHandle, TABPANEL_Co_AIrate, &Co_AIrate); // 从界面得到点数
				DAQmxCreateTask ("", &Co_taskhandle); // 创建模拟输入任务
				//创建模拟输入通道
				DAQmxCreateAIVoltageChan (Co_taskhandle, Co_AIchanel, "Voltage", DAQmx_Val_RSE, -5.0, 5, DAQmx_Val_Volts, "");
				// 设置采样率
				DAQmxCfgSampClkTiming (Co_taskhandle, "", Co_AIrate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 100);
				//启用aitimer，弃用同步定时器改用异步定时器
				//SetCtrlAttribute (MainPanelHandle, AsyncTimerHandle, ATTR_ENABLED, 1);
				SetAsyncTimerAttribute (CoAsyncTimer, ASYNC_ATTR_ENABLED,1);
				//启用保存
				SetCtrlAttribute (MainPanelHandle, MAINPANEL_SaveData, ATTR_DIMMED, 0);
			}
		else
			{
			 SetAsyncTimerAttribute (CoAsyncTimer, ASYNC_ATTR_ENABLED,0);//停止AI采集异步定时器	
 			 DAQmxClearTask (Co_taskhandle);//清除任务
			}
			break;
	}
	return 0;
}
/*温度采集switch*/
int CVICALLBACK Te_SWITCH_Callback (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	int Te_AIrate;
    char Te_AIcontrol[256];
	int statues;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (MainPanelHandle, MAINPANEL_Te_SWITCH, &statues);
			if(statues==1)
			{				
				GetCtrlVal (Te_TabHandle, TABPANEL_2_Te_AIcontrol, Te_AIcontrol);// 从界面得到模拟输入通道
				GetCtrlVal (Te_TabHandle, TABPANEL_2_Te_AIrate, &Te_AIrate); // 从界面得到点数
				DAQmxCreateTask ("Tetask", &Te_taskhandle); // 创建模拟输入任务
				//创建模拟输入通道
				DAQmxCreateAIVoltageChan (Te_taskhandle, Te_AIcontrol, "Voltage", DAQmx_Val_RSE, -5.0, 5, DAQmx_Val_Volts, "");
				// 设置采样率
				DAQmxCfgSampClkTiming (Te_taskhandle, "", Te_AIrate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 100);
				//启用aitimer，弃用同步定时器改用异步定时器
				//SetCtrlAttribute (MainPanelHandle, AsyncTimerHandle, ATTR_ENABLED, 1);
				SetAsyncTimerAttribute (TeAsyncTimer, ASYNC_ATTR_ENABLED,1);
				//启用保存
				SetCtrlAttribute (MainPanelHandle, MAINPANEL_SaveData, ATTR_DIMMED, 0);
			}
			else
			{
			 SetAsyncTimerAttribute (TeAsyncTimer, ASYNC_ATTR_ENABLED,0);//停止AI采集异步定时器	
 			 DAQmxClearTask (Te_taskhandle);//清除任务
			}
			break;
	}
	return 0;
}
//温度浓度同时开始采集
int CVICALLBACK Co_Te_Start_Callback (int panel, int control, int event,
									  void *callbackData, int eventData1, int eventData2)
{
	int Co_statue;
	int Co_AIrate;
    char Co_AIchanel[256]="Dev1/ai0";
	int Te_statue;
	int Te_AIrate;
	char Te_AIcontrol[256]="Dev2/ai0";
	switch (event)
	{
		case EVENT_COMMIT:
		GetCtrlVal (MainPanelHandle, MAINPANEL_Co_SWITCH, &Co_statue);
		GetCtrlVal (MainPanelHandle, MAINPANEL_Te_SWITCH, &Te_statue);
           if (!Te_statue&&!Co_statue)
		   {
				GetCtrlVal (Co_TabHandle, TABPANEL_Co_AIcontrol, Co_AIchanel);// 从界面得到模拟输入通道
				GetCtrlVal (Co_TabHandle, TABPANEL_Co_AIrate, &Co_AIrate); // 从界面得到点数
				DAQmxCreateTask ("", &Co_taskhandle); // 创建模拟输入任务
				//创建模拟输入通道
				DAQmxCreateAIVoltageChan (Co_taskhandle, Co_AIchanel, "Voltage", DAQmx_Val_RSE, -5.0, 5, DAQmx_Val_Volts, "");
				// 设置采样率
				DAQmxCfgSampClkTiming (Co_taskhandle, "", Co_AIrate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 100);
				//启用aitimer，弃用同步定时器改用异步定时器
				//SetCtrlAttribute (MainPanelHandle, AsyncTimerHandle, ATTR_ENABLED, 1);
				SetAsyncTimerAttribute (CoAsyncTimer, ASYNC_ATTR_ENABLED,1);			   
			    SetCtrlVal (MainPanelHandle, MAINPANEL_Te_SWITCH, 1);
			    SetCtrlVal (MainPanelHandle, MAINPANEL_Co_SWITCH, 1);
		   	    GetCtrlVal (Te_TabHandle, TABPANEL_2_Te_AIcontrol, Te_AIcontrol);// 从界面得到模拟输入通道
				GetCtrlVal (Te_TabHandle, TABPANEL_2_Te_AIrate, &Te_AIrate); // 从界面得到点数
				DAQmxCreateTask ("Tetask", &Te_taskhandle); // 创建模拟输入任务
				//创建模拟输入通道
				DAQmxCreateAIVoltageChan (Te_taskhandle, Te_AIcontrol, "Voltage", DAQmx_Val_RSE, -5.0, 5, DAQmx_Val_Volts, "");
				// 设置采样率
				DAQmxCfgSampClkTiming (Te_taskhandle, "", Te_AIrate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 100);
				//启用aitimer，弃用同步定时器改用异步定时器
				//SetCtrlAttribute (MainPanelHandle, AsyncTimerHandle, ATTR_ENABLED, 1);
				SetAsyncTimerAttribute (TeAsyncTimer, ASYNC_ATTR_ENABLED,1);
				//启用保存
				SetCtrlAttribute (MainPanelHandle, MAINPANEL_SaveData, ATTR_DIMMED, 0);
		   } 
        else if(!Te_statue&&Co_statue)
			{
				MessagePopup("警告！！！", "请先将浓度实时采集关闭");
			}
		else if(Te_statue&&!Co_statue)
			{
				MessagePopup("警告！！！", "请先将温度实时采集关闭");
			}	   				
			break;
	}
	return 0;
}
//参数设置按钮
int CVICALLBACK ParameterSet_Callback (int panel, int control, int event,
									   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
            DisplayPanel (SetPanelHandle);
			break;
	}
	return 0;
}

/*退出按钮*/

int CVICALLBACK Quit_Callback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//并非真正退出系统，进入托盘
			SetPanelAttribute (MainPanelHandle, ATTR_VISIBLE, 0);
			break;
	}
	return 0;
}
/*************************************************************************************/
/*                                                                                   */
/*                              下面回调有关于数据分析                                 */
/*                                                                                   */
/*************************************************************************************/
int CVICALLBACK DataAnalysis_Callback (int panel, int event, void *callbackData,
									int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			HidePanel (DataAnalysHandle);
			break;
	}
	return 0;
}

int CVICALLBACK LoadFile_Callback (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
   char Co_In[1000];
   char Te_In[1000];
   switch (event)
	{
		case EVENT_COMMIT:
			 if (FileSelectPopupEx (NewFilePath, "*.txt", "*.txt", "选择要打开的文件", VAL_OK_BUTTON, 0, 1, file_name) > 0)
				 {
					
						FileToArray (file_name, Co_In, VAL_DOUBLE, 3, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, fileTypeI);
						//DeleteGraphPlot (DataAnalysHandle, DataAnalys_History_Co_Chart, -1, VAL_IMMEDIATE_DRAW);
			           // PlotY (DataAnalysHandle, DataAnalys_History_Co_Chart, Co_In, CK, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_GREEN);
						
						
						//FileToArray (file_name, Co_In, VAL_DOUBLE, 3, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, fileTypeI);
						//SetCtrlVal (MainPanelHandle, MAINPANEL_FileAnalysis,file_name);//显示文件路径				
 
					  //SetCtrlVal (MainPanelHandle, MAINPANEL_LED, 1);
				      MessagePopup ("载入文件", "载入成功！");
				 }
	}
	return 0;
}



/*************************************************************************************/
/*                                                                                   */
/*                              下面回调有关于参数设置                                 */
/*                                                                                   */
/*************************************************************************************/
/*设置面板回调*/
int CVICALLBACK SetPanel_Callback (int panel, int event, void *callbackData,
									int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			HidePanel (SetPanelHandle);
			break;
	}
	return 0;
}
/*上下限设置*/
int CVICALLBACK Co_SetAlarms_Callback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_VAL_CHANGED)
    switch (control)
       {		   
/*设置浓度限*/
        case TABPANEL_Co_UPPER_LIMIT :
            GetCtrlVal  (Co_TabHandle, TABPANEL_Co_UPPER_LIMIT, &Co_upper_limit); 
			GetCtrlVal (Co_TabHandle, TABPANEL_Co_LOWER_LIMIT, &Co_lower_limit);
			if (Co_upper_limit<=Co_lower_limit)
				{
					MessagePopup("警告！！！", "请重新设置上限，上限不能低于下限！！！");
					SetCtrlVal (Co_TabHandle, TABPANEL_Co_UPPER_LIMIT, 5000.00);	
				}
			else
				{
	              Co_aidata[0] = Co_upper_limit;
				}	
            break;
        case TABPANEL_Co_LOWER_LIMIT :
			GetCtrlVal (Co_TabHandle, TABPANEL_Co_UPPER_LIMIT, &Co_upper_limit);
            GetCtrlVal (Co_TabHandle, TABPANEL_Co_LOWER_LIMIT, &Co_lower_limit);
			if (Co_lower_limit>=Co_upper_limit)
				{
					MessagePopup("警告！！！", "请重新设置下限，下限不能高于上限！！！");
					
					SetCtrlVal (Co_TabHandle, TABPANEL_Co_LOWER_LIMIT, 2000.00);
					
				}
			else
				{
	              Co_aidata[2] =Co_lower_limit;
				}
            break;
        }
	    return 0;
}
int CVICALLBACK Te_SetAlarms_Callback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
  { 
	 if (event == EVENT_VAL_CHANGED)
	 switch (control)
       {
/*设置温度限*/	
	    case TABPANEL_2_Te_UPPER_LIMIT :
            GetCtrlVal  (Te_TabHandle, TABPANEL_2_Te_UPPER_LIMIT, &Te_upper_limit); 
			GetCtrlVal (Te_TabHandle, TABPANEL_2_Te_LOWER_LIMIT, &Te_lower_limit);
			if (Te_upper_limit<=Te_lower_limit)
				{
					MessagePopup("警告！！！", "请重新设置上限，上限不能低于下限！！！");
					SetCtrlVal (Te_TabHandle, TABPANEL_2_Te_UPPER_LIMIT, 50.00);	
				}
			else
				{
	              Te_aidata[0] = Te_upper_limit;
				}	
            break;
        case TABPANEL_2_Te_LOWER_LIMIT :
			GetCtrlVal (Te_TabHandle, TABPANEL_2_Te_UPPER_LIMIT , &Te_upper_limit);
            GetCtrlVal (Te_TabHandle, TABPANEL_2_Te_LOWER_LIMIT , &Te_lower_limit);
			if (Te_lower_limit>=Te_upper_limit)
				{
					MessagePopup("警告！！！", "请重新设置下限，下限不能高于上限！！！");
					
					SetCtrlVal (Te_TabHandle, TABPANEL_2_Te_LOWER_LIMIT , 10.00);
					
				}
			else
				{
	              Te_aidata[2] = Te_lower_limit;
				}
            break;	
			
			
        }
	    return 0;
}
/*************************************************************************************/
/*                                                                                   */
/*                              下面回调都有关于帮助面板                               */
/*                                                                                   */
/*************************************************************************************/
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
			HidePanel (HelpPanelHandle);
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
	DirSelectPopupEx ("C:\\", "默认文件路径选择", NewFilePath);
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

/*数据分析*/
void CVICALLBACK DataAys_Callback (int menuBar, int menuItem, void *callbackData,
								   int panel)
{
	DisplayPanel (DataAnalysHandle);
}
/*截图*/
void CVICALLBACK ScreenShot_Callback (int menuBar, int menuItem, void *callbackData,
									  int panel)
{


}
/*帮助*/
void CVICALLBACK Help_Callback (int menuBar, int menuItem, void *callbackData,
								int panel)
{
    DisplayPanel (HelpPanelHandle);
}

/*关于*/
void CVICALLBACK About_Callback (int menuBar, int menuItem, void *callbackData,
								 int panel)
{
     MessagePopup("关于", "Lab windows(CVI)课程设计-YouthGT");	
}

 







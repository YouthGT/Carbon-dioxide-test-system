#include "asynctmr.h"
#include <NIDAQmx.h>
#include <formatio.h>
#include "pwctrl.h"
#include <cvirte.h>		
#include <userint.h>
#include "������̼����ϵͳ.h"
#include "������.h"
#include "��������.h"
#include "���ݷ���.h"
#include "����.h"

static int PassWord;
static int panelHandle;
static int MainPanelHandle;
static int HelpPanelHandle;
static int DataAnalysHandle;
static int SetPanelHandle;
static int Icon_Handle;

static int Co_TabHandle;
static int Te_TabHandle;

int CoAsyncTimer;//����ɼ��첽��ʱ��
int TeAsyncTimer;//����ɼ��첽��ʱ��
int DTAsyncTimer;//��������ʱ���첽��ʱ��
int TSAsyncTimer;//���嶨ʱ�����첽��ʱ��
static TaskHandle Co_taskhandle;//����
static TaskHandle Te_taskhandle;

double Co_aidata[3]={4000.0,0,2000.0};//Ĭ�ϸߵ���
double Co_upper_limit =4000,Co_lower_limit =2000;//Ĭ�ϸߵ���

double Te_aidata[3]={40.0,0,20};//Ĭ�ϸߵ���
double Te_upper_limit =40,Te_lower_limit =20;//Ĭ�ϸߵ���

float64 Co_data[1000];
float64 Te_data[1000];

int fileTypeI=1;//Ĭ������ASCII����
int fileTypeO=1;//Ĭ�����ASCII����
char timeString[50],dateString[50];//ʱ�䡢��������
static char NewFilePath[MAX_PATHNAME_LEN]={"f:\\"};//�ļ�������
static char file_name[MAX_PATHNAME_LEN];
int icon_index;//ϵͳ����Ŀ¼
int TimingTime;//��ʱʱ��

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
	if ((panelHandle = LoadPanel (0, "������̼����ϵͳ.uir", PANEL)) < 0)
	    return -1;
	if ((MainPanelHandle = LoadPanel (0, "������.uir", MAINPANEL)) < 0)
	    return -1;
	if ((DataAnalysHandle = LoadPanel (0, "���ݷ���.uir", PANEL)) < 0)
		return -1;
	if ((SetPanelHandle = LoadPanel (0, "��������.uir", PANEL)) < 0)
		return -1;
	if ((HelpPanelHandle = LoadPanel (0, "����.uir", PANEL)) < 0)
	    return -1;
	GetPanelHandleFromTabPage (SetPanelHandle, SetPanel_Set_TAB, 0, &Co_TabHandle);
	GetPanelHandleFromTabPage (SetPanelHandle, SetPanel_Set_TAB, 1, &Te_TabHandle); 
	PassWord = PasswordCtrl_ConvertFromString (panelHandle, PANEL_PASSWORD);//��������ַ�ת��������
	PasswordCtrl_SetAttribute(panelHandle,PANEL_PASSWORD,ATTR_PASSWORD_MASK_CHARACTER,42);//�������������ʾΪ*
	DTAsyncTimer = NewAsyncTimer (0.1, -1, 0, DTAsyncTimer_Callback, 0);//DT�첽��ʱ����������
	CoAsyncTimer = NewAsyncTimer (0.1, -1, 0, CoAsyncTimer_Callback, 0);//Co�첽��ʱ����������
	TeAsyncTimer = NewAsyncTimer (0.1, -1, 0, TeAsyncTimer_Callback, 0);//Te�첽��ʱ����������
	DisplayPanel (panelHandle);//��ʾ�����������
	RunUserInterface ();
	DiscardPanel (MainPanelHandle);
	return 0;
}
/*Ũ���첽��ʱ��*/
int CVICALLBACK CoAsyncTimer_Callback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2)
{
	int32 sampsread;
	int i=0;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			DAQmxReadAnalogF64 (Co_taskhandle, DAQmx_Val_Auto, 10.0, DAQmx_Val_GroupByChannel, Co_data, 1000, &sampsread, 0);	//��ȡ����
		    Co_data[i]=Co_data[i]*1000; //��ȱ任
			Co_aidata[1]=Co_data[i];
			PlotStripChart (MainPanelHandle, MAINPANEL_Co_Chart, Co_aidata, 3, 0, 0, VAL_DOUBLE);
			SetCtrlVal (MainPanelHandle, MAINPANEL_CONCENTRATION, Co_aidata[1]);
			//�¶Ⱦ������������
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
/*�¶��첽��ʱ��*/
int CVICALLBACK TeAsyncTimer_Callback (int reserved, int timerId, int event, void *callbackData, int eventData1, int eventData2)
{

	int32 sampsread;
	int i=0;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			DAQmxReadAnalogF64 (Te_taskhandle, DAQmx_Val_Auto, 10.0, DAQmx_Val_GroupByChannel, Te_data, 1000, &sampsread, 0);	//��ȡ����
		    Te_data[i]=Te_data[i]*12.5+3; //��ȱ任
			Te_aidata[1]=Te_data[i];
			PlotStripChart (MainPanelHandle, MAINPANEL_Te_Chart, Te_aidata, 3, 0, 0, VAL_DOUBLE);
			SetCtrlVal (MainPanelHandle, MAINPANEL_TEMP, Te_aidata[1]);
			//�¶Ⱦ������������
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
/*��ȡ����ʱ���첽��ʱ��*/
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
        Fmt (dateString, "%d��%d��%d��",Year,Month,Day); 
		Fmt (timeString, "%dʱ%d��%d��",Hours,Minutes,Seconds);
		SetCtrlVal (MainPanelHandle, MAINPANEL_Date, dateString);
		SetCtrlVal (MainPanelHandle, MAINPANEL_Time, timeString);
			break;
	}
	return 0;
}
/*��ʱ�����첽��ʱ��*/
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
/*�����������ص�*/
int CVICALLBACK Panel_Callback (int panel, int event, void *callbackData,int eventData1, int eventData2)
{	int Waring;
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
			Waring = ConfirmPopup ("����", "�Ƿ��˳�ϵͳ��");
			if(Waring)
			{
				QuitUserInterface (0);
			}
			break;
	}
	return 0;
}
/*����ϵͳ��ť*/
int CVICALLBACK EnterSystem_Callback (int panel, int control, int event, void *callbackData, int eventData1, int eventData2)
{
	static char Pw[100];
    static char TruePw[100]="youthgt";
	//strcmp(str1, str2);   strcmp�Ƚ��ַ����Ƿ���ͬ����ֵ��
    //�������ֵ < 0�����ʾ str1 С�� str2��
    //�������ֵ > 0�����ʾ str2 С�� str1��
    //�������ֵ = 0�����ʾ str1 ���� str2��
	switch (event)
	{
		case EVENT_COMMIT:
			PasswordCtrl_GetAttribute (panelHandle, PANEL_PASSWORD, ATTR_PASSWORD_VAL, Pw);
			if(!strcmp(TruePw, Pw))

			{	
				SetAsyncTimerAttribute (DTAsyncTimer, ASYNC_ATTR_ENABLED,1);//����ʱ����ʾ
				InstallSysTrayIcon ("e:\\Download\\ũ��_25����.ico", "���Ϸ��͹��̶�����̼����ϵͳ��������", Icon_Callback, &Icon_Handle);//��װϵͳ����
				AttachTrayIconMenu (Icon_Handle);//���ϵͳ���̵Ĳ˵�
				InsertTrayIconMenuItem (Icon_Handle, "�˳�ϵͳ", &icon_index);//��Ӳ˵�����
				InsertTrayIconMenuItem (Icon_Handle, "��ʾ�����", &icon_index);
				EnableDragAndDrop (MainPanelHandle);//�������ʹ��
				DisplayPanel (MainPanelHandle); //��ʾ�����  
				DiscardPanel (panelHandle);//�ͷ������������
				
			}
			else
			{
				MessagePopup("�������,�������룡����", Pw);
			}

			
			break;
	}
	return 0;
}
//ϵͳ���̻ص�
int CVICALLBACK Icon_Callback (int iconHandle, int event, int eventData)
{
	switch (event)
		{
			case EVENT_LEFT_DOUBLE_CLICK:
                SetPanelAttribute (MainPanelHandle, ATTR_VISIBLE, 1);
		}
	if (eventData==1)//���� ��ʾ���
		{
			SetPanelAttribute (MainPanelHandle, ATTR_VISIBLE, 1);
		}
	else if (eventData==2)//���� �˳�,�����˳�ϵͳ
		{
                DAQmxClearTask (Co_taskhandle);
				DAQmxClearTask (Te_taskhandle);
				QuitUserInterface (0);
		}	
	return 0;
}



/*************************************************************************************/
/*                                                                                   */
/*                              ����ص����й���������                                 */
/*                                                                                   */
/*************************************************************************************/

/*������ص�*/
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
			SetPanelAttribute (MainPanelHandle, ATTR_VISIBLE, 0);//��������ص�����
			break;
	}
	return 0;
}

/*ֹͣ��ť**/
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


/*��ʱ���水ť*/
int CVICALLBACK AutoSave_Callback (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		    GetCtrlVal (MainPanelHandle, MAINPANEL_TIMER_mm, &TimingTime);
		    TSAsyncTimer = NewAsyncTimer (TimingTime*10, -1, 0, TSAsyncTimer_Callback, 0);//TS�첽��ʱ����������
            SetAsyncTimerAttribute (TSAsyncTimer, ASYNC_ATTR_ENABLED,1);
			MessagePopup ("��ʾ", "��ʱ�ɹ�");
			break;
	}
	return 0;
}
/*�رն�ʱ��ť*/
int CVICALLBACK OffTiming_Callback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
            SetAsyncTimerAttribute (TSAsyncTimer, ASYNC_ATTR_ENABLED,0);
			DiscardAsyncTimer (TSAsyncTimer);
			MessagePopup ("��ʾ", "�رն�ʱ�ɹ�");	
			break;
	}
	return 0;
}

/*���水ť*/
int CVICALLBACK SaveData_Callback (int panel, int control, int event,void *callbackData, int eventData1, int eventData2)
{
	char TempFilePath[MAX_PATHNAME_LEN];//������ʱ·��
	char CoTempFilePath[MAX_PATHNAME_LEN];
	char TeTempFilePath[MAX_PATHNAME_LEN];
	memset(TempFilePath, 0, sizeof TempFilePath);//�����ʱ·��
	switch (event)
	{
		case EVENT_COMMIT:
			strcpy(TempFilePath,NewFilePath);//����õ���·�����ݸ�������ʱ·��
			strcat(TempFilePath,"//");//��·���ӷָ���
			strcpy(CoTempFilePath,strcat(TempFilePath,"Co"));
			strcpy(TeTempFilePath,strcat(TempFilePath,"Te"));
            strcat(CoTempFilePath,strcat(strcat(dateString, timeString),".txt"));//ƴ�ӳ�����·��
			strcat(TeTempFilePath,strcat(strcat(dateString, timeString),".txt"));//ƴ�ӳ�����·��
			ArrayToFile (CoTempFilePath,Co_data, VAL_DOUBLE, 3, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 10, fileTypeO, VAL_TRUNCATE);
			ArrayToFile (TeTempFilePath,Te_data, VAL_DOUBLE, 3, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 10, fileTypeO, VAL_TRUNCATE);
			MessagePopup ("�����ļ�", "����ɹ���");  
			break;
	}
	return 0;
}
/*���ͼ��ť*/
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
/*Ũ�Ȳɼ�switch*/
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
				GetCtrlVal (Co_TabHandle, TABPANEL_Co_AIcontrol, Co_AIchanel);// �ӽ���õ�ģ������ͨ��
				GetCtrlVal (Co_TabHandle, TABPANEL_Co_AIrate, &Co_AIrate); // �ӽ���õ�����
				DAQmxCreateTask ("", &Co_taskhandle); // ����ģ����������
				//����ģ������ͨ��
				DAQmxCreateAIVoltageChan (Co_taskhandle, Co_AIchanel, "Voltage", DAQmx_Val_RSE, -5.0, 5, DAQmx_Val_Volts, "");
				// ���ò�����
				DAQmxCfgSampClkTiming (Co_taskhandle, "", Co_AIrate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 100);
				//����aitimer������ͬ����ʱ�������첽��ʱ��
				//SetCtrlAttribute (MainPanelHandle, AsyncTimerHandle, ATTR_ENABLED, 1);
				SetAsyncTimerAttribute (CoAsyncTimer, ASYNC_ATTR_ENABLED,1);
				//���ñ���
				SetCtrlAttribute (MainPanelHandle, MAINPANEL_SaveData, ATTR_DIMMED, 0);
			}
		else
			{
			 SetAsyncTimerAttribute (CoAsyncTimer, ASYNC_ATTR_ENABLED,0);//ֹͣAI�ɼ��첽��ʱ��	
 			 DAQmxClearTask (Co_taskhandle);//�������
			}
			break;
	}
	return 0;
}
/*�¶Ȳɼ�switch*/
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
				GetCtrlVal (Te_TabHandle, TABPANEL_2_Te_AIcontrol, Te_AIcontrol);// �ӽ���õ�ģ������ͨ��
				GetCtrlVal (Te_TabHandle, TABPANEL_2_Te_AIrate, &Te_AIrate); // �ӽ���õ�����
				DAQmxCreateTask ("Tetask", &Te_taskhandle); // ����ģ����������
				//����ģ������ͨ��
				DAQmxCreateAIVoltageChan (Te_taskhandle, Te_AIcontrol, "Voltage", DAQmx_Val_RSE, -5.0, 5, DAQmx_Val_Volts, "");
				// ���ò�����
				DAQmxCfgSampClkTiming (Te_taskhandle, "", Te_AIrate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 100);
				//����aitimer������ͬ����ʱ�������첽��ʱ��
				//SetCtrlAttribute (MainPanelHandle, AsyncTimerHandle, ATTR_ENABLED, 1);
				SetAsyncTimerAttribute (TeAsyncTimer, ASYNC_ATTR_ENABLED,1);
				//���ñ���
				SetCtrlAttribute (MainPanelHandle, MAINPANEL_SaveData, ATTR_DIMMED, 0);
			}
			else
			{
			 SetAsyncTimerAttribute (TeAsyncTimer, ASYNC_ATTR_ENABLED,0);//ֹͣAI�ɼ��첽��ʱ��	
 			 DAQmxClearTask (Te_taskhandle);//�������
			}
			break;
	}
	return 0;
}
//�¶�Ũ��ͬʱ��ʼ�ɼ�
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
				GetCtrlVal (Co_TabHandle, TABPANEL_Co_AIcontrol, Co_AIchanel);// �ӽ���õ�ģ������ͨ��
				GetCtrlVal (Co_TabHandle, TABPANEL_Co_AIrate, &Co_AIrate); // �ӽ���õ�����
				DAQmxCreateTask ("", &Co_taskhandle); // ����ģ����������
				//����ģ������ͨ��
				DAQmxCreateAIVoltageChan (Co_taskhandle, Co_AIchanel, "Voltage", DAQmx_Val_RSE, -5.0, 5, DAQmx_Val_Volts, "");
				// ���ò�����
				DAQmxCfgSampClkTiming (Co_taskhandle, "", Co_AIrate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 100);
				//����aitimer������ͬ����ʱ�������첽��ʱ��
				//SetCtrlAttribute (MainPanelHandle, AsyncTimerHandle, ATTR_ENABLED, 1);
				SetAsyncTimerAttribute (CoAsyncTimer, ASYNC_ATTR_ENABLED,1);			   
			    SetCtrlVal (MainPanelHandle, MAINPANEL_Te_SWITCH, 1);
			    SetCtrlVal (MainPanelHandle, MAINPANEL_Co_SWITCH, 1);
		   	    GetCtrlVal (Te_TabHandle, TABPANEL_2_Te_AIcontrol, Te_AIcontrol);// �ӽ���õ�ģ������ͨ��
				GetCtrlVal (Te_TabHandle, TABPANEL_2_Te_AIrate, &Te_AIrate); // �ӽ���õ�����
				DAQmxCreateTask ("Tetask", &Te_taskhandle); // ����ģ����������
				//����ģ������ͨ��
				DAQmxCreateAIVoltageChan (Te_taskhandle, Te_AIcontrol, "Voltage", DAQmx_Val_RSE, -5.0, 5, DAQmx_Val_Volts, "");
				// ���ò�����
				DAQmxCfgSampClkTiming (Te_taskhandle, "", Te_AIrate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 100);
				//����aitimer������ͬ����ʱ�������첽��ʱ��
				//SetCtrlAttribute (MainPanelHandle, AsyncTimerHandle, ATTR_ENABLED, 1);
				SetAsyncTimerAttribute (TeAsyncTimer, ASYNC_ATTR_ENABLED,1);
				//���ñ���
				SetCtrlAttribute (MainPanelHandle, MAINPANEL_SaveData, ATTR_DIMMED, 0);
		   } 
        else if(!Te_statue&&Co_statue)
			{
				MessagePopup("���棡����", "���Ƚ�Ũ��ʵʱ�ɼ��ر�");
			}
		else if(Te_statue&&!Co_statue)
			{
				MessagePopup("���棡����", "���Ƚ��¶�ʵʱ�ɼ��ر�");
			}	   				
			break;
	}
	return 0;
}
//�������ð�ť
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

/*�˳���ť*/

int CVICALLBACK Quit_Callback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//���������˳�ϵͳ����������
			SetPanelAttribute (MainPanelHandle, ATTR_VISIBLE, 0);
			break;
	}
	return 0;
}
/*************************************************************************************/
/*                                                                                   */
/*                              ����ص��й������ݷ���                                 */
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
			 if (FileSelectPopupEx (NewFilePath, "*.txt", "*.txt", "ѡ��Ҫ�򿪵��ļ�", VAL_OK_BUTTON, 0, 1, file_name) > 0)
				 {
					
						FileToArray (file_name, Co_In, VAL_DOUBLE, 3, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, fileTypeI);
						//DeleteGraphPlot (DataAnalysHandle, DataAnalys_History_Co_Chart, -1, VAL_IMMEDIATE_DRAW);
			           // PlotY (DataAnalysHandle, DataAnalys_History_Co_Chart, Co_In, CK, VAL_DOUBLE, VAL_THIN_LINE, VAL_EMPTY_SQUARE, VAL_SOLID, 1, VAL_GREEN);
						
						
						//FileToArray (file_name, Co_In, VAL_DOUBLE, 3, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, fileTypeI);
						//SetCtrlVal (MainPanelHandle, MAINPANEL_FileAnalysis,file_name);//��ʾ�ļ�·��				
 
					  //SetCtrlVal (MainPanelHandle, MAINPANEL_LED, 1);
				      MessagePopup ("�����ļ�", "����ɹ���");
				 }
	}
	return 0;
}



/*************************************************************************************/
/*                                                                                   */
/*                              ����ص��й��ڲ�������                                 */
/*                                                                                   */
/*************************************************************************************/
/*�������ص�*/
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
/*����������*/
int CVICALLBACK Co_SetAlarms_Callback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
    if (event == EVENT_VAL_CHANGED)
    switch (control)
       {		   
/*����Ũ����*/
        case TABPANEL_Co_UPPER_LIMIT :
            GetCtrlVal  (Co_TabHandle, TABPANEL_Co_UPPER_LIMIT, &Co_upper_limit); 
			GetCtrlVal (Co_TabHandle, TABPANEL_Co_LOWER_LIMIT, &Co_lower_limit);
			if (Co_upper_limit<=Co_lower_limit)
				{
					MessagePopup("���棡����", "�������������ޣ����޲��ܵ������ޣ�����");
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
					MessagePopup("���棡����", "�������������ޣ����޲��ܸ������ޣ�����");
					
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
/*�����¶���*/	
	    case TABPANEL_2_Te_UPPER_LIMIT :
            GetCtrlVal  (Te_TabHandle, TABPANEL_2_Te_UPPER_LIMIT, &Te_upper_limit); 
			GetCtrlVal (Te_TabHandle, TABPANEL_2_Te_LOWER_LIMIT, &Te_lower_limit);
			if (Te_upper_limit<=Te_lower_limit)
				{
					MessagePopup("���棡����", "�������������ޣ����޲��ܵ������ޣ�����");
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
					MessagePopup("���棡����", "�������������ޣ����޲��ܸ������ޣ�����");
					
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
/*                              ����ص����й��ڰ������                               */
/*                                                                                   */
/*************************************************************************************/
/*��������Ļص�*/
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
/*                              ����ص����й��ڲ˵���                                 */
/*                                                                                   */
/*************************************************************************************/
/*Ĭ���ļ�·��ѡ��*/
void CVICALLBACK FilePath_Callback (int menuBar, int menuItem, void *callbackData,
									int panel)
{
	DirSelectPopupEx ("C:\\", "Ĭ���ļ�·��ѡ��", NewFilePath);
}
/*����ļ�����*/
void CVICALLBACK ASCII_O_Callback (int menuBar, int menuItem, void *callbackData,
								   int panel)
{
	fileTypeO = 1;
	MessagePopup("��ʾ", "����ļ������Ѹ�ΪASCII");
}

void CVICALLBACK Binary_O_Callback (int menuBar, int menuItem, void *callbackData,
									int panel)
{
	fileTypeO = 0;
	MessagePopup("��ʾ", "����ļ������Ѹ�ΪBinary");
}
/*�����ļ�����*/
void CVICALLBACK ASCII_I_Callback (int menuBar, int menuItem, void *callbackData,
								   int panel)
{
	fileTypeI = 1;
	MessagePopup("��ʾ", "�����ļ������Ѹ�ΪASCII");
	
}
void CVICALLBACK Binary_I_Callback (int menuBar, int menuItem, void *callbackData,
									int panel)
{
	fileTypeI = 0;
	MessagePopup("��ʾ", "�����ļ������Ѹ�ΪBinary");
}

/*���ݷ���*/
void CVICALLBACK DataAys_Callback (int menuBar, int menuItem, void *callbackData,
								   int panel)
{
	DisplayPanel (DataAnalysHandle);
}
/*��ͼ*/
void CVICALLBACK ScreenShot_Callback (int menuBar, int menuItem, void *callbackData,
									  int panel)
{


}
/*����*/
void CVICALLBACK Help_Callback (int menuBar, int menuItem, void *callbackData,
								int panel)
{
    DisplayPanel (HelpPanelHandle);
}

/*����*/
void CVICALLBACK About_Callback (int menuBar, int menuItem, void *callbackData,
								 int panel)
{
     MessagePopup("����", "Lab windows(CVI)�γ����-YouthGT");	
}

 







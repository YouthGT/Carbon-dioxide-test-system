#include <NIDAQmx.h>
#include <formatio.h>
#include "pwctrl.h"
#include <cvirte.h>		
#include <userint.h>
#include "������̼����ϵͳ.h"
#include "������.h"
#include "����.h"

static int PassWord;//
static int panelHandle;
static int MainPanelHandle;
static int HelpPanelHandle;
static TaskHandle AItaskhandle;
double aidata[3]={25.0,0,18.0};//Ĭ�ϸߵ���
double upper_limit =25,lower_limit =18;//Ĭ�ϸߵ���
int fileTypeI=1;//Ĭ������ASCII����
int fileTypeO=1;//Ĭ�����ASCII����
char timeString[50],dateString[50];//ʱ�䡢��������
static char NewFilePath[MAX_PATHNAME_LEN]={"f:\\"};//�ļ�������
static char file_name[MAX_PATHNAME_LEN];


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "������̼����ϵͳ.uir", PANEL)) < 0)
	    return -1;
	if ((MainPanelHandle = LoadPanel (0, "������.uir", MAINPANEL)) < 0)
	    return -1;
	PassWord = PasswordCtrl_ConvertFromString (panelHandle, PANEL_PASSWORD);//��������ַ�ת��������
	PasswordCtrl_SetAttribute(panelHandle,PANEL_PASSWORD,ATTR_PASSWORD_MASK_CHARACTER,42);//�������������ʾΪ*
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (MainPanelHandle);
	return 0;
}
//�����������ص�
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
/*����ϵͳ��ť*/
int CVICALLBACK EnterSystem_Callback (int panel, int control, int event,
									 void *callbackData, int eventData1, int eventData2)
{
	char pw[100];
	char TruePw[100]="youthgt";
	//strcmp(str1, str2);   strcmp�Ƚ��ַ����Ƿ���ͬ����ֵ��
    //�������ֵ < 0�����ʾ str1 С�� str2��
    //�������ֵ > 0�����ʾ str2 С�� str1��
    //�������ֵ = 0�����ʾ str1 ���� str2��
	switch (event)
	{
		case EVENT_COMMIT:
			PasswordCtrl_GetAttribute (panelHandle, PANEL_PASSWORD, ATTR_PASSWORD_VAL, pw);
			if(!strcmp(TruePw, pw))

			{
			//SetCtrlAttribute (MainPanelHandle, MAINPANEL_SystemTime, ATTR_ENABLED, 1);//����ʱ����ʾ
				EnableDragAndDrop (MainPanelHandle);//�������ʹ��
				DisplayPanel (MainPanelHandle); //��ʾ�����  
				DiscardPanel (panelHandle);//�ͷ������������
				
			}
			else
			{
				MessagePopup("��ʾ��Ϣ", pw);
			}

			
			break;
	}
	return 0;
}
/*�˳���ť*/
int CVICALLBACK QuitSystem_Callback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int Waring;
	switch (event)
	{
		case EVENT_COMMIT:
			Waring = ConfirmPopup ("����", "�Ƿ��˳�ϵͳ��");
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
/*                              ����ص����й���������                                 */
/*                                                                                   */
/*************************************************************************************/

/*������ص�*/
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
			SetCtrlVal (MainPanelHandle, MAINPANEL_FileAnalysis,*addr);//��ʾ�ļ�·��				
			SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_up, aiin[0]);//��ʾ��ʱ����
			SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_co, aiin[1]);//��ʾ��ʱŨ��
			SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_do, aiin[2]);//��ʾ��ʱ����
			if(aiin[2]<=aiin[1]&&aiin[1]<=aiin[0])//��ʱŨ������
			   {
				   SetCtrlAttribute (MainPanelHandle, MAINPANEL_LED, ATTR_ON_COLOR, VAL_GREEN);
			   }
			else if(aiin[2]>aiin[1])//��ʱ�¶ȱ��¶����޵�
			   {
				   SetCtrlAttribute (MainPanelHandle, MAINPANEL_LED, ATTR_ON_COLOR, VAL_BLUE);
			   }
			else                    //(aiin[1]>aiin[2])��ʱ�¶ȱ��¶����޸�
			   {
				   SetCtrlAttribute (MainPanelHandle, MAINPANEL_LED, ATTR_ON_COLOR, VAL_RED);
			   }  
		  SetCtrlVal (MainPanelHandle, MAINPANEL_LED, 1);
	      MessagePopup ("�����ļ�", "����ɹ���");
	
	}
	return 0;
}

/*AI����Timer*/
int CVICALLBACK AItimer_Callback (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
    float64 AIdata[1000];
	int32 sampsread;
	int i=0;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			DAQmxReadAnalogF64 (AItaskhandle, DAQmx_Val_Auto, 10.0, DAQmx_Val_GroupByChannel, AIdata, 1000, &sampsread, 0);	//��ȡ����
		    AIdata[i]=AIdata[i]*12.5+3; //��ȱ任
			aidata[1]=AIdata[i];//���ɼ����ݴ��ݸ��ڶ���Cursor,�Ա���ʾ��������ʾ
			PlotStripChart (MainPanelHandle, MAINPANEL_STRIPCHART, aidata, 3, 0, 0, VAL_DOUBLE);
			SetCtrlVal (MainPanelHandle, MAINPANEL_CONCENTRATION, aidata[1]);
			//�¶Ⱦ������������
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
/*��ȡϵͳʱ��Timer*/
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
        Fmt (dateString, "%d��%d��%d��",Year,Month,Day); 
		Fmt (timeString, "%dʱ%d��%d��",Hours,Minutes,Seconds);
		SetCtrlVal (MainPanelHandle, MAINPANEL_Date, dateString);
		SetCtrlVal (MainPanelHandle, MAINPANEL_Time, timeString);
			break;
	}
	return 0;
}

/*����ļ���ʾ��ť**/
int CVICALLBACK CleanFile_Callback (int panel, int control, int event,
									void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//���ļ���������ֵ���ָ�Ĭ��
			SetCtrlVal (MainPanelHandle, MAINPANEL_FileAnalysis, "�ļ�����");
			SetCtrlVal (MainPanelHandle, MAINPANEL_LED, 0);
			SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_up, 0.00);
			SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_co, 0.00);
			SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_do, 0.00);
			break;
	}
	return 0;
}
/*ֹͣ��ť**/
int CVICALLBACK Stop_Callback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 SetCtrlAttribute (MainPanelHandle, MAINPANEL_AItimer, ATTR_ENABLED, 0);//���òɼ�timer
			 SetCtrlAttribute (MainPanelHandle, MAINPANEL_Acquire, ATTR_DIMMED, 0);//�ÿ�ʼ�ɼ�����
			 DAQmxClearTask (AItaskhandle);
			 AItaskhandle=0;	 			
			break;
	}
	return 0;
}
/*�����ļ���ť*/
int CVICALLBACK LoadFile_Callback (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
    double aiin[3];
	switch (event)
	{
		case EVENT_COMMIT:
			 if (FileSelectPopupEx (NewFilePath, "*.txt", "*.txt", "ѡ��Ҫ�򿪵��ļ�", VAL_OK_BUTTON, 0, 1, file_name) > 0)
				 {
					
						FileToArray (file_name, aiin, VAL_DOUBLE, 3, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, fileTypeI);
						SetCtrlVal (MainPanelHandle, MAINPANEL_FileAnalysis,file_name);//��ʾ�ļ�·��				
						SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_up, aiin[0]);//��ʾ��ʱ����
						SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_co, aiin[1]);//��ʾ��ʱŨ��
						SetCtrlVal (MainPanelHandle, MAINPANEL_NUMERIC_do, aiin[2]);//��ʾ��ʱ����
						if(aiin[2]<=aiin[1]&&aiin[1]<=aiin[0])//��ʱŨ������
						   {
							   SetCtrlAttribute (MainPanelHandle, MAINPANEL_LED, ATTR_ON_COLOR, VAL_GREEN);
						   }
						else if(aiin[2]>aiin[1])//��ʱ�¶ȱ��¶����޵�
						   {
							   SetCtrlAttribute (MainPanelHandle, MAINPANEL_LED, ATTR_ON_COLOR, VAL_BLUE);
						   }
   						else                    //(aiin[1]>aiin[2])��ʱ�¶ȱ��¶����޸�
						   {
							   SetCtrlAttribute (MainPanelHandle, MAINPANEL_LED, ATTR_ON_COLOR, VAL_RED);
						   }  
					  SetCtrlVal (MainPanelHandle, MAINPANEL_LED, 1);
				      MessagePopup ("�����ļ�", "����ɹ���");
				 }
	}
	return 0;
}
/*�Զ����水ť*/
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
/*���水ť*/
int CVICALLBACK SaveData_Callback (int panel, int control, int event,
								   void *callbackData, int eventData1, int eventData2)
{
	char TempFilePath[512];//������ʱ·��
	
	switch (event)
	{
		case EVENT_COMMIT:
			strcpy(TempFilePath,NewFilePath);//����õ���·�����ݸ�������ʱ·��
			strcat(TempFilePath,"//");//��·���ӷָ���
            strcat(TempFilePath,strcat(strcat(dateString, timeString),".txt"));//ƴ�ӳ�����·��
			ArrayToFile (TempFilePath, aidata, VAL_DOUBLE, 3, 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 10, fileTypeO, VAL_TRUNCATE);
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
			ClearStripChart (MainPanelHandle, MAINPANEL_STRIPCHART);
			break;
	}
	return 0;
}
/*��ʼ�ɼ���ť*/
int CVICALLBACK Acquire (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	int AIrate;
    char AIchanel[256];
	switch (event)
	{
		case EVENT_COMMIT:
            GetCtrlVal (MainPanelHandle, MAINPANEL_AIcontrol, AIchanel);// �ӽ���õ�ģ������ͨ��
			GetCtrlVal (MainPanelHandle, MAINPANEL_AIrate, &AIrate); // �ӽ���õ�����
			DAQmxCreateTask ("", &AItaskhandle); // ����ģ����������
			//����ģ������ͨ��
			DAQmxCreateAIVoltageChan (AItaskhandle, AIchanel, "Voltage", DAQmx_Val_RSE, -5.0, 5, DAQmx_Val_Volts, "");
			// ���ò�����
			DAQmxCfgSampClkTiming (AItaskhandle, "", AIrate, DAQmx_Val_Rising, DAQmx_Val_ContSamps, 100);
			//����aitimer
			SetCtrlAttribute (MainPanelHandle, MAINPANEL_AItimer, ATTR_ENABLED, 1);
			//���ñ�������
			SetCtrlAttribute (MainPanelHandle, MAINPANEL_SaveData, ATTR_DIMMED, 0);
			//��ֹ�ظ����
			SetCtrlAttribute (MainPanelHandle, MAINPANEL_Acquire, ATTR_DIMMED, 1);
			break;
	}
	return 0;
}
/*�����¶���*/
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
					MessagePopup("���棡����", "�������������ޣ����޲��ܵ������ޣ�����");
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
					MessagePopup("���棡����", "�������������ޣ����޲��ܸ������ޣ�����");
					
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
/*�˳���ť*/

int CVICALLBACK Quit_Callback (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	int Waring;
	switch (event)
	{
		case EVENT_COMMIT:
			Waring = ConfirmPopup ("����", "�Ƿ�ر�ϵͳ��");
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
/*                              ����ص����й��ڲ˵���                                 */
/*                                                                                   */
/*************************************************************************************/
/*Ĭ���ļ�·��ѡ��*/
void CVICALLBACK FilePath_Callback (int menuBar, int menuItem, void *callbackData,
									int panel)
{
	DirSelectPopupEx ("C:\\", "Select Directory", NewFilePath);
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
/*����*/
void CVICALLBACK Help_Callback (int menuBar, int menuItem, void *callbackData,
								int panel)
{
	HelpPanelHandle = LoadPanel (0, "����.uir", HelpPanel);
    DisplayPanel (HelpPanelHandle);
}
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
			DiscardPanel (HelpPanelHandle);
			break;
	}
	return 0;
}
/*����*/
void CVICALLBACK About_Callback (int menuBar, int menuItem, void *callbackData,
								 int panel)
{
     MessagePopup("����", "Lab windows(CVI)�γ����-YouthGT");	
}






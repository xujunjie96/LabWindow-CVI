#include <cvirte.h>		
#include <userint.h>
#include "test1.h"
#include "NIDAQmx.h"
//�ֹ����NI-DAQmx�����ͷ�ļ�
#include "DAQTasktest.h"

static int panelHandle;
 //NI-DAQmx������
static TaskHandle taskHandle = 0;
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "test1.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK stop (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
			DAQmxClearTask(taskHandle);
			taskHandle = 0; 
			break;
	}
	return 0;
}

int CVICALLBACK sample (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//���NI-DAQmx���񲻴��ڣ���ִ�����´���
		//	if (taskHandle <= 0)
	//		{
				//�ڹ����д���һ��NI-DAQmx������
				CreateDAQTasktest(&taskHandle);
				
				//�򿪶�ʱ��
				SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
		//	}
			break;
	}
	return 0;
}

int CVICALLBACK QuitCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//���NI-DAQmx������
			DAQmxClearTask(taskHandle);
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK timer (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int32 samplesread = 100;
	float64 readarray[1000];
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			//�������ݲɼ�
			DAQmxReadAnalogF64 (taskHandle, DAQmx_Val_Auto, 10.0, DAQmx_Val_GroupByChannel, readarray, 1000, &samplesread, 0);
			
			//���ɼ��������ݻ�����Strip Chart�ؼ���
			PlotStripChart (panelHandle, PANEL_STRIPCHART, readarray, 1, 0, 0, VAL_DOUBLE);
			break;
	}
	return 0;
}

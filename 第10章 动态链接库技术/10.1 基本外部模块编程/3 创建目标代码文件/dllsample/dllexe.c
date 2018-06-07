#include <cvirte.h>		
#include <userint.h>
#include "dllexe.h"
#include "dllsample.h" 
static int panelHandle;
 //����dllʱҪѡ��releaseģʽ�������ɾ�̬�����lib�ļ�  1 configurationҲ��releaseģʽ  2 targetsettingҲ��releaseģʽ��change��ѡ��.h�ļ�
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "dllexe.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK locktimer (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	int CapsLock=0;
	int Numlock=0;
	int ScrollLock=0;
	int AsciiA=0;
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			GetVirtualKey(&CapsLock,&Numlock,&ScrollLock,&AsciiA);  //�����Զ��庯�� �жϼ��Ƿ񱻰���    
			 SetCtrlVal(panelHandle, PANEL_LED_CAP, CapsLock); 
			 SetCtrlVal(panelHandle, PANEL_LED_NUM, Numlock);
			 SetCtrlVal(panelHandle, PANEL_LED_SCROLL, ScrollLock);
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
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK keypresstimer (int panel, int control, int event,
							   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			//0.5s��led_aϨ��
			SetCtrlVal (panelHandle, PANEL_LED_A, 0);
			// ������ʾ ��ʱ��ֹͣ��ʱ
			SetCtrlAttribute (panelHandle, PANEL_TIMER_2, ATTR_ENABLED, 0);
			break;
	}
	return 0;
}

int CVICALLBACK panelCB (int panel, int event, void *callbackData,
						 int eventData1, int eventData2)
{
	int CapsLock=0;
	int Numlock=0;
	int ScrollLock=0;
	int AsciiA=0;
	switch (event)
	{
		case EVENT_KEYPRESS:
			GetVirtualKey(&CapsLock,&Numlock,&ScrollLock,&AsciiA);  //�����Զ��庯�� �жϼ��Ƿ񱻰���
			//���� ��led���� ��ʱ����ʼ��ʱ
			 SetCtrlVal(panelHandle, PANEL_LED_A, AsciiA);
			SetCtrlAttribute (panelHandle, PANEL_TIMER_2, ATTR_ENABLED, 1);
			break;
	}
	return 0;
}

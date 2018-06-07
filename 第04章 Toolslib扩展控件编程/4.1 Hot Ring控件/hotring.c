#include "hotringctrl.h"
#include <cvirte.h>		
#include <userint.h>
#include "hotring.h"

static int panelHandle;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "hotring.uir", PANEL)) < 0)
		return -1;
	HotRing_ConvertFromRing (panelHandle, PANEL_HOTRING); //�����ؼ�ת��ΪHotRing�ؼ�
	HotRing_SetAttribute (panelHandle, PANEL_HOTRING, ATTR_HOTRING_TIMEOUT, 1.0);   //���ð�����ʱʱ��Ϊ5s
	HotRing_SetAttribute (panelHandle, PANEL_HOTRING, ATTR_HOTRING_MATCH_SIZE, 2);	//���ü�������ƥ���ַ�Ϊ����
	
	
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK quit (int panel, int control, int event,
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

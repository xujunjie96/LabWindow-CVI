#include "windows.h" 
#include <cvirte.h>		
#include <userint.h>
#include "dragandgrop.h"

static int panelHandle;
static HWND hwnd;
static BOOL flag=0;
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "dragandgrop.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	SetPanelAttribute (panelHandle, ATTR_TITLEBAR_VISIBLE, 0);//���ñ��������ɼ� û�йر� ��� ��С��
	GetPanelAttribute (panelHandle, ATTR_SYSTEM_WINDOW_HANDLE, (int*)&hwnd); //��ô��ھ��
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK panelCB (int panel, int event, void *callbackData,
						 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_CLICK:
			ReleaseCapture();//Ϊ��ǰ��Ӧ�ó��������겶��
			SendMessage(hwnd,WM_NCLBUTTONDOWN,HTCAPTION,0); //����Ϣ������ָ�������� ����Ӧ��ק����
			break;
		case EVENT_LEFT_DOUBLE_CLICK:
			 QuitUserInterface (0);
			break;
		case EVENT_CLOSE:
			 QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK textshow (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	int x;
	switch (event)
	{
		case EVENT_COMMIT:
			 SetCtrlAttribute (panelHandle, PANEL_TEXTBOX, ATTR_VISIBLE, flag);
			 flag=!flag;
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
			//QuitUserInterface (0);
			panelCB(panelHandle,EVENT_LEFT_DOUBLE_CLICK,0,0,0);
			break;
	}
	return 0;
}

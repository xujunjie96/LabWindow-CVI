#include <rs232.h>
#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "uartfilesend.h"

static int panelHandle;
static int byteswritten;
static char filename[MAX_FILENAME_LEN];
static char pathname[MAX_PATHNAME_LEN];

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "uartfilesend.uir", PANEL)) < 0)
		return -1;
	OpenComConfig (1, "", 57600, 0, 8, 1, 32767, 32767); //�򿪲����ô���1
	SetComTime (1, 5.0);	//����ͨ�ų�ʱʱ��
	SetXMode (1, 0);   //��ֹ�����������
	SetCTSMode (1, LWRS_HWHANDSHAKE_OFF); //��ֹӲ������
	
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK filesel (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	int selstatus;
	switch (event)
	{
									 
		case EVENT_COMMIT:
			filename[0]='\0';
			selstatus= FileSelectPopup ("", "*.*", "", "���ļ�", VAL_LOAD_BUTTON, 0, 0, 1, 1, pathname);
			if (selstatus>=0)
			{
				SetCtrlVal (panelHandle, PANEL_STRING, pathname);
				SplitPath (pathname, NULL, NULL, filename);	//�ֽ����ļ���
			}
			break;
	}
	return 0;
}

int CVICALLBACK sendfilename (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	int comstatus;
	int outputqueuelen;
	switch (event)
	{
		case EVENT_COMMIT:
			strcat(filename,"\r");
			byteswritten=ComWrt (1, filename, strlen(filename));//��com1д���ļ����ַ���
		//	byteswritten=ComWrt (1, "adsp", 4);//��com1д���ļ����ַ��� 
			break;
	}
	return 0;
}

int CVICALLBACK sendfile (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	int outputqueuelen;
	switch (event)
	{
		case EVENT_COMMIT:
			XModemConfig (1, 10.0, 10, 5.0, 1024);//���ô���1���ƽ��������
			XModemSend (1, pathname);   //�Ӵ���1�����ļ�����
			outputqueuelen = GetOutQLen (1);	//��ô���1������е��ַ���Ŀ
			if(outputqueuelen==0)
			{
				MessagePopup ("�ļ�����", "�ļ�������ϣ�");
			}

			break;
	}
	return 0;
}

int CVICALLBACK Quitcallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			CloseCom (1);//�رմ���1
			
			QuitUserInterface (0);
			break;
	}
	return 0;
}

#include <rs232.h>
#include <ansi_c.h>
#include "toolbox.h"
#include <cvirte.h>		
#include <userint.h>
#include "uartfilereceive.h"

static int panelHandle;
static int bytesread;
static char filename[MAX_FILENAME_LEN];
static char pathname[MAX_PATHNAME_LEN];

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "uartfilereceive.uir", PANEL)) < 0)
		return -1;
	
	OpenComConfig (2, "", 57600, 0, 8, 1, 32767, 32767);  //�򿪲����ô���2 
	SetComTime (2, 5.0);   //����ͨ�ų�ʱʱ��
	SetXMode (2, 0);   //��ֹ�����������
	SetCTSMode (2, LWRS_HWHANDSHAKE_OFF);  //��ֹ����Ӳ������
	
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
			selstatus = DirSelectPopup ("", "�����ļ�", 1, 1, pathname);
			if (selstatus)
			{
				strcat(pathname,"\\");
				strcat(pathname,filename);
				SetCtrlVal (panelHandle, PANEL_STRING_2, pathname);
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
			CloseCom (2);//�رմ���2
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK receivefilename (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			filename[0]='\0';
			SetCtrlVal (panelHandle, PANEL_STRING, "");
			bytesread = ComRdTerm (2, filename, 256, 13);  //��ȡ�ַ���ֱ���س�����
			filename[bytesread]='\0';	//�����ֻس����� ���������Ͻ�����
			SetCtrlVal (panelHandle, PANEL_STRING, filename);
			break;
	}
	return 0;
}

int CVICALLBACK receivefile (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	int result;
	int filesize;
	int intputqueuelen;
	FILE *stream;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_STRING_2, pathname);
			result=FileExists (pathname, &filesize);  //�ж��ļ��Ƿ����
			if(!result)
			{
				stream = fopen (pathname, "wb+");
				fclose (stream);
			}
			XModemConfig (2, 10.0, 10, 5.0, 1024);   //���ô���2���ƽ��������
			XModemReceive (2, pathname);	//���ô���2�����ļ�����
			intputqueuelen=GetInQLen (2);
			if(intputqueuelen==0)
			{
				MessagePopup ("�ļ�����", "�ļ�������ϣ�");

			}

			break;
	}
	return 0;
}

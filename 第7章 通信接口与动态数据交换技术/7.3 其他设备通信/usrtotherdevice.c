#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <rs232.h>
#include <cvirte.h>		
#include <userint.h>
#include "usrtotherdevice.h"

static int panelHandle;
static void *callbackdata;

void ComCallback(int COMport,int eventMask,void *callbackdata);
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "usrtotherdevice.uir", PANEL)) < 0)
		return -1;
	OpenComConfig (1, "", 1200, 1, 7, 1, 512, 512);   //�򿪲����ô���
	
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

//�����ܵ������ݴ�����ָ���ļ���
int CVICALLBACK timerweight (int panel, int control, int event,
							 void *callbackData, int eventData1, int eventData2)
{
	char *time;
	char *date;
	char filename[260];
	char weight[20];
	double weightvalue;
	switch(event)
	{
		case EVENT_TIMER_TICK:
		date="\0";
		time='\0';	//��ʼ��ָ��
		time = TimeStr ();
		date = DateStr (); //���ϵͳ��ǰʱ�������
		//����Ϊ������һ���ļ��洢·��
		GetProjectDir (filename);
		strcat(filename,"\\");
		strcat(filename,date);
		strcat(filename,"-");
		strcat(filename,"wei.txt");				   
		GetCtrlVal (panelHandle, PANEL_NUMERICGAUGE, &weightvalue);   //�����ƽ����ֵ
		Fmt (weight, "%s<%f", weightvalue);
		strcat(date,time);
		strcat(date," ");
		strcat(date,weight);
		//���ı����ݼ��ϻ��кͻس�
		strcat(date,"\n");
		strcat(date,"\r");
		//������д���ļ���
		ArrayToFile (filename, date, VAL_DOUBLE, strlen(date), 1, VAL_GROUPS_TOGETHER, VAL_GROUPS_AS_COLUMNS, VAL_CONST_WIDTH, 10, VAL_BINARY, VAL_APPEND);
		
		break;
	}
	return 0;
}

int CVICALLBACK rs232quit (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
			CloseCom (1);
			
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK rs232receive (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			InstallComCallback (1, LWRS_RECEIVE, 16, 0, ComCallback, callbackdata);  //������1��װָ���ص����� �����������50���ַ������ж�
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);	//��ʱ��ʹ�� ��ʼ��ʱ

			break;
	}
	return 0;
}

//����1�˿��жϻص�����
void ComCallback(int COMport,int eventMask,void *callbackdata)
{
	int inputqueuelength;
	int bytesread;
	char buffer[512];
	char *matchedchar=0;
	double value;
	int i;
	
	inputqueuelength= GetInQLen (1);  //���ָ���˿���������ַ���
	bytesread = ComRd (1, buffer, inputqueuelength);  //��ȡ��������е��ַ���
	for(i=0;i<=bytesread;i++)
	{
		//���ַ�Ϊ��+��ox2b ʱ ִ�����´���
		if(buffer[i]==0x2B)
		{
			matchedchar=buffer+1+i;//��matchedcharָ���׼�ַ�+����һλ ��ָ����ܵ�����
			Fmt (&value, "%f<%s", matchedchar);  //���ַ�����ʽ����ת����Ϊ˫���ȸ�ʽ
			SetCtrlVal (panelHandle, PANEL_NUMERICGAUGE, value);  
			FlushInQ (1);   //���ָ���˿ڵ��������
		}
	}
	
}

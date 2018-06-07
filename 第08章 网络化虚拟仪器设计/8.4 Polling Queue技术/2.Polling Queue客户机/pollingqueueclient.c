#include "dataskt.h"
#include <cvirte.h>		
#include <userint.h>
#include "pollingqueueclient.h"

static int panelHandle, ctrlarray;
static DSHandle dsHandle;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "pollingqueueclient.uir", PANEL)) < 0)
		return -1;
	ctrlarray = GetCtrlArrayFromResourceID (panelHandle, CTRLARRAY);
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK panelcb (int panel, int event, void *callbackData,
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

int CVICALLBACK connect (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	char url[300];
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_STRING, url);
			//�Զ���ʽ��datasocket���� ����pollingqueue����
			DS_OpenEx (url, DSConst_ReadAutoUpdate, NULL, NULL, DSConst_PollingModel, 5000, &dsHandle);
			break;
	}
	return 0;
}

int CVICALLBACK receive (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(dsHandle)
			{
				SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
			}
			break;
	}
	return 0;
}

//ͨ����ʱ���ؼ���������
int CVICALLBACK timer (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	double value[1];
	BOOL dataupdate;
	double packetfraction;
	double bytefraction;
	unsigned int type;
	char text[300];
	int packetsize;
	int bytesize;
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			DS_GetDataUpdated (dsHandle, &dataupdate);//���polling queue���ݸ�����Ϣ
			if(dataupdate)//������ݸ��� ��ִ�����²���
			{
				DS_GetPollingQueueSizeInBytes (dsHandle, &bytesize);//���polling queue�ֽ���
				DS_GetPollingQueueSizeInPackets (dsHandle, &packetsize); //���polling queue���ݰ���
				SetCtrlVal (panelHandle, PANEL_NUMERIC_BYTES, bytesize);
				SetCtrlVal (panelHandle, PANEL_NUMERIC_PACKETS, packetsize);
				DS_GetPollingQueueBytesUtilization (dsHandle, &bytefraction);//���polling queue�ֽ���ռ���ֽ�������
				DS_GetPollingQueuePacketsUtilization (dsHandle, &packetfraction); //���polling queue���ݰ���ռ�����ݰ�������
				SetCtrlVal (panelHandle, PANEL_NUMERICSLIDE_BYTES, bytefraction);
				SetCtrlVal (panelHandle, PANEL_NUMERICSLIDE_PACKETS, packetfraction);
				
				DS_GetDataType (dsHandle, &type, NULL, NULL);//��ý������ݵ���������
				switch(type)
				{
					case CAVT_DOUBLE://��Ϊdouble������ʱ д��stripchart�ؼ�
						DS_GetDataValue (dsHandle, CAVT_DOUBLE, &value, 0, NULL, NULL);
						PlotStripChart (panelHandle, PANEL_STRIPCHART, value, 1, 0, 0, VAL_DOUBLE); 
						break;
					case CAVT_CSTRING: //��Ϊ�ַ��������� д��Textbox�ؼ�
						DS_GetDataValue (dsHandle, CAVT_CSTRING, text, 300, NULL, NULL);
						SetCtrlVal (panelHandle, PANEL_TEXTBOX, text);
						break;
				}
			}
			
			break;
	}
	return 0;
}

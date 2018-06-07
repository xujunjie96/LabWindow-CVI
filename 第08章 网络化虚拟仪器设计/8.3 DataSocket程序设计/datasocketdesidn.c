#include "dataskt.h"
#include "toolbox.h"
#include "combobox.h"
#include <cvirte.h>		
#include <userint.h>
#include "datasocketdesidn.h"

static int CSmode;
static int panelHandle, ctrlarray;
static DSHandle dshandleread=0;//��DataSocket��������ȡ���ݵľ��
static DSHandle dshandlewrite=0;//��DataSocket������д���ݵľ��

void geturl(int ReadOrWriteURL,char *url);   //�Զ��庯�� ���DataSocket�ĵ�ַ
void insertcombo(void);//��combobox�в�����Ŀ
void CVICALLBACK dsfunctionread(DSHandle dsHandle,int event,void *callbackData);//��DataSocket��������ȡ����
void CVICALLBACK dsfunctionwrite(DSHandle dsHandle,int event,void *callbackData);//��DataSocket������д����

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "datasocketdesidn.uir", PANEL)) < 0)
		return -1;
	
	Combo_NewComboBox (panelHandle, PANEL_COMBO);  //װ��һ��combo�ؼ�
	insertcombo();
	SetCtrlAttribute (panelHandle, PANEL_CMD_SEND, ATTR_DIMMED, 1);  //send��ť���
	DS_ControlLocalServer(DSConst_ServerLaunch);	//����DataSocket����������
	DS_ControlLocalServer (DSConst_ServerHide);	// ����DataSocket���������� 
	ctrlarray = GetCtrlArrayFromResourceID (panelHandle, CTRLARRAY);
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);		// �ر�DataSocket������
	DS_ControlLocalServer (DSConst_ServerClose);
	return 0;
}

int CVICALLBACK disconnect (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (panelHandle, PANEL_CMD_SEND, ATTR_DIMMED, 1);  //send��ť��� 
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);   //�ض�ʱ��
			if(dshandleread)
			{
				//�ͷ�datasocket������������Դ
				DS_DiscardObjHandle (dshandleread);
				dshandleread=0;
			}
			if(dshandlewrite)
			{
				//�ͷ�datasocket������������Դ
				DS_DiscardObjHandle (dshandlewrite);
				dshandlewrite=0;
			}
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
			//����ڴ�ǰdatasocket���Ӿ������  ���ȹر�
			if(dshandleread)
			{
				//�ͷ�datasocket������������Դ
				DS_DiscardObjHandle (dshandleread);
				dshandleread=0;
			}
			if(dshandlewrite)
			{
				//�ͷ�datasocket������������Դ
				DS_DiscardObjHandle (dshandlewrite);
				dshandlewrite=0;
			}
			
			SetCtrlAttribute (panelHandle, PANEL_CMD_SEND, ATTR_DIMMED, 0);
			GetCtrlVal (panelHandle, PANEL_BINARYSWITCH, &CSmode);	//��÷��������߿ͻ���ģʽ
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, CSmode);
			geturl(CSmode,url);
			DS_OpenEx (url, DSConst_ReadAutoUpdate, dsfunctionread, NULL, DSConst_EventModel, DSConst_Asynchronous, &dshandleread);//��datasocket��ȡ����������
			geturl(!CSmode,url);
			DS_OpenEx (url, DSConst_WriteAutoUpdate, dsfunctionwrite, NULL, DSConst_EventModel, DSConst_Asynchronous, &dshandlewrite);//��datasocketд����������
			
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
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0); 
			if(dshandleread)
			{
				//�ͷ�datasocket������������Դ
				DS_DiscardObjHandle (dshandleread);
				dshandleread=0;
			}
			if(dshandlewrite)
			{
				//�ͷ�datasocket������������Դ
				DS_DiscardObjHandle (dshandlewrite);
				dshandlewrite=0;
			}
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK clear (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			ResetTextBox (panelHandle, PANEL_TEXTBOX, "");
			break;
	}
	return 0;
}

int CVICALLBACK send (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	char text[300];
	char url[300];
	
	switch (event)
	{
		case EVENT_COMMIT:
			geturl(CSmode,url);
			GetCtrlVal (panelHandle, PANEL_TEXTBOX, text);
			DS_SetDataValue (dshandlewrite, CAVT_CSTRING, text, 0, 0);	//��datasocket������д������ 
			ResetTextBox (panelHandle, PANEL_TEXTBOX, ""); 
			break;
	}
	return 0;
}

int CVICALLBACK timer (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	double value[1];
	switch (event)
	{
		case EVENT_TIMER_TICK:
			value[0] = Random (0, 100);		//����0-100�������
			PlotStripChart (panelHandle, PANEL_STRIPCHART, value, 1, 0, 0, VAL_DOUBLE);
			DS_SetDataValue (dshandlewrite, CAVT_DOUBLE, value, 1, 0);	//��datasocket������д������
			break;
	}
	return 0;
}

//��DataSocket��������ȡ����
void CVICALLBACK dsfunctionread(DSHandle dsHandle,int event,void *callbackData)
{
	char text[300]; 
	unsigned int type;
	char buffer[1000];
	double value;
	switch(event)
	{
		//��datasocket��������ȡ�����¼�	
		case DS_EVENT_DATAUPDATED:
			DS_GetDataType (dshandleread, &type, NULL, NULL);
			switch(type)
			{
				case CAVT_DOUBLE://��Ϊdouble������ʱ д��stripchart�ؼ�
					DS_GetDataValue (dshandleread, type, &value, 0, NULL, NULL);
					PlotStripChart (panelHandle, PANEL_STRIPCHART, &value, 1, 0, 0, VAL_DOUBLE); 
					break;
				case CAVT_CSTRING: //��Ϊ�ַ��������� д��Textbox�ؼ�
					DS_GetDataValue (dshandleread, type, text, 300, NULL, NULL);
					SetCtrlVal (panelHandle, PANEL_TEXTBOX, text);
					break;
			}
			break;
		//��datasocket��������ȡ״̬��Ϣ
		case DS_EVENT_STATUSUPDATED:
			DS_GetLastMessage (dshandleread, buffer, 1000);
			SetCtrlVal (panelHandle, PANEL_STRING, buffer);	
			break;
	}
}

//��DataSocket������д����
void CVICALLBACK dsfunctionwrite(DSHandle dsHandle,int event,void *callbackData)
{
	char text[300]; 
	unsigned int type;
	char buffer[1000];
	double value;
	switch(event)
	{
		//��datasocket��������ȡ״̬��Ϣ
		case DS_EVENT_STATUSUPDATED:
			DS_GetLastMessage (dshandleread, buffer, 1000);
			SetCtrlVal (panelHandle, PANEL_STRING, buffer);	
			break;
	}	
}


//��combobox�в�����Ŀ 
void insertcombo(void)
{
	Combo_InsertComboItem (panelHandle, PANEL_COMBO, -1, "dstp://localhost/data");
	Combo_InsertComboItem (panelHandle, PANEL_COMBO, -1, "dstp://192.168.74.1/data");
}

//�Զ��庯�� ���DataSocket�ĵ�ַ 
//ReadOrWriteURLΪ1�Ƕ�ȡ���� Ϊ0��д����
void geturl(int ReadOrWriteURL,char *url)
{
	url[0]='\0';
	GetCtrlVal (panelHandle, PANEL_COMBO, url);
	if(ReadOrWriteURL)
	{
		strcat(url,"/");
		strcat(url,"read");
	}
	else
	{
		strcat(url,"/");
		strcat(url,"write");	
	}
}

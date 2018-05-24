#include <utility.h>
#include <tcpsupp.h>
#include <formatio.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "tcpclient.h"

static unsigned int conversationHandle;//TCP���Ӿ��
static int portnumber=0;
static int panelHandle, ctrlarray;

int TCPClientCB(unsigned handle,int xType,int errCode,void *callbackData);//�Զ���������˻ص�����
void gethostinfo(unsigned int conversationHandle);//�Զ��庯�� �ĵ�������Ϣ
void setmode(int connect,int disconnect,int send);//�Զ��庯�� �������� �Ͽ��Լ����Ͱ�ť�Ļ���״̬

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "tcpclient.uir", PANEL)) < 0)
		return -1;
	ctrlarray = GetCtrlArrayFromResourceID (panelHandle, CTRLARRAY);
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK disconnect (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//���tcp���Ӿ������ ��Ͽ�
			if (conversationHandle)	   //
			{
				DisableBreakOnLibraryErrors ();	   //�������⺯������ʱ����ֹ��������ʱ����Ի���
				DisconnectFromTCPServer (conversationHandle);
			}
			setmode(0,1,1);
			//���ÿͻ���״̬��ʾ
			SetCtrlVal (panelHandle, PANEL_STRING_SERVERIP, "");
			SetCtrlVal (panelHandle, PANEL_STRING_SERVERNAME, "");
		//	SetCtrlVal (panelHandle, PANEL_LED_CLIENT, 0);
			SetCtrlVal (panelHandle, PANEL_LED_SERVER, 0); 
			break;
	}
	return 0;
}

int CVICALLBACK connect (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	char address[50];
	unsigned int conversationHandle;
	int status;
	char portnumberstring[10]; 
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_STRING_PORTNUMBER, portnumberstring);	 //���tcp�������˿ں�
			Fmt(&portnumber,"%i<%s",portnumberstring);
			GetCtrlVal (panelHandle, PANEL_STRING_ADDRESS, address);	 //���tcp��������ַ
			status= ConnectToTCPServer (&conversationHandle, portnumber, address, TCPClientCB, 0, 0);
			if(status==0)
			{
				setmode(1,0,0);
				gethostinfo(conversationHandle);//�Զ��庯��
			}
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
	char datapointer[1000]; 
	switch (event)
	{
		case EVENT_COMMIT:
			 GetCtrlVal (panelHandle, PANEL_TEXTBOX, &datapointer[0]);		  //
			 //�������ַ����м��ϻس������Լ�������ʶ��
			 strcat(datapointer,"\r");
			 strcat(datapointer,"\n");
			 strcat(datapointer,"\0");
			 ClientTCPWrite (conversationHandle, datapointer, 1000, 0); //���������������
			 ResetTextBox (panelHandle, PANEL_TEXTBOX, "");
			break;
	}
	return 0;
}

//�Զ���������˻ص�����
int TCPClientCB(unsigned handle,int xType,int errCode,void *callbackData)
{
	char datapointer[1000];
	char buffer[50];
	int datamode;
	double datapointernumber[10];
	int result1,result2; 
	
	unsigned int add1,add2,i,j;
	int a;
	//a=strlen(datapointernumber);
	//add1=&datapointernumber;
	//add2=&datapointer;
	switch(xType)
	 {
		//TCP�Ͽ�ʱ�������¼� 
		case TCP_DISCONNECT :
		
			SetCtrlVal (panelHandle, PANEL_STRING_SERVERIP, "");
			SetCtrlVal (panelHandle, PANEL_STRING_SERVERNAME, "");
			SetCtrlVal (panelHandle, PANEL_LED_CLIENT, 0);
			SetTCPDisconnectMode (conversationHandle, TCP_DISCONNECT_AUTO);//����tcp�Ͽ���ʽ 
			break;
		//TCP����׼���� ��ȡ�����¼� 
		case TCP_DATAREADY :
			//����ǵڶ������ӻ��߸�������ӣ��ӱ�������þ��
			conversationHandle=handle;
			ClientTCPRead (conversationHandle, datapointer, 1000, 0); //��tcp��������ȡ���� 
			//���ĩβ�����س��ͻ��з� ����Ϊ���ַ��� ���û�� ����Ϊ������ д��stripchart��
			result1=FindPattern (datapointer, 0, -1, "\r", 0, 0);
			result2=FindPattern (datapointer, 0, -1, "\n", 0, 0);
			if(result1>=0&&result2>=0)
			{
				SetCtrlVal (panelHandle, PANEL_TEXTBOX, datapointer);//���ַ�������д��Textbox��
			}
			else
			{
			//	ClientTCPRead (conversationHandle, datapointernumber, 10, 0); //��tcp��������ȡ����
				//	Fmt(&datapointernumber,"%i<%s",datapointer);
			/*	for(i=0;i<a;i++)
				{
					*(add1+i) = *(add2+i)	;
				}*/
			memcpy ( datapointernumber, datapointer, sizeof(datapointernumber) );
				PlotStripChart (panelHandle, PANEL_STRIPCHART, datapointernumber, 1, 0, 0, VAL_DOUBLE);
			}
			
			break; 
	 }
	 return 0;
}

//�Զ��庯�� �ĵ�������Ϣ
void gethostinfo(unsigned int conversationHandle)
{
	char buffer[50]; 
	//���Զ��������Ϣ
	GetTCPPeerAddr (conversationHandle, buffer, 50);   
	SetCtrlVal (panelHandle, PANEL_STRING_SERVERIP, buffer);
	GetTCPPeerName (conversationHandle, buffer, 50); 
	SetCtrlVal (panelHandle, PANEL_STRING_SERVERNAME, buffer);
	SetCtrlVal (panelHandle, PANEL_LED_SERVER, 1);
	GetTCPHostAddr (buffer, 50);	//��ȡ���ؼ������IP��ַ
	SetCtrlVal (panelHandle, PANEL_STRING_CLIENTIP, buffer);
	GetTCPHostName (buffer, 50);
	SetCtrlVal (panelHandle, PANEL_STRING_CLIENTIPNAME, buffer);
	SetCtrlVal (panelHandle, PANEL_LED_CLIENT, 1); 
}

//�Զ��庯�� �������� �Ͽ��Լ����Ͱ�ť�Ļ���״̬ 
void setmode(int connect,int disconnect,int send)
{
	SetCtrlAttribute (panelHandle, PANEL_CMD_CONNECT, ATTR_DIMMED, connect);
	SetCtrlAttribute (panelHandle, PANEL_CMD_DISCONNECT, ATTR_DIMMED, disconnect);
	SetCtrlAttribute (panelHandle, PANEL_CMD_SEND, ATTR_DIMMED, send);
}

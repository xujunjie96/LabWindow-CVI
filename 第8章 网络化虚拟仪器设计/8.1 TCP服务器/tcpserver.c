#include <tcpsupp.h>
#include <formatio.h>
#include "toolbox.h"
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "tcpserver.h"

static unsigned int conversationHandle;//TCP���Ӿ��
static int portnumber=0;
static int panelHandle;

int TCPServerCB(unsigned handle,int xType,int errCode,void *callbackData);//�Զ���������˻ص�����
void gethostinfo(void);//�Զ��庯�� �ĵ�������Ϣ
void setmode(int connect,int disconnect,int send);//�Զ��庯�� �������� �Ͽ��Լ����Ͱ�ť�Ļ���״̬

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "tcpserver.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	//���˳�ʱ ���tcp���Ӿ������ ��ɾ����� ��ע��TCP������
	if (conversationHandle)
	{
		DisconnectTCPClient (conversationHandle);
		UnregisterTCPServer (portnumber);
	}
	return 0;
}

int CVICALLBACK connect (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	int status;
	char portnumberstring[10];
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_STRING_PORTNUMBER, portnumberstring);	 //���tcp�������˿ں�
			Fmt(&portnumber,"%i<%s",portnumberstring);
			status=RegisterTCPServer (portnumber, TCPServerCB, 1);//ע��tcp������
			if(status==0)
			{
				SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 1);
				setmode(1,0,0);
				gethostinfo();//�Զ��庯��
			}
			
			break;
	}
	return 0;
}

int CVICALLBACK disconnect (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetCtrlAttribute (panelHandle, PANEL_TIMER, ATTR_ENABLED, 0);
			//���tcp���Ӿ������ ��Ͽ�
			if (conversationHandle)
			{
				DisconnectTCPClient (conversationHandle);
			}
			conversationHandle=0;
			UnregisterTCPServer (portnumber);   //ע��������
			setmode(0,1,1);
			//���ÿͻ���״̬��ʾ
			SetCtrlVal (panelHandle, PANEL_STRING_CLIENTIP, "");
			SetCtrlVal (panelHandle, PANEL_STRING_CLIENTIPNAME, "");
			SetCtrlVal (panelHandle, PANEL_LED_CLIENT, 0);
			SetCtrlVal (panelHandle, PANEL_LED_SERVER, 0); 
			break;
	}
	return 0;
}

int CVICALLBACK timer (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	double datapointernumber[10];
	double value[1];
	double result;
	
	switch (event)
	{
		case EVENT_TIMER_TICK:
			result=Random (0, 100);		//����0-100�������
			value[0]=result;
			PlotStripChart (panelHandle, PANEL_STRIPCHART, value, 1, 0, 0, VAL_DOUBLE);
			
			//�������tcp���� �����ݷ��͵�tcp������
			if(conversationHandle)
			{
				 datapointernumber[0]=result;
				 ServerTCPWrite (conversationHandle, datapointernumber, 10, 0);
			}
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
			 ServerTCPWrite (conversationHandle, datapointer, 1000, 0); //��tcp�ͻ�����������
			 ResetTextBox (panelHandle, PANEL_TEXTBOX, "");
			break;
	}
	return 0;
}

//�Զ���������˻ص����� 
int TCPServerCB(unsigned handle,int xType,int errCode,void *callbackData)
{
	 char datapointer[1000];
	 char buffer[50];
	 
	 switch(xType)
	 {
	 	//TCP����ʱ�������¼�
		 case TCP_CONNECT :
			 conversationHandle=handle; //���пͻ�������ʱ�����tcp���Ӿ��
			 //��ÿͻ�����Ϣ
			 GetTCPPeerAddr (conversationHandle, buffer, 50);   
			 SetCtrlVal (panelHandle, PANEL_STRING_CLIENTIP, buffer);
			 GetTCPPeerName (conversationHandle, buffer, 50); 
	 		 SetCtrlVal (panelHandle, PANEL_STRING_CLIENTIPNAME, buffer);
			 SetCtrlVal (panelHandle, PANEL_LED_CLIENT, 1);
			 SetTCPDisconnectMode (conversationHandle, TCP_DISCONNECT_AUTO);//����tcp�Ͽ���ʽ
			 break;
		//TCP�Ͽ�ʱ�������¼� 
		case TCP_DISCONNECT :
			if(conversationHandle==handle)
			{
				 conversationHandle=0;
				 //���ÿͻ���״̬��ʾ
				SetCtrlVal (panelHandle, PANEL_STRING_CLIENTIP, "");
				SetCtrlVal (panelHandle, PANEL_STRING_CLIENTIPNAME, "");
				SetCtrlVal (panelHandle, PANEL_LED_CLIENT, 0);
			}
			break;
		//TCP����׼���� ��ȡ�����¼� 
		case TCP_DATAREADY :
			ServerTCPRead (conversationHandle, datapointer, 1000, 0); //��tcp�ͻ�����ȡ����
			SetCtrlVal (panelHandle, PANEL_TEXTBOX, datapointer);
			break; 
	 }
	 return 0;
}

//�Զ��庯�� �ĵ�������Ϣ 
void gethostinfo(void) 
{
	char buffer[50];
	GetTCPHostAddr (buffer, 50);
	SetCtrlVal (panelHandle, PANEL_STRING_SERVERIP, buffer);
	SetCtrlVal (panelHandle, PANEL_STRING_ADDRESS, buffer);
	GetTCPHostName (buffer, 50);
	SetCtrlVal (panelHandle, PANEL_STRING_SERVERNAME, buffer);
	SetCtrlVal (panelHandle, PANEL_LED_SERVER, 1); 
}

//�Զ��庯�� �������� �Ͽ��Լ����Ͱ�ť�Ļ���״̬ 
void setmode(int connect,int disconnect,int send)
{
	SetCtrlAttribute (panelHandle, PANEL_CMD_CONNECT, ATTR_DIMMED, connect);
	SetCtrlAttribute (panelHandle, PANEL_CMD_DISCONNECT, ATTR_DIMMED, disconnect);
	SetCtrlAttribute (panelHandle, PANEL_CMD_SEND, ATTR_DIMMED, send);
}




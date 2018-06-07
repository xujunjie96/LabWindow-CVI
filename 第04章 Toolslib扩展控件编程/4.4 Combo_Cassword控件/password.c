#include "toolbox.h"
#include "pwctrl.h"
#include "combobox.h"
#include <cvirte.h>		
#include <userint.h>
#include "password.h"

static int panelHandle;
static int panelpassword;
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "password.uir", PANEL)) < 0)
		return -1;
	PasswordCtrl_ConvertFromString (panelHandle, PANEL_PASSWORD);	//string�ؼ�ת����password�ؼ�
	Combo_NewComboBox (panelHandle, PANEL_COMBO);   //string�ؼ�ת����combo�ؼ�
	//��combo�ؼ���������
	Combo_InsertComboItem (panelHandle, PANEL_COMBO, -1, "2b");
	Combo_InsertComboItem (panelHandle, PANEL_COMBO, -1, "a2");
	Combo_InsertComboItem (panelHandle, PANEL_COMBO, -1, "9s");
	Combo_InsertComboItem (panelHandle, PANEL_COMBO, -1, "lem");
	SetCtrlToolTipAttribute (panelHandle, PANEL_CMD_OK, CTRL_TOOLTIP_ATTR_ENABLE, 1);//���ڿ����Ƿ���ʾ��ʾ���� Ĭ����ʾ
	//�������ͣʱ ����ÿ���ؼ�Ӧ��ʾ������
	SetCtrlToolTipAttribute (panelHandle, PANEL_CMD_OK, CTRL_TOOLTIP_ATTR_TEXT, "����ȷ����ť");
	SetCtrlToolTipAttribute (panelHandle, PANEL_CMD_CLEAR, CTRL_TOOLTIP_ATTR_TEXT, "����ȡ����ť");
	SetCtrlToolTipAttribute (panelHandle, PANEL_CMD_VIEW, CTRL_TOOLTIP_ATTR_TEXT, "���ǲ鿴���밴ť");
	SetCtrlToolTipAttribute (panelHandle, PANEL_COMBO, CTRL_TOOLTIP_ATTR_TEXT, "�������û���");
	SetCtrlToolTipAttribute (panelHandle, PANEL_PASSWORD, CTRL_TOOLTIP_ATTR_TEXT, "����������");

	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK panelCB (int panel, int event, void *callbackData,
						 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK hidepanel (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DiscardPanel (panelpassword);
			break;
	}
	return 0;
}

int CVICALLBACK passwoedview (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			panelpassword=LoadPanel (0, "password.uir", PANEL_2);
			InstallPopup(panelpassword);  //����ģʽ�Ի���  InstallPopup�����Ĺ�������ʾ������һ���������
			break;
	}
	return 0;
}

int CVICALLBACK cancel (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			PasswordCtrl_SetAttribute (panelHandle, PANEL_PASSWORD, ATTR_PASSWORD_VAL, "");  //�������

			break;
	}
	return 0;
}


//ȷ��ʱ��������ȷ
int CVICALLBACK ok (int panel, int control, int event,
					void *callbackData, int eventData1, int eventData2)
{
	int len;
	int plen;
	
	char *username;
	char *passwordstring;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlAttribute (panelHandle, PANEL_COMBO, ATTR_STRING_TEXT_LENGTH,&len );		//���combo box���ַ�������
			username=(char*)malloc(len*sizeof(char)+1);   //��̬�����ַ������ȼ�1�Ŀռ� ���һ���ֽڴ洢������ \0 ֵ
			username[0]='\0';	//����û���������
			GetCtrlVal (panelHandle, PANEL_COMBO, username); //���û����洢��usrname
			PasswordCtrl_GetAttribute (panelHandle, PANEL_PASSWORD, ATTR_PASSWORD_MAX_LENGTH,&plen );		//���password box���ַ������� 
			passwordstring=(char*)malloc(plen*sizeof(char)+1);   //��̬�����ַ������ȼ�1�Ŀռ� ���һ���ֽڴ洢������ \0 ֵ
		   	passwordstring[0]='\0';	//���password������	
			PasswordCtrl_GetAttribute (panelHandle, PANEL_PASSWORD, ATTR_PASSWORD_VAL, passwordstring);//ȡ��password�д洢������
			//�ж������Ƿ���ȷ
			if( ((strcmp(username,"2b")==0)&&(strcmp(passwordstring,"2b")==0)) || ((strcmp(username,"a2")==0)&&(strcmp(passwordstring,"a2")==0)) || ((strcmp(username,"9s")==0)&&(strcmp(passwordstring,"9s")==0))   || ((strcmp(username,"ezio")==0)&&(strcmp(passwordstring,"ezio")==0))  )
			{
				MessagePopup("����","������ȷ��");
			}
			else
			{
				MessagePopup("����","�������");
			}
			//�ͷŶ�̬�����ڴ�
			free(username);
			free(passwordstring);
			break;
	}
	return 0;
}

int CVICALLBACK timer (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	int  keymodifiers;
	int rightdown=0;
	int leftdown=0;
	int y,x;
	int len;
	char *passwordstring;
	char message[100];
	switch (event)
	{
		case EVENT_TIMER_TICK:
			GetGlobalMouseState (&panelHandle, &x, &y, &leftdown, &rightdown, &keymodifiers);
			if(leftdown==1 && rightdown==1 && keymodifiers==VAL_MENUKEY_MODIFIER)
			{
			PasswordCtrl_GetAttribute (panelHandle, PANEL_PASSWORD, ATTR_PASSWORD_MAX_LENGTH,&len );		//���password box���ַ������� 
			passwordstring=(char*)malloc(len*sizeof(char)+1);   //��̬�����ַ������ȼ�1�Ŀռ� ���һ���ֽڴ洢������ \0 ֵ
		   	message[0]='\0';	//���password������	
			PasswordCtrl_GetAttribute (panelHandle, PANEL_PASSWORD, ATTR_PASSWORD_VAL, passwordstring);//ȡ��password�д洢������	
			strcat(message,"�����������:\n");
			strcat(message,passwordstring);
			MessagePopup("����",message); 
			}
			break;
	}
	return 0;
}

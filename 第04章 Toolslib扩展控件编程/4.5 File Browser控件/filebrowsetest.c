#include <ansi_c.h>
#include "toolbox.h"
#include <utility.h>
#include <formatio.h>
#include "FileBrowser.h"
#include "pathctrl.h"
#include <cvirte.h>		
#include <userint.h>
#include "filebrowsetest.h"
#include "userint.h"
static int panelHandle;
static int j=0;
static int flag=0;

//�Զ��庯�� ������������̷�����Ŀ
void drivevolume();

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "filebrowsetest.uir", PANEL)) < 0)
		return -1;
	FileBrowser_Create (panelHandle, PANEL_FILEBROWSER, ROOT_LEVEL_STRING);   //����һ���µ�FileBrowser�ؼ�
    NewPathCtrl (panelHandle, PANEL_PATH, 8, 1);   //����һ���µ�PathCtrl�ؼ� 
	DisplayPanel (panelHandle);
	drivevolume(); //��ʾ�������̷�
	//�����ļ�ϵͳ����·��
	FileBrowser_SetAttribute (panelHandle, PANEL_FILEBROWSER, ATTR_PATH_RELATIVE, 0);
	
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK panelcb (int panel, int event, void *callbackData,
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

int CVICALLBACK drivevol (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	int value;
	char drive2[10];
	switch (event)
	{
		//Ring����Ӧ�¼�Ϊ EVENT_COMMIT   EVENT_VAL_CHANGED
		case EVENT_COMMIT || EVENT_VAL_CHANGED:
			drivevolume(); //��ʾ�������̷� 
			GetCtrlAttribute (panelHandle, PANEL_RING, ATTR_CTRL_VAL, &value);
			Fmt(drive2,"%2c<%i",value);		//���ݸ�ʽ������
			drive2[1]='\0';
			strcat(drive2,":\\");
			SetCtrlVal (panelHandle, PANEL_PATH, drive2);
			SetActiveCtrl (panelHandle, PANEL_PATH);
			SetCtrlAttribute (panelHandle, PANEL_PATH, ATTR_TEXT_SELECTION_START, 10);
			FileBrowser_SetAttribute (panelHandle, PANEL_FILEBROWSER, ATTR_START_PATH, drive2);
			break;
	}
	return 0;
}

int CVICALLBACK path (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	char drive2[MAX_PATHNAME_LEN];
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_PATH, &drive2);//�������̷���ð�ź� ������ \ �����ʾi��Ŀ¼�����е��ļ��Լ�Ŀ¼
			FileBrowser_SetAttribute (panelHandle, PANEL_FILEBROWSER, ATTR_START_PATH, drive2);	//����file browser��ʼ·�� 
			break;
	}
	return 0;
}

int CVICALLBACK fileopen (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	char **files;
	switch (event)
	{
		case EVENT_COMMIT:
			FileBrowser_GetSelectedFiles (panelHandle, PANEL_FILEBROWSER, &files); //���filebrowers�б�ѡ�е��ļ�
			OpenDocumentInDefaultViewer(*files,VAL_NO_ZOOM); //��windowsĬ�Ϸ�ʽ��ѡ����ļ�
			free(files);//�ͷ��ļ�ָ��
			break;
	}
	return 0;
}


void drivevolume()
{
	 int  numberofdrives;
	 int currentdrivenumber;
	 int i;
	 int drive1=0;
	 char drive2[10];
	 unsigned int messagenumber=0;
	 
	 if(flag==0)
	 {
	 	GetDrive (&currentdrivenumber,&numberofdrives);	 //���ϵͳ����������
		j=numberofdrives;
		//���� ��Ӳ���̷�C��ʼ һֱ���������̷�������Ϊֹ
		for(i=0;i<numberofdrives-2;i++)
		{
			drive1='c'+i;
			Fmt(drive2,"%2c<%i",drive1);
			drive2[1]='\0';
			strcat(drive2,":");
			InsertListItem (panelHandle, PANEL_RING, -1, drive2, drive1);
			drive2[0]='\0';
		}
		flag=1;
		
	 }
	 GetDrive (&currentdrivenumber,&numberofdrives);	 //���ϵͳ����������  
	 
	 if (j!=numberofdrives)
	 {
	 	  DeleteListItem (panelHandle, PANEL_RING, 0, -1);
		  	//���� ��Ӳ���̷�C��ʼ һֱ���������̷�������Ϊֹ
		for(i=0;i<numberofdrives-2;i++)
		{
			drive1='c'+i;
			Fmt(drive2,"%2c<%i",drive1);
			drive2[1]='\0';
			strcat(drive2,":");
			InsertListItem (panelHandle, PANEL_RING, -1, drive2, drive1);
			drive2[0]='\0';
		}
		flag=1;
	 }
}

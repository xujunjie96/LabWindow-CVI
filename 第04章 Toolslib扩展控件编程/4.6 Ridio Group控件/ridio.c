#include <utility.h>
#include <ansi_c.h>
#include "toolbox.h"
#include "radioGroup.h"
#include <cvirte.h>		
#include <userint.h>
#include "ridio.h"

static int panelHandle, ctrlarray;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "ridio.uir", PANEL)) < 0)
		return -1;
	Radio_ConvertFromTree (panelHandle, PANEL_RADIOGROUP);	  //��tree�ؼ�ת��Ϊ��ѡ�ؼ�
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
	
		case EVENT_CLOSE:
			QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK help (int panel, int control, int event,
					  void *callbackData, int eventData1, int eventData2)
{
	char prjdir[MAX_PATHNAME_LEN];
	char filedir[MAX_PATHNAME_LEN];
	char filename[MAX_PATHNAME_LEN]; 
	int radiovalue;
	switch (event)
	{
		//�¼����������ǵ�ǰѡ���ֵ�����ı�ʱ
		case EVENT_VAL_CHANGED:
			GetCtrlVal (panelHandle, PANEL_RADIOGROUP, &radiovalue);	//���radiogroup������ֵ
			GetProjectDir (prjdir);  
			GetProjectDir (filedir);  
			strcat(prjdir,"\\input.hlp");
			switch(radiovalue)
			{
				case 0:
					//��ʾ��������
					SystemHelp ("prjdir", HELP_CONTENTS, 0, 0);
				break;
				case 1:
					//��ʾ��������
					SystemHelp ("prjdir", HELP_CONTEXT , 7, 0);
				break;
				case 2:
					 //��ʾ��������
					SystemHelp ("prjdir", HELP_PARTIALKEY,0,"");
				break;
				case 3:
					  //�ı��ļ����ݵ���ʾ��ʽ
					  filename[0]='\0';
					  strcat(filename,"notepad.exe");
					  strcat(filename," ");
					  strcat(filename,filedir); 
					  strcat(filename,"\\input.txt");
					  LaunchExecutable (filename);
				break;
				case 4:
					 //��ʾhtml��ʽ�İ����ļ�
					 filename[0]='\0';
					 strcat(filename,filedir); 
					 strcat(filename,"\\sounds.chm");
					 ShowHtmlHelp (filename, HH_DISPLAY_TOPIC, 0);
				break;
				case 5:
					//��windowsĬ�Ϸ�ʽ�������
					filename[0]='\0';
					strcat(filename,filedir) ;
					strcat(filename,"\\input.txt");
					OpenDocumentInDefaultViewer (filename, VAL_NO_ZOOM);
				break;
			}
			break;
	}
	return 0;
}
	 

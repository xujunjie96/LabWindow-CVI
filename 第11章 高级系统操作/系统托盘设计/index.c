#include "toolbox.h"
#include <cvirte.h>		
#include <userint.h>
#include "index.h"

static int panelHandle;
static int iconhandle;//ϵͳ���̾��
static int itemindex;
int CVICALLBACK iconCB (int iconHandle, int event, int eventData);//ϵͳ������Ӧ����¼��Ļص�����

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "index.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	//���ó�������ʱ�����ԣ�ʹ�䲻����������ʾ��ť 
	SetPanelAttribute (panelHandle, ATTR_HAS_TASKBAR_BUTTON, 0);
	SetSystemAttribute (ATTR_TASKBAR_BUTTON_VISIBLE, 0);  //����ʾ��������
	//װ��һ����ɫͼ�� ��װ��ϵͳ���̻ص�����
	InstallSysTrayIcon ("hide.ico", "ϵͳ����", iconCB, &iconhandle);
	
	RunUserInterface ();
	DiscardPanel (panelHandle);
	//ϵͳ�˳�ʱ��ж��ϵͳ���� �ͷ�ռ�õ���Դ
	DetachTrayIconMenu (iconhandle);
	RemoveSysTrayIcon (iconhandle);
	return 0;
}

int CVICALLBACK timer (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
		double xarray[7];
	unsigned long availablevirtual,availablepagefile,availablephysical,totalvirtual,totalpagefile,totalphysical,percentload;
	switch (event)
	{
		case EVENT_TIMER_TICK:
			GetMemoryInfo (&percentload, &totalphysical, &totalpagefile, &totalvirtual, &availablephysical, &availablepagefile, &availablevirtual);//�õ�������Ϣ
			xarray[0]=percentload;
			xarray[1]=totalphysical;
			xarray[2]=totalpagefile;
			xarray[3]=totalvirtual;
			xarray[4]=availablephysical;
			xarray[5]=availablepagefile;
			xarray[6]=availablevirtual;
			PlotStripChart (panelHandle, PANEL_STRIPCHART, xarray, 7, 0, 0, VAL_DOUBLE);
			break;
	}
	return 0;
}

int CVICALLBACK OkCallback (int panel, int control, int event,
							void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetPanelAttribute (panelHandle, ATTR_HAS_TASKBAR_BUTTON, 0);
			SetSystemAttribute (ATTR_TASKBAR_BUTTON_VISIBLE, 0);
			//ж��ϵͳ����
			DetachTrayIconMenu (iconhandle);
			RemoveSysTrayIcon (iconhandle);
			//����װ�ز�ɫɫͼ�� ��װ��ϵͳ���̻ص�����
			InstallSysTrayIcon ("show.ico", "ϵͳ����", iconCB, &iconhandle);
			AttachTrayIconMenu (iconhandle);//ʹϵͳ��Ӧ����һ��¼�
			//����ϵͳ�����Ҽ��˵� ��1��ʼ ���Ⱥ��ۼ�
			InsertTrayIconMenuItem (iconhandle, "��ʾ", &itemindex); // ��ʾ�˵���   itemindex=1
			InsertTrayIconMenuItem (iconhandle, NULL, &itemindex);//NULL��ʾ�ָ���  itemindex=2
			InsertTrayIconMenuItem (iconhandle, "����ʾ״̬", &itemindex);
			InsertTrayIconMenuItem (iconhandle, 0, &itemindex);//0��ʾ�ָ���  itemindex=4
			InsertTrayIconMenuItem (iconhandle, "�˳�", &itemindex);
			SetTrayIconMenuAttr (iconhandle, ATTR_POPUP_DEFAULT_ITEM, 3);//���ô���ʾ״̬���� ������Ӵ�
			SetTrayIconMenuItemAttr (iconhandle, 3, ATTR_CHECKED, 1);//���ô���ʾ״̬�˵�Ϊѡ��״̬
			SetPanelAttribute (panelHandle, ATTR_VISIBLE, 0);//�������
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
			//ϵͳ�˳�ʱ��ж��ϵͳ���� �ͷ�ռ�õ���Դ
			DetachTrayIconMenu (iconhandle);
			RemoveSysTrayIcon (iconhandle);
			QuitUserInterface (0);
			break;
	}
	return 0;
}

//ϵͳ������Ӧ����¼��Ļص�����
int CVICALLBACK iconCB (int iconHandle, int event, int eventData)
{
	int check;
	switch(event)
	{
		case EVENT_MENU_ITEM:
			if(eventData==1) //��ʾ�˵�
			{
				DetachTrayIconMenu (iconhandle);
				RemoveSysTrayIcon (iconhandle);	
				InstallSysTrayIcon ("hide.ico", "ϵͳ����", iconCB, &iconhandle); 
				SetPanelAttribute (panelHandle, ATTR_HAS_TASKBAR_BUTTON, 0);
				SetSystemAttribute (ATTR_TASKBAR_BUTTON_VISIBLE, 0);
				DisplayPanel (panelHandle); 
			}
			if(eventData==3) //����ʾ״̬
			{
				GetTrayIconMenuItemAttr (iconhandle, 3, ATTR_CHECKED, &check);
				SetTrayIconMenuItemAttr (iconhandle, 3, ATTR_CHECKED, !check);		//
			}
			if(eventData==5)//�˳�
			{
				QuitUserInterface (0);   	
			}
			break;
	}
	return 0;
}

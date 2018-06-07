#include "windows.h" 
#include <cvirte.h>		
#include <userint.h>
#include "specialdisplay.h"

static int panelHandle;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "specialdisplay.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	CloseCVIRTE ();
	return 0;
}

int CVICALLBACK panelCB (int panel, int event, void *callbackData,
						 int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_LEFT_DOUBLE_CLICK:
			QuitUserInterface(0);  //˫�����ʱ�˳�����
			break;
		case EVENT_CLOSE:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

int CVICALLBACK color (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	int color;
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal (panelHandle, PANEL_COLORNUM, &color); 
			SetPanelAttribute (panelHandle, ATTR_BACKCOLOR, color);//���������ɫ
			break;		
		case EVENT_VAL_CHANGED:

			break;
	}
	return 0;
}

int CVICALLBACK shape (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	int shapevalue,titlebar;
	int titlebarheight=0;
	HWND hwnd;
	RECT windowrect,clientrect,shaperect;//����RECT cvi��ΪRect windows��ΪRECT ע�����ִ�Сд
	int fullheight,fullwidth,verticalborderwidth,horizentalborderwidth;
	HRGN winrgn,inner;
	int arcvalue=0;
	
	switch (event)
	{
		case EVENT_COMMIT:
			GetPanelAttribute (panelHandle, ATTR_SYSTEM_WINDOW_HANDLE, (int*)&hwnd); //��ô��ھ��
			GetPanelAttribute (panelHandle, ATTR_TITLEBAR_VISIBLE, &titlebar);//��ñ������Ŀɼ���
			if(titlebar)
			{
				titlebarheight=GetSystemMetrics(SM_CYCAPTION);	 //WINDOWS SDK���� ��ô��ڱ���ĸ߶�
			}
			//����������ڵľ��η�Χ ���ڵı߿� ������ �������Լ��˵��������������
			//hwnd ���þ��η�Χ���Ǹ����ھ��
			//windowrect ��Ļ��������ͬ����װ�ص��Ǹ�����
			GetWindowRect(hwnd,&windowrect);
			GetClientRect(hwnd,&clientrect);//����ָ�����ڿͻ������εĴ�С
			fullheight=windowrect.bottom-windowrect.top;//����������߶�
			fullwidth=windowrect.right-windowrect.left;//������������
			verticalborderwidth=(fullheight-clientrect.bottom-titlebarheight)/2; //������������߿�����һ�߿�ĸ߶�
			horizentalborderwidth=(fullwidth-clientrect.right);//������������߿�����һ�߿�ĸ߶� 
			//�õ�����ͼ�ε�����
			shaperect.left= horizentalborderwidth;
			shaperect.top=titlebarheight+ verticalborderwidth;
			shaperect.right=clientrect.right+horizentalborderwidth;
			shaperect.bottom=clientrect.bottom+ verticalborderwidth+ titlebarheight;
			//��û�ͼ��״
			GetCtrlVal (panelHandle, PANEL_RING, &shapevalue);
			
			switch(shapevalue)
			{
				case 0:
					winrgn=CreateEllipticRgn(shaperect.left,shaperect.top,shaperect.right,shaperect.bottom);//����һ����Բ
					SetWindowRgn(hwnd,winrgn,1); //�ı䴰�ڵ�����
					break;
				case 1:
					GetCtrlVal (panelHandle, PANEL_NUMERICGAUGE, &arcvalue);
					winrgn=CreateRoundRectRgn(shaperect.left,shaperect.top,shaperect.right,shaperect.bottom,arcvalue,arcvalue);//����һ��Բ�Ǿ��� 
					SetWindowRgn(hwnd,winrgn,1); //�ı䴰�ڵ����� 
					break;
				case 2:
					GetCtrlVal (panelHandle, PANEL_NUMERICGAUGE, &arcvalue);
					winrgn=CreateEllipticRgn(shaperect.left,shaperect.top,shaperect.right,shaperect.bottom);//����һ����Բ
					inner=CreateEllipticRgn(shaperect.left+arcvalue,shaperect.top+arcvalue,shaperect.right-arcvalue,shaperect.bottom-arcvalue);//����һ����Բ
					CombineRgn(winrgn,winrgn,inner,RGN_DIFF);//���������ཻ�Ķ��������Ϊһ���µ�����
					DeleteObject(inner); //ɾ��gdi����
					SetWindowRgn(hwnd,winrgn,1); //�ı䴰�ڵ�����  
					break;
			}
			break;
			
		case EVENT_VAL_CHANGED:

			break;
	}
	return 0;
}

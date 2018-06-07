#include <utility.h>
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "game.h"

static int i=0;
static char *data;
static int hh=9;
static int vv=-8;

struct porperty{	//���� ball,bar,panelΪ�ṹ�����ͱ���
	int left;
	int top;
	int width;
	int height;
}ball,bar,panel1;

static int setpanelHandle; 
static int panelHandle;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "game.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	
	GetPanelAttribute(panelHandle,ATTR_HEIGHT,&panel1.height);	//������ĸߣ�������ֵ����panel1.height
	SetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_TOP,panel1.height-20);	//����bar�ĸ�Ϊpanel�߼�ȥ20
	
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

//��ʱ���ص�����
int CVICALLBACK timer (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK: 
			i++;
			GetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_LEFT,&bar.left);
			GetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_TOP,&bar.top);  
			GetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_HEIGHT,&bar.height);  
			GetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_WIDTH,&bar.width);  
			GetCtrlAttribute(panelHandle,PANEL_BALL,ATTR_LEFT,&ball.left);  
			GetCtrlAttribute(panelHandle,PANEL_BALL,ATTR_TOP,&ball.top);  
			GetCtrlAttribute(panelHandle,PANEL_BALL,ATTR_HEIGHT,&ball.height);  
			GetCtrlAttribute(panelHandle,PANEL_BALL,ATTR_WIDTH,&ball.width);  
			GetPanelAttribute(panelHandle,ATTR_HEIGHT,&panel1.height);  
			GetPanelAttribute(panelHandle,ATTR_WIDTH,&panel1.width); //ÿ���趨ʱ�� ��bar ball panel���»��λ������ 
			
			if((ball.left<=0)||((ball.left+ball.width)>=panel1.width))	//���ball����������������򷴷��򵯻�
			{
				hh=-1*hh; //���ҷ���ı�
				Beep();//����
			}
			if((ball.top<=0)||((ball.top>=bar.top-10)&&(ball.left>=bar.left)&&(ball.left<=bar.left+bar.width)))	//���ball��������ϻ���bar���򷴷��򵯻�
			{
				vv=-1*vv; //���·���ı�
				Beep();//����
			}
			
			ball.left+=hh;
			ball.top+=vv;
			SetCtrlAttribute(panelHandle,PANEL_BALL,ATTR_TOP,ball.top);
			SetCtrlAttribute(panelHandle,PANEL_BALL,ATTR_LEFT,ball.left);
			if(ball.top>=bar.top+40) //δ����ס �������PANEL_SET
			{
				SetCtrlAttribute(panelHandle, PANEL_TIMER,ATTR_ENABLED,0);
				setpanelHandle=LoadPanel(0,"game.uir",PANEL_SET);	//����panel_set���
				InstallPopup(setpanelHandle);
				SetCtrlVal(setpanelHandle,PANEL_SET_NUMERIC,(int)(i/20));	//ʱ����Ϊ����
				data=DateStr();	//��õ�ǰ������
				SetCtrlVal(setpanelHandle,PANEL_SET_STRING,data);
			}
			break;
	}
	return 0;
}

//ȷ����ť ���¿�ʼ��Ϸ
int CVICALLBACK ok (int panel, int control, int event,
					void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			hh=9;
			vv=-8;
			 SetCtrlAttribute(panelHandle,PANEL_BALL,ATTR_TOP,50);
			 SetCtrlAttribute(panelHandle,PANEL_BALL,ATTR_LEFT,50); 
			 SetCtrlAttribute(panelHandle, PANEL_TIMER,ATTR_ENABLED,1);
			 DiscardPanel (setpanelHandle);
			break;
	}
	return 0;
}

//�˳���Ϸ
int CVICALLBACK exitsys (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 QuitUserInterface(0);
			break;
	}
	return 0;
}

//panel���ص�����  
int CVICALLBACK panelcb (int panel, int event, void *callbackData,
						 int eventData1, int eventData2)
{	   
	int lowerasc;
	int vk;
	int asc;
	int barleft;
	switch (event)
	{
		case EVENT_KEYPRESS: //��Ӧ�����¼�

			vk=eventData1&VAL_VKEY_MASK;	//����ASCLL��ֵ
			asc=eventData1&VAL_ASCII_KEY_MASK; //�������ֵ
			lowerasc=tolower(asc);	//����Ϊ��д��ת��ΪСд
			switch(vk)
			{
				case VAL_RIGHT_ARROW_VKEY:  //�������Ϊ�ҷ��������bar������40
					GetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_LEFT,&barleft);
					SetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_LEFT,barleft+40); 
					break;
				case VAL_LEFT_ARROW_VKEY:  //�������Ϊ���������bar������40
					GetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_LEFT,&barleft);
					SetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_LEFT,barleft-40);
					break;
				default:
				break;
			}
			switch(lowerasc)
			{
				case 'd':  //�������Ϊd����bar������40
					GetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_LEFT,&barleft);
					SetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_LEFT,barleft+40); 
					break;
				case 'a':  //�������Ϊa����bar������40
					GetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_LEFT,&barleft);
					SetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_LEFT,barleft-40);
					break;
				default:
				break;
			}
			break;
		case EVENT_CLOSE:

			break;
		case EVENT_PANEL_SIZE:	  //���ߴ�ı� ���»�ȡ
			GetPanelAttribute(panelHandle,ATTR_HEIGHT,&panel1.height);	//������ĸߣ�������ֵ����panel1.height
			SetCtrlAttribute(panelHandle,PANEL_BAR,ATTR_TOP,panel1.height-20);	//����bar�ĸ�Ϊpanel�߼�ȥ20
			break;
	}
	return 0;
}

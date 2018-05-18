#include <ansi_c.h>
#include <utility.h>
#include <analysis.h>
#include <cvirte.h>		
#include <userint.h>
#include "�¶�.h"

static int panelHandle;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "�¶�.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK Acquire (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	static double max,min;
	static int max_index,min_index;
	int i,j,value;
	double datapoints[100];
	switch (event)
	{
		case EVENT_COMMIT:
			GetCtrlVal(panelHandle,PANEL_BINARYSWITCH,&value);		  //��ȡ����״̬
			if(value==1)
			{
				SetCtrlVal(panelHandle,PANEL_LED,1);	//����LED
				SetCtrlAttribute(panelHandle,PANEL_LED,ATTR_LABEL_TEXT,"��"); //LED��������Ϊ��
				for(i=0;i<100;i++)
				{
					datapoints[i]=100*rand()/32767.0;	//���������
					Delay(0.01);	//��ʱ0.01�� ��λs
					SetCtrlVal(panelHandle,PANEL_NUMERICTHERM,datapoints[i]);	//�����¶ȼƿؼ�ֵ
					PlotStripChartPoint(panelHandle,PANEL_STRIPCHART,datapoints[i]);	//����ͼ�й�����ʾ����
				}
				MaxMin1D(datapoints,100,&max,&max_index,&min,&min_index);	//�ҳ�������е����ֵ��Сֵ
				SetCtrlVal(panelHandle,PANEL_NUMERIC,max);	// 
				SetCtrlVal(panelHandle,PANEL_NUMERIC_2,min);	//�����Сֵ���õ��ؼ�
				SetCtrlVal(panelHandle,PANEL_BINARYSWITCH,0);		  //�رղ������� 
				SetCtrlVal(panelHandle,PANEL_LED,0);	//�ر�LED  
				SetCtrlVal(panelHandle,PANEL_NUMERICTHERM,0.00);	//�¶ȼƹ���
				SetCtrlAttribute(panelHandle,PANEL_LED,ATTR_LABEL_TEXT,"��"); //LED��������Ϊ��
			}
			else
			{
				SetCtrlVal(panelHandle,PANEL_LED,0);	//�ر�LED  
				SetCtrlVal(panelHandle,PANEL_NUMERICTHERM,0.00);	//�¶ȼƹ���
				SetCtrlAttribute(panelHandle,PANEL_LED,ATTR_LABEL_TEXT,"��"); //LED��������Ϊ��	
			}					    
			break;
	}
	return 0;
}

int CVICALLBACK Quit (int panel, int control, int event,
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

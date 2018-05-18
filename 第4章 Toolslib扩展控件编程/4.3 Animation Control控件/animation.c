#include "animate.h"
#include <cvirte.h>		
#include <userint.h>
#include "animation.h"

static int panelHandle;
//��ͣ�����б�־ 0Ϊ���� 1Ϊ��ͣ
static int flagpause=0;
int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "animation.uir", PANEL)) < 0)
		return -1;
	//��picture ring�ؼ�ת��Ϊanimation�ؼ�
	AnimateCtrl_ConvertFromPictRing (panelHandle, PANEL_ANIMATION);
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

int CVICALLBACK pause (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 if(flagpause==0)
			 {	   
				//����
				 AnimateCtrl_SetAttribute (panelHandle, PANEL_ANIMATION, ATTR_ANIMATE_ENABLED, 0);
			 	
			 }
			 flagpause=1;
			break;
	}
	return 0;
}

int CVICALLBACK restart (int panel, int control, int event,
						 void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			 if(flagpause==1)
			 {	   
				//��ͣ
				 AnimateCtrl_SetAttribute (panelHandle, PANEL_ANIMATION, ATTR_ANIMATE_ENABLED, 1);
			 	
			 }
			 flagpause=0;
			break;
	}
	return 0;
}

int CVICALLBACK quitsys (int panel, int control, int event,
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

int CVICALLBACK interval (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	double intervaltime;
	switch (event)
	{
		case EVENT_COMMIT:
		   GetCtrlVal (panelHandle, PANEL_NUMERIC, &intervaltime);
		   AnimateCtrl_SetAttribute (panelHandle, PANEL_ANIMATION, ATTR_ANIMATE_FRAME_INTERVAL, intervaltime);   //������֡���ʱ��
			break;
	}
	return 0;
}

int CVICALLBACK animationevent (int panel, int control, int event,
								void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_ANIMATE_PRE_FRAME:
			SetCtrlVal (panelHandle, PANEL_NUMERIC_CUR, eventData1);
			SetCtrlVal (panelHandle, PANEL_NUMERIC_NEXT, eventData2);

			break;
		case EVENT_ANIMATE_POST_FRAME:
			SetCtrlVal (panelHandle, PANEL_NUMERIC_CUR_2, eventData1);
			SetCtrlVal (panelHandle, PANEL_NUMERIC_NEXT_2, eventData2);
			break;
	}
	return 0;
}


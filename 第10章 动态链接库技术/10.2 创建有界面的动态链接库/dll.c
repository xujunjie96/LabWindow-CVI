#include <utility.h>
#include <cvirte.h>		
#include <userint.h>
#include "dll.h"
 //����ʱ��ѡ��build/target setting/embed porject uirs ,���Ὣuir�ļ�Ƕ��dll�ļ��ڣ�����ʱuir�ļ����Բ����ļ����ڣ�����dll�ļ���С����
int InitUIForDLL (void);
void DiscardUIObjectsForDLL (void);

static int panelHandle;
static int exehandle;
static char pathname[MAX_PATHNAME_LEN];


int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			if (InitCVIRTE (hinstDLL, 0, 0) == 0)
				return 0;	/* out of memory */
			break;
		case DLL_PROCESS_DETACH:
			//���cvirte.dll (cvi���п�)�Ѿ����ͷ� ���ܵ���cvi����
			if (!CVIRTEHasBeenDetached())		/* Do not call CVI functions if cvirte.dll has already been detached. */
			{
				DiscardUIObjectsForDLL();//�ͷ��û�������Դ
				CloseCVIRTE ();
			}
			break;
	}

	return 1;
}

//int __stdcall DllEntryPoint (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
//{
//	/* Included for compatibility with Borland */
//	//borland c++ builder��dll�������ڵ�
//	return DllMain (hinstDLL, fdwReason, lpvReserved);
//}


int InitUIForDLL (void)
{
	/* Call this function from the appropriate place in your code */
	/* to load and display startup panels.                        */
	//���н����dll������ڵ�
	if ((panelHandle = LoadPanelEx (0, "dll.uir", PANEL, __CVIUserHInst)) < 0)
		return -1;
	DisplayPanel (panelHandle);  //�����û�����
	 RunUserInterface ();	 //������������� ���潫��һ������
	/* Uncomment this call to RunUserInterface or call it from elsewhere */
	/* in your code to begin issuing user interface events to panels     */
	/* RunUserInterface (); */
	return 0;
}

void DiscardUIObjectsForDLL (void)
{
	/* Discard the panels loaded in InitUIForDLL */
	/* Call this function from the appropriate place in your code */
	/* to discard UI objects.                                     */
	 //�ͷ�inituifordll������ڵ㴦װ�صĽ��溯��
	if (panelHandle > 0)
	{
		DiscardPanel (panelHandle);
		panelHandle = 0;
	}
}

int CVICALLBACK QuitCallback (int panel, int control, int event,
							  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface (0);
			break;
		case EVENT_KEYPRESS:

			break;
	}
	return 0;
}

int CVICALLBACK terminate (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			TerminateExecutable (exehandle);   //��ֹexe�ļ������� �ͷ�exe������
			break;
	}
	return 0;
}

int CVICALLBACK lanch (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		    GetCtrlVal (panelHandle, PANEL_STRING, pathname);
			LaunchExecutableEx (pathname, LE_SHOWNORMAL, &exehandle);//�õ�exe������
			
			break;
	}
	return 0;
}

int CVICALLBACK openfile (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			if(FileSelectPopup ("", "*.exe", "*.exe", "��", VAL_OK_BUTTON, 0, 0, 1, 1, pathname))
		   {
		   	SetCtrlVal (panelHandle, PANEL_STRING, pathname);
		   }
			break;
	}
	return 0;
}

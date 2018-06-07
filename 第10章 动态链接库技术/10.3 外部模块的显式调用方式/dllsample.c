#include <userint.h>
#include <cvirte.h>

int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			if (InitCVIRTE (hinstDLL, 0, 0) == 0)
				return 0;	  /* out of memory */
			break;
		case DLL_PROCESS_DETACH:
			CloseCVIRTE ();
			break;
	}
	
	return 1;
}

int __stdcall DllEntryPoint (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	/* Included for compatibility with Borland */

	return DllMain (hinstDLL, fdwReason, lpvReserved);
}

void __stdcall function1(void)
{
	MessagePopup ("DLL��ʽ����", "����һ��DLL��ʽ����ʾ����");	//�޲����뷵��ֵ ��ʾ�����Ի���
}

int __stdcall function2(int x,int y)
{
	return(x+y);   //�з���ֵ
}

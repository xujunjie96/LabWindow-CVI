#include <windows.h>  
#include <cvirte.h>
  //����dllʱҪѡ��releaseģʽ�������ɾ�̬�����lib�ļ�  1 configurationҲ��releaseģʽ  2 targetsettingҲ��releaseģʽ��change��ѡ��.h�ļ� 
int __stdcall DllMain (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
		//��ʼ��cvi��rte װ��d11
		case DLL_PROCESS_ATTACH:
			if (InitCVIRTE (hinstDLL, 0, 0) == 0)
				return 0;	  /* out of memory */
			break;
		//�ر�cvi��rte ж��dll �ͷŷ������Դ
		case DLL_PROCESS_DETACH:
			CloseCVIRTE ();
			break;
	}
	
	return 1; //����ֵ1 ˵��dllmain������ʼ���ɹ�
}

int __stdcall DllEntryPoint (HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	/* Included for compatibility with Borland */
	//borland c++ builder��dll�������ڵ�
	return DllMain (hinstDLL, fdwReason, lpvReserved);
}

int __stdcall GetVirtualKey(int *Capslock,int *NumLock,int *ScrollLock,int *AsciiA)
{
	int asciiA=65;
	*Capslock=GetKeyState(VK_CAPITAL);//���capslock������ֵ 1Ϊ����״̬
	*NumLock=GetKeyState(VK_NUMLOCK);//���numlock������ֵ 1Ϊ����״̬
	*ScrollLock=GetKeyState(VK_SCROLL);//���scrolllock������ֵ 1Ϊ����״̬ 
	*AsciiA=GetKeyState(asciiA);//�����ĸA��ֵ��ֵ 1Ϊ����״̬ 
}

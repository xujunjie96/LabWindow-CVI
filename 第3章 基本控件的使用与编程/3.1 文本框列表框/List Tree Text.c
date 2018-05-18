#include <formatio.h>
#include <cvirte.h>		
#include <userint.h>  
#include "List Tree Text.h"

static int panelHandle;

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (0, "List Tree Text.uir", PANEL)) < 0)
		return -1;
	DisplayPanel (panelHandle);
	RunUserInterface ();
	DiscardPanel (panelHandle);
	return 0;
}

//�����ѡĿ¼��ť�ص�����
int CVICALLBACK clear (int panel, int control, int event,
					   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
		 	ClearListCtrl(panelHandle,PANEL_TREE);	//����ı�����
			InsertTextBoxLine(panelHandle,PANEL_TEXTBOX,-1,"�����ѡĿ¼");	//���ı���ĩβ���һ���ı� 	
			break;
	}
	return 0;
}

//ת�ư�ť�ص�����
int CVICALLBACK transfer (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	int maxitems;
	int i;
	char label[10];
	char Tag[32];
	int value;
	int checked;
	static int j=0,k;
	
	switch (event)
	{
		case EVENT_COMMIT:
			ClearListCtrl(panelHandle,PANEL_TREE);	//��������б�� ����
			InsertTextBoxLine(panelHandle,PANEL_TEXTBOX,-1,"ת�Ʋ���ʾĿ¼");	//���ı���ĩβ���һ���ı�
			j=0;
			GetNumListItems(panelHandle,PANEL_LISTBOX,&maxitems);	//����б����Ŀ����Ŀ
			for(i=0;i<maxitems;i++)
			{
				IsListItemChecked(panelHandle,PANEL_LISTBOX,i,&checked);	//����б����ָ����Ŀ��ѡ��״̬
				if(checked)
				{
					j++;
					GetValueFromIndex(panelHandle,PANEL_LISTBOX,i,&value);	//���ָ��������Ŀvalueֵ
					GetLabelFromIndex(panelHandle,PANEL_LISTBOX,i,label);	//���ָ��������Ŀ�ı���
					InsertTreeItem(panelHandle,PANEL_TREE,VAL_SIBLING,j-2,VAL_NEXT,label,"",Tag,value);//��ѡ�е���Ŀ���뵽ָ�������ؼ�
				}
			}
			break;
	}
	return 0;
}

//���밴ť�ص�����
int CVICALLBACK insert (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	char newitem[10];
	
	switch (event)
	{
		case EVENT_COMMIT:
			InsertTextBoxLine(panelHandle,PANEL_TEXTBOX,-1,"���Ŀ¼");	//���ı���ĩβ���һ���ı� 
			GetCtrlVal(panel,PANEL_STRING_INSERT,newitem);		//�õ�ָ��String�ؼ���ֵ
			InsertListItem(panelHandle,PANEL_LISTBOX,-1,newitem,0);	//���б������µ���Ŀ
			break;
	}
	return 0;
}

//�Ƴ���ť�ص�����
int CVICALLBACK delete (int panel, int control, int event,
						void *callbackData, int eventData1, int eventData2)
{
	int maxitems;
	char removeitem[10];
	int i;
	char label[10];
	
	switch (event)
	{
		case EVENT_COMMIT:
			InsertTextBoxLine(panelHandle,PANEL_TEXTBOX,-1,"�Ƴ�Ŀ¼");	//���ı���ĩβ���һ���ı� 	 
			GetCtrlVal(panel,PANEL_STRING_DELETE,removeitem);
			GetNumListItems(panelHandle,PANEL_LISTBOX,&maxitems);
			for(i=0;i<maxitems;i++)   
			{
				GetLabelFromIndex(panelHandle,PANEL_LISTBOX,i,label);	//���ָ��������Ŀ�ı���
				if(CompareStrings(label,0,removeitem,0,1)==0)   //�жϸñ����ָ�������Ƿ���һ��
				{
					DeleteListItem(panelHandle,PANEL_LISTBOX,i--,1);	//ɾ���б��ָ����Ŀ
					maxitems--;
				}
			}
			  
			break;
	}
	return 0;
}

int CVICALLBACK quit (int panel, int control, int event,
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

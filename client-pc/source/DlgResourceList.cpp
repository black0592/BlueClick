// DlgResourceList.cpp : implementation file
//

#include "stdafx.h"
#include "BlueClick.h"
#include "DlgResourceList.h"
#include "BlueClickDlg.h"
#include "BuffreeMessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgResourceList dialog


CDlgResourceList::CDlgResourceList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgResourceList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgResourceList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgResourceList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgResourceList)
	DDX_Control(pDX, IDC_LIST_RESOURCE, m_listResource);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgResourceList, CDialog)
	//{{AFX_MSG_MAP(CDlgResourceList)
	ON_COMMAND(ID_MENUITEM_START_DOWNLOAD, OnMenuitemStartDownload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgResourceList message handlers

BOOL CDlgResourceList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd *pParentWnd = GetParent();
	CRect parentRect;
	pParentWnd->GetClientRect(&parentRect);
	SetWindowPos(NULL, 0, 0, parentRect.Width(), parentRect.Height(), SWP_SHOWWINDOW); 
	
	CRect listRect;
	GetClientRect(&listRect);
	m_listResource.SetWindowPos(NULL, 0, 0, listRect.Width(), listRect.Height(), SWP_SHOWWINDOW); 
	
	m_listResource.SetExtendedStyle( LVS_EX_FLATSB | LVS_EX_FULLROWSELECT |
		LVS_EX_ONECLICKACTIVATE |LVS_EX_GRIDLINES | LVS_OWNERDRAWFIXED | LVS_EX_DOUBLEBUFFER);
	//������Դ�б���ͼ�иߣ�ǰ������Ҫ�����Ի��񣬼�LVS_OWNERDRAWFIXED
	m_listResource.SetItemHeight(40);
	m_listResource.ModifyStyle( LVS_OWNERDRAWFIXED, 0, 0 );//ȥ���Ի���
	m_listResource.SetColumnType(1, BUFFREE_COLUMN_ICON);//���õڶ�����ʾͼ��
	//	m_listResource.SetColumnType(3, BUFFREE_COLUMN_PROGRESS);//���õ�������ʾ������
	m_listResource.SetRightMenuID(IDR_MENU_RESOURCE);
	
	
	//�����Ҫע����ǣ���һ�п������Ϊ0������ʹ�õ�һ�У���Ϊ��һ�в������þ��ж���
	//����ͨ��GetClientRect()��ȡ���ĵ�һ�п�Ȳ����������ĵ�һ�п�ȣ����������б�Ŀ��
	m_listResource.InsertColumn(0, "��һ�У��ѷ���", LVCFMT_LEFT, 0, 0);
	m_listResource.InsertColumn(1, "����", LVCFMT_LEFT, 80, 1);//�ڶ���
	m_listResource.InsertColumn(2, "�ļ���", LVCFMT_LEFT, 200, 2);
	m_listResource.InsertColumn(3, "ʱ��", LVCFMT_LEFT, listRect.Width()-298, 3);//ͨ�����㣬ʹ�����һ�иպ������б���
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgResourceList::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN) {  
        if(pMsg->wParam == VK_ESCAPE) {  
            return true;  
        }
		if(pMsg->wParam == VK_RETURN) {  
            return true;  
        }  
    }	
	return CDialog::PreTranslateMessage(pMsg);	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgResourceList::OnMenuitemStartDownload() 
{
	// TODO: Add your command handler code here
	int nItem = m_listResource.GetNextItem(-1, LVNI_SELECTED);
	CString csFileType = m_listResource.GetItemText(nItem, 1);
	CString csFilename = m_listResource.GetItemText(nItem, 2);
	
	CBuffreeListCtrl *listDownload = &(((CBlueClickDlg*)(this->GetParent()->GetParent()))->m_dlgDownloadList.m_listDownload);
	
	int count = listDownload->GetItemCount();
	listDownload->InsertItem(count, "");
	listDownload->SetItemText(count, 1, csFileType);
	listDownload->SetItemText(count, 2, csFilename);
	listDownload->SetItemText(count, 3, "");
	listDownload->SetItemData(count, 0);
	
	CBuffreeMessageBox dlgMessage("�Ѽ������ض���");
	dlgMessage.DoModal();
}

// DlgDownloadList.cpp : implementation file
//

#include "stdafx.h"
#include "BlueClick.h"
#include "DlgDownloadList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgDownloadList dialog


CDlgDownloadList::CDlgDownloadList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDownloadList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDownloadList)
	//}}AFX_DATA_INIT
}


void CDlgDownloadList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDownloadList)
	DDX_Control(pDX, IDC_LIST_UPLOAD, m_listDownload);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDownloadList, CDialog)
	//{{AFX_MSG_MAP(CDlgDownloadList)
	ON_COMMAND(ID_MENUITEM_CONTINUE_DOWNLOAD, OnMenuitemContinueDownload)
	ON_COMMAND(ID_MENUITEM_PAUSE_DOWNLOAD, OnMenuitemPauseDownload)
	ON_COMMAND(ID_MENUITEM_DELETE_DOWNLOAD, OnMenuitemDeleteDownload)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDownloadList message handlers

BOOL CDlgDownloadList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd *pParentWnd = GetParent();
	CRect parentRect;
	pParentWnd->GetClientRect(&parentRect);
	SetWindowPos(NULL, 0, 0, parentRect.Width(), parentRect.Height(), SWP_SHOWWINDOW); 
	
	CRect listRect;
	GetClientRect(&listRect);
	m_listDownload.SetWindowPos(NULL, 0, 0, listRect.Width(), listRect.Height(), SWP_SHOWWINDOW); 
	
	m_listDownload.SetExtendedStyle( LVS_EX_FLATSB | LVS_EX_FULLROWSELECT |
		LVS_EX_ONECLICKACTIVATE | LVS_OWNERDRAWFIXED );
	//������Դ�б���ͼ�иߣ�ǰ������Ҫ�����Ի��񣬼�LVS_OWNERDRAWFIXED
	m_listDownload.SetItemHeight(40);
	m_listDownload.ModifyStyle( LVS_OWNERDRAWFIXED, 0, 0 );//ȥ���Ի���
	m_listDownload.SetColumnType(1, BUFFREE_COLUMN_ICON);//���õڶ�����ʾͼ��
	m_listDownload.SetColumnType(4, BUFFREE_COLUMN_PROGRESS);//���õ�������ʾ������
	m_listDownload.SetRightMenuID(IDR_MENU_DOWNLOAD);

	//�����Ҫע����ǣ���һ�п������Ϊ0������ʹ�õ�һ�У���Ϊ��һ�в������þ��ж���
	//����ͨ��GetClientRect()��ȡ���ĵ�һ�п�Ȳ����������ĵ�һ�п�ȣ����������б�Ŀ��
	m_listDownload.InsertColumn(0, "�ļ���ʶ", LVCFMT_LEFT, 0, 0);
	m_listDownload.InsertColumn(1, "����", LVCFMT_LEFT, 100, 1);//�ڶ���
	m_listDownload.InsertColumn(2, "�ļ���", LVCFMT_LEFT, 250, 2);
	m_listDownload.InsertColumn(3, "��С", LVCFMT_LEFT, 70, 3);
	m_listDownload.InsertColumn(4, "����", LVCFMT_LEFT, listRect.Width()-438, 4);//ͨ�����㣬ʹ�����һ�иպ������б���
	m_listDownload.InsertColumn(5, "״̬", LVCFMT_LEFT, 0, 5);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgDownloadList::PreTranslateMessage(MSG* pMsg) 
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
}

void CDlgDownloadList::OnMenuitemContinueDownload() 
{
	// TODO: Add your command handler code here
	
}

void CDlgDownloadList::OnMenuitemPauseDownload() 
{
	// TODO: Add your command handler code here
	
}

void CDlgDownloadList::OnMenuitemDeleteDownload() 
{
	// TODO: Add your command handler code here
	int nItem = m_listDownload.GetNextItem(-1, LVNI_SELECTED);
	m_listDownload.DeleteItem(nItem);
	
	UpdateData(TRUE);	
}
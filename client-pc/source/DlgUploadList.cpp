// DlgUploadList.cpp : implementation file
//

#include "stdafx.h"
#include "BlueClick.h"
#include "DlgUploadList.h"
#include "BlueClickDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUploadList dialog


CDlgUploadList::CDlgUploadList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUploadList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUploadList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgUploadList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUploadList)
	DDX_Control(pDX, IDC_LIST_UPLOAD, m_listUpload);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUploadList, CDialog)
	//{{AFX_MSG_MAP(CDlgUploadList)
	ON_COMMAND(ID_MENUITEM_NEW_SHARE, OnMenuitemNewShare)
	ON_COMMAND(ID_MENUITEM_DELETE_SHARE, OnMenuitemDeleteShare)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUploadList message handlers

BOOL CDlgUploadList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CWnd *pParentWnd = GetParent();
	CRect parentRect;
	pParentWnd->GetClientRect(&parentRect);
	SetWindowPos(NULL, 0, 0, parentRect.Width(), parentRect.Height(), SWP_SHOWWINDOW); 
	
	CRect listRect;
	GetClientRect(&listRect);
	m_listUpload.SetWindowPos(NULL, 0, 0, listRect.Width(), listRect.Height(), SWP_SHOWWINDOW); 
	
	m_listUpload.SetExtendedStyle( LVS_EX_FLATSB | LVS_EX_FULLROWSELECT |
		LVS_EX_ONECLICKACTIVATE | LVS_OWNERDRAWFIXED );
	//������Դ�б���ͼ�иߣ�ǰ������Ҫ�����Ի��񣬼�LVS_OWNERDRAWFIXED
	m_listUpload.SetItemHeight(40);
	m_listUpload.ModifyStyle( LVS_OWNERDRAWFIXED, 0, 0 );//ȥ���Ի���
	m_listUpload.SetColumnType(1, BUFFREE_COLUMN_ICON);//���õڶ�����ʾͼ��
	m_listUpload.SetColumnType(4, BUFFREE_COLUMN_PROGRESS);//���õ�������ʾ������
	m_listUpload.SetRightMenuID(IDR_MENU_UPLOAD);//�����Ҽ��˵�

	//�����Ҫע����ǣ���һ�п������Ϊ0��������ʾ��һ�У���Ϊ��һ�в������þ��ж���
	//����ͨ��GetClientRect()��ȡ���ĵ�һ�п�Ȳ����������ĵ�һ�п�ȣ����������б�Ŀ��
	//�ϴ��б�
	// 0�����ã����������洢��Դ·����
	// 1���ļ���׺��
	// 2���ļ�����
	// 3���ļ���С
	// 4�����������ִ���Ч�����������洢��Դ��ǩ��
	// 5���ļ�MD5
	// data������������
	m_listUpload.InsertColumn(0, "�ļ�·��", LVCFMT_LEFT, 0, 0);
	m_listUpload.InsertColumn(1, "����", LVCFMT_LEFT, 100, 1);//�ڶ���
	m_listUpload.InsertColumn(2, "�ļ���", LVCFMT_LEFT, 250, 2);
	m_listUpload.InsertColumn(3, "��С", LVCFMT_LEFT, 70, 3);
	m_listUpload.InsertColumn(4, "����", LVCFMT_LEFT, listRect.Width()-438, 4);//ͨ�����㣬ʹ�����һ�иպ������б���
	m_listUpload.InsertColumn(5, "��Դ��ʶ", LVCFMT_LEFT, 0, 5);//����ʾ

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgUploadList::PreTranslateMessage(MSG* pMsg) 
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

void CDlgUploadList::OnMenuitemNewShare() 
{
	// TODO: Add your command handler code here
	CBlueClickDlg * mainWnd = (CBlueClickDlg*)AfxGetMainWnd();
	mainWnd->AddNewShare();
}

void CDlgUploadList::OnMenuitemDeleteShare() 
{
	// TODO: Add your command handler code here
	int nItem = m_listUpload.GetNextItem(-1, LVNI_SELECTED);
	
	CBlueClickDlg *mainWnd = (CBlueClickDlg*)(this->GetParent()->GetParent());

	mainWnd->DeleteShare(nItem);
}
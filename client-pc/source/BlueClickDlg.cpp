// BlueClickDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BlueClick.h"
#include "BlueClickDlg.h"
#include "DlgSplash.h"
#include "DlgNewShare.h"
#include "DlgSystemSetting.h"
#include "DlgThemeSelecter.h"
#include "BuffreeMessageBox.h"
#include "FileMD5.h"
#include "Converter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	CBrush m_brushBg;
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CAnimateButton	m_btnOk;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg UINT OnNcHitTest(CPoint point);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDOK, m_btnOk);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CBlueClickDlg *mainWnd = (CBlueClickDlg*)AfxGetMainWnd();
	CString themePath = mainWnd->m_themePath;
	CString okBtnBmpPath = themePath + "btn-ok.bmp";
	
	CenterWindow();
	
	CRect rcRect;
	GetClientRect(&rcRect);
	m_btnOk.SetWindowPos(NULL, rcRect.Width()/2-30, rcRect.Height()-35, 60, 27, SWP_SHOWWINDOW);
	m_btnOk.LoadBitmap(okBtnBmpPath);
	
	CString bmpBgPath = themePath + _T("dlg-bg.bmp");
	HBITMAP hBmpBg = CBlueClickApp::LoadBmpFromFile(bmpBgPath);
	CBitmap bmpBg;
	bmpBg.Attach(hBmpBg);
	m_brushBg.CreatePatternBrush(&bmpBg);
	bmpBg.Detach();
	DeleteObject(hBmpBg);
	
	//CFont font;
	//font.CreatePointFont(120, "����");
	//	m_staticMsg.SetFont(&font);
	
	DWORD dwStyle = AW_BLEND;
	HINSTANCE hInst = LoadLibrary("User32.DLL");
	typedef BOOL (WINAPI MYFUNC(HWND, DWORD, DWORD));
	MYFUNC* AnimateWindow;
	AnimateWindow = (MYFUNC*)::GetProcAddress(hInst, "AnimateWindow");
	AnimateWindow(this->m_hWnd, 500, dwStyle);
	FreeLibrary(hInst);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if((CTLCOLOR_STATIC==nCtlColor))
    {
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(RGB(255,255,255));
		
        return  m_brushBg;// (HBRUSH)::GetStockObject(NULL_BRUSH);
    }
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CAboutDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rcRect;
	GetClientRect(&rcRect);
	pDC->FillRect(&rcRect, &m_brushBg);
	
	return TRUE;	
	//return CDialog::OnEraseBkgnd(pDC);
}

UINT CAboutDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ScreenToClient(&point);
	
	CRect rc;
	GetClientRect(&rc);
	if (rc.PtInRect(point)) {
		//UpdateData(FALSE);
		return HTCAPTION;
	} else {
		return CDialog::OnNcHitTest(point);
	}
}

void CAboutDlg::OnOK() 
{
	// TODO: Add extra validation here
	DWORD dwStyle = AW_BLEND;
	HINSTANCE hInst = LoadLibrary("User32.DLL");
	typedef BOOL (WINAPI MYFUNC(HWND, DWORD, DWORD));
	MYFUNC* AnimateWindow;
	AnimateWindow = (MYFUNC*)::GetProcAddress(hInst, "AnimateWindow");
	AnimateWindow(this->m_hWnd, 750, AW_HIDE | dwStyle);
	FreeLibrary(hInst);		
	
	CDialog::OnOK();
}

/////////////////////////////////////////////////////////////////////////////
// CBlueClickDlg dialog

CBlueClickDlg::CBlueClickDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBlueClickDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlueClickDlg)
	m_csKeyword = _T("");
	m_width = 780;
	m_height = 550;
	m_listItemHeight = 50;
	m_csServerAddr = "0.0.0.0";
	m_nServerPort = 5566;
	m_clientNum = 0;
	m_queryNum = 0;
	m_downloadNum = 0;
	m_serviceNum = 0;
	m_nServiceThrdIndex = 1;
	m_nRecvSize = 0;
	m_nResSize = 0;
	m_nSpeedDownload = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
}


void CBlueClickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlueClickDlg)
	DDX_Control(pDX, IDC_STATIC_PEER_NUM_ONLINE, m_staticSpeed);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_staticStatus);
	DDX_Control(pDX, IDC_STATIC_LOGO, m_staticLogo);
	DDX_Control(pDX, IDC_STATIC_CAPTION, m_staticCaption);
	DDX_Text(pDX, IDC_EDIT_SEARCH, m_csKeyword);
	DDX_Control(pDX, IDC_STATIC_TAB, m_staticListTab);
	DDX_Control(pDX, IDC_TREE_IDC_TREE_DOWNLOAD, m_treeDownload);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_editSearch);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BUTTON_UPLOAD_LIST_TAB, m_btnUploadListTab);
	DDX_Control(pDX, IDC_BUTTON_THEME, m_btnTheme);
	DDX_Control(pDX, IDC_BUTTON_RESOURCE_LIST_TAB, m_btnResourceListTab);
	DDX_Control(pDX, IDC_BUTTON_MIN, m_btnMin);
	DDX_Control(pDX, IDC_BUTTON_MENU, m_btnSysMenu);
	DDX_Control(pDX, IDC_BUTTON_DOWNLOAD_LIST_TAB, m_btnDownloadListTab);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBlueClickDlg, CDialog)
	//{{AFX_MSG_MAP(CBlueClickDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_MIN, OnButtonMin)
	ON_BN_CLICKED(IDC_BUTTON_MENU, OnButtonMenu)
	ON_BN_CLICKED(IDC_BUTTON_THEME, OnButtonTheme)
	ON_BN_CLICKED(IDC_BUTTON_RESOURCE_LIST_TAB, OnButtonResourceListTab)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD_LIST_TAB, OnButtonDownloadListTab)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD_LIST_TAB, OnButtonUploadListTab)
	ON_WM_ERASEBKGND()
	ON_WM_NCHITTEST()
	ON_MESSAGE(WM_SHOWTASK,OnShowTask)
	ON_COMMAND(ID_MENUITEM_SYSTEM_SETTING, OnMenuitemSystemSetting)
	ON_COMMAND(ID_MENUITEM_ABOUT_US, OnMenuitemAboutUs)
	ON_COMMAND(ID_MENUITEM_QUIT_PROG, OnMenuitemQuitProg)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(MSG_CLOSE_SOCKET, OnCloseSocket)
	ON_MESSAGE(MSG_CLOSE_THREAD, OnCloseThread)
	ON_MESSAGE(WM_DOWNLOAD_COMPLETE,OnFileComplete)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlueClickDlg message handlers

BOOL CBlueClickDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	//��Ա������ʼ��
	CBlueClickApp::GetWorkSpacePath(m_csWorkSpace);
	m_csConfigFilename = m_csWorkSpace + "/Config.ini";
	m_csResListFilename = m_csWorkSpace + "/ResList.bc";
	m_themePath = m_csWorkSpace + "/resource/theme/";
	for (int i = 0; i < BLUECLICK_MAX_SYN_COUNT; i++) {
		m_hThread[i] = NULL;
		m_Socket[i] = NULL;
	}

	CDlgSplash dlgSplash(this);
	if (dlgSplash.DoModal() != IDOK) {
		CDialog::OnCancel();
	}

	//������Դ·��
	CString bgBmpPath = m_themePath + "bg.bmp";
	CString logoBmpPath = m_themePath + "logo.bmp";
	CString cancelBtnBmpPath = m_themePath + "close-btn.bmp";
	CString minBtnBmpPath = m_themePath + "min-btn.bmp";
	CString menuBtnBmpPath = m_themePath + "menu-btn.bmp";
	CString themeBtnBmpPath = m_themePath + "theme-btn.bmp";
	CString searchBtnBmpPath = m_themePath + "search-btn.bmp";
	CString resouceBtnBmpPath = m_themePath + "resource-list-btn.bmp";
	CString downloadBtnBmpPath = m_themePath + "download-list-btn.bmp";
	CString uploadBtnBmpPath = m_themePath + "upload-list-btn.bmp";
	CString insertBtnBmpPath = m_themePath + "insert-btn.bmp";
	//���ÿؼ�����λ��	
	HDC hDC = ::GetDC(HWND(NULL));		// �õ���ĻDC  
	m_screenWidth = ::GetDeviceCaps(hDC,HORZRES); // ��Ļ��  
	m_screenHeight = ::GetDeviceCaps(hDC,VERTRES);	// ��Ļ��   
	::ReleaseDC(HWND(NULL),hDC);		// �ͷ�DC
	
	MoveWindow((m_screenWidth-m_width)/2, (m_screenHeight-m_height)/2, m_width, m_height, TRUE);
	
	m_staticCaption.SetWindowPos(NULL, 10, 5, 200, 20, SWP_SHOWWINDOW);
	m_staticLogo.SetWindowPos(NULL, 20, 25, 50, 50, SWP_SHOWWINDOW);
	m_staticStatus.SetWindowPos(NULL, 10, m_height-25, 400, 20, SWP_SHOWWINDOW);
	m_staticSpeed.SetWindowPos(NULL, m_width-150, m_height-25, 140, 20, SWP_SHOWWINDOW);
	m_btnCancel.SetWindowPos(NULL, CBlueClickDlg::m_width-45, 0, 45, 20, SWP_SHOWWINDOW);
	m_btnMin.SetWindowPos(NULL, CBlueClickDlg::m_width-80, 0, 35, 29, SWP_SHOWWINDOW);
	m_btnSysMenu.SetWindowPos(NULL, CBlueClickDlg::m_width-115, 0, 35, 29, SWP_SHOWWINDOW);
	m_btnTheme.SetWindowPos(NULL, CBlueClickDlg::m_width-150, 0, 35, 29, SWP_SHOWWINDOW);
	m_editSearch.SetWindowPos(NULL, CBlueClickDlg::m_width-179, 45, 140, 29, SWP_SHOWWINDOW);
	m_btnSearch.SetWindowPos(NULL, CBlueClickDlg::m_width-39, 45, 29, 29, SWP_SHOWWINDOW);
	m_staticListTab.SetWindowPos(NULL, 10, 80, CBlueClickDlg::m_width-20, this->m_height-110, SWP_SHOWWINDOW); 
	m_treeDownload.SetWindowPos(NULL, 10, 70, 0, this->m_height-80, SWP_SHOWWINDOW); 
	m_btnResourceListTab.SetWindowPos(NULL, 150, 45, 35, 29, SWP_SHOWWINDOW);
	m_btnDownloadListTab.SetWindowPos(NULL, 200, 45, 35, 29, SWP_SHOWWINDOW);
	m_btnUploadListTab.SetWindowPos(NULL, 250, 45, 35, 29, SWP_SHOWWINDOW);
//	m_btnInsert.SetWindowPos(NULL, 10, 20, 35, 29, SWP_SHOWWINDOW);

	m_editSearch.SetMargins(5, 5);

	//LIST TREE
//	m_treeDownload.SetBkMode( CBuffreeTreeCtrl::BK_MODE_FILL );

	//����б��Ӵ���
	CWnd *pTabPage = GetDlgItem(IDC_STATIC_TAB);
	m_dlgResourceList.Create(IDD_DLGRESOURCELIST_DIALOG, pTabPage);
	m_dlgResourceList.ShowWindow(SW_SHOW);
	
	m_dlgDownloadList.Create(IDD_DLGDOWNLOADLIST_DIALOG, pTabPage);
	m_dlgDownloadList.ShowWindow(SW_HIDE);
	
	m_dlgUploadList.Create(IDD_DLGUPLOADLIST_DIALOG, pTabPage);
	m_dlgUploadList.ShowWindow(SW_HIDE);
	
	//���������ڱ�������
	HBITMAP hBmpBg = CBlueClickApp::LoadBmpFromFile(bgBmpPath);
	HBITMAP hBmpLogo = CBlueClickApp::LoadBmpFromFile(logoBmpPath);
	CBitmap bmpBg;
	bmpBg.Attach(hBmpBg);
	m_brushBg.CreatePatternBrush(&bmpBg);
	bmpBg.Detach();
	DeleteObject(hBmpBg);

	//���ؿؼ�������Դ
	m_staticLogo.SetBitmap(hBmpLogo);
	m_btnCancel.LoadBitmap(cancelBtnBmpPath);
	m_btnMin.LoadBitmap(minBtnBmpPath);
	m_btnSysMenu.LoadBitmap(menuBtnBmpPath);
	m_btnTheme.LoadBitmap(themeBtnBmpPath);
	m_btnSearch.LoadBitmap(searchBtnBmpPath);
	m_btnResourceListTab.LoadBitmap(resouceBtnBmpPath);
	m_btnDownloadListTab.LoadBitmap(downloadBtnBmpPath);
	m_btnUploadListTab.LoadBitmap(uploadBtnBmpPath);
//	m_btnInsert.LoadBitmap(insertBtnBmpPath);
	UpdateData(FALSE);


	//��ʼ��������
	m_dlgSuspension = new CDlgSuspension();
	m_dlgSuspension->Create(IDD_DLGSUSPENSION_DIALOG,this);
	m_dlgSuspension->ShowWindow(SW_HIDE);

	m_dlgSuspension->OnUpdateTransparent(220);
/*
	int count = m_dlgResourceList.m_listResource.GetItemCount();
	m_dlgResourceList.m_listResource.InsertItem(count, "");
	m_dlgResourceList.m_listResource.SetItemText(count, 1, "rmvb");
	m_dlgResourceList.m_listResource.SetItemText(count, 2, "�һ�����ս�ջ���.rmvb");
	m_dlgResourceList.m_listResource.SetItemText(count, 3, "1035M");
	m_dlgResourceList.m_listResource.SetItemText(count, 4, "65A7CC070BEFAB5A6AE94A174BC8878B");
	m_dlgResourceList.m_listResource.SetItemData(count, 90);
	
	count = m_dlgResourceList.m_listResource.GetItemCount();
	m_dlgResourceList.m_listResource.InsertItem(count, "");
	m_dlgResourceList.m_listResource.SetItemText(count, 1, "zip");
	m_dlgResourceList.m_listResource.SetItemText(count, 2, "�㶮��.zip");
	m_dlgResourceList.m_listResource.SetItemText(count, 3, "1035M");
	m_dlgResourceList.m_listResource.SetItemText(count, 4, "65A7CC070BEFAB5A6AE94A174BC8878B");
	m_dlgResourceList.m_listResource.SetItemData(count, 50);
	
	count = m_dlgResourceList.m_listResource.GetItemCount();
	m_dlgResourceList.m_listResource.InsertItem(count, "");
	m_dlgResourceList.m_listResource.SetItemText(count, 1, "exe");
	m_dlgResourceList.m_listResource.SetItemText(count, 2, "360ɵ��ר��ɱ�����.exe");
	m_dlgResourceList.m_listResource.SetItemText(count, 3, "1035M");
	m_dlgResourceList.m_listResource.SetItemText(count, 4, "65A7CC070BEFAB5A6AE94A174BC8878B");
	m_dlgResourceList.m_listResource.SetItemData(count, 72);
	
	count = m_dlgResourceList.m_listResource.GetItemCount();
	m_dlgResourceList.m_listResource.InsertItem(count, "");
	m_dlgResourceList.m_listResource.SetItemText(count, 1, "txt");
	m_dlgResourceList.m_listResource.SetItemText(count, 2, "�����а�����ճ�.txt");
	m_dlgResourceList.m_listResource.SetItemText(count, 3, "1035M");
	m_dlgResourceList.m_listResource.SetItemText(count, 4, "65A7CC070BEFAB5A6AE94A174BC8878B");
	m_dlgResourceList.m_listResource.SetItemData(count, 10);
	
	count = m_dlgResourceList.m_listResource.GetItemCount();
	m_dlgResourceList.m_listResource.InsertItem(count, "");
	m_dlgResourceList.m_listResource.SetItemText(count, 1, "rmvb");
	m_dlgResourceList.m_listResource.SetItemText(count, 2, "�һ�����ս�ջ���.rmvb");
	m_dlgResourceList.m_listResource.SetItemText(count, 3, "1035M");
	m_dlgResourceList.m_listResource.SetItemText(count, 4, "65A7CC070BEFAB5A6AE94A174BC8878B");
	m_dlgResourceList.m_listResource.SetItemData(count, 90);
	
	count = m_dlgResourceList.m_listResource.GetItemCount();
	m_dlgResourceList.m_listResource.InsertItem(count, "");
	m_dlgResourceList.m_listResource.SetItemText(count, 1, "zip");
	m_dlgResourceList.m_listResource.SetItemText(count, 2, "�㶮��.zip");
	m_dlgResourceList.m_listResource.SetItemText(count, 3, "1035M");
	m_dlgResourceList.m_listResource.SetItemText(count, 4, "65A7CC070BEFAB5A6AE94A174BC8878B");
	m_dlgResourceList.m_listResource.SetItemData(count, 50);
	
	count = m_dlgResourceList.m_listResource.GetItemCount();
	m_dlgResourceList.m_listResource.InsertItem(count, "");
	m_dlgResourceList.m_listResource.SetItemText(count, 1, "exe");
	m_dlgResourceList.m_listResource.SetItemText(count, 2, "360ɵ��ר��ɱ�����.exe");
	m_dlgResourceList.m_listResource.SetItemText(count, 3, "1035M");
	m_dlgResourceList.m_listResource.SetItemText(count, 4, "65A7CC070BEFAB5A6AE94A174BC8878B");
	m_dlgResourceList.m_listResource.SetItemData(count, 72);
	
	count = m_dlgResourceList.m_listResource.GetItemCount();
	m_dlgResourceList.m_listResource.InsertItem(count, "");
	m_dlgResourceList.m_listResource.SetItemText(count, 1, "txt");
	m_dlgResourceList.m_listResource.SetItemText(count, 2, "�����а�����ճ�.txt");
	m_dlgResourceList.m_listResource.SetItemText(count, 3, "1035M");
	m_dlgResourceList.m_listResource.SetItemText(count, 4, "65A7CC070BEFAB5A6AE94A174BC8878B");
	m_dlgResourceList.m_listResource.SetItemData(count, 10);

	count = m_dlgResourceList.m_listResource.GetItemCount();
	m_dlgResourceList.m_listResource.InsertItem(count, "");
	m_dlgResourceList.m_listResource.SetItemText(count, 1, "rmvb");
	m_dlgResourceList.m_listResource.SetItemText(count, 2, "�һ�����ս�ջ���.rmvb");
	m_dlgResourceList.m_listResource.SetItemText(count, 3, "1035M");
	m_dlgResourceList.m_listResource.SetItemText(count, 4, "65A7CC070BEFAB5A6AE94A174BC8878B");
	m_dlgResourceList.m_listResource.SetItemData(count, 90);
	
	count = m_dlgResourceList.m_listResource.GetItemCount();
	m_dlgResourceList.m_listResource.InsertItem(count, "");
	m_dlgResourceList.m_listResource.SetItemText(count, 1, "zip");
	m_dlgResourceList.m_listResource.SetItemText(count, 2, "�㶮��.zip");
	m_dlgResourceList.m_listResource.SetItemText(count, 3, "1035M");
	m_dlgResourceList.m_listResource.SetItemText(count, 4, "65A7CC070BEFAB5A6AE94A174BC8878B");
	m_dlgResourceList.m_listResource.SetItemData(count, 50);
	
	count = m_dlgResourceList.m_listResource.GetItemCount();
	m_dlgResourceList.m_listResource.InsertItem(count, "");
	m_dlgResourceList.m_listResource.SetItemText(count, 1, "exe");
	m_dlgResourceList.m_listResource.SetItemText(count, 2, "360ɵ��ר��ɱ�����.exe");
	m_dlgResourceList.m_listResource.SetItemText(count, 3, "1035M");
	m_dlgResourceList.m_listResource.SetItemText(count, 4, "65A7CC070BEFAB5A6AE94A174BC8878B");
	m_dlgResourceList.m_listResource.SetItemData(count, 72);
	
	count = m_dlgResourceList.m_listResource.GetItemCount();
	m_dlgResourceList.m_listResource.InsertItem(count, "");
	m_dlgResourceList.m_listResource.SetItemText(count, 1, "txt");
	m_dlgResourceList.m_listResource.SetItemText(count, 2, "�����а�����ճ�.txt");
	m_dlgResourceList.m_listResource.SetItemText(count, 3, "1035M");
	m_dlgResourceList.m_listResource.SetItemText(count, 4, "65A7CC070BEFAB5A6AE94A174BC8878B");
	m_dlgResourceList.m_listResource.SetItemData(count, 10);

	count = m_dlgUploadList.m_listUpload.GetItemCount();
	m_dlgUploadList.m_listUpload.InsertItem(count, "");
	m_dlgUploadList.m_listUpload.SetItemText(count, 1, "rmvb");
	m_dlgUploadList.m_listUpload.SetItemText(count, 2, "���ֶ�����.rmvb");
	m_dlgUploadList.m_listUpload.SetItemText(count, 3, "20%");
	m_dlgUploadList.m_listUpload.SetItemData(count, 20);
*/
	m_listenSocket = new CListenSocket(this);
	
	if (m_listenSocket->Create(m_nHostPort, SOCK_STREAM) == NULL) {
		CBuffreeMessageBox dlgMessageBox("����001�������ʼ��ʧ��");
		dlgMessageBox.DoModal();

		return 0;
	}
	
	BOOL bRet = m_listenSocket->Listen();
	if (!bRet) {
		CBuffreeMessageBox dlgMessageBox("����002�������ʼ��ʧ��");
		dlgMessageBox.DoModal();

		return 0;
	}

	m_hThreadSearch = CreateThread(NULL, 0, InitThreadProc, this, 0, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBlueClickDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBlueClickDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBlueClickDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CBlueClickDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rcRect;
	GetClientRect(&rcRect);
	pDC->FillRect(&rcRect, &m_brushBg);
	
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CBlueClickDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if((CTLCOLOR_STATIC==nCtlColor))
    {
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(RGB(255,255,255));
		
        return  m_brushBg;// (HBRUSH)::GetStockObject(NULL_BRUSH);
    }
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

UINT CBlueClickDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	ScreenToClient(&point);
	
	CRect rc;
	GetClientRect(&rc);
	if (rc.PtInRect(point)) {
		//UpdateData(FALSE);
		return HTCAPTION;
	} else {
		return CDialog::OnNcHitTest(point);
	}
}

BOOL CBlueClickDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN) {  
        if(pMsg->wParam == VK_ESCAPE) {  
            return true;  
        }

//		if(pMsg->wParam == VK_RETURN) {  
//            return true;  
//      }  
    }
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CBlueClickDlg::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
//	this->ShowWindow(SW_HIDE);
	AnimateWindow(0);
	m_dlgSuspension->ShowWindow(SW_SHOW);
}

void CBlueClickDlg::OnButtonMin() 
{
	// TODO: Add your control notification handler code here
	AnimateWindow(0);
	ToTray();
}

LRESULT CBlueClickDlg::OnShowTask(WPARAM wParam,LPARAM lParam)
{
	if(wParam!=IDR_MAINFRAME)
		return 1;
	switch(lParam) {
	case WM_RBUTTONUP://�Ҽ�����ʱ������ݲ˵�������ֻ��һ�����رա�
		{
			LPPOINT lpoint=new tagPOINT;
			::GetCursorPos(lpoint);//�õ����λ��
			CMenu menu;
			menu.CreatePopupMenu();//����һ������ʽ�˵�
			menu.AppendMenu(MF_STRING,WM_DESTROY,"�ر�"); //���Ӳ˵���رա������������ϢWM_DESTROY�������ڣ������أ��������������
			menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,lpoint->y,this); //ȷ������ʽ�˵���λ��
			HMENU hmenu=menu.Detach();
			menu.DestroyMenu(); //��Դ����
			delete lpoint;
		}
		break;
	case WM_LBUTTONDBLCLK: //˫������Ĵ���
		{
//			this->ShowWindow(SW_SHOW);//�򵥵���ʾ���������¶�
			DeleteTray();
			AnimateWindow(1);
		}
		break;
	default:
		break;
	}
	return 0;
}

void CBlueClickDlg::ToTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd=this->m_hWnd;
	nid.uID=IDR_MAINFRAME;
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;
	nid.uCallbackMessage=WM_SHOWTASK;//�Զ������Ϣ����
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	
	strcpy(nid.szTip,"����������"); //��Ϣ��ʾ��
	Shell_NotifyIcon(NIM_ADD,&nid); //�����������ͼ��
//	ShowWindow(SW_HIDE); //����������
}

void CBlueClickDlg::DeleteTray()
{
	NOTIFYICONDATA nid;
	nid.cbSize=(DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd=this->m_hWnd;
	nid.uID=IDR_MAINFRAME;
	nid.uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP ;
	nid.uCallbackMessage=WM_SHOWTASK; //�Զ������Ϣ����
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	
	strcpy(nid.szTip,"����������"); //��Ϣ��ʾ��Ϊ���ƻ��������ѡ�
	Shell_NotifyIcon(NIM_DELETE,&nid); //��������ɾ��ͼ��
}

void CBlueClickDlg::OnButtonMenu() 
{
	// TODO: Add your control notification handler code here
	CMenu menu, *pPopup;
	menu.LoadMenu(IDR_MENU_SYSTEM);
	pPopup = menu.GetSubMenu(0);
	CPoint myPoint;

	ClientToScreen(&myPoint);
	GetCursorPos(&myPoint);
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, myPoint.x, myPoint.y+10, this);		
}

void CBlueClickDlg::OnButtonTheme() 
{
	// TODO: Add your control notification handler code here
	CDlgThemeSelecter dlgThemeSelector;
	dlgThemeSelector.DoModal();
}

void CBlueClickDlg::OnButtonResourceListTab() 
{
	// TODO: Add your control notification handler code here
	m_dlgDownloadList.ShowWindow(SW_HIDE);
	m_dlgUploadList.ShowWindow(SW_HIDE);	
	m_dlgResourceList.ShowWindow(SW_SHOW);		
}

void CBlueClickDlg::OnButtonDownloadListTab() 
{
	// TODO: Add your control notification handler code here
	m_dlgResourceList.ShowWindow(SW_HIDE);
	m_dlgUploadList.ShowWindow(SW_HIDE);
	m_dlgDownloadList.ShowWindow(SW_SHOW);
}

void CBlueClickDlg::OnButtonUploadListTab() 
{
	// TODO: Add your control notification handler code here
	m_dlgResourceList.ShowWindow(SW_HIDE);
	m_dlgDownloadList.ShowWindow(SW_HIDE);
	m_dlgUploadList.ShowWindow(SW_SHOW);	
}

void CBlueClickDlg::OnMenuitemSystemSetting() 
{
	// TODO: Add your command handler code here
	CDlgSystemSetting dlgSysSetting;
	dlgSysSetting.DoModal();
}

void CBlueClickDlg::OnMenuitemAboutUs() 
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CBlueClickDlg::OnMenuitemQuitProg() 
{
	// TODO: Add your command handler code here
	AnimateWindow(0);
	CDialog::OnCancel();
}


void CBlueClickDlg::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	if (m_hThreadSearch != NULL) {
		TerminateThread(m_hThreadSearch, 0);
	}

	UpdateData(TRUE);

	m_hThreadSearch = CreateThread(NULL, 0, SearchThreadProc, this, 0, NULL);
	OnButtonResourceListTab();
}

/*********************************************************
�������ƣ�StartDownload
�����������û�ѡ����Դ�����������¼����������̡߳�
���ߣ�	  ������
����ʱ�䣺2014-08-29
����˵����nItem��ѡ����Դ����Դ�б��е�����
�� �� ֵ����
*********************************************************/
void CBlueClickDlg::StartDownload(UINT nItem) 
{	
	CString csDownloadPath;
	SelectPath(csDownloadPath);
	// TODO: Add your command handler code here
	//��Դ�б�
	// 0������ʾ
	// 1���ļ���׺��
	// 2���ļ�����
	// 3���ļ���С
	// 4���ļ�MD5
	// data����Դ��Ƭ��
	CString csResExt = m_dlgResourceList.m_listResource.GetItemText(nItem, 1);
	CString csResName = m_dlgResourceList.m_listResource.GetItemText(nItem, 2);
	CString csResSize = m_dlgResourceList.m_listResource.GetItemText(nItem, 3);
	CString csResMD5 = m_dlgResourceList.m_listResource.GetItemText(nItem, 4);
	UINT nResPieceCount = (UINT)m_dlgResourceList.m_listResource.GetItemData(nItem);

	//�����б�
	// 0������ʾ���洢��ԴMD5
	// 1����Դ���ͣ��ļ���׺��
	// 2����Դ��
	// 3����Դ��С
	// data����Դ��Ƭ��
	int nDownloadItem = m_dlgDownloadList.m_listDownload.GetItemCount();
	m_dlgDownloadList.m_listDownload.InsertItem(nDownloadItem, "");
	m_dlgDownloadList.m_listDownload.SetItemText(nDownloadItem, 0, csResMD5);
	m_dlgDownloadList.m_listDownload.SetItemText(nDownloadItem, 1, csResExt);
	m_dlgDownloadList.m_listDownload.SetItemText(nDownloadItem, 2, csResName);
	m_dlgDownloadList.m_listDownload.SetItemText(nDownloadItem, 3, csResSize);
	m_dlgDownloadList.m_listDownload.SetItemData(nDownloadItem, 0);

	for (UINT i = 0; i < BLUECLICK_MAX_SYN_COUNT; i++) {
		CDownloadThread *pThread = (CDownloadThread*)AfxBeginThread(RUNTIME_CLASS(CDownloadThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		m_criticalSection.Lock();
		m_downloadThrdList.AddTail(pThread);
		m_criticalSection.Unlock();
		pThread->m_nThreadIndex = i;
		pThread->m_nListItem = nDownloadItem;
		pThread->m_csResMD5 = csResMD5;
		pThread->m_csSavePath = csDownloadPath;
		pThread->m_csServerAddr = m_csServerAddr;
		pThread->m_nServerPort = m_nServerPort;
		pThread->m_csHostAddr = m_csHostAddr;
		pThread->SetMainWnd(this);
		pThread->ResumeThread();
		Sleep(30);
	}

	//����ˢ�¶�ʱ��
	SetTimer(1,100,NULL);
	//�ٶ�ˢ�¶�ʱ��
	SetTimer(2,1000,NULL);

	//�л��������б���ͼ
	OnButtonDownloadListTab();
}

void CBlueClickDlg::DeleteShare(UINT nItem)
{
	m_dlgUploadList.m_listUpload.DeleteItem(nItem);

	CFile fileResList(m_csResListFilename, CFile::modeWrite);
	fileResList.Seek(nItem, CFile::SeekPosition::begin);
	char buf[1] = {'\0'};
	fileResList.Write(buf, 1);
	fileResList.Close();

	CBuffreeMessageBox dlgMessageBox("��ȡ������");
	dlgMessageBox.DoModal();
}

void CBlueClickDlg::ReceiveResourceList()
{
	char buf[BLUECLICK_MSG_BUF_SIZE];
	memset(buf, 0, BLUECLICK_MSG_BUF_SIZE);
	int nRet = m_searchSocket->Receive(buf, BLUECLICK_MSG_BUF_SIZE);

	if (nRet <= 0) {
		return;
	}

	cJSON *pRoot;
	pRoot=cJSON_Parse(buf);

	if (!pRoot) {
		//AfxMessageBox("����JSONʧ��");
		return;
	}

	CString msgType = cJSON_GetObjectItem(pRoot, "MsgType")->valuestring;
	if (msgType != "MsgOnlineResponse") {
		//AfxMessageBox("�������Ϣ����");
		return;
	}

	m_csServerAddr = cJSON_GetObjectItem(pRoot, "ServerIP")->valuestring;
	cJSON_Delete(pRoot);

	CBuffreeMessageBox dlgMessageBox(m_csServerAddr);
	dlgMessageBox.DoModal();
}

/*********************************************************
�������ƣ�AcceptClient
����������������Դ���ؿͻ��˵��������������ǰ�б��е�����������5�� �����µ����Ӽ���ͻ��б���
���ߣ�	  ������
����ʱ�䣺2014-08-29
����˵������
�� �� ֵ����
*********************************************************/
void CBlueClickDlg::AcceptClient()
{

}


/*********************************************************
�������ƣ�CloseSocket
�����������ر��׽���
���ߣ�	  ������
����ʱ�䣺2014-08-29
����˵����nIndex���׽����������е�����
�� �� ֵ����
*********************************************************/
void CBlueClickDlg::OnCloseSocket(WPARAM wParam, LPARAM lParam)
{
	UINT nSocketIndex = wParam;
	if (nSocketIndex >= BLUECLICK_MAX_SYN_COUNT || nSocketIndex < 0) {
		return;
	}
	if (m_Socket[nSocketIndex] != NULL) {
		return;
	}
	
	m_Socket[nSocketIndex]->Close();
	delete m_Socket[nSocketIndex];
	m_Socket[nSocketIndex] = NULL;
}

void CBlueClickDlg::OnCloseThread(WPARAM wParam, LPARAM lParam)
{
	UINT nThreadIndex = wParam;
	if (nThreadIndex >= BLUECLICK_MAX_SYN_COUNT || nThreadIndex < 0) {
		return;
	}
	if (m_hThread[nThreadIndex] != NULL) {
		return;
	}
	
	TerminateThread(m_hThread[nThreadIndex], 0);
	m_hThread[nThreadIndex] = NULL;
}


/*********************************************************
�������ƣ�ReceiveDownloadRequest
����������������Դ���ؿͻ��˵��������󣬲����������������
���ߣ�	  ������
����ʱ�䣺2014-08-29
����˵����downloadSocket������������Ϣ��SOCKETָ��
�� �� ֵ����
*********************************************************/
void CBlueClickDlg::ReceiveDownloadRequest(CDownloadSocket *downloadSocket)
{
	char buf[BLUECLICK_MSG_BUF_SIZE];

	memset(buf, 0, BLUECLICK_MSG_BUF_SIZE);
	int nRet = downloadSocket->Receive(buf, BLUECLICK_MSG_BUF_SIZE);
	if (nRet <= 0) {
		return;
	}

	AfxMessageBox(buf);

	cJSON *pRoot;
	pRoot=cJSON_Parse(buf);
	
	if (!pRoot) {
		//AfxMessageBox("����JSONʧ��");
		return;
	}
	
	CString msgType = cJSON_GetObjectItem(pRoot, "MsgType")->valuestring;
	if (msgType != "MsgDownloadRes") {
		CBuffreeMessageBox dlgMessageBox("�������Ϣ����");
		dlgMessageBox.DoModal();

		return;
	}
	CString resMD5 = cJSON_GetObjectItem(pRoot, "ResMD5")->valuestring;
	cJSON_Delete(pRoot);

	CBuffreeMessageBox dlgMessageBox(resMD5);
	dlgMessageBox.DoModal();

	pRoot=cJSON_CreateObject();
	cJSON_AddStringToObject(pRoot,"MsgType", "MsgDownloadResponse");   
	cJSON_AddStringToObject(pRoot,"ResMD5", "1C7D74B459AB47C0403BD27A2DEC77E2");
	cJSON_AddNumberToObject(pRoot, "ResSize", 122222);
	
	char *strJson = cJSON_Print(pRoot);
	memset(buf, 0, BLUECLICK_MSG_BUF_SIZE);
	strcpy(buf, strJson);
	cJSON_Delete(pRoot);
	delete strJson;
	
	downloadSocket->Send(buf, BLUECLICK_MSG_BUF_SIZE);	
}

/*********************************************************
�������ƣ�AddNewShare
������:   ��ȡ�����ļ�������Ϣ����ӹ����б�д�빲���б��ļ�
���ߣ�	  ������
����ʱ�䣺2014-08-29
����˵������
�� �� ֵ����
*********************************************************/
void CBlueClickDlg::AddNewShare() 
{
	CDlgNewShare dlgNewShare;
	if (dlgNewShare.DoModal() != IDOK) {
		return;
	}

	CString csResName = dlgNewShare.m_csFilename;
	CString csResPath = dlgNewShare.m_csFilePath;
	CString csResTag = dlgNewShare.m_csFileTags;
	CString csResExt = csResName.Right(csResName.GetLength() - csResName.Find("."));

	csResTag.Replace("��", "#");
	csResTag.Replace(",", "#");

	CFileFind finder;
	BOOL bRet = finder.FindFile(csResPath);  
	if( !bRet ) {  
		CBuffreeMessageBox dlgMessageBox("�ļ������ڣ�������ѡ��");
		dlgMessageBox.DoModal();
		
		return;
	}

	//�ϴ��б�
	// 0�����ã����������洢��Դ·����
	// 1���ļ���׺��
	// 2���ļ�����
	// 3���ļ���С
	// 4�����������ִ���Ч�����������洢��Դ��ǩ��
	// 5���ļ�MD5
	// data������������
	int count = m_dlgUploadList.m_listUpload.GetItemCount();
	m_dlgUploadList.m_listUpload.InsertItem(count, "");
	m_dlgUploadList.m_listUpload.SetItemText(count, 0, csResPath);
	m_dlgUploadList.m_listUpload.SetItemText(count, 1, csResExt);
	m_dlgUploadList.m_listUpload.SetItemText(count, 2, csResName);
	m_dlgUploadList.m_listUpload.SetItemText(count, 3, "");//�ļ���С�ִ������߳��м������
	m_dlgUploadList.m_listUpload.SetItemText(count, 4, csResTag);
	m_dlgUploadList.m_listUpload.SetItemText(count, 5, "");//MD5: ���ڼ�����ļ���MD5�ȽϺ�ʱ�����Է����߳��м��㲢��д����ֶ�

	m_dlgUploadList.m_listUpload.SetItemData(count, 0);

//	CBuffreeMessageBox dlgMessageBox("�Ѽ�������б�");
//	dlgMessageBox.DoModal();
	
	for (int i = 0; i < BLUECLICK_MAX_SYN_COUNT; i++) {
		if (m_hThread[i] == NULL) {
			memset(&m_uploadInfo[i], 0, sizeof(STRUCT_THREAD_PARAMETER));
			m_uploadInfo[i].m_mainWnd = this;
			m_uploadInfo[i].m_nUploadItem = count;
			m_uploadInfo[i].m_nThreadIndex = i;
			m_hThread[i] = CreateThread(NULL, 0, UploadThreadProc, &m_uploadInfo[i], 0, NULL);
			break;
		}
	}
}

/*********************************************************
�������ƣ�InitThreadProc
������:
���ߣ�	  ������
����ʱ�䣺2014-08-29
����˵����lpParameter��������ָ��
�� �� ֵ����
*********************************************************/
DWORD _stdcall InitThreadProc(LPVOID lpParameter) {
	CBlueClickDlg *pDlg = (CBlueClickDlg*)lpParameter;
	CBuffreeListCtrl *listResource = &(pDlg->m_dlgResourceList.m_listResource);
	CBuffreeListCtrl *listUpload = &(pDlg->m_dlgUploadList.m_listUpload);
	CString csKeyWord = pDlg->m_csKeyword;
	char buf[BLUECLICK_MSG_BUF_SIZE];
	CSearchSocket searchSocket(pDlg);

	if (!AfxSocketInit())
	{
		return 0;
	}

//	pDlg->m_listenSocket = new CListenSocket(pDlg);

//	if (pDlg->m_listenSocket->Create(6667, SOCK_STREAM) == NULL) {
//		AfxMessageBox("�����ʼ��ʧ��");
//		return 0;
//	}

//	BOOL bRet = pDlg->m_listenSocket->Listen();
//	if (!bRet) {
//		AfxMessageBox("�󶨶˿�ʧ��");
//		return 0;
//	}

	if (searchSocket.Create() == FALSE) {
		return 0;
	}

//	CString msg;
//	msg.Format("ip:%s, port:%d", pDlg->m_csServerAddr, pDlg->m_nServerPort);
//	AfxMessageBox(msg);
	if (!searchSocket.Connect(pDlg->m_csServerAddr, pDlg->m_nServerPort)) {
		pDlg->m_staticStatus.SetWindowText("��������ʧ�ܣ�������");
		//CBuffreeMessageBox dlgMessageBox("��������ʧ�ܣ�������");
		//dlgMessageBox.DoModal();
	
		return 0;
	}
	pDlg->m_staticStatus.SetWindowText("���������ӳɹ�");

	cJSON *pRoot, *pResArray, *pRes;
	pRoot=cJSON_CreateObject();
	cJSON_AddStringToObject(pRoot,"MsgType", "MsgGetPush");   
	cJSON_AddNumberToObject(pRoot,"NumWanted", 10);
	
	char *strJson = cJSON_Print(pRoot);
	memset(buf, 0, BLUECLICK_MSG_BUF_SIZE);
	strcpy(buf, strJson);		
	cJSON_Delete(pRoot);
	delete strJson;
	
	searchSocket.Send(buf, BLUECLICK_MSG_BUF_SIZE);
	
	memset(buf, 0, BLUECLICK_MSG_BUF_SIZE);
	int nRet = searchSocket.Receive(buf, BLUECLICK_MSG_BUF_SIZE);
	
	if (nRet <= 0) {
		return 0;
	}

	pRoot=cJSON_Parse(buf);
	if (!pRoot) {
		//AfxMessageBox("����JSONʧ��");
		return 0;
	}

	CString msgType = cJSON_GetObjectItem(pRoot, "MsgType")->valuestring;
	if (msgType != "ResList") {
		pDlg->m_staticStatus.SetWindowText("������Ϣ����ʧ��");
		//CBuffreeMessageBox dlgMessageBox("�������Ϣ����");
		//dlgMessageBox.DoModal();

		return 0;
	}
	pDlg->m_staticStatus.SetWindowText("������Ϣ����ɹ�");
	//��ȷ��������������ر��׽���
	searchSocket.Close();

	pResArray = cJSON_GetObjectItem(pRoot, "Resource");
	if (pResArray == NULL) {
		return 0;
	}
	UINT resCount = cJSON_GetObjectItem(pRoot, "ResCount")->valueint;//cJSON_GetArraySize(pResArray);

	for (UINT i = 0; i < resCount; i++) {
		
		pRes = cJSON_GetArrayItem(pResArray, i);
		if (pRes == NULL) {
			continue;
		}

		char *json = cJSON_Print(pRes);
		delete json;
		
		CString csResName = cJSON_GetObjectItem(pRes, "ResName")->valuestring;
		CConverter::UTF8toANSI(csResName);
		CString csResExt = csResName.Right(csResName.GetLength()-csResName.Find("."));
		CString csResSize = cJSON_GetObjectItem(pRes, "ResSize")->valuestring;
		CString csResMD5 = cJSON_GetObjectItem(pRes, "ResMD5")->valuestring;
		UINT nResPiece= cJSON_GetObjectItem(pRes, "ResPieceCount")->valueint;

		int count = listResource->GetItemCount();
		listResource->InsertItem(count, "");
		listResource->SetItemText(count, 0, csResMD5);
		listResource->SetItemText(count, 1, csResExt);
		listResource->SetItemText(count, 2, csResName);
		listResource->SetItemText(count, 3, csResSize);
		listResource->SetItemText(count, 4, csResMD5);
		listResource->SetItemData(count, nResPiece);
	}
	cJSON_Delete(pRoot);

	//�ϴ��б�
	// 0�����ã����������洢��Դ·����
	// 1���ļ���׺��
	// 2���ļ�����
	// 3���ļ���С
	// 4�����������ִ���Ч�����������洢��Դ��ǩ��
	// 5���ļ�MD5
	// data������������
	CFile file(pDlg->m_csResListFilename, CFile::modeRead);
	char status[BLUECLICK_MAX_SHARE_COUNT] = {'\0'};
	file.Read(status, BLUECLICK_MAX_SHARE_COUNT);

	STRUCT_SHARE_FILE_INFO fileInfo;
	for (i = 0; i < BLUECLICK_MAX_SHARE_COUNT; i++) {
		if (status[i] != '\0') {
			file.Read(&fileInfo, sizeof(STRUCT_SHARE_FILE_INFO));
			
			int count = listUpload->GetItemCount();
			listUpload->InsertItem(count, "");
			listUpload->SetItemText(count, 0, fileInfo.m_resPath);
			listUpload->SetItemText(count, 1, fileInfo.m_resExt);
			listUpload->SetItemText(count, 2, fileInfo.m_resName);
			listUpload->SetItemText(count, 3, fileInfo.m_resSize);
			listUpload->SetItemText(count, 4, fileInfo.m_resTag);
			listUpload->SetItemText(count, 5, fileInfo.m_resMD5);
			listUpload->SetItemData(count, 100);
		} else {
			file.Seek(sizeof(STRUCT_SHARE_FILE_INFO), CFile::SeekPosition::current);
		}
	}
	pDlg->m_staticStatus.SetWindowText("���");

	return 0;
}

/*********************************************************
�������ƣ�SearchThreadProc
������������Դ�����̣߳����������˷����������󣬲�����������
���ߣ�	  ������
����ʱ�䣺2014-08-29
����˵����lpParameter��������ָ��
�� �� ֵ����
*********************************************************/
DWORD _stdcall SearchThreadProc(LPVOID lpParameter) {
	CBlueClickDlg *pDlg = (CBlueClickDlg*)lpParameter;
	CBuffreeListCtrl *listResource = &(pDlg->m_dlgResourceList.m_listResource);
	CString csKeyWord = pDlg->m_csKeyword;
	char buf[BLUECLICK_MSG_BUF_SIZE];
	CSearchSocket searchSocket(pDlg);

	if (!AfxSocketInit())
	{
		return 0;
	}

	if (searchSocket.Create() == FALSE) {
		return 0;
	}

	if (!searchSocket.Connect(pDlg->m_csServerAddr, pDlg->m_nServerPort)) {
		pDlg->m_staticStatus.SetWindowText("����ʧ�ܣ�������");
		//CBuffreeMessageBox dlgMessageBox("����ʧ�ܣ�������");
		//dlgMessageBox.DoModal();
		
		return 0;
	}
	
	CConverter::ANSItoUTF8(csKeyWord);

	cJSON *pRoot, *pResArray, *pRes;
	pRoot=cJSON_CreateObject();
	cJSON_AddStringToObject(pRoot,"MsgType", "MsgQueryRes");   
	cJSON_AddStringToObject(pRoot,"QueryKey", csKeyWord.GetBuffer(0));

	char *strJson = cJSON_PrintUnformatted(pRoot);
	memset(buf, 0, BLUECLICK_MSG_BUF_SIZE);
	strcpy(buf, strJson);
	cJSON_Delete(pRoot);
	delete strJson;

	searchSocket.Send(buf, BLUECLICK_MSG_BUF_SIZE);

	memset(buf, 0, BLUECLICK_MSG_BUF_SIZE);
	int nRet = searchSocket.Receive(buf, BLUECLICK_MSG_BUF_SIZE);
	
	searchSocket.Close();
	
	if (nRet <= 0) {
		pDlg->m_staticStatus.SetWindowText("����ʧ�ܣ�������");
		return 0;
	}

	pRoot=cJSON_Parse(buf);
	
	if (!pRoot) {
		pDlg->m_staticStatus.SetWindowText("����ʧ�ܣ�������");
		//AfxMessageBox("����JSONʧ��");
		return 0;
	}
	
	CString msgType = cJSON_GetObjectItem(pRoot, "MsgType")->valuestring;
	if (msgType != "ResList") {
		pDlg->m_staticStatus.SetWindowText("����ʧ�ܣ�������");
//		CBuffreeMessageBox dlgMessageBox("�������Ϣ����");
//		dlgMessageBox.DoModal();

		return 0;
	}
	
	pResArray = cJSON_GetObjectItem(pRoot, "Resource");
	if (pResArray == NULL) {
		pDlg->m_staticStatus.SetWindowText("����ʧ�ܣ�������");
		return 0;
	}
	
	listResource->DeleteAllItems();
	UINT resCount = cJSON_GetArraySize(pResArray);
	for (UINT i = 0; i < resCount; i++) {
		pRes = cJSON_GetArrayItem(pResArray, i);
		if (pRes == NULL) {
			continue;
		}

		CString resName = cJSON_GetObjectItem(pRes, "ResName")->valuestring;
		CConverter::UTF8toANSI(resName);
		CString resExt = resName.Right(resName.GetLength()-resName.Find("."));
		CString resSize = cJSON_GetObjectItem(pRes, "ResSize")->valuestring;
		CString resMD5 = cJSON_GetObjectItem(pRes, "ResMD5")->valuestring;
		UINT nResPieceCount = cJSON_GetObjectItem(pRes, "ResPieceCount")->valueint;
		
		int count = listResource->GetItemCount();
		listResource->InsertItem(count, "");
		listResource->SetItemText(count, 0, resMD5);
		listResource->SetItemText(count, 1, resExt);
		listResource->SetItemText(count, 2, resName);
		listResource->SetItemText(count, 3, resSize);
		listResource->SetItemText(count, 4, resMD5);
		listResource->SetItemData(count, nResPieceCount);
	}
	pDlg->m_staticStatus.SetWindowText("���������");

	cJSON_Delete(pRoot);
	return 0;
}

/*********************************************************
�������ƣ�UploadThreadProc
������������Դ�ϴ��̣߳����������˷�����Դ��������
���ߣ�	  ������ 
����ʱ�䣺2014-09-01
����˵����lpParameter����Դ�����Ϣ�ṹ�壨��Դ���������б������ڵ�λ�ã�
�� �� ֵ����
*********************************************************/
DWORD _stdcall UploadThreadProc(LPVOID lpParameter) {
	STRUCT_THREAD_PARAMETER *param = (STRUCT_THREAD_PARAMETER*)lpParameter;
	CBlueClickDlg *pDlg = (CBlueClickDlg*)param->m_mainWnd;
	CBuffreeListCtrl *listUpload = &pDlg->m_dlgUploadList.m_listUpload;
	UINT nItem = param->m_nUploadItem;
	UINT nThreadIndex = param->m_nThreadIndex;
	CDownloadSocket uploadSocket;
	char buf[BLUECLICK_MSG_BUF_SIZE];
	delete param;

	for (int i = 0; i < 30; i++) {
		listUpload->SetItemData(nItem, i);
		Sleep(10);
	}

	//�ϴ��б�
	// 0�����ã����������洢��Դ·����
	// 1���ļ���׺��
	// 2���ļ�����
	// 3���ļ���С
	// 4�����������ִ���Ч�����������洢��Դ��ǩ��
	// 5���ļ�MD5
	// data������������
	CString csResPath = listUpload->GetItemText(nItem, 0);
	CFile file(csResPath, CFile::modeRead);
	UINT nFileLength = file.GetLength();
	file.Close();

	CString csResName = listUpload->GetItemText(nItem, 2);
	CString csResExt = listUpload->GetItemText(nItem, 1);
	CString csResSize = CBlueClickApp::GetFileSizeStr(nFileLength);
	CString csResTag = listUpload->GetItemText(nItem, 4);
	CString csResMD5 = CFileMD5::GetFileMD5(csResPath);
	CString csResOwner = pDlg->m_csHostMAC;
	UINT nResPieceCount = (nFileLength % (1024*256) == 0) ? (nFileLength / 1024 / 256) : ((nFileLength / 1024 / 256) + 1);

	//֮ǰû�����÷����б�������������ֶΣ������ﲹ��
	listUpload->SetItemText(nItem, 3, csResSize);
	listUpload->SetItemText(nItem, 5, csResMD5);

	//����ת��
	CConverter::ANSItoUTF8(csResName);
	CConverter::ANSItoUTF8(csResTag);

	cJSON *pRoot, *pRes;
	pRoot= cJSON_CreateObject();
	cJSON_AddStringToObject(pRoot,"MsgType", "MsgShareRes"); 

	pRes = cJSON_CreateObject();
	cJSON_AddStringToObject(pRes, "ResName", csResName);
	cJSON_AddStringToObject(pRes, "ResTag", csResTag);
	cJSON_AddStringToObject(pRes, "ResSize", csResSize);
	cJSON_AddStringToObject(pRes, "ResMD5", csResMD5);
	cJSON_AddStringToObject(pRes, "ResOwner", csResOwner);
	cJSON_AddNumberToObject(pRes, "ResPieceCount", nResPieceCount);

	cJSON_AddItemToObject(pRoot, "Resource", pRes);

	char *strJson = cJSON_PrintUnformatted(pRoot);
	memset(buf, 0, BLUECLICK_MSG_BUF_SIZE);
	strcpy(buf, strJson);
	cJSON_Delete(pRoot);
	delete strJson;

	if (!AfxSocketInit()) {
		pDlg->m_staticStatus.SetWindowText("����ʧ�ܣ�������");
		PostMessage(pDlg->m_hWnd, MSG_CLOSE_THREAD, nThreadIndex, NULL);
		return 0;
	}
	
	if (uploadSocket.Create() == FALSE) {
		pDlg->m_staticStatus.SetWindowText("����ʧ�ܣ�������");
		PostMessage(pDlg->m_hWnd, MSG_CLOSE_THREAD, nThreadIndex, NULL);
		return 0;
	}

	if (!uploadSocket.Connect(pDlg->m_csServerAddr, pDlg->m_nServerPort)) {
		pDlg->m_staticStatus.SetWindowText("����ʧ�ܣ�������");
		CBuffreeMessageBox dlgMessageBox("����003����������ʧ��");
		dlgMessageBox.DoModal();
		PostMessage(pDlg->m_hWnd, MSG_CLOSE_THREAD, nThreadIndex, NULL);
		return 0;
	}
	
	uploadSocket.Send(buf, BLUECLICK_MSG_BUF_SIZE);

	//�ϴ��б�
	// 0�����ã����������洢��Դ·����
	// 1���ļ���׺��
	// 2���ļ�����
	// 3���ļ���С
	// 4�����������ִ���Ч�����������洢��Դ��ǩ��
	// 5���ļ�MD5
	// data������������

	CConverter::UTF8toANSI(csResName);
	CConverter::UTF8toANSI(csResTag);

	STRUCT_SHARE_FILE_INFO fileInfo;
	strcpy(fileInfo.m_resPath, csResPath.GetBuffer(0));
	strcpy(fileInfo.m_resName, csResName.GetBuffer(0));
	strcpy(fileInfo.m_resExt, csResExt.GetBuffer(0));
	strcpy(fileInfo.m_resSize, csResSize.GetBuffer(0));
	strcpy(fileInfo.m_resTag, csResTag.GetBuffer(0));
	strcpy(fileInfo.m_resMD5, csResMD5.GetBuffer(0));

	//д�뱾�ط����ļ��б��ļ�
	CFile fileResList(pDlg->m_csResListFilename, CFile::modeReadWrite);
	char status[BLUECLICK_MAX_SHARE_COUNT] = {'\0'};
	fileResList.Read(status, BLUECLICK_MAX_SHARE_COUNT);
	
	for (i = 0; i < BLUECLICK_MAX_SHARE_COUNT; i++) {
		if (status[i] == '\0') {
			fileResList.Seek(i * sizeof(STRUCT_SHARE_FILE_INFO), CFile::SeekPosition::current);
			fileResList.Write(&fileInfo, sizeof(STRUCT_SHARE_FILE_INFO));

			fileResList.Seek(i, CFile::SeekPosition::begin);
			fileResList.Write((void*)"1", 1);
			break;
		}
	}
	fileResList.Close();

	//���·��������
	for (i = 30; i <= 100; i++) {
		listUpload->SetItemData(nItem, i);
		if (i < 85) {
			if (nFileLength >= BLUECLICK_GB_SIZE) {
				Sleep(50);
			} else if (nFileLength >= BLUECLICK_MB_SIZE) {
				Sleep(30);
			} else {
				Sleep(20);
			}
		} else {
			Sleep(10);
		}
	}

	/*
	memset(buf, 0, BLUECLICK_MSG_BUF_SIZE);
	int nRet = uploadSocket.Receive(buf, BLUECLICK_MSG_BUF_SIZE);
	
	if (nRet <= 0) {
		pDlg->m_hThreadUpload[nThreadIndex] = NULL;
		return 0;
	}
	
	pRoot=cJSON_Parse(buf);
	
	if (!pRoot) {
		//AfxMessageBox("����JSONʧ��");
		pDlg->m_hThreadUpload[nThreadIndex] = NULL;
		return 0;
	}
	
	CString msgType = cJSON_GetObjectItem(pRoot, "MsgType")->valuestring;
	if (msgType != "MsgDownloadResponse") {
		CBuffreeMessageBox dlgMessageBox("�������Ϣ����");
		dlgMessageBox.DoModal();
		pDlg->m_hThreadUpload[nThreadIndex] = NULL;
		return 0;
	}
	
	cJSON_Delete(pRoot);
	*/
	pDlg->m_staticStatus.SetWindowText("��Դ����ɹ�");

	uploadSocket.Close();
	PostMessage(pDlg->m_hWnd, MSG_CLOSE_THREAD, nThreadIndex, NULL);
	return 0;
}

/*********************************************************
�������ƣ�AnimateWindow
�����������������ڶ����������ڴ򿪻�ر�ʱ��������
���ߣ�	  ������
����ʱ�䣺2014-08-30
����˵����flag����ʶ���ڴ�(1)���߹ر�(0)
�� �� ֵ����
*********************************************************/
void CBlueClickDlg::AnimateWindow(UINT flag)
{
	if (flag == 0) {
		CRect rcRect;

		GetClientRect(&rcRect);
		while (rcRect.Height() > 10) {
			GetClientRect(&rcRect);

			int width = rcRect.Width();
			int height = rcRect.Height() - 30;
			height = (height > 10) ? height : 10;
			int left = (m_screenWidth - width) / 2;
			int top = (m_screenHeight - height) / 2;
			MoveWindow(left, top, width, height, TRUE);

			Sleep(15);
		}
		Sleep(100);

		while (rcRect.Width() > 0 ) {
			GetClientRect(&rcRect);
			
			int width = rcRect.Width() - 50;
			int height = rcRect.Height();
			if (width < 0) {
				width = 0;
				height = 0;
			}
			int left = (m_screenWidth - width) / 2;
			int top = (m_screenHeight - height) / 2;
			MoveWindow(left, top, width, height, TRUE);

			Sleep(15);
		}
		ShowWindow(SW_HIDE);
	} else {
		CRect rcRect;

		ShowWindow(SW_SHOW);
		GetClientRect(&rcRect);
		while (rcRect.Width() < m_width ) {
			GetClientRect(&rcRect);
			
			int width = rcRect.Width() + 50;
			width = (width > m_width) ? m_width : width;
			int height = 10;
			int left = (m_screenWidth - width) / 2;
			int top = (m_screenHeight - height) / 2;
			MoveWindow(left, top, width, height, TRUE);
			
			Sleep(15);
		}

		Sleep(100);

		while (rcRect.Height() < m_height) {
			GetClientRect(&rcRect);
			
			int width = rcRect.Width();
			int height = rcRect.Height() + 30;
			height = (height > m_height) ? m_height : height;
			int left = (m_screenWidth - width) / 2;
			int top = (m_screenHeight - height) / 2;
			MoveWindow(left, top, width, height, TRUE);
			
			Sleep(15);
		}
	}

	//OnCancel();	
}

LRESULT CBlueClickDlg::OnFileComplete(WPARAM wParam, LPARAM lParam)
{
	CString csTmpFilePath;
	CBlueClickApp::GetWorkSpacePath(csTmpFilePath);
	csTmpFilePath += "/temp/";

	//ɾ����ʱ�ļ�
	for (int i = 1; i < BLUECLICK_MAX_SYN_COUNT+1; i++) {
		CString filename;
		CFileFind filefind;
		filename.Format("%s%d.tmp",csTmpFilePath, i);
		
		if (filefind.FindFile(filename))
		{
			TRY
			{
				CFile::Remove(filename);
			}
			CATCH( CFileException, e )
			{
			}
			END_CATCH
		}
	}
	
	m_nRecvSize = 0;
	m_nEllipseTime = 0;

	KillTimer(1);
	KillTimer(2);
	m_staticStatus.SetWindowText("�������");
	m_staticSpeed.SetWindowText("");
	return 0;
	
}

void CBlueClickDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString strSpeed;
	CString strTime;
	CString strRatio;
	UINT progress;
	
	double r=0;
	switch (nIDEvent)
	{
		
	case 1:
		progress = m_nRecvSize / (m_nResSize / 100);
		m_dlgDownloadList.m_listDownload.SetItemData(0, progress);
		break;
		
	case 2:
		//������Ϣ���ٶȡ�ʣ��ʱ�䣩
		if (m_nSpeedDownload / 1024 < 1024) {
			strSpeed.Format("�����ٶ� %d Kb/s",m_nSpeedDownload / 1024);
		} else {
			strSpeed.Format("�����ٶ� %.2f Mb/s",m_nSpeedDownload/(float)(1024*1024));
		}
		
		m_staticSpeed.SetWindowText(strSpeed);
		
		//����ʱ���Լ�
		m_nEllipseTime++;
		if (m_nEllipseTime < 60) {
			strTime.Format("��ʱ��%d��",m_nEllipseTime);	
		} else {
			int m = m_nEllipseTime/60;
			int s = m_nEllipseTime%60;
			
			strTime.Format("��ʱ��%d��%d��", m, s);
		}

		m_nSpeedDownload = 0;
		
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CBlueClickDlg::SelectPath(CString &csPath)
{
	TCHAR	szFolderPath[MAX_PATH] = {0};  
	CString strFolderPath = TEXT("");  
	
	BROWSEINFO      sInfo;  
	::ZeroMemory(&sInfo, sizeof(BROWSEINFO));  
	sInfo.pidlRoot = 0;  
	sInfo.lpszTitle = _T("��ѡ��һ���ļ��У�");  
	sInfo.ulFlags = BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS  | BIF_EDITBOX;  
	sInfo.lpfn = NULL;  
	
	// ��ʾ�ļ���ѡ��Ի���  
	LPITEMIDLIST lpidlBrowse = ::SHBrowseForFolder(&sInfo);   
	if (lpidlBrowse != NULL) {  
		// ȡ���ļ�����  
		if (::SHGetPathFromIDList(lpidlBrowse,szFolderPath)) {  
			strFolderPath = szFolderPath;  
		}  
	} 
	if(lpidlBrowse != NULL) {  
		::CoTaskMemFree(lpidlBrowse);  
	}  
	
	csPath =  strFolderPath;
}

// DlgSplash.cpp : implementation file
//

#include "stdafx.h"
#include "BlueClick.h"
#include "DlgSplash.h"
#include "BlueClickDlg.h"
#include "DlgServerSetting.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSplash dialog


CDlgSplash::CDlgSplash(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSplash::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSplash)
	m_width = 400;
	m_height = 250;
	m_csConfigFilename = ((CBlueClickDlg*)m_pParentWnd)->m_csConfigFilename;
	m_csResListFilename = ((CBlueClickDlg*)m_pParentWnd)->m_csResListFilename;
	//}}AFX_DATA_INIT
}


void CDlgSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSplash)
	DDX_Control(pDX, IDC_STATIC_SPLASH_MSG, m_staticSplashMsg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSplash, CDialog)
	//{{AFX_MSG_MAP(CDlgSplash)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSplash message handlers

BOOL CDlgSplash::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	HDC hDC = ::GetDC(HWND(NULL));		// �õ���ĻDC
	int x = ::GetDeviceCaps(hDC,HORZRES); // ��Ļ��
	int y = ::GetDeviceCaps(hDC,VERTRES);	// ��Ļ��
	DeleteDC(hDC);
	
	MoveWindow((x-m_width)/2, (y-m_height)/2, m_width, m_height, TRUE);
	m_staticSplashMsg.SetWindowPos(NULL, 10, 210, 300, 30, SWP_SHOWWINDOW); 
	
	CRect rcRect;
	GetClientRect(&rcRect);	
	
	CString themePath = ((CBlueClickDlg*)m_pParentWnd)->m_themePath;
	CString bmpBgPath = themePath + _T("splash-bg.bmp");
	HBITMAP hBmpBg = CBlueClickApp::LoadBmpFromFile(bmpBgPath);
	CBitmap bmpBg;
	bmpBg.Attach(hBmpBg);
	m_brushBg.CreatePatternBrush(&bmpBg);
	bmpBg.Detach();
	DeleteObject(hBmpBg);

	DWORD dwStyle = AW_BLEND;
	HINSTANCE hInst = LoadLibrary("User32.DLL");
	typedef BOOL (WINAPI MYFUNC(HWND, DWORD, DWORD));
	MYFUNC* AnimateWindow;
	AnimateWindow = (MYFUNC*)::GetProcAddress(hInst, "AnimateWindow");
	AnimateWindow(this->m_hWnd, 750, dwStyle);
	FreeLibrary(hInst);

	m_staticSplashMsg.SetWindowText("���ڳ�ʼ��...");

	m_udpSocket = new COnlineSocket(this);

	if(!m_udpSocket->Create(0, SOCK_DGRAM, NULL)) {
		m_staticSplashMsg.SetWindowText("���绷����ʼ��ʧ�ܣ������˳�...");
		Close();
		return FALSE;
	}

	BOOL bBroadCast = true;
	m_udpSocket->SetSockOpt(SO_BROADCAST, &bBroadCast, sizeof(BOOL));

	m_hThreadInit = CreateThread(NULL, 0, SplashInitProc, this, 0, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgSplash::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rcRect;
	GetClientRect(&rcRect);
	pDC->FillRect(&rcRect, &m_brushBg);
	
	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CDlgSplash::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
    if((CTLCOLOR_STATIC==nCtlColor) && (pWnd==&m_staticSplashMsg))
    {
        pDC->SetBkMode(TRANSPARENT);
        pDC->SetTextColor(RGB(255,255,255));
        return  m_brushBg;// (HBRUSH)::GetStockObject(NULL_BRUSH);
    }
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgSplash::OnReceive()
{
	char buf[BLUECLICK_MSG_BUF_LENGTH];
	memset(buf, 0, BLUECLICK_MSG_BUF_LENGTH);
	int nRet = m_udpSocket->Receive(buf, BLUECLICK_MSG_BUF_LENGTH);

	if (nRet <= 0) {
		return;
	}

	cJSON *pRoot;
	pRoot=cJSON_Parse(buf);

	if (!pRoot) {
		//AfxMessageBox("����JSONʧ��");
		return;
	}

	CString csMsgType = cJSON_GetObjectItem(pRoot, "MsgType")->valuestring;
	if (csMsgType != "MsgOnlineResponse") {
		//AfxMessageBox("�������Ϣ����");
		return;
	}

	CString csMsgReserved = cJSON_GetObjectItem(pRoot, "Reserved")->valuestring;
	if (csMsgReserved != "null") {
		//AfxMessageBox("�������Ϣ����");
		return;
	}

	m_staticSplashMsg.SetWindowText("�ѷ��ַ���������������...");

	//����JSON��ȡ
	m_csServerAddr = cJSON_GetObjectItem(pRoot, "ServerIP")->valuestring;
	cJSON_Delete(pRoot);

	//AfxMessageBox(m_csServerAddr);

	WritePrivateProfileString("���������", "IP", m_csServerAddr, m_csConfigFilename);
	
	((CBlueClickDlg*)m_pParentWnd)->m_csServerAddr = m_csServerAddr;
	((CBlueClickDlg*)m_pParentWnd)->m_nServerPort = m_nServerPort;
	((CBlueClickDlg*)m_pParentWnd)->m_csHostAddr = m_csHostAddr;
	((CBlueClickDlg*)m_pParentWnd)->m_csHostMAC = m_csHostMAC;
	((CBlueClickDlg*)m_pParentWnd)->m_nHostPort = m_nHostPort;
	
	Sleep(2000);

	CDlgSplash::OnOK();
}

void CDlgSplash::OnOK() {
	if (m_hThreadInit != NULL) {
		TerminateThread(m_hThreadInit, 0);
	}

	CDialog::OnOK();
}

void CDlgSplash::Close()
{
	Sleep(2000);

	DWORD dwStyle = AW_BLEND;
	HINSTANCE hInst = LoadLibrary("User32.DLL");
	typedef BOOL (WINAPI MYFUNC(HWND, DWORD, DWORD));
	MYFUNC* AnimateWindow;
	AnimateWindow = (MYFUNC*)::GetProcAddress(hInst, "AnimateWindow");
	AnimateWindow(this->m_hWnd, 750, AW_HIDE | dwStyle);
	FreeLibrary(hInst);		

	//if (m_hThreadInit != NULL) {
	//	TerminateThread(m_hThreadInit, 0);
	//	m_hThreadInit = NULL;
	//}

	CDialog::OnCancel();
}

DWORD _stdcall SplashInitProc(LPVOID lpParameter) {
	CDlgSplash *pDlg = (CDlgSplash*)lpParameter;
	CString csConfigFilename = pDlg->m_csConfigFilename;
	CString csServerAddr, csHostAddr, csHostMAC;
	UINT nServerPort, nHostPort;
	char jsonOnline[BLUECLICK_MSG_BUF_LENGTH];

	//�����Ƿ���������ļ����������ڣ�������һ���µ�Ĭ�����õ�ini�ļ�
	CFileFind finder;
	BOOL bRet = finder.FindFile(pDlg->m_csConfigFilename);  
	if( !bRet ) {
		CBlueClickApp::GetHostAddress(csHostAddr);
		CBlueClickApp::GetHostMAC(csHostMAC);
		WritePrivateProfileString("���������", "IP", "0.0.0.0", csConfigFilename);
		WritePrivateProfileString("���������", "Port", "5566", csConfigFilename);
		WritePrivateProfileString("�ͻ�������", "IP", csHostAddr, csConfigFilename);
		WritePrivateProfileString("�ͻ�������", "MAC", csHostMAC, csConfigFilename);
		WritePrivateProfileString("�ͻ�������", "Port", "6666", csConfigFilename);
	}

	//��д���������ļ�
	GetPrivateProfileString("���������", "IP", "0.0.0.0", csServerAddr.GetBuffer(BLUECLICK_PROFILE_VAL_LENGTH), BLUECLICK_PROFILE_VAL_LENGTH, csConfigFilename);
	nServerPort = GetPrivateProfileInt("���������", "Port", 5566, csConfigFilename);
	GetPrivateProfileString("�ͻ�������", "IP", "0.0.0.0", csHostAddr.GetBuffer(BLUECLICK_PROFILE_VAL_LENGTH), BLUECLICK_PROFILE_VAL_LENGTH, csConfigFilename);
	GetPrivateProfileString("�ͻ�������", "MAC", "00-00-00-00-00-00", csHostMAC.GetBuffer(BLUECLICK_PROFILE_VAL_LENGTH), BLUECLICK_PROFILE_VAL_LENGTH, csConfigFilename);
	nHostPort = GetPrivateProfileInt("�ͻ�������", "Port", 6666, csConfigFilename);
	csServerAddr.ReleaseBuffer();
	csHostAddr.ReleaseBuffer();
	csHostMAC.ReleaseBuffer();

	//��ȡ������Ϣ
	//CBlueClickApp::GetHostAddress(csHostAddr);
	//CBlueClickApp::GetHostMAC(csHostMAC);

	//�����Ƿ������Դ�б��ļ����������ڣ�������һ���µ�Ĭ���б��ļ�
	//��Դ�б��ȡ���������ڳ�ʼ���߳�������ַ�����֮��
	bRet = finder.FindFile(pDlg->m_csResListFilename);  
	if( !bRet ) {
		CFile file(pDlg->m_csResListFilename, CFile::modeCreate|CFile::modeWrite);
		char buf[BLUECLICK_MAX_SHARE_COUNT] = {'\0'};

		file.Write(buf, BLUECLICK_MAX_SHARE_COUNT);
		file.Close();
	}

	//splash���ڳ�Ա������ֵ 
	pDlg->m_csServerAddr = csServerAddr;
	pDlg->m_nServerPort = nServerPort;
	pDlg->m_csHostAddr = csHostAddr;
	pDlg->m_csHostMAC = csHostMAC;
	pDlg->m_nHostPort = nHostPort;

	cJSON *pRoot;
	pRoot=cJSON_CreateObject();
	cJSON_AddStringToObject(pRoot,"MsgType", "MsgOnline");   
	cJSON_AddStringToObject(pRoot,"ClientIP", csHostAddr.GetBuffer(0));   
	cJSON_AddStringToObject(pRoot,"ClientMAC", csHostMAC.GetBuffer(0));
	cJSON_AddNumberToObject(pRoot,"ClientListenPort", nHostPort);

	char *strJson = cJSON_Print(pRoot);
	memset(jsonOnline, 0, BLUECLICK_MSG_BUF_LENGTH);
	strcpy(jsonOnline, strJson);		
	delete strJson;

	/*
	//��ʼ�����绷��
	if (!AfxSocketInit())
	{
		pDlg->m_staticSplashMsg.SetWindowText("���绷����ʼ��ʧ�ܣ������˳�...");
		pDlg->Close();
	}

	//pDlg->m_udpSocket = new COnlineSocket(pDlg);

	if(!pDlg->m_udpSocket->Create(0, SOCK_DGRAM, NULL)) {
		pDlg->m_staticSplashMsg.SetWindowText("���绷����ʼ��ʧ�ܣ������˳�...");
		pDlg->Close();
		return FALSE;
	}

	BOOL bBroadCast = true;
	pDlg->m_udpSocket->SetSockOpt(SO_BROADCAST, &bBroadCast, sizeof(BOOL));
	*/

	//���ӷ�����
	for (int i = 1; i <= 3; i++) {
		pDlg->m_staticSplashMsg.SetWindowText("�������ӷ�����...");
		pDlg->m_udpSocket->SendTo(jsonOnline, BLUECLICK_MSG_BUF_LENGTH, nServerPort, csServerAddr);
		Sleep(1000);
	}

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = INADDR_BROADCAST;
	addr.sin_port = htons(nServerPort);
	
	//�����������ַ��ȡ����ȡ����IP��ַ������'.'֮ǰ���ִ�
	UINT nDotPos = csHostAddr.Find(".");
	nDotPos = csHostAddr.Find(".", nDotPos+1);
	nDotPos = csHostAddr.Find(".", nDotPos+1);
	CString csAddrPrefix = csHostAddr.Left(nDotPos+1);
	
	//���ӷ�������ʱ, �Զ����ַ�����
	for (i = 1; i <= 5; i++) {
		CString csMsg = "���ӷ�����ʧ�ܣ���������������...";
		pDlg->m_staticSplashMsg.SetWindowText(csMsg);
	
		//���þ������㲥�ķ�ʽ���ַ�����������ʵ���з���̫��
		//for (int j = 1; j < 255; j++) {
		//	csServerAddr.Format("%s%d", csAddrPrefix, j);
		//	pDlg->m_udpSocket->SendTo(jsonOnline, BLUECLICK_MSG_BUF_LENGTH, nServerPort, csServerAddr, 0);
		//}
		
		//���þ������㲥�ķ�ʽ���ַ�������//����ʵ���з����еľ������ڷ��͹㲥��Ϣ��Ч����˸���һ��ѭ�������������������������������
		pDlg->m_udpSocket->SendTo(jsonOnline, BLUECLICK_MSG_BUF_LENGTH, (SOCKADDR*)&addr, sizeof(addr));
		Sleep(1000*i);
	}

	//CDlgServerSetting dlgServerSetting(pDlg);
	//if (dlgServerSetting.DoModal() == IDOK) {
	//	m_csServerAddr = dlgServerSetting.m_csServerAddr;
	//	m_nServerPort = dlgServerSetting.m_nServerPort;
	//	SetTimer(1, 0, NULL);
	//} else {
	pDlg->m_staticSplashMsg.SetWindowText("δ�ҵ������������������˳�...");
	pDlg->Close();
	//}
	return 0;
}

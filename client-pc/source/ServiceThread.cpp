// ServiceThread.cpp : implementation file
//

#include "stdafx.h"
#include "BlueClick.h"
#include "ServiceThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServiceThread

IMPLEMENT_DYNCREATE(CServiceThread, CWinThread)

CServiceThread::CServiceThread()
{
}

CServiceThread::~CServiceThread()
{
	m_nResSize = -1;
	m_nThreadIndex = -1;
	m_nPieceSize = -1;
	m_nPieceOffset = -1;
}

BOOL CServiceThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here

	if (!AfxSocketInit()) {
		SetThreadPriority(THREAD_PRIORITY_HIGHEST);
		PostThreadMessage(WM_QUIT,0,0);	
		return 0;
	}
	m_serviceSocket.Attach(m_hSocket);
	return TRUE;
}

int CServiceThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	CBlueClickDlg *pMainWnd = (CBlueClickDlg*)m_pWnd;
	
	//���߳�������ɾ����ǰ�߳�
	POSITION pos = pMainWnd->m_serviceThrdList.Find(this);
	if (pos!=NULL)
	{	
		pMainWnd->m_serviceThrdList.RemoveAt(pos);
	}
	//�ر�Socket
	if (m_serviceSocket != NULL)
	{
		m_serviceSocket.Close();
	}
	
	//	dlg->m_completedThread++;
	return CWinThread::ExitInstance();
}

void CServiceThread::GetPieceOffset(int nThreadNum, int nThreadIndex, DWORD nResSize, DWORD &nPieceOffset, DWORD &nPieceSize)
{
	DWORD nLastPieceSize;
	//����ļ���ƽ����С
	nPieceSize = nResSize / nThreadNum;
	//��ʣ�µ��ļ���С
	nLastPieceSize = nResSize;
	
	nPieceOffset = 0;
	//��õ�ThreadIndex���ļ��Ŀ�ʼλ�ʹ�С
	for (int i=1; i<= nThreadIndex-1; i++) {
		nLastPieceSize = nLastPieceSize - nPieceSize;
		nPieceOffset += nPieceSize;
	}
	
	//������ļ����һ�飬ȡ�����һ���С
	if(nThreadIndex == nThreadNum)
	{
		nPieceSize = nLastPieceSize;
	}

	return;	
}

void CServiceThread::SetMainWnd(CWnd *pWnd)
{
	m_pWnd = pWnd;
}

int CServiceThread::Run() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CWinThread::Run();
}


BEGIN_MESSAGE_MAP(CServiceThread, CWinThread)
	//{{AFX_MSG_MAP(CServiceThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServiceThread message handlers

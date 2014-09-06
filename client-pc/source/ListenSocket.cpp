// ListenSocket.cpp : implementation file
//

#include "stdafx.h"
#include "BlueClick.h"
#include "ListenSocket.h"
#include "BlueClickDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListenSocket

CListenSocket::CListenSocket(CWnd *pParentWnd)
{
	m_pParentWnd = pParentWnd;
}

CListenSocket::~CListenSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CListenSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CListenSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CListenSocket member functions

void CListenSocket::OnAccept(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_pWnd = AfxGetMainWnd();
	CBlueClickDlg *pMainWnd = (CBlueClickDlg*)m_pWnd;
	
	if (nErrorCode==0)
	{	
		CServiceSocket serviceSocket;
		pMainWnd->m_listenSocket->Accept(serviceSocket);
		//����һ�������߳�
		CServiceThread *pThread = (CServiceThread*)AfxBeginThread(RUNTIME_CLASS(CServiceThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
		
		//��ӵ��߳��б���
		pMainWnd->m_criticalSection.Lock();
		pMainWnd->m_serviceThrdList.AddTail(pThread);
		pMainWnd->m_criticalSection.Unlock();

		pThread->SetMainWnd(pMainWnd);
		// �̱߳���socketָ��
		pThread->m_hSocket=serviceSocket.Detach();
		//�����̱߳��
		pThread->m_nThreadIndex = pMainWnd->m_nServiceThrdIndex;

		//�����ļ���
		pThread->m_csResName;// = pMainWnd->m_csResName;
		//�����߳�		
		pThread->ResumeThread();
		//�̱߳����һ
		pMainWnd->m_nServiceThrdIndex++;
		//����ʼһ��������ʱ��m_threadIndex��Ϊ1
		if (pMainWnd->m_nServiceThrdIndex==BLUECLICK_MAX_SYN_COUNT+1)
		{
			pMainWnd->m_nServiceThrdIndex = 1;
		}
	}
	
	CAsyncSocket::OnAccept(nErrorCode);
}

void CListenSocket::SetMainWnd(CWnd *pWnd)
{
	m_pWnd = pWnd;
}
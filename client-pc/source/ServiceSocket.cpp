// ServiceSocket.cpp : implementation file
//

#include "stdafx.h"
#include "BlueClick.h"
#include "ServiceSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CServiceSocket

CServiceSocket::CServiceSocket()
{
}

CServiceSocket::~CServiceSocket()
{

}

void CServiceSocket::OnReceive(int nErrorCode) 
{
	CServiceThread *pThread = (CServiceThread*)AfxGetThread();
	CBlueClickDlg* pMainWnd = (CBlueClickDlg*)pThread->m_pWnd;
	CBuffreeListCtrl *listUpload = &pMainWnd->m_dlgUploadList.m_listUpload;

	char msgBuf[BLUECLICK_MSG_BUF_SIZE];	
	UINT nRet = Receive(msgBuf, BLUECLICK_MSG_BUF_SIZE);

	cJSON *pQueryMsgJson=cJSON_Parse(msgBuf);

	if (!pQueryMsgJson) {
		return;
	}

	CString csMsgType = cJSON_GetObjectItem(pQueryMsgJson, "MsgType")->valuestring;
	if (csMsgType != "MsgResPieceQuery") {
		return;
	}

	CString csQueryResMD5 = cJSON_GetObjectItem(pQueryMsgJson, "QueryResMD5")->valuestring;	
	UINT nQueryPieceId = cJSON_GetObjectItem(pQueryMsgJson, "QueryPieceId")->valueint;
	cJSON_Delete(pQueryMsgJson);

//	AfxMessageBox(csQueryResMD5);

	// ��ȡ�����б����ȡ�����ļ���Ϣ
	// 0�����ã����������洢��Դ·����
	// 1���ļ���׺��
	// 2���ļ�����
	// 3���ļ���С
	// 4�����������ִ���Ч�����������洢��Դ��ǩ��
	// 5���ļ�MD5
	// data������������
	CString csQueryResPath;
	CString csQueryResName;
	UINT nShareResCount = listUpload->GetItemCount();
	for (UINT i = 0; i < nShareResCount; i++) {
		CString csResMD5 = listUpload->GetItemText(i, 5);
		if (csResMD5 == csQueryResMD5) {
			csQueryResPath = listUpload->GetItemText(i, 0);
			csQueryResName = listUpload->GetItemText(i, 2);
			break;
		}
	}

	if (i >= nShareResCount) {
		pMainWnd->m_staticStatus.SetWindowText("������ļ�û���ҵ�");
		return;
	}

	DWORD ReadOnce, LeftToRead, nReadSize;
	CFile fileRes;
	//�Թ���Ͷ���ʽ���ļ�
	if(!fileRes.Open(csQueryResPath, CFile::shareDenyWrite) | CFile::modeRead)
	{
		return;
	}
	pThread->m_nResSize = fileRes.GetLength();
	
	//����ļ���ʼλ���ļ����С
	pThread->GetPieceOffset(BLUECLICK_MAX_SYN_COUNT, pThread->m_nThreadIndex, pThread->m_nResSize, pThread->m_nPieceOffset, pThread->m_nPieceSize);
	
	//��ʣ�µ��ļ���С
	LeftToRead = pThread->m_nPieceSize;
	fileRes.Seek(pThread->m_nPieceOffset, CFile::begin);

	RES_HEADER cResHeader;
	memset(cResHeader.m_csResName, 0, MAX_PATH);
	strcpy(cResHeader.m_csResName, csQueryResName.GetBuffer(0));
	cResHeader.m_nResOffset = pThread->m_nPieceOffset;
	cResHeader.m_nResSize = pThread->m_nResSize;
	cResHeader.m_nPieceSize = pThread->m_nPieceSize;
	cResHeader.m_nThreadIndex = pThread->m_nThreadIndex;

	Send(&cResHeader, sizeof(RES_HEADER));

	PIECE_DATA cPieceData;
	//��ȡ�ļ��������ļ���
	while(LeftToRead > 0) {	
		if (LeftToRead > BLUECLICK_RES_PIECE_SIZE) {
			ReadOnce = BLUECLICK_RES_PIECE_SIZE;
		}
		else
		{
			ReadOnce=LeftToRead;
		}
		//����һ���ļ�
		nReadSize = fileRes.Read(cPieceData.m_csData, BLUECLICK_RES_PIECE_SIZE);
		//�����ļ���
		nRet = Send(cPieceData.m_csData, nReadSize);
		LeftToRead = LeftToRead - nReadSize;
	}
	
	//�ر��ļ�
	fileRes.Close();
	pMainWnd->m_staticStatus.SetWindowText("�ļ��ѷ������");

	//֪ͨ���߳�ֹͣ
	//SetThreadPriority(THREAD_PRIORITY_HIGHEST);
	//PostThreadMessage(pThread, WM_QUIT,0,0);	
}
// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServiceSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CServiceSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServiceSocket member functions

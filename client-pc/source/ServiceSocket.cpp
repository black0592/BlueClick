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

typedef struct {
	CServiceSocket *m_socket;
	UINT m_socketIndex;
	char m_buf[BLUECLICK_MSG_BUF_SIZE];
}ServiceThreadParam;

/////////////////////////////////////////////////////////////////////////////
// CServiceSocket

CServiceSocket::CServiceSocket(CWnd *pParentWnd, UINT nSocketIndex)
{
	m_pParentWnd = pParentWnd;
	m_nSocketIndex = nSocketIndex;
}

CServiceSocket::~CServiceSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CServiceSocket, CSocket)
	//{{AFX_MSG_MAP(CServiceSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CServiceSocket member functions
#if 0
void CServiceSocket::OnReceive(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	char buf[BLUECLICK_MSG_BUF_SIZE];
	
	memset(buf, 0, BLUECLICK_MSG_BUF_SIZE);
	UINT nRet = Receive(buf, BLUECLICK_MSG_BUF_SIZE);
	if (nRet <= 0) {
		return;
	}

	ServiceThreadParam *param = new ServiceThreadParam;
	param->m_socket = this; 
	param->m_socketIndex = m_nSocketIndex;
	strcpy(param->m_buf, buf);

	m_hThreadService = CreateThread(NULL, 0, ServiceThreadProc, param, 0, NULL);
}

/*********************************************************
�������ƣ�ServiceThreadProc
������������Դ�����̣߳���������Դ���󷽷�������
���ߣ�	  ������
����ʱ�䣺2014-09-02
����˵����lpParameter������ָ��
�� �� ֵ����
*********************************************************/
DWORD _stdcall ServiceThreadProc(LPVOID lpParameter)
{
	ServiceThreadParam *param = (ServiceThreadParam*)lpParameter;
	CServiceSocket *serviceSocket = (CServiceSocket*)param->m_socket;
	UINT nSocketIndex = param->m_socketIndex;
	char msgBuf[BLUECLICK_MSG_BUF_SIZE];
	strncpy(msgBuf, param->m_buf, BLUECLICK_MSG_BUF_SIZE);
	delete param;

	cJSON *pQueryMsgJson=cJSON_Parse(msgBuf);
	if (!pQueryMsgJson) {
		return 0;
	}

	CString csMsgType = cJSON_GetObjectItem(pQueryMsgJson, "MsgType")->valuestring;
	if (csMsgType != "MsgResPieceQuery") {
		return 0;
	}

	CString csQueryResMD5 = cJSON_GetObjectItem(pQueryMsgJson, "QueryResMD5")->valuestring;	
	UINT nQueryPieceId = cJSON_GetObjectItem(pQueryMsgJson, "QueryPieceId")->valueint;
	cJSON_Delete(pQueryMsgJson);

	// ��ȡ�����б����ȡ�����ļ���Ϣ
	// 0�����ã����������洢��Դ·����
	// 1���ļ���׺��
	// 2���ļ�����
	// 3���ļ���С
	// 4�����������ִ���Ч�����������洢��Դ��ǩ��
	// 5���ļ�MD5
	// data������������
	CBlueClickDlg*	  mainWnd = (CBlueClickDlg*)AfxGetMainWnd();
	CBuffreeListCtrl* listUpload = &mainWnd->m_dlgUploadList.m_listUpload;

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
		mainWnd->m_staticStatus.SetWindowText("������ļ�û���ҵ�");
		return 0;
	}

	// �������ļ�����ȡ�ļ���Ϣ���Լ����͵�Ƭ������
	CFile queryFile(csQueryResPath, CFile::modeRead);
	UINT nQueryResSize = queryFile.GetLength();
	UINT nQueryPieceSize = BLUECLICK_RES_PIECE_SIZE;
	UINT nResPieceCount = (nQueryResSize % nQueryPieceSize == 0) ? (nQueryResSize/nQueryPieceSize) : (nQueryResSize/nQueryPieceSize) + 1;
	
	//��������Ƭ�β����ڣ� �򷵻�
	if (nQueryPieceId >= nResPieceCount || nQueryPieceId < 0) {
		mainWnd->m_staticStatus.SetWindowText("������ļ�Ƭ�β�����");
		queryFile.Close();
		return 0;
	}

	//�������������һ��Ƭ��
	if (nQueryPieceId == nResPieceCount - 1) {
		nQueryPieceSize = nQueryResSize % nQueryPieceSize;
	}

	//��ȡ����Ƭ������
	char queryPieceBuf[BLUECLICK_RES_PIECE_SIZE];
	memset(queryPieceBuf, 0, BLUECLICK_RES_PIECE_SIZE);
	UINT nFileOffset = nQueryPieceId*BLUECLICK_RES_PIECE_SIZE;
	queryFile.Seek(nFileOffset, CFile::SeekPosition::begin);
	queryFile.Read(queryPieceBuf, nQueryPieceSize);	
	queryFile.Close();
/*
	//����Ƭ����Ϣ
	cJSON *pResPiece = cJSON_CreateObject();
	cJSON_AddStringToObject(pResPiece, "MsgType", "MsgPieceQueryResponse");
	cJSON_AddNumberToObject(pResPiece, "QueryPieceId", nQueryPieceId);
	cJSON_AddNumberToObject(pResPiece, "QueryPieceSize", nQueryPieceSize);

	char *cpJson = cJSON_PrintUnformatted(pResPiece);
	char buf[BLUECLICK_MSG_BUF_SIZE];
	strcpy(buf, cpJson);
	free(cpJson);
	cJSON_Delete(pResPiece);

	serviceSocket->Send(buf, BLUECLICK_MSG_BUF_SIZE);
	mainWnd->m_staticStatus.SetWindowText("�����Ƭ����Ϣ�ѷ���");
*/
	//��������Ƭ������
	UINT nQueryPackCount = ((nQueryPieceSize%1024) == 0) ? (nQueryPieceSize/1024) : (nQueryPieceSize/1024)+1;
	UINT nQueryPackSize = 1024;
	for (UINT nQueryPackId = 0; nQueryPackId < nQueryPackCount; nQueryPackId++) {
		//���һ������Ҫ���������С
		if (nQueryPackId == nQueryPackCount-1) {
			nQueryPackSize = ((nQueryPieceSize%1024) == 0) ? 1024 : (nQueryPieceSize % 1024);
		}
		serviceSocket->Send(queryPieceBuf+nQueryPackId*1024, nQueryPackSize);
		
		//��ʾ����״̬��Ϣ
		CString csMsg;
		csMsg.Format("��%u�����ѷ���", nQueryPackId+1);
		mainWnd->m_staticStatus.SetWindowText(csMsg);
	}

	PostMessage(mainWnd->m_hWnd, MSG_CLOSE_SERVICE_SOCKET, nSocketIndex, NULL);

	return 0;
}

/*********************************************************
�������ƣ�OnClose
�����������������̵߳�CloseServiceSocket�������ر��׽��ֲ��ͷſռ�
���ߣ�	  ������
����ʱ�䣺2014-09-02
����˵����lpParameter������ָ��
�� �� ֵ����
*********************************************************/
void CServiceSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	CBlueClickDlg *mainWnd = (CBlueClickDlg*)AfxGetMainWnd();

	PostMessage(mainWnd->m_hWnd, MSG_CLOSE_SERVICE_SOCKET, m_nSocketIndex, NULL);

	CSocket::OnClose(nErrorCode);
}

#endif
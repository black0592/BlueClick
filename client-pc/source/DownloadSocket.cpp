// DownloadSocket1.cpp : implementation file
//

#include "stdafx.h"
#include "BlueClick.h"
#include "DownloadSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDownloadSocket

CDownloadSocket::CDownloadSocket()
{
	m_nPieceSize = 0;
	m_nRecvSize = 0;
}

CDownloadSocket::~CDownloadSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CDownloadSocket, CAsyncSocket)
	//{{AFX_MSG_MAP(CDownloadSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CDownloadSocket member functions

void CDownloadSocket::SetMainWnd(CWnd *pWnd)
{
	m_pWnd = pWnd;
}

void CDownloadSocket::OnConnect(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class

	CSocket::OnConnect(nErrorCode);
}

void CDownloadSocket::OnReceive(int nErrorCode) 
{
	CDownloadThread *pThread=(CDownloadThread*)AfxGetThread();
	CBlueClickDlg *pMainWnd=(CBlueClickDlg*)m_pWnd;
	PIECE_DATA cPieceData;
	RES_HEADER cResHeader;
	UINT nRet = 0;
	memset(&cPieceData, 0, sizeof(PIECE_DATA));

	CFile fileTmp;
	CFile fileRes;
	CFileFind fileFinder;
	CFileException e;

	CString csTmpFilePath;
	CBlueClickApp::GetWorkSpacePath(csTmpFilePath);
	csTmpFilePath += "/temp/";
	
	if (!CBlueClickApp::MakeDiectory(csTmpFilePath)) {
		return;
	}

	CString csDownloadPath = pThread->m_csSavePath + "\\";

	//���հ�ͷ
	if (!fileFinder.FindFile(m_csTmpResName)) {
		memset(cResHeader.m_csResName, 0, sizeof(RES_HEADER));
		nRet = Receive(&cResHeader, sizeof(RES_HEADER));
		if (nRet == sizeof(RES_HEADER)) {
			m_csTmpResName.Format("%s%d.tmp", csTmpFilePath, cResHeader.m_nThreadIndex);
			//������ʱ��־�ļ�
			if(!fileTmp.Open(m_csTmpResName, CFile::modeCreate | CFile::modeReadWrite)) {
				pMainWnd->m_staticStatus.SetWindowText("��ʱ�ļ�����ʧ��");
				return;
			}
			fileTmp.Close();

			////////////////////////////////////////////////
			m_csResName = cResHeader.m_csResName;
			m_csResName = csDownloadPath+m_csResName;

			//������Ϣ
			pThread->m_nPieceOffset = cResHeader.m_nResOffset;
			pThread->m_nThreadIndex = cResHeader.m_nThreadIndex;
			pThread->m_nPieceSize = cResHeader.m_nPieceSize;
			m_nPieceSize = cResHeader.m_nPieceSize;
			if (pThread->m_nThreadIndex==1) {
				//���������ļ�
				if (!fileRes.Open(m_csResName, CFile::modeCreate | CFile::modeReadWrite)) {
					pMainWnd->m_staticStatus.SetWindowText("�ļ�����ʧ��");
					return;
				}
				
				//�����ļ��ܳ���
				pMainWnd->m_nResSize = cResHeader.m_nResSize;
				fileRes.Close();
			}	
		}
		
	} else { // ����Ƭ������
		//��������	
		nRet = Receive(cPieceData.m_csData, BLUECLICK_RES_PIECE_SIZE * sizeof(char));
		//�����յ��ĵ�����������
		cPieceData.m_nSize = nRet;
		//��������ܵ���Ŀ
		pMainWnd->m_nRecvSize += nRet;
		//�����߳̽��յ���Ŀ
		m_nRecvSize += nRet;
		//�ٶȱ������
		pMainWnd->m_nSpeedDownload += nRet;	
		//������ݰ�С��10��5M��
		if (m_pieceDataList.GetCount() < 20)
		{
			m_pieceDataList.AddTail(cPieceData);	
		}
		//����5Mʱ��д���ļ�
		else {	
			m_pieceDataList.AddTail(cPieceData);	
			//�򿪱����ļ�
			if(!fileRes.Open(m_csResName, CFile::modeReadWrite | CFile::shareDenyNone, &e)) {
				if (e.m_cause==CFileException::sharingViolation) {
					pMainWnd->m_staticStatus.SetWindowText("�ļ���ʧ��");	
				}
				return;
			}	
			//����������д�뵽�ļ���
			//�ߵ����ʵ�λ��
			//file.Seek(pThread->m_threadBgPos+pThread->m_saveNum,CFile::begin);
			//�Լ�д��֧��64λ��Seek����
			CustomFileSeek((HWND)fileRes.m_hFile, pThread->m_nPieceOffset+pThread->m_nSaveSize, FILE_BEGIN);
			POSITION pos= m_pieceDataList.GetHeadPosition();
			while(pos != NULL) {	
				PIECE_DATA cData= m_pieceDataList.GetHead();
				//д���ļ�
				fileRes.Write(cData.m_csData, cData.m_nSize);
				//�����ļ�����
				pThread->m_nSaveSize += cData.m_nSize;
				
				m_pieceDataList.RemoveHead();
				pos=m_pieceDataList.GetHeadPosition();
			}			

			fileRes.Close();
			/////////////////////////
		}	
		
	}
	
	//2010.3.29�޸�
	//*����ܹؼ����Է������Ͷˣ��ر�Socket��ʱ��
	//*��߻���һ���������
	//*���Բ������Ϲر�
	if (m_nRecvSize == m_nPieceSize) {				
		if (m_pieceDataList.GetCount()!=0) {
			if (!fileRes.Open(m_csResName, CFile::modeReadWrite | CFile::shareDenyNone,&e)) {
				if (e.m_cause==CFileException::sharingViolation) {
					pMainWnd->m_staticStatus.SetWindowText("�ļ���ʧ��");	
				}
				return;
			}
			
			//����������д�뵽�ļ���
			//�ߵ����ʵ�λ��
			//file.Seek(pThread->m_threadBgPos+pThread->m_saveNum,CFile::begin);
			//�Լ�д��֧��64λ��Seek����
			CustomFileSeek((HWND)fileRes.m_hFile, pThread->m_nPieceOffset+pThread->m_nSaveSize, FILE_BEGIN);
	
			//����������д�뵽�ļ���
			POSITION pos= m_pieceDataList.GetHeadPosition();
			while (pos != NULL) {	
				PIECE_DATA cData= m_pieceDataList.GetHead();
				//д���ļ�
				fileRes.Write(cData.m_csData, cData.m_nSize);
				//�����ļ�����
				pThread->m_nSaveSize += cData.m_nSize;
				
				m_pieceDataList.RemoveHead();
				pos=m_pieceDataList.GetHeadPosition();
			}
			
			fileRes.Close();
			
		}

		//֪ͨ���߳�ֹͣ
		AfxGetThread()->PostThreadMessage(WM_QUIT,0,0);		
	}

	CAsyncSocket::OnReceive(nErrorCode);
}

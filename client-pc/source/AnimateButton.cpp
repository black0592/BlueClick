// AnimateButton.cpp : implementation file
//

#include "stdafx.h"
#include "AnimateButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_TIMER WM_USER + 1

/////////////////////////////////////////////////////////////////////////////
// CAnimateButton

CAnimateButton::CAnimateButton()
{
	m_brushNull.CreateStockObject(NULL_BRUSH);

	m_aniBtnWidth = 0;
	m_aniBtnHeight = 0;
	m_pMemDC = NULL;

	m_bAllowTrack = TRUE;

	//BLENDFUNCTION�ı���m_bf
	m_bf.BlendOp = AC_SRC_OVER;
	m_bf.BlendFlags = 0;
	m_bf.AlphaFormat = 0;

	//	m_nStartPos = 0;
	m_aniBtnState = STATE_NORMAL;
	//	m_bPressDown = FALSE;

	m_bEnable = TRUE;
	m_hCursor = NULL;
	m_strBtnText = _T("");
	// 	m_pLogfont = NULL;
	// 	m_pFontTxt = NULL;

	m_strSndHover = _T("");
	m_strSndClick = _T("");
	m_nIDSndHover = 0;
	m_nIDSndClick = 0;
}

CAnimateButton::~CAnimateButton()
{
	delete m_pMemDC;
	m_pMemDC = NULL;
}


BEGIN_MESSAGE_MAP(CAnimateButton, CButton)
	//{{AFX_MSG_MAP(CAnimateButton)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_ENABLE()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimateButton message handlers

//////////////////////////////////////////////////////////////////////////
// - ��̬����
BOOL CAnimateButton::Create(DWORD dwStyle, const POINT& pos, CWnd* pParentWnd, UINT nID)
{
	LPCTSTR lpszCaption = NULL;
	return CButton::Create(lpszCaption, dwStyle, CRect(pos, CSize(5, 5)), pParentWnd, nID);	
}

//////////////////////////////////////////////////////////////////////////
//��������BitmapToRegion
//���ܣ�����ͼ�������õ��ٳ����ɰ�ɫ������
//ԭ���ߣ�Jean-Edouard Lachand Robert, August 5, 1998
//�޸��ˣ�C�ϸ磨www.cguage.com��
HRGN CAnimateButton::BitmapToRegion(HBITMAP hBmp, int nSplit, int n, 
			COLORREF cTransparentColor = RGB(255, 0, 255), 	COLORREF cTolerance = RGB(255, 0, 255))
{
	HRGN hRgn = NULL;

	if (hBmp)
	{
		// Create a memory DC inside which we will scan the bitmap content
		HDC hMemDC = CreateCompatibleDC(NULL);
		if (hMemDC)
		{
			// Get bitmap size
			BITMAP bm;
			GetObject(hBmp, sizeof(bm), &bm);

			// Create a 32 bits depth bitmap and select it into the memory DC 
			BITMAPINFOHEADER RGB32BITSBITMAPINFO =
			{	
				sizeof(BITMAPINFOHEADER),	// biSize 
				bm.bmWidth,					// biWidth; 
				bm.bmHeight,				// biHeight; 
				1,							// biPlanes; 
				32,							// biBitCount 
				BI_RGB,						// biCompression; 
				0,							// biSizeImage; 
				0,							// biXPelsPerMeter; 
				0,							// biYPelsPerMeter; 
				0,							// biClrUsed; 
				0							// biClrImportant; 
			};

			//ÿ��״̬ͼ�Ŀ��
			int nBlockWidth = bm.bmWidth / nSplit;

			VOID * pbits32; 
			HBITMAP hbm32 = CreateDIBSection(hMemDC, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0);
			if (hbm32)
			{
				HBITMAP holdBmp = (HBITMAP)SelectObject(hMemDC, hbm32);

				// Create a DC just to copy the bitmap into the memory DC
				HDC hDC = CreateCompatibleDC(hMemDC);
				if (hDC)
				{
					// Get how many bytes per row we have for the bitmap bits (rounded up to 32 bits)
					BITMAP bm32;
					GetObject(hbm32, sizeof(bm32), &bm32);
					while (bm32.bmWidthBytes % 4)
						bm32.bmWidthBytes++;

					// Copy the bitmap into the memory DC
					HBITMAP holdBmp = (HBITMAP)SelectObject(hDC, hBmp);
					BitBlt(hMemDC, 0, 0, nBlockWidth, bm.bmHeight, hDC, nBlockWidth * n, 0, SRCCOPY);

					// For better performances, we will use the ExtCreateRegion() function to create the
					// region. This function take a RGNDATA structure on entry. We will add rectangles by
					// amount of ALLOC_UNIT number in this structure.
#define ALLOC_UNIT	100
					DWORD maxRects = ALLOC_UNIT;
					HANDLE hData = GlobalAlloc(GMEM_MOVEABLE, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects));
					RGNDATA *pData = (RGNDATA *)GlobalLock(hData);
					pData->rdh.dwSize = sizeof(RGNDATAHEADER);
					pData->rdh.iType = RDH_RECTANGLES;
					pData->rdh.nCount = pData->rdh.nRgnSize = 0;
					SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);

					// Keep on hand highest and lowest values for the "transparent" pixels
					BYTE lr = GetRValue(cTransparentColor);
					BYTE lg = GetGValue(cTransparentColor);
					BYTE lb = GetBValue(cTransparentColor);
					BYTE hr = min(0xff, lr + GetRValue(cTolerance));
					BYTE hg = min(0xff, lg + GetGValue(cTolerance));
					BYTE hb = min(0xff, lb + GetBValue(cTolerance));

					// Scan each bitmap row from bottom to top (the bitmap is inverted vertically)
					BYTE *p32 = (BYTE *)bm32.bmBits + (bm32.bmHeight - 1) * bm32.bmWidthBytes;
					for (int y = 0; y < bm.bmHeight; y++)
					{
						// Scan each bitmap pixel from left to right
						for (int x = 0; x < nBlockWidth; x++)
						{
							// Search for a continuous range of "non transparent pixels"
							int x0 = x;
							LONG *p = (LONG *)p32 + x;
							while (x < nBlockWidth)
							{

								BYTE b = GetRValue(*p);
								if (b >= lr && b <= hr)
								{
									b = GetGValue(*p);
									if (b >= lg && b <= hg)
									{
										b = GetBValue(*p);
										if (b >= lb && b <= hb)
											// This pixel is "transparent"
											break;
									}
								}
								p++;
								x++;
							}

							if (x > x0)
							{
								// Add the pixels (x0, y) to (x, y+1) as a new rectangle in the region
								if (pData->rdh.nCount >= maxRects)
								{
									GlobalUnlock(hData);
									maxRects += ALLOC_UNIT;
									hData = GlobalReAlloc(hData, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), GMEM_MOVEABLE);
									pData = (RGNDATA *)GlobalLock(hData);
								}
								RECT *pr = (RECT *)&pData->Buffer;
								SetRect(&pr[pData->rdh.nCount], x0, y, x, y+1);
								if (x0 < pData->rdh.rcBound.left)
									pData->rdh.rcBound.left = x0;
								if (y < pData->rdh.rcBound.top)
									pData->rdh.rcBound.top = y;
								if (x > pData->rdh.rcBound.right)
									pData->rdh.rcBound.right = x;
								if (y+1 > pData->rdh.rcBound.bottom)
									pData->rdh.rcBound.bottom = y+1;
								pData->rdh.nCount++;

								// On Windows98, ExtCreateRegion() may fail if the number of rectangles is too
								// large (ie: > 4000). Therefore, we have to create the region by multiple steps.
								if (pData->rdh.nCount == 2000)
								{
									HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
									if (hRgn)
									{
										CombineRgn(hRgn, hRgn, h, RGN_OR);
										DeleteObject(h);
									}
									else
										hRgn = h;
									pData->rdh.nCount = 0;
									SetRect(&pData->rdh.rcBound, MAXLONG, MAXLONG, 0, 0);
								}
							}
						}

						// Go to next row (remember, the bitmap is inverted vertically)
						p32 -= bm32.bmWidthBytes;
					}

					// Create or extend the region with the remaining rectangles
					HRGN h = ExtCreateRegion(NULL, sizeof(RGNDATAHEADER) + (sizeof(RECT) * maxRects), pData);
					if (hRgn)
					{
						CombineRgn(hRgn, hRgn, h, RGN_OR);
						DeleteObject(h);
					}
					else
						hRgn = h;

					// Clean up
					SelectObject(hDC, holdBmp);
					DeleteDC(hDC);
				}

				DeleteObject(SelectObject(hMemDC, holdBmp));
			}

			DeleteDC(hMemDC);
		}	
	}// if hBmp

	return hRgn;
}

//////////////////////////////////////////////////////////////////////////
// ����ͼƬ
BOOL CAnimateButton::LoadBitmap(CString szFileName, COLORREF clrTrans)
{
	HBITMAP hBitmap = (HBITMAP)::LoadImage(NULL, szFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	if(!hBitmap)
	{
		MessageBox(_T("������Դδ����ʧ��"), _T("����"), MB_OK | MB_ICONERROR);
		return FALSE;
	}

	m_clrTrans = clrTrans;
	PrepareBitmap(hBitmap);

	return TRUE;
}

BOOL CAnimateButton::LoadBitmap(UINT nIDBmp, COLORREF clrTrans)
{
	HBITMAP hBitmap = ::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIDBmp));
	m_clrTrans = clrTrans;
	PrepareBitmap(hBitmap);

	return TRUE;
}

void CAnimateButton::PrepareBitmap(HBITMAP hBitmap)
{
	int nStates = 4;

	//ȡ��ͼ���������򼰼����״̬��ť��������
	m_arBmpRgn.SetSize(nStates);
	m_arBmpRgn.SetSize(nStates);

	CRect rcTmp;
	for(int i = 0; i < nStates; i ++)
	{
		m_arBmpRgn[i] = BitmapToRegion(hBitmap, nStates, i);
	}

	CBitmap* pBmpWhole = CBitmap::FromHandle(hBitmap);
	BITMAP bmp;
	pBmpWhole->GetBitmap(&bmp);

	m_aniBtnWidth = bmp.bmWidth / nStates;
	m_aniBtnHeight = bmp.bmHeight;

	m_pMemDC = new CDC;
	CDC* pDC = GetDC();
	m_pMemDC->CreateCompatibleDC(pDC);
	m_pMemDC->SelectObject(pBmpWhole);
	ReleaseDC(pDC);

 	CRgn RgnWnd;
 	RgnWnd.CreateRectRgn(0, 0, m_aniBtnWidth, m_aniBtnHeight);
 	SetWindowRgn(HRGN(RgnWnd), FALSE);
	//������С(������DrawItem����ſ����޶����ķ�Χ)
	SetWindowPos(NULL, 0, 0, m_aniBtnWidth, m_aniBtnHeight, SWP_NOMOVE);
}

//////////////////////////////////////////////////////////////////////////
// ���������Ӱ�ť�Ի���
void CAnimateButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CButton::PreSubclassWindow();

	ModifyStyle(0, BS_OWNERDRAW);
}

//////////////////////////////////////////////////////////////////////////
// ʹ����͸���������������б���ͼ��ʱ����ť�����Ų�����ֻ�ɫ����
HBRUSH CAnimateButton::CtlColor(CDC* pDC, UINT nCtlColor)
{
//	pDC->SetBkMode(TRANSPARENT);
	return m_brushNull;
}

//////////////////////////////////////////////////////////////////////////
//
//DEL BOOL CAnimateButton::OnEraseBkgnd(CDC* pDC)
//DEL {
//DEL 	CButton::OnEraseBkgnd(pDC);
//DEL 
//DEL //	pDC->SetBkMode(TRANSPARENT);
//DEL 	return TRUE;
//DEL }

//////////////////////////////////////////////////////////////////////////
// ����(ע��һ��Ҫ�õ����麯���Ǹ�DrawItemŶ~)
void CAnimateButton::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	// TODO: Add your code to draw the specified item
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	pDC->SetBkMode(TRANSPARENT);

	CRect rcItem = lpDIS->rcItem;
	int nState = lpDIS->itemState;
	int nAction = lpDIS->itemAction;

	if(m_aniBtnState == STATE_DISABLED)
	{
		//Ҫע�⵱ȱ��Focused��״̬ʱ��Ҫ�жϺ�Disabled״̬��ͼƬ��λ��
		::TransparentBlt(pDC->m_hDC, 0, 0, m_aniBtnWidth, m_aniBtnHeight,
				m_pMemDC->m_hDC, m_aniBtnWidth * 3, 
				0,  m_aniBtnWidth, m_aniBtnHeight, m_clrTrans);
	} else if(m_aniBtnState != STATE_HOVERED)	//����״̬
	{
		::TransparentBlt(pDC->m_hDC, 0, 0, m_aniBtnWidth, m_aniBtnHeight,
 				 m_pMemDC->m_hDC, 0, 0, 
				 m_aniBtnWidth, m_aniBtnHeight, m_clrTrans);
	}

	//��������(����ͼ������ʱ)
	if(!m_strBtnText.IsEmpty() && m_ato.nPosition != TXTPOS_IN)
		DrawBtnText(pDC);

	ReleaseDC(pDC);

}

//////////////////////////////////////////////////////////////////////////
// ��ϸ����ODA_SELECT
void CAnimateButton::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_aniBtnState == STATE_HOVERED) {
		m_nTrans = 0;
		m_aniBtnState = STATE_PRESSED;	//Press״̬ͼ
		SetTimer(ID_TIMER, TIMER_ELAPSE, NULL);
	}

	CButton::OnLButtonDown(nFlags, point);
}

void CAnimateButton::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call defaults
	if (m_aniBtnState == STATE_PRESSED) {
		m_nTrans = 0;
		m_aniBtnState = STATE_HOVERED;	//Hover״̬ͼ
		SetTimer(ID_TIMER, TIMER_ELAPSE, NULL);
	}

	CButton::OnLButtonUp(nFlags, point);
}


//////////////////////////////////////////////////////////////////////////
// ���������Ƿ����Ƿ�Hover����Leave
void CAnimateButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_bAllowTrack && m_aniBtnState == STATE_NORMAL)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.dwHoverTime = 1;//HOVER_DEFAULT;
		tme.hwndTrack = m_hWnd;
		_TrackMouseEvent(&tme);

		m_bAllowTrack = FALSE;
	}

	CButton::OnMouseMove(nFlags, point);
}

//////////////////////////////////////////////////////////////////////////
// �������������¼�
LRESULT CAnimateButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if (m_aniBtnState == STATE_NORMAL) {
		m_nTrans = 0;
		m_aniBtnState = STATE_HOVERED;
		SetTimer(ID_TIMER, TIMER_ELAPSE, NULL);
	}

	return 0;
}

LRESULT CAnimateButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bAllowTrack = TRUE;
	m_nTrans = 0;
	m_aniBtnState = STATE_NORMAL;
	SetTimer(ID_TIMER, TIMER_ELAPSE, NULL);

	return 0;
}

//////////////////////////////////////////////////////////////////////////
// ʧ����, ��normal״̬
void CAnimateButton::OnKillFocus(CWnd* pNewWnd) 
{
	CButton::OnKillFocus(pNewWnd);

	// TODO: Add your message handler code here

	m_aniBtnState = STATE_NORMAL;
	KillTimer(ID_TIMER);//�û�δ�����֮ǰ״̬��״̬ͼֹͣ����

	CDC* pDC = GetDC();
	::TransparentBlt(*pDC, 0, 0, m_aniBtnWidth, m_aniBtnHeight, *m_pMemDC, 
		0, 0, m_aniBtnWidth, m_aniBtnHeight, m_clrTrans);

	ReleaseDC(pDC);
}


//////////////////////////////////////////////////////////////////////////
// ��͸������ͼ�ĺ���
// ע���������ر��лCSDN����: �ƿ���, ID: hkf314
void CAnimateButton::AlphaBitmap(int nItem)
{
	CDC* pDC = GetDC();
	CDC TmpDC;
	TmpDC.CreateCompatibleDC(pDC);
	//��Ȼ,TmpBmp������¼��������
	CBitmap TmpBmp;
	TmpBmp.CreateCompatibleBitmap(pDC, m_aniBtnWidth, m_aniBtnHeight);
	TmpDC.SelectObject(&TmpBmp);
	
	//�Ѱ�ť�µı�����������
	TmpDC.BitBlt(0, 0, m_aniBtnWidth, m_aniBtnHeight, pDC, 0, 0, SRCCOPY);
	//���ڴ�DC�е�ͼ��͸���ػ�����

	AlphaBlend(TmpDC.m_hDC, 0, 0, m_aniBtnWidth, m_aniBtnHeight, m_pMemDC->m_hDC,
		 nItem * m_aniBtnWidth, 0, m_aniBtnWidth, m_aniBtnHeight, m_bf);

	//�������ͼ�����������Ϊmaskɫ
	CRgn rgn;
	rgn.CreateRectRgn(0, 0, m_aniBtnWidth, m_aniBtnHeight);
	rgn.CombineRgn(&rgn, CRgn::FromHandle(m_arBmpRgn[nItem]), RGN_DIFF);
//	TRACE("CRgn::FromHandle(m_arBmpRgn[nItem]): %d\n", nItem);
	CBrush maskBrh;
	maskBrh.CreateSolidBrush(m_clrTrans);
	TmpDC.FillRgn(&rgn, &maskBrh);
//	TmpDC.FillRgn(CRgn::FromHandle(m_arBmpRgn[nItem]), &maskBrh);

	//�ٳ�maskɫ��ֱ�ӻ�����
 	::TransparentBlt(pDC->m_hDC, 0, 0, m_aniBtnWidth, m_aniBtnHeight, TmpDC.m_hDC,
 		 0, 0, m_aniBtnWidth, m_aniBtnHeight, m_clrTrans);
// 	pDC->BitBlt(0, 0, m_aniBtnWidth, m_aniBtnHeight, &TmpDC, 0, 0, SRCCOPY);
// 	pDC->FillRgn(CRgn::FromHandle(m_arBmpRgn[nItem]), &maskBrh);

	ReleaseDC(pDC);
}

//////////////////////////////////////////////////////////////////////////
// ����Ч��������ʵ��
void CAnimateButton::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(m_nTrans >= 255)
		KillTimer(ID_TIMER);
	else
	{
		m_nTrans += TRANS_INCREMENT;
		m_bf.SourceConstantAlpha = m_nTrans;
		AlphaBitmap(m_aniBtnState);
	}

	CButton::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////////////////////////////
// OnEnable
void CAnimateButton::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);	

	// TODO: Add your message handler code here
	m_bEnable = bEnable;
}

//////////////////////////////////////////////////////////////////////////
// ���ð�ť����
void CAnimateButton::SetBtnText(LPCTSTR lpszString)
{
	m_strBtnText = lpszString;
	AnibtnTextOpinion ato;
	SetBtnText(lpszString, ato);
}

void CAnimateButton::SetBtnText(LPCTSTR lpszString, AnibtnTextOpinion ato)
{
	m_strBtnText = lpszString;
	m_ato = ato;

// 		if(m_pLogfont)
// 			delete m_pLogfont;
//
// 		m_pLogfont = new LOGFONT;
// 		m_ato.pFont->GetLogFont(m_pLogfont);
// 		TRACE("m_ato: font-size: %d\n", m_pLogfont->lfHeight);
// 		
// 		if(m_pFontTxt)
// 			delete m_pFontTxt;
// 		m_pFontTxt = new CFont;
// 		m_pFontTxt->CreatePointFontIndirect(m_pLogfont);	//�ô�����
// 		m_ato.pFont = m_pFontTxt;

//���� - д��ǰ����ں�������·����ҷ����ֿ��С
	if(m_fontTxt.GetSafeHandle())
		m_fontTxt.DeleteObject();

	if(ato.pLf)
		m_fontTxt.CreateFontIndirect(ato.pLf);	
	else
	{
		LOGFONT lf;
		m_pMemDC->GetCurrentFont()->GetLogFont(&lf); 
		lf.lfCharSet = DEFAULT_CHARSET; 
		lf.lfHeight = 0; 
		lf.lfWidth = 0; 
		strcpy(lf.lfFaceName,"΢���ź�"); 
		
		m_fontTxt.CreateFontIndirect(&lf);
	}// if ato.pFont
	


	if(m_ato.nPosition == TXTPOS_IN)
		DrawBtnText(m_pMemDC);
	else
	{
		m_pMemDC->SelectObject(m_fontTxt);

	//ͨ����ǰ�������������ڰ�ť����ʱ����ť����ͼ�������֣�����ť��С
		CRgn RgnWnd, RgnText;
		RgnWnd.CreateRectRgn(0, 0, m_aniBtnWidth, m_aniBtnHeight);

		TEXTMETRIC tm;
		m_pMemDC->GetTextMetrics(&tm);

		BOOL bIsUnderline = FALSE;
		if(ato.pLf)
			bIsUnderline = ato.pLf->lfUnderline;

		int nLength = m_strBtnText.GetLength();
		int nBoxWidth  = (int)tm.tmAveCharWidth * nLength + 2,
			nBoxHeight = (int)tm.tmHeight + (bIsUnderline ? 10 : 1);
		
		int nLeft, nTop;
		switch(ato.nPosition)
		{
		case TXTPOS_DOWN:
			nLeft = (m_aniBtnWidth - nBoxWidth) / 2;
			nTop = m_aniBtnHeight + ato.nDistance;
			//������ť��С
			SetWindowPos(NULL, 0, 0, m_aniBtnWidth, m_aniBtnHeight + m_ato.nDistance 
				+ nBoxHeight, SWP_NOMOVE);
			break;

		case TXTPOS_RIGHT:
			nLeft = m_aniBtnWidth + m_ato.nDistance;
			nTop = (m_aniBtnHeight - nBoxHeight) / 2; 
			//������ť��С
			SetWindowPos(NULL, 0, 0, m_aniBtnWidth + m_ato.nDistance + nBoxWidth,
				m_aniBtnHeight, SWP_NOMOVE);
			break;
		}// switch
		
		m_rcTextBox.SetRect(nLeft, nTop, nLeft + nBoxWidth, nTop + nBoxHeight);
		RgnText.CreateRectRgnIndirect(&m_rcTextBox);
		RgnWnd.CombineRgn(&RgnWnd, &RgnText, RGN_OR);
		SetWindowRgn(HRGN(RgnWnd), FALSE);
	}
}


//////////////////////////////////////////////////////////////////////////
// ����ť����
void CAnimateButton::DrawBtnText(CDC *pDC)
{
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(m_fontTxt);
		
	CRect dcRect;
	if(m_ato.nPosition == TXTPOS_IN)
		dcRect.SetRect(0, 0, m_aniBtnWidth, m_aniBtnHeight);
	else
		dcRect = m_rcTextBox;
	
	CPen penBorder(PS_SOLID, m_ato.nBorderThickness, m_ato.clrBorder);
	int i = m_ato.nPosition == TXTPOS_IN ? 0 : m_aniBtnState;
	int nUpbound = m_ato.nPosition == TXTPOS_IN ? 4 : m_aniBtnState;

	for(; i <= nUpbound; i ++)
	{
		if(i >= 3)
			dcRect.SetRect(m_aniBtnWidth * i, 0, m_aniBtnWidth * (i + 1), m_aniBtnHeight);

		if(m_ato.n3DStyle)
		{
			//��Ӱ:
			if((m_ato.n3DStyle & TEXT3D_SHADOW) && (i != STATE_PRESSED))//Ϊ����״̬ʱ��Ҫ��ӰЧ����
			{				
				pDC->SetTextColor(RGB(128, 128, 128));
				CRect rcShadow = dcRect;
				rcShadow.OffsetRect(m_ato.nOffsetShadow, m_ato.nOffsetShadow);
				pDC->DrawText(m_strBtnText, &rcShadow, m_ato.nFormat);
				pDC->SetTextColor(m_ato.clrText);					
			}
			
			//ֻ����������䣨������������ƽ����
			if(m_ato.n3DStyle & TEXT3D_STROKE)
			{
				pDC->SelectObject(&penBorder);
				
				pDC->BeginPath();
				pDC->DrawText(m_strBtnText, &dcRect, m_ato.nFormat);
				pDC->EndPath();
				pDC->StrokePath();
			}
			
		}
		
		i == STATE_HOVERED ? pDC->SetTextColor(m_ato.clrTextHovered) :
			pDC->SetTextColor(m_ato.clrText);

		pDC->DrawText(m_strBtnText, &dcRect, m_ato.nFormat);

		if(i < 3)
			dcRect.OffsetRect(m_aniBtnWidth + i % 3, i % 3);

	}
}

//////////////////////////////////////////////////////////////////////////
// ���ù��
void CAnimateButton::SetCursor(LPCTSTR lpszAniCur)
{
	m_hCursor = ::LoadCursorFromFile(lpszAniCur);
	SendMessage(WM_SETCURSOR);
}

void CAnimateButton::SetCursor(UINT nIDCursor)
{
	m_hCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(nIDCursor));
	SendMessage(WM_SETCURSOR);
}

BOOL CAnimateButton::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default

//	ע�⣺
//  Ϊ��ʹ���������ָ��
// 	VC++6.0����Ҫ�޸�winuser.hԴ�ļ�
// 	#if(WINVER > = 0x0500)
// 	#define IDC_HAND MAKEINTRESOURCE(32649)
// 	#endif /* WINVER >= 0x0500 */
// 	��Ϊ
// 	#define IDC_HAND MAKEINTRESOURCE(32649) 

	if(!m_hCursor)
	{
//		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
		return TRUE;
	}
	else
		::SetClassLong(GetSafeHwnd(),GCL_HCURSOR,(LONG)m_hCursor); 
	
	return CButton::OnSetCursor(pWnd, nHitTest, message);
}

//////////////////////////////////////////////////////////////////////////
//�õ���ťλ��
CPoint CAnimateButton::GetWindowPos()
{	
	CRect rc;
	GetClientRect(&rc);
	ClientToScreen(&rc);
	CWnd* pParentWnd = GetParent();
	pParentWnd->ScreenToClient(&rc);
	
	return CPoint(rc.left, rc.top);
}

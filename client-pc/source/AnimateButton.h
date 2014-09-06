/***************************************************************************/
/*                        ������CAnimateButton                             */
/*				 	   (C)CopyRight www.cguage.com		     			   */
/***************************************************************************/

/////////////////////////////////////////////////////////////////////////////
//���ࣺCButton
//���ܣ�1������Դ�����ⲿ����ͼƬ���ٳ�maskɫ��ʵ�ְ�ť����;
//		2��֧�ְ�ť3̬��4̬��5̬��֧�������ڰ�ť�·����ҷ�,֧�ֶ�̬����;
//		3��ʵ�ְ�ť��͸������Ч��;
//		4��3D��ť����Ч��;
//		5����ťHover���������Ч��;
//		6�����ð�ť�����;
//		7���õ���ť���Ͻ�λ��.
//���ߣ�C�ϸ�
//		blog:	www.cguage.com 
//		Email:	tangcraig@gmail.com 
//		QQ:		1019714877
//�汾��
//		V1.0(2010��6��14��22:16:59)
//		v1.1(2010��7��26��12:33:02) 
//			�Ż���͸������ͼ������������ʵ���˵õ�����״̬�����������������ֿ�����
//			��ť��������ԣ������˶�̬�������ܵ�
//��Ҫ������
//		����
//ע��:	��ť��״̬��һ��ͼƬ�ϣ���ÿ״̬�Ŀ��Ӧһ�£���״̬˳��Normal��
//		Hovered��Pressed��(Focused��disabled) �������Ϊ��ѡ, LoadBitmap��ʱ
//		��Ҫָ������״̬������(Ĭ��Ϊ5̬��ť)		
/////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_ANIMATEBUTTON_H__FDD30F99_A0DC_4004_9A70_DA0F83338D28__INCLUDED_)
#define AFX_ANIMATEBUTTON_H__FDD30F99_A0DC_4004_9A70_DA0F83338D28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimateButton.h : header file
//

#include <afxtempl.h>
#include <Mmsystem.h>
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")

enum 
{
	TEXT3D_NONE,	//��3DЧ��
	TEXT3D_STROKE,	//���
	TEXT3D_SHADOW,	//��Ӱ
	TXTPOS_IN = 1,	//���ڰ�ťͼ����
	TXTPOS_DOWN,	//���ڰ�ť�·�
	TXTPOS_RIGHT	//���ڰ�ť�ҷ�
};

struct AnibtnTextOpinion
{
	int nPosition;			//������������(�ڲ������桢�ұ�?)
	int nDistance;			//�·����ҷ������밴ťͼƬ�ľ���(�������ڲ�ʱ,�˱�־λ��Ч)
	UINT nFormat;			//���ֶ��뷽ʽ(��CDC::DrawText�е�nFormatһ������)
	LOGFONT* pLf;			//���������Ϣ
//	CFont* pFont;			//����
	COLORREF clrText;		//������ɫ
	COLORREF clrTextHovered;//Hover��������ɫ
	UINT n3DStyle;			//3DЧ������(�ɺϳ�)
	COLORREF clrBorder;		//�����ɫ(���n3DStyle����TEXT3D_STROKE,�����Դ˲���)
	UINT nBorderThickness;	//��Ե���(���n3DStyle����TEXT3D_STROKE,�����Դ˲���)
	UINT nOffsetShadow;		//��Ӱ����(���n3DStyle����TEXT3D_SHADOW,�����Դ˲���)

	AnibtnTextOpinion()
	{
		nPosition = TXTPOS_IN;
		nDistance = 5;
		nFormat = 37;//��DT_SINGLELINE | DT_CENTER | DT_VCENTER
		pLf = NULL;
//		pFont = NULL;
		clrText = 0;
		clrTextHovered = clrText;
		n3DStyle = TEXT3D_NONE;
		clrBorder = 0;
		nBorderThickness = 1;
		nOffsetShadow = 2;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CAnimateButton window

class CAnimateButton : public CButton
{
// Construction
public:
	CAnimateButton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimateButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	CPoint GetWindowPos();													//ȡ�ð�ťλ��(left, top)
	BOOL Create(DWORD dwStyle,const POINT& pos, CWnd* pParentWnd, UINT nID);//��̬����,pos:��ťλ��,ע:��ť��С��ͼ���С����
	void SetBtnText(LPCTSTR lpszString);									//���ð�ť����
	void SetBtnText(LPCTSTR lpszString, AnibtnTextOpinion ato);				//���ð�ť����,ato:������ʽstruct AnibtnTextOpinion
	void SetCursor(LPCTSTR lpszAniCur);										//���ù��,lpszAniCur�����Դ�ļ�(*.cur��*.ani)
	void SetCursor(UINT nIDCursor);											//���ù��,nIDCursor�����ԴID
	//����ͼ��,������:ͼƬ�ļ�����͸��ɫ���Ƿ��еõ�����״̬���Ƿ���ʧЧ״̬
	BOOL LoadBitmap(CString szFileName, COLORREF clrTrans = RGB(255, 0, 255));
	//����ͼ��,������:ͼƬ��ԴID��͸��ɫ���Ƿ��еõ�����״̬���Ƿ���ʧЧ״̬
	BOOL LoadBitmap(UINT nIDBmp, COLORREF clrTrans = RGB(255, 0, 255));		
	virtual ~CAnimateButton();

	// Generated message map functions
protected:
	void DrawBtnText(CDC* pDC);
	void AlphaBitmap(int nItem);
	//{{AFX_MSG(CAnimateButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
		
	HRGN BitmapToRegion(HBITMAP hBmp, int nSplit, int n, COLORREF cTransparentColor, COLORREF);
	void PrepareBitmap(HBITMAP hBitmap);

private:
	int m_aniBtnHeight;
	int m_aniBtnWidth;
	int m_aniBtnState;
	CBrush m_brushNull;
	CFont m_fontTxt;
	CRect m_rcTextBox;
	AnibtnTextOpinion m_ato;
	COLORREF m_clrTrans;
	CArray<HRGN, HRGN> m_arBmpRgn;
	CString m_strSndClick;
	CString m_strSndHover;
	UINT m_nIDSndClick;
	UINT m_nIDSndHover;
	CString m_strBtnText;
	HCURSOR m_hCursor;
	BOOL m_bEnable;
	int m_nTrans;
	BLENDFUNCTION m_bf;
	BOOL m_bAllowTrack;
	CDC* m_pMemDC;
 	enum 
	{
		TIMER_ELAPSE = 15,		//TIMERʱ��
		TRANS_INCREMENT = 3,	//ÿһ�ε�͸����������
		STATE_NORMAL = 0,		//��ť��ͨ״̬
		STATE_HOVERED,			//�����밴ť״̬
		STATE_PRESSED,			//��ť�����µ�״̬
		STATE_DISABLED,			//��ťʧЧ״̬
		STATE_FOCUSED,			//��ť�õ�����״̬
	};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMATEBUTTON_H__FDD30F99_A0DC_4004_9A70_DA0F83338D28__INCLUDED_)

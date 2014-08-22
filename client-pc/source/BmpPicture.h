// bmpPicture.h: interface for the CbmpPicture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMPPICTURE_H__19F0C118_9229_4DFF_BF92_5A0791573222__INCLUDED_)
#define AFX_BMPPICTURE_H__19F0C118_9229_4DFF_BF92_5A0791573222__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
����ԭ�ͣ�BOOL TransparentBlt(HDC hdcDest, int nXOriginDest, int nYOriginDest, int nWidthDest, int hHeightDest, 
	HDC hdcSrc, int nXOriginSrc, int nYOriginSrc, int nWidthSrc, int nHeightSrc, UINT crTransparent)��
������
hdcDest��ָ��Ŀ���豸�����ľ����
nXOriginDest��ָ��Ŀ��������Ͻǵ�X�����꣬�������߼���λ��ʾ��
nYOriginDest��ָ��Ŀ��������Ͻǵ�Y�����꣬�������߼���λ��ʾ��
nWidthDest��ָ��Ŀ����εĿ�ȡ�
nHeightDest��ָ��Ŀ����εĸ߶ȡ�
hdcsrc��ָ��Դ�豸�����ľ����
nXOriginSrc��ָ��Դ���Σ����Ͻǣ���X�����꣬�������߼���λ��ʾ��
nYOriginsrc��ָ��Դ���Σ����Ͻǣ���Y�����꣬�������߼���λ��ʾ��
nWidthSrc��ָ��Դ���εĿ�ȡ�
nHeightSrc��ָ��Դ���εĸ߶ȡ�
crTransparent��Դλͼ�е�RGBֵ����͸����ɫ��(��RGB(0,0,0)Ҳ���Ǻ�ɫ����)����ֵ���������ִ�гɹ�����ô����ֵΪTRUE���������ִ��ʧ�ܣ���ô����ֵΪFALSE��
*/

#pragma comment(lib, "msimg32.lib")
//#pragma comment(lib, "lib/AtlImage.lib")


//typedef HBITMAP (*LoadAtlImageFunc)(const char *pStrFileName);

class CbmpPicture  
{
public:
	CBitmap * getBitmap();
	HBITMAP getHBitmap();
	BOOL loadBmpFromFile(CString &path);
	BOOL TansParentBlt(CDC &desDc, RECT rectPos, COLORREF color);
	BOOL PaintZoom(CDC &dc, RECT &rectPos);
	BOOL Paint(CDC &dc, const RECT &rcPos);
	BOOL loadBmpFromFile(char *path);
	bool loadBmpPicture(UINT resID);
	int getWidth();
	int getHeight();

	void unLoad();

	CbmpPicture();
	virtual ~CbmpPicture();
private:
	BITMAP Bitmap;
	CBitmap bmp;
	HBITMAP hBmp;
	int bmpWidth, bmpHeight;
};

#endif // !defined(AFX_BMPPICTURE_H__19F0C118_9229_4DFF_BF92_5A0791573222__INCLUDED_)

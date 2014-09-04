// Converter.cpp: implementation of the CConverter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueClick.h"
#include "Converter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConverter::CConverter()
{

}

CConverter::~CConverter()
{

}

/*********************************************************
�������ƣ�ANSItoUTF8
����������ANSI�ַ���ת��UTF8����
���ߣ�	  ������
���ã�    ����ܹ�ũ http://blog.csdn.net/charlessimonyi/article/details/8722859
����ʱ�䣺2014-08-30
����˵����strAnsi����Ҫת�����ַ�����ͬʱ��������ת�����
�� �� ֵ����
*********************************************************/
void CConverter::ANSItoUTF8(CString &strAnsi)
{
    //��ȡת��Ϊ���ֽں���Ҫ�Ļ�������С���������ֽڻ�������936Ϊ��������GB2312����ҳ
    UINT nLen = MultiByteToWideChar(936,NULL,strAnsi,-1,NULL,NULL);
    WCHAR *wszBuffer = new WCHAR[nLen+1];
    nLen = MultiByteToWideChar(936,NULL,strAnsi,-1,wszBuffer,nLen);
    wszBuffer[nLen] = 0;
    //��ȡתΪUTF8���ֽں���Ҫ�Ļ�������С���������ֽڻ�����
    nLen = WideCharToMultiByte(CP_UTF8,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);
    CHAR *szBuffer = new CHAR[nLen+1];
    nLen = WideCharToMultiByte(CP_UTF8,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);
    szBuffer[nLen] = 0;

    strAnsi = szBuffer;
    //�ڴ�����
    delete []wszBuffer;
    delete []szBuffer;
}

/*********************************************************
�������ƣ�UTF8toANSI
����������UTF8�ַ���ת������ANSI
���ߣ�	  ������
���ã�    ����ܹ�ũ http://blog.csdn.net/charlessimonyi/article/details/8722859
����ʱ�䣺2014-08-30
����˵����strUTF8����Ҫת�����ַ�����ͬʱ��������ת�����
�� �� ֵ����
*********************************************************/
void CConverter::UTF8toANSI(CString &strUTF8)
{
    //��ȡת��Ϊ���ֽں���Ҫ�Ļ�������С���������ֽڻ�����
    UINT nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8,-1,NULL,NULL);
    WCHAR *wszBuffer = new WCHAR[nLen+1];
    nLen = MultiByteToWideChar(CP_UTF8,NULL,strUTF8,-1,wszBuffer,nLen);
    wszBuffer[nLen] = 0;

    nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,NULL,NULL,NULL,NULL);
    CHAR *szBuffer = new CHAR[nLen+1];
    nLen = WideCharToMultiByte(936,NULL,wszBuffer,-1,szBuffer,nLen,NULL,NULL);
    szBuffer[nLen] = 0;

    strUTF8 = szBuffer;
    //�����ڴ�
    delete []szBuffer;
    delete []wszBuffer;
}
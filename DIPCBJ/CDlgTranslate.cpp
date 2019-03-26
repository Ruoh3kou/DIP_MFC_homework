// CDlgTranslate.cpp: 实现文件
//

#include "stdafx.h"
#include "DIPCBJ.h"
#include "CDlgTranslate.h"
#include "afxdialogex.h"


// CDlgTranslate 对话框

IMPLEMENT_DYNAMIC(CDlgTranslate, CDialogEx)

CDlgTranslate::CDlgTranslate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_TRANSLATE, pParent)
	, m_Hori(0)
	, m_Vert(0)
{

}

CDlgTranslate::~CDlgTranslate()
{
}

void CDlgTranslate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Hori);
	DDX_Text(pDX, IDC_EDIT2, m_Vert);
}


BEGIN_MESSAGE_MAP(CDlgTranslate, CDialogEx)
END_MESSAGE_MAP()


// CDlgTranslate 消息处理程序

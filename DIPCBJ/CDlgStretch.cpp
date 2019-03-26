// CDlgStretch.cpp: 实现文件
//

#include "stdafx.h"
#include "DIPCBJ.h"
#include "CDlgStretch.h"
#include "afxdialogex.h"


// CDlgStretch 对话框

IMPLEMENT_DYNAMIC(CDlgStretch, CDialogEx)

CDlgStretch::CDlgStretch(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_STRETCH, pParent)
	, m_StretchWidth(0)
	, m_StretchHeight(0)
{

}

CDlgStretch::~CDlgStretch()
{
}

void CDlgStretch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDD_EDIT_STRETCH_WIDTH, m_StretchWidth);
	DDX_Text(pDX, IDD_EDIT_STRETCH_HEIGHT, m_StretchHeight);
}


BEGIN_MESSAGE_MAP(CDlgStretch, CDialogEx)
END_MESSAGE_MAP()


// CDlgStretch 消息处理程序

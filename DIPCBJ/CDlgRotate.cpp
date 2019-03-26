// CDlgRotate.cpp: 实现文件
//

#include "stdafx.h"
#include "DIPCBJ.h"
#include "CDlgRotate.h"
#include "afxdialogex.h"


// CDlgRotate 对话框

IMPLEMENT_DYNAMIC(CDlgRotate, CDialogEx)

CDlgRotate::CDlgRotate(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ROTATE, pParent)
	, m_Rotate(0)
{

}

CDlgRotate::~CDlgRotate()
{
}

void CDlgRotate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Rotate);
}


BEGIN_MESSAGE_MAP(CDlgRotate, CDialogEx)
END_MESSAGE_MAP()


// CDlgRotate 消息处理程序

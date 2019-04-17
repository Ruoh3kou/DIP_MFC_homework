// CDlgNNMid.cpp: 实现文件
//

#include "stdafx.h"
#include "DIPCBJ.h"
#include "CDlgNNMid.h"
#include "afxdialogex.h"


// CDlgNNMid 对话框

IMPLEMENT_DYNAMIC(CDlgNNMid, CDialog)

CDlgNNMid::CDlgNNMid(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_NNMid, pParent)
	, n(0)
{

}

CDlgNNMid::~CDlgNNMid()
{
}

void CDlgNNMid::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, n);
}


BEGIN_MESSAGE_MAP(CDlgNNMid, CDialog)
END_MESSAGE_MAP()


// CDlgNNMid 消息处理程序

#pragma once


// CDlgNNMid 对话框

class CDlgNNMid : public CDialog
{
	DECLARE_DYNAMIC(CDlgNNMid)

public:
	CDlgNNMid(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgNNMid();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_NNMid };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// n*n的n
	int n;
};

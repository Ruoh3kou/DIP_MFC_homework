#pragma once


// CDlgStretch 对话框

class CDlgStretch : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgStretch)

public:
	CDlgStretch(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgStretch();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_STRETCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_StretchWidth;
	float m_StretchHeight;
};

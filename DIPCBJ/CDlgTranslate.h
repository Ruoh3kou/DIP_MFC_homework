#pragma once


// CDlgTranslate 对话框

class CDlgTranslate : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgTranslate)

public:
	CDlgTranslate(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgTranslate();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_TRANSLATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_Hori;
	float m_Vert;
};

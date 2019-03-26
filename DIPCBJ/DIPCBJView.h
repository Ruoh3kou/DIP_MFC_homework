
// DIPCBJView.h: CDIPCBJView 类的接口
//

#pragma once


class CDIPCBJView : public CScrollView
{
protected: // 仅从序列化创建
	CDIPCBJView() noexcept;
	DECLARE_DYNCREATE(CDIPCBJView)

	// 特性
public:
	CDIPCBJDoc* GetDocument() const;
	int m_ShowMode;  //显示模式：普通缩放；适合窗口
	int m_ShowScale;  //显示比例，百分之

// 操作
public:

	// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // 构造后第一次调用

// 实现
public:
	virtual ~CDIPCBJView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowEnlarge();
	afx_msg void OnShowReduce();
	afx_msg void OnShowOriginal();
	afx_msg void OnShowFit();
	afx_msg void OnScanUptodown();
	afx_msg void OnOnscansidein();
	afx_msg void OnScanrandom();
};

#ifndef _DEBUG  // DIPCBJView.cpp 中的调试版本
inline CDIPCBJDoc* CDIPCBJView::GetDocument() const
{
	return reinterpret_cast<CDIPCBJDoc*>(m_pDocument);
}
#endif


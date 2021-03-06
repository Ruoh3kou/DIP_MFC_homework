
// DIPCBJDoc.h: CDIPCBJDoc 类的接口
//


#pragma once
class CDIPCBJDoc : public CDocument
{
protected: // 仅从序列化创建
	CDIPCBJDoc() noexcept;
	DECLARE_DYNCREATE(CDIPCBJDoc)

	// 特性
public:
	class LImage* m_pCurImage, * m_pLastImage;
	// 操作
public:

	// 重写
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

	// 实现
public:
	virtual ~CDIPCBJDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 用于为搜索处理程序设置搜索内容的 Helper 函数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	afx_msg void OnEditUndo();
	afx_msg void OnProcStretch();
	afx_msg void OnProcFliph();
	afx_msg void OnProcTranslate();
	afx_msg void OnProcFlipv();
	afx_msg void OnProcRotate();
	afx_msg void OnGrayDoublethreshold();
	afx_msg void OnGrayWindow();
	afx_msg void OnGrayEqual();
	afx_msg void OnGrayPolyline();
	afx_msg void OnSmoothElimatenoise();
	afx_msg void OnSmoothElimateblackpot();
	afx_msg void OnSmoothadaptive();
	afx_msg void OnNnmid();
	afx_msg void OnCross();
	afx_msg void OnNnmax();
	afx_msg void OnAddnoise();
	afx_msg void OnEdgetwodir();
	afx_msg void OnThreshold();
	afx_msg void OnRobot();
	afx_msg void OnSobel();
};

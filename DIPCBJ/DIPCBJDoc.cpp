#include "stdafx.h"
// DIPCBJDoc.cpp: CDIPCBJDoc 类的实现
//
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DIPCBJ.h"
#endif

#ifdef DEBUG
#include "CDlgStretch.h"
#include "CDlgTranslate.h"
#include "CDlgRotate.h"
#include "CDlgNNMid.h"
#include "DIPCBJDoc.h"
#include "LImageProc.h"
#include <propkey.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CDIPCBJDoc

IMPLEMENT_DYNCREATE(CDIPCBJDoc, CDocument)

BEGIN_MESSAGE_MAP(CDIPCBJDoc, CDocument)
	ON_COMMAND(ID_EDIT_UNDO, &CDIPCBJDoc::OnEditUndo)
	ON_COMMAND(ID_PROC_STRETCH, &CDIPCBJDoc::OnProcStretch)
	ON_COMMAND(ID_PROC_FLIPH, &CDIPCBJDoc::OnProcFliph)
	ON_COMMAND(ID_PROC_TRANSLATE, &CDIPCBJDoc::OnProcTranslate)
	ON_COMMAND(ID_PROC_FLIPV, &CDIPCBJDoc::OnProcFlipv)
	ON_COMMAND(ID_PROC_ROTATE, &CDIPCBJDoc::OnProcRotate)
	ON_COMMAND(ID_Gray_DoubleThreshold, &CDIPCBJDoc::OnGrayDoublethreshold)
	ON_COMMAND(ID_Gray_Window, &CDIPCBJDoc::OnGrayWindow)
	ON_COMMAND(ID_Gray_Equal, &CDIPCBJDoc::OnGrayEqual)
	ON_COMMAND(ID_Gray_PolyLine, &CDIPCBJDoc::OnGrayPolyline)
	ON_COMMAND(ID_SMOOTH_ElimateNoise, &CDIPCBJDoc::OnSmoothElimatenoise)
	ON_COMMAND(ID_SMOOTH_ElimateBlackPot, &CDIPCBJDoc::OnSmoothElimateblackpot)
	ON_COMMAND(ID_SmoothAdaptive, &CDIPCBJDoc::OnSmoothadaptive)
	ON_COMMAND(ID_NNmid, &CDIPCBJDoc::OnNnmid)
	ON_COMMAND(ID_Cross, &CDIPCBJDoc::OnCross)
	ON_COMMAND(ID_NNMAX, &CDIPCBJDoc::OnNnmax)
	ON_COMMAND(ID_AddNoise, &CDIPCBJDoc::OnAddnoise)
	ON_COMMAND(ID_EdgeTwoDir, &CDIPCBJDoc::OnEdgetwodir)
	ON_COMMAND(ID_Threshold, &CDIPCBJDoc::OnThreshold)
	ON_COMMAND(ID_Robot, &CDIPCBJDoc::OnRobot)
	ON_COMMAND(ID_Sobel, &CDIPCBJDoc::OnSobel)
END_MESSAGE_MAP()


// CDIPCBJDoc 构造/析构

CDIPCBJDoc::CDIPCBJDoc() noexcept
{
	// TODO: 在此添加一次性构造代码
	m_pCurImage = new LImage;
	m_pLastImage = new LImage;

}

CDIPCBJDoc::~CDIPCBJDoc()
{
	delete m_pCurImage;
	delete m_pLastImage;

}

BOOL CDIPCBJDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}


// CDIPCBJDoc 序列化

void CDIPCBJDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CDIPCBJDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CDIPCBJDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:     strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CDIPCBJDoc::SetSearchContent(const CString & value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl* pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CDIPCBJDoc 诊断

#ifdef _DEBUG
void CDIPCBJDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDIPCBJDoc::Dump(CDumpContext & dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CDIPCBJDoc 命令


BOOL CDIPCBJDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  在此添加您专用的创建代码
	return m_pCurImage->LoadBmpFile(lpszPathName);
}


BOOL CDIPCBJDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 在此添加专用代码和/或调用基类

	return m_pCurImage->SaveBitmap(lpszPathName);
}


void CDIPCBJDoc::OnEditUndo()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pCurImage->IsValid() && m_pLastImage->IsValid())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}

}


void CDIPCBJDoc::OnProcStretch()
{
	CDlgStretch dlg;
	if (dlg.DoModal() == IDOK)
	{
		LImageProc lip;
		lip.m_pSrcImg = m_pCurImage;
		lip.m_pDestImg = m_pLastImage;
		if (lip.Stretch(dlg.m_StretchWidth, dlg.m_StretchHeight))
		{
			LImage* p = m_pCurImage;
			m_pCurImage = m_pLastImage;
			m_pLastImage = p;
			UpdateAllViews(NULL);
		}
	}
}


void CDIPCBJDoc::OnProcFliph()
{
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.FlipH())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}


void CDIPCBJDoc::OnProcTranslate()
{
	// TODO: 在此添加命令处理程序代码
	CDlgTranslate dlg;
	if (dlg.DoModal() == IDOK)
	{
		LImageProc lip;
		lip.m_pSrcImg = m_pCurImage;
		lip.m_pDestImg = m_pLastImage;
		if (lip.Translate(dlg.m_Hori, dlg.m_Vert))
		{
			LImage* p = m_pCurImage;
			m_pCurImage = m_pLastImage;
			m_pLastImage = p;
			UpdateAllViews(NULL);
		}
	}
}


void CDIPCBJDoc::OnProcFlipv()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.FlipV())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}


void CDIPCBJDoc::OnProcRotate()
{
	LImageProc lip;
	CDlgRotate dlg;
	if (dlg.DoModal() == IDOK)
	{
		LImageProc lip;
		lip.m_pSrcImg = m_pCurImage;
		lip.m_pDestImg = m_pLastImage;
		if (lip.Rotate(dlg.m_Rotate))
		{
			LImage* p = m_pCurImage;
			m_pCurImage = m_pLastImage;
			m_pLastImage = p;
			UpdateAllViews(NULL);
		}
	}
}


void CDIPCBJDoc::OnGrayDoublethreshold()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.DoubleThrehold())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}


void CDIPCBJDoc::OnGrayWindow()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.WindowGray())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}


void CDIPCBJDoc::OnGrayEqual()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.GrayEqual())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}


void CDIPCBJDoc::OnGrayPolyline()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.PolylineGray())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}


void CDIPCBJDoc::OnSmoothElimatenoise()
{
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.ElimateNoise())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}

void CDIPCBJDoc::OnSmoothElimateblackpot()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.ElimateBlackPot())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}

void CDIPCBJDoc::OnSmoothadaptive()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.AdaptiveSmooth())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}

void CDIPCBJDoc::OnNnmid()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	CDlgNNMid dlg;
	if (dlg.DoModal() == IDOK)
	{
		LImageProc lip;
		lip.m_pSrcImg = m_pCurImage;
		lip.m_pDestImg = m_pLastImage;
		if (lip.NNMedFilter(dlg.n))
		{
			LImage* p = m_pCurImage;
			m_pCurImage = m_pLastImage;
			m_pLastImage = p;
			UpdateAllViews(NULL);
		}
	}
}

void CDIPCBJDoc::OnCross()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	CDlgNNMid dlg;
	if (dlg.DoModal() == IDOK)
	{
		LImageProc lip;
		lip.m_pSrcImg = m_pCurImage;
		lip.m_pDestImg = m_pLastImage;
		if (lip.CrossFilter(dlg.n))
		{
			LImage* p = m_pCurImage;
			m_pCurImage = m_pLastImage;
			m_pLastImage = p;
			UpdateAllViews(NULL);
		}
	}
}

void CDIPCBJDoc::OnNnmax()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	CDlgNNMid dlg;
	if (dlg.DoModal() == IDOK)
	{
		LImageProc lip;
		lip.m_pSrcImg = m_pCurImage;
		lip.m_pDestImg = m_pLastImage;
		if (lip.NNMaxFilter(dlg.n))
		{
			LImage* p = m_pCurImage;
			m_pCurImage = m_pLastImage;
			m_pLastImage = p;
			UpdateAllViews(NULL);
		}
	}
}


void CDIPCBJDoc::OnAddnoise()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.ADDNoise())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}


void CDIPCBJDoc::OnEdgetwodir()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.EdgeTwoDir())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}

void CDIPCBJDoc::OnThreshold()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.Threshold())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}


void CDIPCBJDoc::OnRobot()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.Robert())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}


void CDIPCBJDoc::OnSobel()
{
	// TODO: 在此添加命令处理程序代码
	LImageProc lip;
	lip.m_pSrcImg = m_pCurImage;
	lip.m_pDestImg = m_pLastImage;
	if (lip.Sobel())
	{
		LImage* p = m_pCurImage;
		m_pCurImage = m_pLastImage;
		m_pLastImage = p;
		UpdateAllViews(NULL);
	}
}

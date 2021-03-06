#include "stdafx.h"

// DIPCBJView.cpp: CDIPCBJView 类的实现
//

// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "DIPCBJ.h"
#endif
#include "LImageProc.h"
#include "MainFrm.h"
#include "DIPCBJDoc.h"
#include "DIPCBJView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDIPCBJView

IMPLEMENT_DYNCREATE(CDIPCBJView, CScrollView)

BEGIN_MESSAGE_MAP(CDIPCBJView, CScrollView)
	ON_WM_SIZE()
	ON_COMMAND(ID_SHOW_ENLARGE, &CDIPCBJView::OnShowEnlarge)
	ON_COMMAND(ID_SHOW_REDUCE, &CDIPCBJView::OnShowReduce)
	ON_COMMAND(ID_SHOW_ORIGINAL, &CDIPCBJView::OnShowOriginal)
	ON_COMMAND(ID_SHOW_FIT, &CDIPCBJView::OnShowFit)
	ON_COMMAND(ID_SCAN_UptoDown, &CDIPCBJView::OnScanUptodown)
	ON_COMMAND(ID_OnScanSideIn, &CDIPCBJView::OnOnscansidein)
	ON_COMMAND(ID_ScanRandom, &CDIPCBJView::OnScanrandom)
END_MESSAGE_MAP()

// CDIPCBJView 构造/析构
enum {
	SHOWMODE_SCALE, SHOWMODE_FIT, SHOWMODE_SCAN_UP_DOWN, SHOWMODE_SIDE_IN, SHOWMODE_RANDOM
};
CDIPCBJView::CDIPCBJView() noexcept
{
	// TODO: 在此处添加构造代码
	m_ShowMode = SHOWMODE_FIT;
	m_ShowScale = 100;

}

CDIPCBJView::~CDIPCBJView()
{
}

BOOL CDIPCBJView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CScrollView::PreCreateWindow(cs);
}

// CDIPCBJView 绘图

void CDIPCBJView::OnDraw(CDC* pDC)
{
	CDIPCBJDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CRect rect;
	if (m_ShowMode == SHOWMODE_FIT)
	{
		GetClientRect(&rect);
		m_ShowScale = pDoc->m_pCurImage->FitBlt(pDC->m_hDC, 0, 0, rect.Width(),
			rect.Height(), 0, 0, pDoc->m_pCurImage->m_Width,
			pDoc->m_pCurImage->m_Height);
	}
	else if (m_ShowMode == SHOWMODE_SCALE)
	{
		CSize size;
		size = GetTotalSize();
		GetClientRect(&rect);
		int x0 = 0, y0 = 0;
		if (rect.Width() > size.cx)
			x0 = (rect.Width() - size.cx) / 2;
		if (rect.Height() > size.cy)
			y0 = (rect.Height() - size.cy) / 2;
		if (m_ShowScale == 100)
			pDoc->m_pCurImage->BitBlt(pDC->m_hDC, x0, y0, pDoc->m_pCurImage->m_Width,
				pDoc->m_pCurImage->m_Height, 0, 0);
		else
			pDoc->m_pCurImage->StretchBlt(pDC->m_hDC, x0, y0, size.cx, size.cy,
				0, 0, pDoc->m_pCurImage->m_Width, pDoc->m_pCurImage->m_Height);
	}
	else if (m_ShowMode == SHOWMODE_SCAN_UP_DOWN)
	{
		int m_height = pDoc->m_pCurImage->m_Height;
		int m_width = pDoc->m_pCurImage->m_Width;
		for (int i = 0; i < m_height; i++)
		{
			pDoc->m_pCurImage->StretchBlt(pDC->m_hDC, 0, i, m_width,
				1, 0, i, m_width, 1);
			Sleep(1.0f);
		}
		for (int i = 0; i < m_width; i++)
		{
			pDoc->m_pCurImage->StretchBlt(pDC->m_hDC, i, 0, 1,
				m_height, i, 0, 1, m_height);
			Sleep(1.0f);
		}
		m_ShowMode = SHOWMODE_FIT;
	}
	else if (m_ShowMode == SHOWMODE_SIDE_IN)
	{
		int m_height = pDoc->m_pCurImage->m_Height;
		int m_width = pDoc->m_pCurImage->m_Width;
		int mid = m_width / 2;
		for (int i = 0; i <= mid; i++)
		{
			pDoc->m_pCurImage->StretchBlt(pDC->m_hDC, i, 0, 1,
				m_height, i, 0, 1, m_height);
			pDoc->m_pCurImage->StretchBlt(pDC->m_hDC, m_width - i, 0, 1,
				m_height, m_width - i, 0, 1, m_height);
			Sleep(1.0f);
		}

		m_ShowMode = SHOWMODE_FIT;
	}
	else if (m_ShowMode = SHOWMODE_RANDOM)
	{
		int m_height = pDoc->m_pCurImage->m_Height;
		int m_width = pDoc->m_pCurImage->m_Width;

		int mx = 0;
		int my = 0;
		int m_row = (m_width % 20) != 0 ? 1 : 0;
		int m_col = (m_height % 20) != 0 ? 1 : 0;
		long nums = (m_width / 20 + m_row)*(m_height / 20 + m_col);
		POINT* point = new POINT[nums];
		for (int i = 0; i < nums; i++)
		{
			point[i].x = mx;
			point[i].y = my;
			mx = mx + 20;
			if (mx > m_width)
			{
				mx = 0; my = my + 20;
			}
		}
		for (int i = 0; i <= 1; i++)
			for (int mi = nums - 1; mi > 0; mi--)
			{
				int n = (int)((double)nums*rand() / RAND_MAX);
				mx = point[n].x; my = point[n].y;
				pDoc->m_pCurImage->StretchBlt(pDC->m_hDC, mx, my, 20, 20,
					mx, my, 20, 12);
				point[n].x = point[mi].x;
				point[n].y = point[mi].y;
				Sleep(1);
			}
		m_ShowMode = SHOWMODE_FIT;
	}


}

void CDIPCBJView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CDIPCBJDoc* pDoc = GetDocument();
	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	if (pDoc->m_pCurImage->IsValid())
	{
		if (m_ShowMode == SHOWMODE_FIT)
		{
			CRect rect;
			this->GetClientRect(&rect);
			sizeTotal.cx = rect.Width();
			sizeTotal.cy = rect.Height();
		}
		else
		{
			sizeTotal.cx = (int)(pDoc->m_pCurImage->m_Width*m_ShowScale / 100.0 + 0.5);
			sizeTotal.cy = (int)(pDoc->m_pCurImage->m_Height*m_ShowScale / 100.0 + 0.5);
		}
		((CMainFrame*)AfxGetApp()->m_pMainWnd)->ShowScale(m_ShowScale);
	}
	else
	{
		sizeTotal.cx = sizeTotal.cy = 100;
	}
	SetScrollSizes(MM_TEXT, sizeTotal);

}


// CDIPCBJView 诊断

#ifdef _DEBUG
void CDIPCBJView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDIPCBJView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDIPCBJDoc* CDIPCBJView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDIPCBJDoc)));
	return (CDIPCBJDoc*)m_pDocument;
}
#endif //_DEBUG


// CDIPCBJView 消息处理程序


void CDIPCBJView::OnSize(UINT nType, int cx, int cy)
{
	CScrollView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	OnInitialUpdate();
}


void CDIPCBJView::OnShowEnlarge()
{
	// TODO: 在此添加命令处理程序代码
	m_ShowMode = SHOWMODE_SCALE;
	if (m_ShowScale >= 400) return;
	if (m_ShowScale < 100)
		m_ShowScale = (m_ShowScale + 10) / 10 * 10;
	else if (m_ShowScale < 200)
		m_ShowScale = (m_ShowScale + 25) / 25 * 25;
	else if (m_ShowScale < 400)
		m_ShowScale = (m_ShowScale + 50) / 50 * 50;
	OnInitialUpdate();

}


void CDIPCBJView::OnShowReduce()
{
	// TODO: 在此添加命令处理程序代码
	m_ShowMode = SHOWMODE_SCALE;
	if (m_ShowScale <= 10) return;
	if (m_ShowScale <= 100)
		m_ShowScale = (m_ShowScale - 10) / 10 * 10;
	else if (m_ShowScale <= 200)
		m_ShowScale = (m_ShowScale - 25) / 25 * 25;
	else
		m_ShowScale = (m_ShowScale - 50) / 50 * 50;
	OnInitialUpdate();

}


void CDIPCBJView::OnShowOriginal()
{
	// TODO: 在此添加命令处理程序代码
	m_ShowMode = SHOWMODE_SCALE;
	m_ShowScale = 100;
	OnInitialUpdate();

}


void CDIPCBJView::OnShowFit()
{
	// TODO: 在此添加命令处理程序代码
	m_ShowMode = SHOWMODE_FIT;
	OnInitialUpdate();

}


void CDIPCBJView::OnScanUptodown()
{
	m_ShowMode = SHOWMODE_SCAN_UP_DOWN;
	OnInitialUpdate();
}


void CDIPCBJView::OnOnscansidein()
{
	m_ShowMode = SHOWMODE_SIDE_IN;
	OnInitialUpdate();
}


void CDIPCBJView::OnScanrandom()
{
	m_ShowMode = SHOWMODE_RANDOM;
	OnInitialUpdate();
}


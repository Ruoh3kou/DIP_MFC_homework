#include "stdafx.h"
#include "LImage.h"


LImage::LImage()
{
	m_hBitmap = NULL;
	m_pBits = NULL;
	m_hMemDC = NULL;
}

LImage::~LImage()
{
	Destroy();
}
BOOL LImage::Create(int a_Width, int a_Height)
{
	Destroy();
	if (a_Width == 0 || a_Height == 0)	return FALSE;
	if (a_Width < 0) a_Width = -a_Width;
	if (a_Height < 0) a_Height = -a_Height;

	BITMAPINFO bi;
	bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth = a_Width;
	bi.bmiHeader.biHeight = -a_Height;  //使图像原点在左上角；若为正数则在左下角
	bi.bmiHeader.biPlanes = 1;
	bi.bmiHeader.biBitCount = 24;
	bi.bmiHeader.biCompression = BI_RGB;
	bi.bmiHeader.biSizeImage = 0;
	bi.bmiHeader.biXPelsPerMeter = 11810;
	bi.bmiHeader.biYPelsPerMeter = 11810;
	bi.bmiHeader.biClrUsed = 0;
	bi.bmiHeader.biClrImportant = 0;

	HDC hdc = CreateCompatibleDC(NULL);
	m_hBitmap = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, (void**)&m_pBits, NULL, 0);
	DeleteDC(hdc);

	if (m_hBitmap == NULL || m_pBits == NULL)
	{
		m_hBitmap = NULL;
		m_pBits = NULL;
		return FALSE;
	}
	BITMAP bm;
	GetObject(m_hBitmap, sizeof(BITMAP), &bm);
	m_Width = bm.bmWidth;
	m_Height = bm.bmHeight;
	m_WidthBytes = bm.bmWidthBytes;
	m_ImageSize = m_WidthBytes * m_Height;

	m_hMemDC = CreateCompatibleDC(NULL);
	if (m_hMemDC == NULL)
	{
		DeleteObject(m_hBitmap);
		m_hBitmap = NULL;
		m_pBits = NULL;
		m_hMemDC = NULL;
		return FALSE;
	}
	m_hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, m_hBitmap);
	m_DC.Attach(m_hMemDC);
	return TRUE;
}
void LImage::Destroy()
{
	if (m_hBitmap != NULL && m_pBits != NULL && m_hMemDC != NULL)
	{
		m_DC.Detach();
		SelectObject(m_hMemDC, m_hOldBitmap);
		DeleteDC(m_hMemDC);
		DeleteObject(m_hBitmap);
	}
	m_hBitmap = NULL;
	m_pBits = NULL;
	m_hMemDC = NULL;
}
BOOL LImage::IsValid()
{
	return (m_hBitmap != NULL && m_pBits != NULL && m_hMemDC != NULL);
}

BOOL LImage::CopyFrom(LImage *a_pImg)
{
	if (a_pImg == NULL || !a_pImg->IsValid())
		return FALSE;
	if (!Create(a_pImg->m_Width, a_pImg->m_Height))
		return FALSE;
	memcpy((void*)m_pBits, (void*)a_pImg->m_pBits, m_WidthBytes*m_Height);
	return TRUE;
}

BOOL LImage::BitBlt(HDC a_DestDC, int a_DestX, int a_DestY, int a_Width, int a_Height,
	int a_SrcX, int a_SrcY, DWORD a_Rop)
{
	return ::BitBlt(a_DestDC, a_DestX, a_DestY, a_Width, a_Height,
		m_hMemDC, a_SrcX, a_SrcY, a_Rop);
}

BOOL LImage::StretchBlt(HDC a_DestDC, int a_DestX, int a_DestY, int a_DestWidth,
	int a_DestHeight, int a_SrcX, int a_SrcY, int a_SrcWidth, int a_SrcHeight, DWORD a_Rop)
{
	::SetStretchBltMode(a_DestDC, COLORONCOLOR);
	return ::StretchBlt(a_DestDC, a_DestX, a_DestY, a_DestWidth, a_DestHeight,
		m_hMemDC, a_SrcX, a_SrcY, a_SrcWidth, a_SrcHeight, a_Rop);
}

BOOL LImage::FitBlt(HDC a_DestDC, int a_DestX, int a_DestY, int a_DestWidth,
	int a_DestHeight, int a_SrcX, int a_SrcY, int a_SrcWidth, int a_SrcHeight, DWORD a_Rop)
{
	int dx, dy, dw, dh;
	double sx, sy;
	sx = (double)a_SrcWidth / (double)a_DestWidth;
	sy = (double)a_SrcHeight / (double)a_DestHeight;
	if (sx <= 1 && sy <= 1)
	{
		dx = a_DestX + (a_DestWidth - a_SrcWidth) / 2;
		dy = a_DestY + (a_DestHeight - a_SrcHeight) / 2;
		dw = a_SrcWidth;
		dh = a_SrcHeight;
	}
	else if (sx > sy)
	{
		dx = a_DestX;
		dw = a_DestWidth;
		dh = (int)(a_SrcHeight / sx);
		dy = a_DestY + (a_DestHeight - dh) / 2;
	}
	else if (sy > sx)
	{
		dw = (int)(a_SrcWidth / sy);
		dx = a_DestX + (a_DestWidth - dw) / 2;
		dy = a_DestY;
		dh = a_DestHeight;
	}
	else
	{
		dx = a_DestX; dy = a_DestY;
		dw = a_DestWidth; dh = a_DestHeight;
	}
	::SetStretchBltMode(a_DestDC, COLORONCOLOR);
	return ::StretchBlt(a_DestDC, dx, dy, dw, dh,
		m_hMemDC, a_SrcX, a_SrcY, a_SrcWidth, a_SrcHeight, a_Rop);
}
BOOL LImage::LoadBmpFile(CString a_Filename)
{
	FILE *pf = fopen(a_Filename, "rb");
	if (pf == NULL) return FALSE;

	BITMAPFILEHEADER bmfHeader;
	if (fread((LPSTR)&bmfHeader, 1, sizeof(bmfHeader), pf) != sizeof(bmfHeader))
	{
		fclose(pf); return FALSE;
	}
	if (bmfHeader.bfType != ((WORD)('M' << 8) | 'B'))
	{
		fclose(pf); return FALSE;
	}

	int leng = bmfHeader.bfSize - sizeof(bmfHeader);
	BYTE *pBmp = (BYTE *)calloc(leng, 1);
	if (pBmp == NULL)
	{
		fclose(pf); return FALSE;
	}

	if ((int)fread(pBmp, 1, leng, pf) != leng)  //读入位图文件头后的所有数据
	{
		free(pBmp); fclose(pf);
		return FALSE;
	}
	fclose(pf);

	BYTE *pImg = (BYTE*)(pBmp + bmfHeader.bfOffBits - sizeof(BITMAPFILEHEADER));

	BITMAPINFO *pbi;
	pbi = (BITMAPINFO*)pBmp;
	if (pbi->bmiHeader.biBitCount != 24)
	{
		free(pBmp); return FALSE;
	}
	int widthBytes = ((pbi->bmiHeader.biWidth*pbi->bmiHeader.biBitCount + 31) / 32) * 4;

	//将读入的bmp象素数据填入m_pBits
	int i;
	BYTE *p1, *p2;
	RGBQUAD *ci = pbi->bmiColors;
	if (Create(pbi->bmiHeader.biWidth, abs(pbi->bmiHeader.biHeight)))
	{
		int bytes = m_WidthBytes;
		if (bytes > widthBytes) bytes = widthBytes;
		for (i = 0; i < m_Height; i++)
		{
			p1 = m_pBits + i * m_WidthBytes;
			if (pbi->bmiHeader.biHeight < 0)  //文件中扫描线由上向下存储
				p2 = pImg + i * widthBytes;
			else	//文件中扫描线由下向上存储
				p2 = pImg + (m_Height - 1 - i)*widthBytes;
			memcpy(p1, p2, bytes);
		}
		free(pBmp); return TRUE;
	}
	else
	{
		free(pBmp); return FALSE;
	}
}
BOOL LImage::SaveBitmap(CString a_Filename)
{
	if (!IsValid()) return FALSE;

	FILE *pf = fopen(a_Filename, "wb");
	if (pf == NULL) return FALSE;

	BITMAPINFO *pbmi;
	pbmi = (BITMAPINFO*)calloc(1, sizeof(BITMAPINFOHEADER));

	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = m_Width;
	pbmi->bmiHeader.biHeight = m_Height;
	pbmi->bmiHeader.biPlanes = 1;
	pbmi->bmiHeader.biBitCount = 24;
	pbmi->bmiHeader.biCompression = BI_RGB;
	UINT widthBytes = ((m_Width * 24 + 31) / 32) * 4;
	pbmi->bmiHeader.biSizeImage = widthBytes * m_Height;
	pbmi->bmiHeader.biXPelsPerMeter = 11810;
	pbmi->bmiHeader.biYPelsPerMeter = 11810;
	pbmi->bmiHeader.biClrUsed = 0;
	pbmi->bmiHeader.biClrImportant = 0;

	BITMAPFILEHEADER hdr;       // bitmap file-header 
	BITMAPINFOHEADER *pbih;     // bitmap info-header 

	pbih = (BITMAPINFOHEADER*)pbmi;
	hdr.bfType = 0x4d42;	// 0x42 = "B" 0x4d = "M" 
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		pbih->biSize + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize;
	fwrite(&hdr, sizeof(BITMAPFILEHEADER), 1, pf);  //写入位图文件头
	fwrite(pbih, sizeof(BITMAPINFOHEADER), 1, pf);  //写入位图信息头

	int i;
	BYTE *p1;
	for (i = m_Height - 1; i >= 0; i--)
	{
		p1 = m_pBits + m_WidthBytes * i;
		if (fwrite(p1, 1, widthBytes, pf) != widthBytes)
		{
			free((HLOCAL)pbmi); fclose(pf); return FALSE;
		}
	}

	free((HLOCAL)pbmi);
	fclose(pf);
	return TRUE;
}

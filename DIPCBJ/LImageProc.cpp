#include "stdafx.h"
#include "LImageProc.h"
#include "math.h"
#include "LImage.h"

LImageProc::LImageProc()
{
	m_pSrcImg = m_pDestImg = NULL;
}


LImageProc::~LImageProc()
{

}
BOOL LImageProc::ImageIsValid()
{
	if (m_pSrcImg == NULL || m_pDestImg == NULL)
		return FALSE;
	return m_pSrcImg->IsValid();
}

// 水平镜像
BOOL LImageProc::FlipH()
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE *sd = m_pSrcImg->m_pBits;
	BYTE *dd = m_pDestImg->m_pBits;
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j += 3)
		{
			for (int k = 0; k < 3; k++)
			{
				dd[i*m_pDestImg->m_WidthBytes + j + k] = sd[i*m_pDestImg->m_WidthBytes + m_pDestImg->m_WidthBytes - j + k - 3];
			}
		}
	}
	return TRUE;
}

// 垂直镜像
BOOL LImageProc::FlipV()
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE *sd = m_pSrcImg->m_pBits;
	BYTE *dd = m_pDestImg->m_pBits;
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j += 3)
		{
			dd[i*m_pDestImg->m_WidthBytes + j] = sd[(m_pDestImg->m_Height - i - 1)*m_pDestImg->m_WidthBytes + j];
			dd[i*m_pDestImg->m_WidthBytes + j + 1] = sd[(m_pDestImg->m_Height - i - 1)*m_pDestImg->m_WidthBytes + j + 1];
			dd[i*m_pDestImg->m_WidthBytes + j + 2] = sd[(m_pDestImg->m_Height - i - 1)*m_pDestImg->m_WidthBytes + j + 2];
		}
	}

	return TRUE;
}

// 缩放
BOOL LImageProc::Stretch(float aW, float aH)
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create((int)m_pSrcImg->m_Width*aW, (int)m_pSrcImg->m_Height*aH);
	BYTE *sd = m_pSrcImg->m_pBits;
	BYTE *dd = m_pDestImg->m_pBits;
	int newX = 1;
	int newY = 1;
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j += 3)
		{
			newX = (int)(j / aW);
			newY = (int)(i / aH);
			dd[i*m_pDestImg->m_WidthBytes + j] = sd[newY*m_pSrcImg->m_WidthBytes + newX];
			dd[i*m_pDestImg->m_WidthBytes + j + 1] = sd[newY*m_pSrcImg->m_WidthBytes + newX + 1];
			dd[i*m_pDestImg->m_WidthBytes + j + 2] = sd[newY*m_pSrcImg->m_WidthBytes + newX + 2];
		}
	}
	return TRUE;
}

// 平移
BOOL LImageProc::Translate(int Hori, int Vert)
{
	if (Hori > m_pSrcImg->m_Width || Vert > m_pSrcImg->m_Height)
	{
		AfxMessageBox("图像平移不能超过原始长度:", MB_OK, 0);
		return FALSE;
	}
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create((int)m_pSrcImg->m_Width, (int)m_pSrcImg->m_Height);
	BYTE *sd = m_pSrcImg->m_pBits;
	BYTE *dd = m_pDestImg->m_pBits;

	int x, y;

	for (y = 0; y < m_pDestImg->m_Height; y++)
	{

		if (((y < Vert) && (Vert >= 0)) || ((y >= (m_pDestImg->m_Height + Vert)) && (Vert < 0)))
		{
			for (x = 0; x < m_pDestImg->m_Width * 3; x += 3)
			{
				dd[y*m_pDestImg->m_WidthBytes + x] = 255;
				dd[y*m_pDestImg->m_WidthBytes + x + 1] = 255;
				dd[y*m_pDestImg->m_WidthBytes + x + 2] = 255;
			}
		}
		else if (((y >= Vert) && (Vert >= 0)) || ((y < (m_pDestImg->m_Height + Vert)) && (Vert < 0)))
		{
			for (x = 0; x < m_pDestImg->m_Width * 3; x += 3)
			{
				// 右移
				if (Hori >= 0) {
					if (x < 3 * Hori)
					{
						dd[y*m_pDestImg->m_WidthBytes + x] = 255;
						dd[y*m_pDestImg->m_WidthBytes + x + 1] = 255;
						dd[y*m_pDestImg->m_WidthBytes + x + 2] = 255;
					}
					else if (x >= 3 * Hori)
					{
						dd[y*m_pDestImg->m_WidthBytes + x] = sd[(y - Vert)*m_pDestImg->m_WidthBytes + x - Hori * 3];
						dd[y*m_pDestImg->m_WidthBytes + x + 1] = sd[(y - Vert)*m_pDestImg->m_WidthBytes + x - Hori * 3 + 1];
						dd[y*m_pDestImg->m_WidthBytes + x + 2] = sd[(y - Vert)*m_pDestImg->m_WidthBytes + x - Hori * 3 + 2];
					}
				}
				// 左移动
				else {
					if (x >= (m_pDestImg->m_Width + 3 * Hori))
					{
						dd[y*m_pDestImg->m_WidthBytes + x] = 255;
						dd[y*m_pDestImg->m_WidthBytes + x + 1] = 255;
						dd[y*m_pDestImg->m_WidthBytes + x + 2] = 255;
					}
					else
					{
						dd[y*m_pDestImg->m_WidthBytes + x] = sd[(y - Vert)*m_pDestImg->m_WidthBytes + x - Hori * 3];
						dd[y*m_pDestImg->m_WidthBytes + x + 1] = sd[(y - Vert)*m_pDestImg->m_WidthBytes + x - Hori * 3 + 1];
						dd[y*m_pDestImg->m_WidthBytes + x + 2] = sd[(y - Vert)*m_pDestImg->m_WidthBytes + x - Hori * 3 + 2];
					}
				}
			}
		}
	}
	return TRUE;
}

// 旋转
BOOL LImageProc::Rotate(int angle)  //旋转，弧度
{
	if (!ImageIsValid()) return FALSE;
	double PI = asin(0.5) * 6;
	double degree = 1.0 * angle * PI / 180;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE *sd = m_pSrcImg->m_pBits;
	BYTE *dd = m_pDestImg->m_pBits;
	int tX = 0, tY = 0;
	for (int i = 0; i < m_pSrcImg->m_Height; i++)
	{
		for (int j = 0; j < m_pSrcImg->m_Width; j++)
		{
			//转换到以图像为中心的坐标系，并进行逆旋转
			tX = (int)(j*cos(degree) - i * sin(degree));
			tY = (int)(j*sin(degree) + i * cos(degree));
			if (tX < i * m_pSrcImg->m_Width && tY < m_pSrcImg->m_Height && tX >= 0 && tY >= 0)
			{
				dd[(i*m_pSrcImg->m_Width + j) * 3] = sd[(tY*m_pSrcImg->m_Width + tX) * 3];
				dd[(i*m_pSrcImg->m_Width + j) * 3 + 1] = sd[(tY*m_pSrcImg->m_Width + tX) * 3 + 1];
				dd[(i*m_pSrcImg->m_Width + j) * 3 + 2] = sd[(tY*m_pSrcImg->m_Width + tX) * 3 + 2];

			}
			else if (tX >= i * m_pSrcImg->m_Width || tY >= m_pSrcImg->m_Height || tX < 0 || tY < 0)
			{
				dd[(i*m_pSrcImg->m_Width + j) * 3] = 255;
				dd[(i*m_pSrcImg->m_Width + j) * 3 + 1] = 255;
				dd[(i*m_pSrcImg->m_Width + j) * 3 + 2] = 255;

			}
		}
	}
	return TRUE;
}

// 双固定阈值
BOOL LImageProc::DoubleThrehold() {
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE *sd = m_pSrcImg->m_pBits;
	BYTE *dd = m_pDestImg->m_pBits;
	int T1 = 100;
	int T2 = 200;
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j++)
		{
			if (sd[i*m_pDestImg->m_WidthBytes + j] > T1&&sd[i*m_pDestImg->m_WidthBytes + j] < T2) {
				dd[i*m_pDestImg->m_WidthBytes + j] = 255;
			}
			else
			{
				dd[i*m_pDestImg->m_WidthBytes + j] = 0;
			}

		}
	}
	return TRUE;
}

// 窗口灰度变换
BOOL LImageProc::WindowGray() {
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE *sd = m_pSrcImg->m_pBits;
	BYTE *dd = m_pDestImg->m_pBits;
	int L = 50;
	int U = 180;
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j++)
		{
			if (sd[i*m_pDestImg->m_WidthBytes + j] > U) {
				dd[i*m_pDestImg->m_WidthBytes + j] = 255;
			}
			else if (sd[i*m_pDestImg->m_WidthBytes + j] < L)
			{
				dd[i*m_pDestImg->m_WidthBytes + j] = 0;
			}
			else
			{
				dd[i*m_pDestImg->m_WidthBytes + j] = sd[i*m_pDestImg->m_WidthBytes + j];
			}

		}
	}
	return TRUE;
}
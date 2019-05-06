#include "stdafx.h"
#include "LImageProc.h"
#include "math.h"
#include "LImage.h"
#include <vector>
#include <algorithm>
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
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j += 3)
		{
			for (int k = 0; k < 3; k++)
			{
				dd[i * m_pDestImg->m_WidthBytes + j + k] = sd[i * m_pDestImg->m_WidthBytes + m_pDestImg->m_WidthBytes - j + k - 3];
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
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j += 3)
		{
			dd[i * m_pDestImg->m_WidthBytes + j] = sd[(m_pDestImg->m_Height - i - 1) * m_pDestImg->m_WidthBytes + j];
			dd[i * m_pDestImg->m_WidthBytes + j + 1] = sd[(m_pDestImg->m_Height - i - 1) * m_pDestImg->m_WidthBytes + j + 1];
			dd[i * m_pDestImg->m_WidthBytes + j + 2] = sd[(m_pDestImg->m_Height - i - 1) * m_pDestImg->m_WidthBytes + j + 2];
		}
	}

	return TRUE;
}

// 缩放
BOOL LImageProc::Stretch(float aW, float aH)
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create((int)m_pSrcImg->m_Width * aW, (int)m_pSrcImg->m_Height * aH);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	int newX = 1;
	int newY = 1;
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j += 3)
		{
			newX = (int)(j / aW);
			newY = (int)(i / aH);
			dd[i * m_pDestImg->m_WidthBytes + j] = sd[newY * m_pSrcImg->m_WidthBytes + newX];
			dd[i * m_pDestImg->m_WidthBytes + j + 1] = sd[newY * m_pSrcImg->m_WidthBytes + newX + 1];
			dd[i * m_pDestImg->m_WidthBytes + j + 2] = sd[newY * m_pSrcImg->m_WidthBytes + newX + 2];
		}
	}
	return TRUE;
}

// 平移
BOOL LImageProc::Translate(int Hori, int Vert)
{
	if (Hori > m_pSrcImg->m_Width || Vert > m_pSrcImg->m_Height)
	{
		AfxMessageBox(_T("图像平移不能超过原始长度:"), MB_OK, 0);
		return FALSE;
	}
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create((int)m_pSrcImg->m_Width, (int)m_pSrcImg->m_Height);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;

	int x, y;

	for (y = 0; y < m_pDestImg->m_Height; y++)
	{

		if (((y < Vert) && (Vert >= 0)) || ((y >= (m_pDestImg->m_Height + Vert)) && (Vert < 0)))
		{
			for (x = 0; x < m_pDestImg->m_Width * 3; x += 3)
			{
				dd[y * m_pDestImg->m_WidthBytes + x] = 255;
				dd[y * m_pDestImg->m_WidthBytes + x + 1] = 255;
				dd[y * m_pDestImg->m_WidthBytes + x + 2] = 255;
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
						dd[y * m_pDestImg->m_WidthBytes + x] = 255;
						dd[y * m_pDestImg->m_WidthBytes + x + 1] = 255;
						dd[y * m_pDestImg->m_WidthBytes + x + 2] = 255;
					}
					else if (x >= 3 * Hori)
					{
						dd[y * m_pDestImg->m_WidthBytes + x] = sd[(y - Vert) * m_pDestImg->m_WidthBytes + x - Hori * 3];
						dd[y * m_pDestImg->m_WidthBytes + x + 1] = sd[(y - Vert) * m_pDestImg->m_WidthBytes + x - Hori * 3 + 1];
						dd[y * m_pDestImg->m_WidthBytes + x + 2] = sd[(y - Vert) * m_pDestImg->m_WidthBytes + x - Hori * 3 + 2];
					}
				}
				// 左移动
				else {
					if (x >= (m_pDestImg->m_Width + 3 * Hori))
					{
						dd[y * m_pDestImg->m_WidthBytes + x] = 255;
						dd[y * m_pDestImg->m_WidthBytes + x + 1] = 255;
						dd[y * m_pDestImg->m_WidthBytes + x + 2] = 255;
					}
					else
					{
						dd[y * m_pDestImg->m_WidthBytes + x] = sd[(y - Vert) * m_pDestImg->m_WidthBytes + x - Hori * 3];
						dd[y * m_pDestImg->m_WidthBytes + x + 1] = sd[(y - Vert) * m_pDestImg->m_WidthBytes + x - Hori * 3 + 1];
						dd[y * m_pDestImg->m_WidthBytes + x + 2] = sd[(y - Vert) * m_pDestImg->m_WidthBytes + x - Hori * 3 + 2];
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
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	int tX = 0, tY = 0;
	for (int i = 0; i < m_pSrcImg->m_Height; i++)
	{
		for (int j = 0; j < m_pSrcImg->m_Width; j++)
		{
			//转换到以图像为中心的坐标系，并进行逆旋转
			tX = (int)(j * cos(degree) - i * sin(degree));
			tY = (int)(j * sin(degree) + i * cos(degree));
			if (tX < i * m_pSrcImg->m_Width && tY < m_pSrcImg->m_Height && tX >= 0 && tY >= 0)
			{
				dd[(i * m_pSrcImg->m_Width + j) * 3] = sd[(tY * m_pSrcImg->m_Width + tX) * 3];
				dd[(i * m_pSrcImg->m_Width + j) * 3 + 1] = sd[(tY * m_pSrcImg->m_Width + tX) * 3 + 1];
				dd[(i * m_pSrcImg->m_Width + j) * 3 + 2] = sd[(tY * m_pSrcImg->m_Width + tX) * 3 + 2];

			}
			else if (tX >= i * m_pSrcImg->m_Width || tY >= m_pSrcImg->m_Height || tX < 0 || tY < 0)
			{
				dd[(i * m_pSrcImg->m_Width + j) * 3] = 255;
				dd[(i * m_pSrcImg->m_Width + j) * 3 + 1] = 255;
				dd[(i * m_pSrcImg->m_Width + j) * 3 + 2] = 255;

			}
		}
	}
	return TRUE;
}

// 双固定阈值
BOOL LImageProc::DoubleThrehold() {
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	int T1 = 100;
	int T2 = 200;
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j++)
		{
			if (sd[i * m_pDestImg->m_WidthBytes + j] > T1 && sd[i * m_pDestImg->m_WidthBytes + j] < T2) {
				dd[i * m_pDestImg->m_WidthBytes + j] = 255;
			}
			else
			{
				dd[i * m_pDestImg->m_WidthBytes + j] = 0;
			}

		}
	}
	return TRUE;
}

// 窗口灰度变换
BOOL LImageProc::WindowGray() {
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	int L = 100;
	int U = 220;
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j++)
		{
			if (sd[i * m_pDestImg->m_WidthBytes + j] > U) {
				dd[i * m_pDestImg->m_WidthBytes + j] = 255;
			}
			else if (sd[i * m_pDestImg->m_WidthBytes + j] < L)
			{
				dd[i * m_pDestImg->m_WidthBytes + j] = 0;
			}
			else
			{
				dd[i * m_pDestImg->m_WidthBytes + j] = sd[i * m_pDestImg->m_WidthBytes + j];
			}

		}
	}
	return TRUE;
}

// 折现变换
BOOL LImageProc::PolylineGray() {
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	int x1 = 130, y1 = 50, x2 = 150, y2 = 200;
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j++)
		{
			int x = sd[i * m_pDestImg->m_WidthBytes + j];
			if (x < x1)
				dd[i * m_pDestImg->m_WidthBytes + j] = y1 * x / x1;
			else if (x >= x1 && x <= x2)
				dd[i * m_pDestImg->m_WidthBytes + j] = (y2 - y1) * (x - x1) / (x2 - x1) + y1;
			else
				dd[i * m_pDestImg->m_WidthBytes + j] = (255 - y2) * (x - x2) / (255 - x2) + y2;
		}
	}
	return TRUE;
}

// 灰度均衡化
BOOL LImageProc::GrayEqual() {

	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	float prR[256], prG[256], prB[256];
	// 获取R G B的灰级概率
	GetHist(prR, prG, prB);
	// 进行均衡化处理 
	float tempR[256], tempG[256], tempB[256];
	int newR[256], newG[256], newB[256];
	for (int i = 0; i < 256; i++)
	{
		if (i == 0)
		{
			tempR[0] = prR[0];
			tempG[0] = prG[0];
			tempB[0] = prB[0];
		}
		else
		{
			tempR[i] = tempR[i - 1] + prR[i];
			tempG[i] = tempG[i - 1] + prG[i];
			tempB[i] = tempB[i - 1] + prB[i];
		}
		newR[i] = (int)(255.0f * tempR[i] + 0.5f);
		newG[i] = (int)(255.0f * tempG[i] + 0.5f);
		newB[i] = (int)(255.0f * tempB[i] + 0.5f);
	}
	//赋值回图像
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j += 3)
		{
			dd[i * m_pDestImg->m_WidthBytes + j] = newB[sd[i * m_pDestImg->m_WidthBytes + j]];
			dd[i * m_pDestImg->m_WidthBytes + j + 1] = newG[sd[i * m_pDestImg->m_WidthBytes + j + 1]];
			dd[i * m_pDestImg->m_WidthBytes + j + 2] = newR[sd[i * m_pDestImg->m_WidthBytes + j + 2]];
		}
	}
	return TRUE;
}

// 得到直方图（灰级概率Pr)
void LImageProc::GetHist(float* prR, float* prG, float* prB)
{
	BYTE* sd = m_pSrcImg->m_pBits;
	int counterR[256] = { 0 }, counterG[256] = { 0 }, counterB[256] = { 0 };
	for (int i = 0; i < m_pSrcImg->m_Height; i++)
	{
		for (int j = 0; j < m_pSrcImg->m_Width * 3; j += 3)
		{
			counterB[sd[i * m_pSrcImg->m_WidthBytes + j]]++;
			counterG[sd[i * m_pSrcImg->m_WidthBytes + j + 1]]++;
			counterR[sd[i * m_pSrcImg->m_WidthBytes + j + 2]]++;
		}
	}
	long long bytes = m_pSrcImg->m_Height * m_pSrcImg->m_Width;
	for (int i = 0; i < 256; i++)
	{
		prR[i] = counterR[i] / (bytes * 1.0f);
		prG[i] = counterG[i] / (bytes * 1.0f);
		prB[i] = counterB[i] / (bytes * 1.0f);
	}
}

// 消除黑点
BOOL LImageProc::ElimateBlackPot()
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	for (int i = 1; i < m_pDestImg->m_Height - 1; i++)
	{
		for (int j = 1; j < m_pDestImg->m_Width - 1; j++)
		{
			int averg_R = 0;
			int averg_G = 0;
			int averg_B = 0;
			averg_B += (
				sd[(i - 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3]
				+ sd[(i - 1) * m_pDestImg->m_WidthBytes + j * 3]
				+ sd[(i - 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3]
				+ sd[i * m_pDestImg->m_WidthBytes + (j - 1) * 3]
				+ sd[i * m_pDestImg->m_WidthBytes + (j + 1) * 3]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + j * 3]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3]);
			averg_G += (
				sd[(i - 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + 1]
				+ sd[(i - 1) * m_pDestImg->m_WidthBytes + j * 3 + 1]
				+ sd[(i - 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + 1]
				+ sd[i * m_pDestImg->m_WidthBytes + (j - 1) * 3 + 1]
				+ sd[i * m_pDestImg->m_WidthBytes + (j + 1) * 3 + 1]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + 1]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + j * 3 + 1]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + 1]);
			averg_R += (
				sd[(i - 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + 2]
				+ sd[(i - 1) * m_pDestImg->m_WidthBytes + j * 3 + 2]
				+ sd[(i - 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + 2]
				+ sd[i * m_pDestImg->m_WidthBytes + (j - 1) * 3 + 2]
				+ sd[i * m_pDestImg->m_WidthBytes + (j + 1) * 3 + 2]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + 2]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + j * 3 + 2]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + 2]);
			if (averg_B == 255 * 8)
				dd[i * m_pDestImg->m_WidthBytes + j * 3] = 255;
			else
				dd[i * m_pDestImg->m_WidthBytes + j * 3] = sd[i * m_pDestImg->m_WidthBytes + j * 3];

			if (averg_G == 255 * 8)
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 1] = 255;
			else
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 1] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + 1];

			if (averg_R == 255 * 8)
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 2] = 255;
			else
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 2] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + 2];
		}
	}
	return TRUE;
}

// 消除噪声
BOOL LImageProc::ElimateNoise()
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	float Threshold = 127.5;
	for (int i = 1; i < m_pDestImg->m_Height - 1; i++)
	{
		for (int j = 1; j < m_pDestImg->m_Width - 1; j++)
		{
			float averg_R = 0;
			float averg_G = 0;
			float averg_B = 0;
			averg_B += (
				sd[(i - 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3]
				+ sd[(i - 1) * m_pDestImg->m_WidthBytes + j * 3]
				+ sd[(i - 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3]
				+ sd[i * m_pDestImg->m_WidthBytes + (j - 1) * 3]
				+ sd[i * m_pDestImg->m_WidthBytes + (j + 1) * 3]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + j * 3]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3]) / 8;
			averg_G += (
				sd[(i - 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + 1]
				+ sd[(i - 1) * m_pDestImg->m_WidthBytes + j * 3 + 1]
				+ sd[(i - 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + 1]
				+ sd[i * m_pDestImg->m_WidthBytes + (j - 1) * 3 + 1]
				+ sd[i * m_pDestImg->m_WidthBytes + (j + 1) * 3 + 1]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + 1]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + j * 3 + 1]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + 1]) / 8;
			averg_R += (
				sd[(i - 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + 2]
				+ sd[(i - 1) * m_pDestImg->m_WidthBytes + j * 3 + 2]
				+ sd[(i - 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + 2]
				+ sd[i * m_pDestImg->m_WidthBytes + (j - 1) * 3 + 2]
				+ sd[i * m_pDestImg->m_WidthBytes + (j + 1) * 3 + 2]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + 2]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + j * 3 + 2]
				+ sd[(i + 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + 2]) / 8;
			if (abs(averg_B - sd[i * m_pDestImg->m_WidthBytes + j * 3]) > Threshold)
				dd[i * m_pDestImg->m_WidthBytes + j * 3] = averg_B;
			else
				dd[i * m_pDestImg->m_WidthBytes + j * 3] = sd[i * m_pDestImg->m_WidthBytes + j * 3];
			if (abs(averg_G - sd[i * m_pDestImg->m_WidthBytes + j * 3 + 1]) > Threshold)
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 1] = averg_G;
			else
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 1] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + 1];
			if (abs(averg_R - sd[i * m_pDestImg->m_WidthBytes + j * 3 + 2]) > Threshold)
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 2] = averg_R;
			else
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 2] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + 2];
		}
	}
	return TRUE;
}

// 自适应平滑处理
BOOL LImageProc::AdaptiveSmooth()
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	int wide = m_pDestImg->m_Width;
	int height = m_pDestImg->m_Height;
	int t[9] = { 0 };		//存储临时窗口数据
	float aver[9] = { 0 };	//灰度级均值
	float K[9] = { 0 };		//方差
	int temp = 0;
	for (int k = 0; k < 3; k++)
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < wide; j++)
			{
				// 处理边界的点
				if (i<2 || j<2 || i>height - 3 || j>wide - 3)
				{
					dd[i * m_pDestImg->m_WidthBytes + j * 3 + k] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + k];
					continue;
				}
				// 第一种领域窗口 九宫格
				t[0] = sd[(i - 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[1] = sd[(i - 1) * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[2] = sd[(i - 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[3] = sd[i * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[4] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[i * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[6] = sd[(i + 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[7] = sd[(i + 1) * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[8] = sd[(i + 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				temp = 0;
				for (int ind = 0; ind < 9; ++ind)
					temp += t[ind];
				aver[0] = (float)temp / 9;
				for (int ind = 0; ind < 9; ++ind)
					K[0] += t[ind] * t[ind] - aver[0] * aver[0];

				// 第二种领域窗口 上两层
				t[0] = sd[(i - 2) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[1] = sd[(i - 2) * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[2] = sd[(i - 2) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[3] = sd[(i - 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[4] = sd[(i - 1) * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[(i - 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[6] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + k];
				temp = 0;
				for (int ind = 0; ind < 7; ++ind)
					temp += t[ind];
				aver[1] = (float)temp / 7;
				for (int ind = 0; ind < 7; ++ind)
					K[1] += t[ind] * t[ind] - aver[1] * aver[1];

				// 第三种领域窗口 左两层
				t[0] = sd[(i - 1) * m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[1] = sd[(i - 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[2] = sd[i * m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[3] = sd[i * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[4] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[(i + 1) * m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[6] = sd[(i + 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				temp = 0;
				for (int ind = 0; ind < 7; ++ind)
					temp += t[ind];
				aver[2] = (float)temp / 7;
				for (int ind = 0; ind < 7; ++ind)
					K[2] += t[ind] * t[ind] - aver[2] * aver[2];

				// 第四种领域窗口 下两层
				t[0] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[1] = sd[(i + 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[2] = sd[(i + 1) * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[3] = sd[(i + 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[4] = sd[(i + 2) * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[(i + 2) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[6] = sd[(i + 2) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				temp = 0;
				for (int ind = 0; ind < 7; ++ind)
					temp += t[ind];
				aver[3] = (float)temp / 7;
				for (int ind = 0; ind < 7; ++ind)
					K[3] += t[ind] * t[ind] - aver[3] * aver[3];

				// 第五种领域窗口 右两层
				t[0] = sd[(i - 1) * m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[1] = sd[(i - 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[2] = sd[i * m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[3] = sd[i * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[4] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[(i + 1) * m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[6] = sd[(i + 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				temp = 0;
				for (int ind = 0; ind < 7; ++ind)
					temp += t[ind];
				aver[4] = (float)temp / 7;
				for (int ind = 0; ind < 7; ++ind)
					K[4] += t[ind] * t[ind] - aver[4] * aver[4];

				// 第六种领域窗口	右上六边形
				t[0] = sd[(i - 2) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[1] = sd[(i - 2) * m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[2] = sd[(i - 1) * m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[3] = sd[(i - 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[4] = sd[(i - 1) * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[i * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[6] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + k];
				temp = 0;
				for (int ind = 0; ind < 7; ++ind)
					temp += t[ind];
				aver[5] = (float)temp / 7;
				for (int ind = 0; ind < 7; ++ind)
					K[5] += t[ind] * t[ind] - aver[5] * aver[5];

				// 第七种领域窗口	左上六边形
				t[0] = sd[(i - 2) * m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[1] = sd[(i - 2) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[2] = sd[(i - 1) * m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[3] = sd[(i - 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[4] = sd[(i - 1) * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[i * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[6] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + k];
				temp = 0;
				for (int ind = 0; ind < 7; ++ind)
					temp += t[ind];
				aver[6] = (float)temp / 7;
				for (int ind = 0; ind < 7; ++ind)
					K[6] += t[ind] * t[ind] - aver[6] * aver[6];

				// 第八种领域窗口	左下六边形
				t[0] = sd[(i + 2) * m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[1] = sd[(i + 2) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[2] = sd[(i + 1) * m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[3] = sd[(i + 1) * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[4] = sd[(i + 1) * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[i * m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[6] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + k];
				temp = 0;
				for (int ind = 0; ind < 7; ++ind)
					temp += t[ind];
				aver[7] = (float)temp / 7;
				for (int ind = 0; ind < 7; ++ind)
					K[7] += t[ind] * t[ind] - aver[7] * aver[7];

				// 第九种领域窗口	右下六边形
				t[0] = sd[(i + 2) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[1] = sd[(i + 2) * m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[2] = sd[(i + 1) * m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[3] = sd[(i + 1) * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[4] = sd[(i + 1) * m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[i * m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[6] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + k];
				temp = 0;
				for (int ind = 0; ind < 7; ++ind)
					temp += t[ind];
				aver[8] = (float)temp / 7;
				for (int ind = 0; ind < 7; ++ind)
					K[8] += t[ind] * t[ind] - aver[8] * aver[8];

				//求方差最小的近邻区域
				float kmin = K[0];
				float amin = 0;
				for (int n = 0; n < 9; ++n)
				{
					if (kmin > K[n])
					{
						kmin = K[n];
						amin = aver[n];
					}
				}
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + k] = (int)(amin + 0.5);
			}
		}
	return TRUE;
}

//十字形中值滤波
BOOL LImageProc::CrossFilter(int n)
{
	if (!ImageIsValid()) return FALSE;
	if (n < 3 || n % 2 != 1)
		return FALSE;
	int wide = m_pDestImg->m_Width;
	int height = m_pDestImg->m_Height;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE * sd = m_pSrcImg->m_pBits;
	BYTE * dd = m_pDestImg->m_pBits;

	int n2 = 0;
	if (n >= 3 && n % 2 == 1)
		n2 = (n - 1) / 2;
	int nn = n + n - 1;
	int mid = (nn - 1) / 2;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < wide; j++)
		{
			// 处理边界的点
			if (i < n2 || j < n2 || i >= height - n2 || j >= wide - n2)
			{
				dd[i * m_pDestImg->m_WidthBytes + j * 3] = sd[i * m_pDestImg->m_WidthBytes + j * 3];
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 1] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + 1];
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 2] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + 2];
				continue;
			}
			std::vector<int> value1;
			std::vector<int> value2;
			std::vector<int> value3;
			for (int ii = i - n2; ii <= i + n2; ii++)
			{
				value1.push_back(sd[ii * m_pDestImg->m_WidthBytes + j * 3]);
				value2.push_back(sd[ii * m_pDestImg->m_WidthBytes + j * 3 + 1]);
				value3.push_back(sd[ii * m_pDestImg->m_WidthBytes + j * 3 + 2]);
			}
			for (int jj = j - n2; jj <= j + n2; jj++) {
				value1.push_back(sd[i * m_pDestImg->m_WidthBytes + jj * 3]);
				value2.push_back(sd[i * m_pDestImg->m_WidthBytes + jj * 3 + 1]);
				value3.push_back(sd[i * m_pDestImg->m_WidthBytes + jj * 3 + 2]);
			}
			std::sort(value1.begin(), value1.end());
			std::sort(value2.begin(), value2.end());
			std::sort(value3.begin(), value3.end());
			dd[i * m_pDestImg->m_WidthBytes + j * 3] = value1[mid];
			dd[i * m_pDestImg->m_WidthBytes + j * 3 + 1] = value2[mid];
			dd[i * m_pDestImg->m_WidthBytes + j * 3 + 2] = value3[mid];
		}
	}
	return TRUE;
}
//N*N最大值滤波
BOOL LImageProc::NNMaxFilter(int n)
{
	if (!ImageIsValid()) return FALSE;
	if (n < 3 || n % 2 != 1)
		return FALSE;
	int wide = m_pDestImg->m_Width;
	int height = m_pDestImg->m_Height;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE * sd = m_pSrcImg->m_pBits;
	BYTE * dd = m_pDestImg->m_pBits;

	int n2 = 0;
	if (n >= 3 && n % 2 == 1)
		n2 = (n - 1) / 2;
	int nn = n * n;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < wide; j++)
		{
			// 处理边界的点
			if (i < n2 || j < n2 || i >= height - n2 || j >= wide - n2)
			{
				dd[i * m_pDestImg->m_WidthBytes + j * 3] = sd[i * m_pDestImg->m_WidthBytes + j * 3];
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 1] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + 1];
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 2] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + 2];
				continue;
			}
			std::vector<int> value1;
			std::vector<int> value2;
			std::vector<int> value3;
			for (int ii = i - n2; ii <= i + n2; ii++)
				for (int jj = j - n2; jj <= j + n2; jj++) {
					value1.push_back(sd[ii * m_pDestImg->m_WidthBytes + jj * 3]);
					value2.push_back(sd[ii * m_pDestImg->m_WidthBytes + jj * 3 + 1]);
					value3.push_back(sd[ii * m_pDestImg->m_WidthBytes + jj * 3 + 2]);
				}
			std::sort(value1.begin(), value1.end());
			std::sort(value2.begin(), value2.end());
			std::sort(value3.begin(), value3.end());
			dd[i * m_pDestImg->m_WidthBytes + j * 3] = value1.front();
			dd[i * m_pDestImg->m_WidthBytes + j * 3 + 1] = value2.front();
			dd[i * m_pDestImg->m_WidthBytes + j * 3 + 2] = value3.front();
		}
	}
	return TRUE;
}
BOOL LImageProc::ADDNoise()
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	float Threshold = 127.5;
	for (int i = 0; i < m_pDestImg->m_Height; i++)
		for (int j = 0; j < m_pDestImg->m_Width; j++)
			for (int k = 0; k < 3; k++)
			{
				// 生成0-10的数字，若为奇数则噪。
				int jud = rand() % 11;
				if ((jud & 1) != 0) {
					dd[i * m_pDestImg->m_WidthBytes + j * 3 + k] = 0;
				}
				else
					dd[i * m_pDestImg->m_WidthBytes + j * 3 + k] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + k];
			}
	return TRUE;
}

//N*N中值滤波
BOOL LImageProc::NNMedFilter(int n)
{
	if (!ImageIsValid()) return FALSE;
	if (n < 3 || n % 2 != 1)
		return FALSE;
	int wide = m_pDestImg->m_Width;
	int height = m_pDestImg->m_Height;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE * sd = m_pSrcImg->m_pBits;
	BYTE * dd = m_pDestImg->m_pBits;

	int n2 = 0;
	if (n >= 3 && n % 2 == 1)
		n2 = (n - 1) / 2;
	int nn = n * n;
	int mid = (nn - 1) / 2;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < wide; j++)
		{
			// 处理边界的点
			if (i < n2 || j < n2 || i >= height - n2 || j >= wide - n2)
			{
				dd[i * m_pDestImg->m_WidthBytes + j * 3] = sd[i * m_pDestImg->m_WidthBytes + j * 3];
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 1] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + 1];
				dd[i * m_pDestImg->m_WidthBytes + j * 3 + 2] = sd[i * m_pDestImg->m_WidthBytes + j * 3 + 2];
				continue;
			}
			std::vector<int> value1;
			std::vector<int> value2;
			std::vector<int> value3;
			for (int ii = i - n2; ii <= i + n2; ii++)
				for (int jj = j - n2; jj <= j + n2; jj++) {
					value1.push_back(sd[ii * m_pDestImg->m_WidthBytes + jj * 3]);
					value2.push_back(sd[ii * m_pDestImg->m_WidthBytes + jj * 3 + 1]);
					value3.push_back(sd[ii * m_pDestImg->m_WidthBytes + jj * 3 + 2]);
				}
			std::sort(value1.begin(), value1.end());
			std::sort(value2.begin(), value2.end());
			std::sort(value3.begin(), value3.end());
			dd[i * m_pDestImg->m_WidthBytes + j * 3] = value1[mid];
			dd[i * m_pDestImg->m_WidthBytes + j * 3 + 1] = value2[mid];
			dd[i * m_pDestImg->m_WidthBytes + j * 3 + 2] = value3[mid];
		}
	}
	return TRUE;
}

// 双向微分
BOOL LImageProc::EdgeTwoDir()
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create((int)m_pSrcImg->m_Width, (int)m_pSrcImg->m_Height);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	for (int i = 1; i < m_pDestImg->m_Height - 1; i++)
	{
		for (int j = 3; j < m_pDestImg->m_WidthBytes - 3; j++)
		{
			int p1 = 0, p2 = 0;
			p1 = sd[(i - 1) * m_pDestImg->m_WidthBytes + j - 3] - sd[(i - 1) * m_pDestImg->m_WidthBytes + j];
			p2 = sd[(i - 1) * m_pDestImg->m_WidthBytes + j] - sd[i * m_pDestImg->m_WidthBytes + j];
			dd[(i - 1) * m_pDestImg->m_WidthBytes + j] = (int)sqrt(p1 * p1 + p2 * p2);
		}
	}
	return TRUE;
}
// 门限梯度
BOOL LImageProc::Threshold()
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create((int)m_pSrcImg->m_Width, (int)m_pSrcImg->m_Height);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;
	for (int i = 1; i < m_pDestImg->m_Height - 1; i++)
	{
		for (int j = 3; j < m_pDestImg->m_WidthBytes - 3; j++)
		{
			int p1 = 0, p2 = 0;
			p1 = sd[(i - 1) * m_pDestImg->m_WidthBytes + j - 3] - sd[(i - 1) * m_pDestImg->m_WidthBytes + j];
			p2 = sd[(i - 1) * m_pDestImg->m_WidthBytes + j] - sd[i * m_pDestImg->m_WidthBytes + j];
			int temp = (int)sqrt(p1 * p1 + p2 * p2);
			if (temp >= 30) {
				if ((temp + 100) > 255)
					dd[(i - 1) * m_pDestImg->m_WidthBytes + j] = 255;
				else
					dd[(i - 1) * m_pDestImg->m_WidthBytes + j] = temp + 100;
			}
			else
				dd[(i - 1) * m_pDestImg->m_WidthBytes + j] = sd[(i - 1) * m_pDestImg->m_WidthBytes + j];
		}
	}
	return TRUE;
}

BOOL LImageProc::Robert()
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create((int)m_pSrcImg->m_Width, (int)m_pSrcImg->m_Height);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;

	int p[4];   //Robert算子
	for (int i = 0; i < m_pDestImg->m_Height - 1; i++)
	{
		for (int j = 0; j < m_pDestImg->m_WidthBytes - 3; j++)
		{
			p[0] = sd[i * m_pDestImg->m_WidthBytes + j];
			p[1] = sd[i * m_pDestImg->m_WidthBytes + j + 3];
			p[2] = sd[(i + 1) * m_pDestImg->m_WidthBytes + j];
			p[3] = sd[(i + 1) * m_pDestImg->m_WidthBytes + j + 3];
			dd[i * m_pDestImg->m_WidthBytes + j] = (int)sqrt((p[0] - p[3]) * (p[0] - p[3]) + (p[1] - p[2]) * (p[1] - p[2]));
		}
	}
	return TRUE;
}

BOOL LImageProc::Sobel()
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create((int)m_pSrcImg->m_Width, (int)m_pSrcImg->m_Height);
	BYTE* sd = m_pSrcImg->m_pBits;
	BYTE* dd = m_pDestImg->m_pBits;

	int tempH;  //模板高度
	int tempW;  //模板宽度
	float tempC;  //模板系数
	int tempMY;   //模板中心元素Y坐标
	int tempMX;   //模板中心元素X坐标
	float Template[9];   //模板数组
	BYTE* p_temp1 = new BYTE[m_pSrcImg->m_Height * m_pSrcImg->m_WidthBytes];
	BYTE* p_temp2 = new BYTE[m_pSrcImg->m_Height * m_pSrcImg->m_WidthBytes];
	//将缓存中的图像复制到原图数据区
	memcpy(p_temp1, sd, m_pSrcImg->m_WidthBytes * m_pSrcImg->m_Height);
	memcpy(p_temp2, sd, m_pSrcImg->m_WidthBytes * m_pSrcImg->m_Height);
	//设置Sobel模板参数
	tempW = 3;
	tempH = 3;
	tempC = 1.0;
	tempMY = 1;
	tempMX = 1;
	Template[0] = -1.0;
	Template[1] = -2.0;
	Template[2] = -1.0;
	Template[3] = 0.0;
	Template[4] = 0.0;
	Template[5] = 0.0;
	Template[6] = 1.0;
	Template[7] = 2.0;
	Template[8] = 1.0;
	Templat(p_temp1, m_pSrcImg->m_WidthBytes, m_pSrcImg->m_Height, tempH, tempW, tempMX, tempMY, Template, tempC);
	//设置Sobel模板参数
	Template[0] = -1.0;
	Template[1] = 0.0;
	Template[2] = 1.0;
	Template[3] = -2.0;
	Template[4] = 0.0;
	Template[5] = 2.0;
	Template[6] = -1.0;
	Template[7] = 0.0;
	Template[8] = 1.0;
	Templat(p_temp2, m_pSrcImg->m_WidthBytes, m_pSrcImg->m_Height, tempH, tempW, tempMX, tempMY, Template, tempC);
	//求两幅缓存图像的最大值
	for (int j = 0; j < m_pSrcImg->m_Height; j++)
		for (int i = 0; i < m_pSrcImg->m_WidthBytes; i++)
			if (p_temp2[j * m_pSrcImg->m_WidthBytes + i] > p_temp1[j * m_pSrcImg->m_WidthBytes + i])
				p_temp1[j * m_pSrcImg->m_WidthBytes + i] = p_temp2[j * m_pSrcImg->m_WidthBytes + i];
	memcpy(dd, p_temp1, m_pSrcImg->m_Height * m_pSrcImg->m_WidthBytes);  // 复制处理后的图像
	delete[]p_temp1;  //删除暂时分配内存
	delete[]p_temp2;  //删除暂时分配内存
	return TRUE;
}

void LImageProc::Templat(BYTE * m_pdata, int DibWidth, int height, int tempH, int tempW, int tempMX, int tempMY, float* fpArray, float fCoef)
{
	// 将原图复制到新缓冲区
	BYTE* p_temp = new BYTE[DibWidth * height];
	memcpy(p_temp, m_pdata, DibWidth * height);
	float fResult;
	for (int i = tempMY; i < height - tempH + tempMY + 1; i++)
		for (int j = 3 * tempMX; j < DibWidth - 3 * tempW + 3 * tempMX + 1; j++)
		{
			//计算像素值
			fResult = 0;
			for (int k = 0; k < tempH; k++)
				for (int l = 0; l < tempW; l++)
					fResult = fResult + m_pdata[(i - tempMY + k) * DibWidth + (j - 3 * tempMX + l * 3)] * fpArray[k * tempW + l];
			//乘上系数
			fResult *= fCoef;
			//取绝对值
			fResult = (float)fabs(fResult);
			//判断是否超过255
			if (fResult > 255)
				//若超过255，直接赋值为255
				p_temp[i * DibWidth + j] = 255;
			else
				//未超过255，赋值为计算结果
				p_temp[i * DibWidth + j] = (int)(fResult + 0.5);
		}
	memcpy(m_pdata, p_temp, DibWidth * height);   //复制处理后的图像
	delete[]p_temp;
}

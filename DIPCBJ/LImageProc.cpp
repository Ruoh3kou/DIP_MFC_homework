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

// ˮƽ����
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

// ��ֱ����
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

// ����
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

// ƽ��
BOOL LImageProc::Translate(int Hori, int Vert)
{
	if (Hori > m_pSrcImg->m_Width || Vert > m_pSrcImg->m_Height)
	{
		AfxMessageBox(_T("ͼ��ƽ�Ʋ��ܳ���ԭʼ����:"), MB_OK, 0);
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
				// ����
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
				// ���ƶ�
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

// ��ת
BOOL LImageProc::Rotate(int angle)  //��ת������
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
			//ת������ͼ��Ϊ���ĵ�����ϵ������������ת
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

// ˫�̶���ֵ
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

// ���ڻҶȱ任
BOOL LImageProc::WindowGray() {
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE *sd = m_pSrcImg->m_pBits;
	BYTE *dd = m_pDestImg->m_pBits;
	int L = 100;
	int U = 220;
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

// ���ֱ任
BOOL LImageProc::PolylineGray() {
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE *sd = m_pSrcImg->m_pBits;
	BYTE *dd = m_pDestImg->m_pBits;
	int x1 = 130, y1 = 50, x2 = 150, y2 = 200;
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j++)
		{
			int x = sd[i*m_pDestImg->m_WidthBytes + j];
			if (x < x1)
				dd[i*m_pDestImg->m_WidthBytes + j] = y1 * x / x1;
			else if (x >= x1 && x <= x2)
				dd[i*m_pDestImg->m_WidthBytes + j] = (y2 - y1)*(x - x1) / (x2 - x1) + y1;
			else
				dd[i*m_pDestImg->m_WidthBytes + j] = (255 - y2)*(x - x2) / (255 - x2) + y2;
		}
	}
	return TRUE;
}

// �ҶȾ��⻯
BOOL LImageProc::GrayEqual() {

	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE *sd = m_pSrcImg->m_pBits;
	BYTE *dd = m_pDestImg->m_pBits;
	float prR[256], prG[256], prB[256];
	// ��ȡR G B�ĻҼ�����
	GetHist(prR, prG, prB);
	// ���о��⻯���� 
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
	//��ֵ��ͼ��
	for (int i = 0; i < m_pDestImg->m_Height; i++)
	{
		for (int j = 0; j < m_pDestImg->m_Width * 3; j += 3)
		{
			dd[i*m_pDestImg->m_WidthBytes + j] = newB[sd[i*m_pDestImg->m_WidthBytes + j]];
			dd[i*m_pDestImg->m_WidthBytes + j + 1] = newG[sd[i*m_pDestImg->m_WidthBytes + j + 1]];
			dd[i*m_pDestImg->m_WidthBytes + j + 2] = newR[sd[i*m_pDestImg->m_WidthBytes + j + 2]];
		}
	}
	return TRUE;
}

// �õ�ֱ��ͼ���Ҽ�����Pr)
void LImageProc::GetHist(float *prR, float *prG, float *prB)
{
	BYTE *sd = m_pSrcImg->m_pBits;
	int counterR[256] = { 0 }, counterG[256] = { 0 }, counterB[256] = { 0 };
	for (int i = 0; i < m_pSrcImg->m_Height; i++)
	{
		for (int j = 0; j < m_pSrcImg->m_Width * 3; j += 3)
		{
			counterB[sd[i*m_pSrcImg->m_WidthBytes + j]]++;
			counterG[sd[i*m_pSrcImg->m_WidthBytes + j + 1]]++;
			counterR[sd[i*m_pSrcImg->m_WidthBytes + j + 2]]++;
		}
	}
	long long bytes = m_pSrcImg->m_Height*m_pSrcImg->m_Width;
	for (int i = 0; i < 256; i++)
	{
		prR[i] = counterR[i] / (bytes * 1.0f);
		prG[i] = counterG[i] / (bytes * 1.0f);
		prB[i] = counterB[i] / (bytes * 1.0f);
	}
}

// �����ڵ�
BOOL LImageProc::ElimateBlackPot()
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE *sd = m_pSrcImg->m_pBits;
	BYTE *dd = m_pDestImg->m_pBits;
	for (int i = 1; i < m_pDestImg->m_Height - 1; i++)
	{
		for (int j = 1; j < m_pDestImg->m_Width - 1; j++)
		{
			int averg_R = 0;
			int averg_G = 0;
			int averg_B = 0;
			averg_B += (
				sd[(i - 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3]
				+ sd[(i - 1)*m_pDestImg->m_WidthBytes + j * 3]
				+ sd[(i - 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3]
				+ sd[i*m_pDestImg->m_WidthBytes + (j - 1) * 3]
				+ sd[i*m_pDestImg->m_WidthBytes + (j + 1) * 3]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + j * 3]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3]);
			averg_G += (
				sd[(i - 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + 1]
				+ sd[(i - 1)*m_pDestImg->m_WidthBytes + j * 3 + 1]
				+ sd[(i - 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + 1]
				+ sd[i*m_pDestImg->m_WidthBytes + (j - 1) * 3 + 1]
				+ sd[i*m_pDestImg->m_WidthBytes + (j + 1) * 3 + 1]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + 1]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + j * 3 + 1]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + 1]);
			averg_R += (
				sd[(i - 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + 2]
				+ sd[(i - 1)*m_pDestImg->m_WidthBytes + j * 3 + 2]
				+ sd[(i - 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + 2]
				+ sd[i*m_pDestImg->m_WidthBytes + (j - 1) * 3 + 2]
				+ sd[i*m_pDestImg->m_WidthBytes + (j + 1) * 3 + 2]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + 2]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + j * 3 + 2]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + 2]);
			if (averg_B == 255 * 8)
				dd[i*m_pDestImg->m_WidthBytes + j * 3] = 255;
			else
				dd[i*m_pDestImg->m_WidthBytes + j * 3] = sd[i*m_pDestImg->m_WidthBytes + j * 3];

			if (averg_G == 255 * 8)
				dd[i*m_pDestImg->m_WidthBytes + j * 3 + 1] = 255;
			else
				dd[i*m_pDestImg->m_WidthBytes + j * 3 + 1] = sd[i*m_pDestImg->m_WidthBytes + j * 3 + 1];

			if (averg_R == 255 * 8)
				dd[i*m_pDestImg->m_WidthBytes + j * 3 + 2] = 255;
			else
				dd[i*m_pDestImg->m_WidthBytes + j * 3 + 2] = sd[i*m_pDestImg->m_WidthBytes + j * 3 + 2];
		}
	}
	return TRUE;
}

// ��������
BOOL LImageProc::ElimateNoise()
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE *sd = m_pSrcImg->m_pBits;
	BYTE *dd = m_pDestImg->m_pBits;
	float Threshold = 127.5;
	for (int i = 1; i < m_pDestImg->m_Height - 1; i++)
	{
		for (int j = 1; j < m_pDestImg->m_Width - 1; j++)
		{
			float averg_R = 0;
			float averg_G = 0;
			float averg_B = 0;
			averg_B += (
				sd[(i - 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3]
				+ sd[(i - 1)*m_pDestImg->m_WidthBytes + j * 3]
				+ sd[(i - 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3]
				+ sd[i*m_pDestImg->m_WidthBytes + (j - 1) * 3]
				+ sd[i*m_pDestImg->m_WidthBytes + (j + 1) * 3]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + j * 3]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3]) / 8;
			averg_G += (
				sd[(i - 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + 1]
				+ sd[(i - 1)*m_pDestImg->m_WidthBytes + j * 3 + 1]
				+ sd[(i - 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + 1]
				+ sd[i*m_pDestImg->m_WidthBytes + (j - 1) * 3 + 1]
				+ sd[i*m_pDestImg->m_WidthBytes + (j + 1) * 3 + 1]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + 1]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + j * 3 + 1]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + 1]) / 8;
			averg_R += (
				sd[(i - 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + 2]
				+ sd[(i - 1)*m_pDestImg->m_WidthBytes + j * 3 + 2]
				+ sd[(i - 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + 2]
				+ sd[i*m_pDestImg->m_WidthBytes + (j - 1) * 3 + 2]
				+ sd[i*m_pDestImg->m_WidthBytes + (j + 1) * 3 + 2]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + 2]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + j * 3 + 2]
				+ sd[(i + 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + 2]) / 8;
			if (abs(averg_B - sd[i*m_pDestImg->m_WidthBytes + j * 3]) > Threshold)
				dd[i*m_pDestImg->m_WidthBytes + j * 3] = averg_B;
			else
				dd[i*m_pDestImg->m_WidthBytes + j * 3] = sd[i*m_pDestImg->m_WidthBytes + j * 3];
			if (abs(averg_G - sd[i*m_pDestImg->m_WidthBytes + j * 3 + 1]) > Threshold)
				dd[i*m_pDestImg->m_WidthBytes + j * 3 + 1] = averg_G;
			else
				dd[i*m_pDestImg->m_WidthBytes + j * 3 + 1] = sd[i*m_pDestImg->m_WidthBytes + j * 3 + 1];
			if (abs(averg_R - sd[i*m_pDestImg->m_WidthBytes + j * 3 + 2]) > Threshold)
				dd[i*m_pDestImg->m_WidthBytes + j * 3 + 2] = averg_R;
			else
				dd[i*m_pDestImg->m_WidthBytes + j * 3 + 2] = sd[i*m_pDestImg->m_WidthBytes + j * 3 + 2];
		}
	}
	return TRUE;
}

// ����Ӧƽ������
BOOL LImageProc::AdaptiveSmooth()
{
	if (!ImageIsValid()) return FALSE;
	m_pDestImg->Create(m_pSrcImg->m_Width, m_pSrcImg->m_Height);
	BYTE *sd = m_pSrcImg->m_pBits;
	BYTE *dd = m_pDestImg->m_pBits;
	int wide = m_pDestImg->m_Width;
	int height = m_pDestImg->m_Height;
	int t[9] = { 0 };		//�洢��ʱ��������
	float aver[9] = { 0 };	//�Ҷȼ���ֵ
	float K[9] = { 0 };		//����
	for (int k = 0; k < 3; k++)
		for (int i = 2; i < height - 2; i++)
		{
			for (int j = 2; j < wide - 2; j++)
			{
				// ��һ�����򴰿� �Ź���
				t[0] = sd[(i - 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[1] = sd[(i - 1)*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[2] = sd[(i - 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[3] = sd[i*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[4] = sd[i*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[i*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[6] = sd[(i + 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[7] = sd[(i + 1)*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[8] = sd[(i + 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				for (int i = 0; i < 9; ++i)
					aver[0] += (float)t[i];
				aver[0] /= 9;
				for (int i = 0; i < 9; ++i)
					t[0] += t[i] * t[i] - aver[0] * aver[0];

				// �ڶ������򴰿� ������
				t[0] = sd[(i - 2)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[1] = sd[(i - 2)*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[2] = sd[(i - 2)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[3] = sd[(i - 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[4] = sd[(i - 1)*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[(i - 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[6] = sd[i*m_pDestImg->m_WidthBytes + j * 3 + k];
				for (int i = 0; i < 7; ++i)
					aver[1] += (float)t[i];
				aver[1] /= 9;
				for (int i = 0; i < 7; ++i)
					t[1] += t[i] * t[i] - aver[1] * aver[1];

				// ���������򴰿� ������
				t[0] = sd[(i - 1)*m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[1] = sd[(i - 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[2] = sd[i*m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[3] = sd[i*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[4] = sd[i*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[(i + 1)*m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[6] = sd[(i + 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				for (int i = 0; i < 7; ++i)
					aver[2] += (float)t[i];
				aver[2] /= 9;
				for (int i = 0; i < 7; ++i)
					t[2] += t[i] * t[i] - aver[2] * aver[2];

				// ���������򴰿� ������
				t[0] = sd[i*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[1] = sd[(i + 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[2] = sd[(i + 1)*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[3] = sd[(i + 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[4] = sd[(i + 2)*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[(i + 2)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[6] = sd[(i + 2)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				for (int i = 0; i < 7; ++i)
					aver[3] += (float)t[i];
				aver[3] /= 9;
				for (int i = 0; i < 7; ++i)
					t[3] += t[i] * t[i] - aver[3] * aver[3];

				// ���������򴰿� ������
				t[0] = sd[(i - 1)*m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[1] = sd[(i - 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[2] = sd[i*m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[3] = sd[i*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[4] = sd[i*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[(i + 1)*m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[6] = sd[(i + 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				for (int i = 0; i < 7; ++i)
					aver[4] += (float)t[i];
				aver[4] /= 9;
				for (int i = 0; i < 7; ++i)
					t[4] += t[i] * t[i] - aver[4] * aver[4];

				// ���������򴰿�	����������
				t[0] = sd[(i - 2)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[1] = sd[(i - 2)*m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[2] = sd[(i - 1)*m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[3] = sd[(i - 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[4] = sd[(i - 1)*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[i*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[6] = sd[i*m_pDestImg->m_WidthBytes + j * 3 + k];
				for (int i = 0; i < 7; ++i)
					aver[5] += (float)t[i];
				aver[5] /= 9;
				for (int i = 0; i < 7; ++i)
					t[5] += t[i] * t[i] - aver[5] * aver[5];

				// ���������򴰿�	����������
				t[0] = sd[(i - 2)*m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[1] = sd[(i - 2)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[2] = sd[(i - 1)*m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[3] = sd[(i - 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[4] = sd[(i - 1)*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[i*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[6] = sd[i*m_pDestImg->m_WidthBytes + j * 3 + k];
				for (int i = 0; i < 7; ++i)
					aver[6] += (float)t[i];
				aver[6] /= 9;
				for (int i = 0; i < 7; ++i)
					t[6] += t[i] * t[i] - aver[6] * aver[6];

				// �ڰ������򴰿�	����������
				t[0] = sd[(i + 2)*m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[1] = sd[(i + 2)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[2] = sd[(i + 1)*m_pDestImg->m_WidthBytes + (j - 2) * 3 + k];
				t[3] = sd[(i + 1)*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[4] = sd[(i + 1)*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[i*m_pDestImg->m_WidthBytes + (j - 1) * 3 + k];
				t[6] = sd[i*m_pDestImg->m_WidthBytes + j * 3 + k];
				for (int i = 0; i < 7; ++i)
					aver[7] += (float)t[i];
				aver[7] /= 9;
				for (int i = 0; i < 7; ++i)
					t[7] += t[i] * t[i] - aver[7] * aver[7];

				// �ھ������򴰿�	����������
				t[0] = sd[(i + 2)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[1] = sd[(i + 2)*m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[2] = sd[(i + 1)*m_pDestImg->m_WidthBytes + (j + 2) * 3 + k];
				t[3] = sd[(i + 1)*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[4] = sd[(i + 1)*m_pDestImg->m_WidthBytes + j * 3 + k];
				t[5] = sd[i*m_pDestImg->m_WidthBytes + (j + 1) * 3 + k];
				t[6] = sd[i*m_pDestImg->m_WidthBytes + j * 3 + k];
				for (int i = 0; i < 7; ++i)
					aver[8] += (float)t[i];
				aver[8] /= 9;
				for (int i = 0; i < 7; ++i)
					t[8] += t[i] * t[i] - aver[8] * aver[8];
				//�󷽲���С�Ľ�������
				float kmin = K[0];
				float amin = 0;
				for (int i = 0; i < 9; ++i)
				{
					if (kmin > K[i])
					{
						kmin = K[i];
						amin = aver[i];
					}
					dd[j* wide * 3 + i * 3 + k] = (int)(amin + 0.5);
				}
			}
		}
	return TRUE;
}
//ʮ������ֵ�˲�
BOOL LImageProc::CrossFilter()
{
	return 0;
}
//N*N���ֵ�˲�
BOOL LImageProc::NNMaxFilter()
{
	return 0;
}
//N*N��ֵ�˲�
BOOL LImageProc::NNMedFilter()
{
	return 0;
}
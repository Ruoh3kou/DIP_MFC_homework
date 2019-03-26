#pragma once
class LImageProc
{
public:
	LImageProc();
	~LImageProc();
	class LImage *m_pSrcImg, *m_pDestImg;
	BOOL ImageIsValid();
	BOOL FlipH();  //水平镜像
	BOOL FlipV();  //垂直镜像
	BOOL Stretch(float aW, float aH);  //缩小/放大
	BOOL Translate(int W, int H);  //平移
	BOOL Rotate(int angle);  //旋转
	// 双固定阈值
	BOOL DoubleThrehold();
	// 窗口灰度变换
	BOOL WindowGray();
};


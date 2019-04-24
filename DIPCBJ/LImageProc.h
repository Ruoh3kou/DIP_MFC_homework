#pragma once
class LImageProc
{
public:
	LImageProc();
	~LImageProc();
	class LImage* m_pSrcImg, * m_pDestImg;
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
	// 折现变换
	BOOL PolylineGray();
	BOOL GrayEqual(); 	// 灰度均衡化
	// 得到直方图（灰级概率Pr)
	void GetHist(float* prR, float* prG, float* prB);
	/* -- 平滑处理 --*/
	BOOL ElimateBlackPot();//消除黑点
	BOOL ElimateNoise();//消除噪声
	BOOL AdaptiveSmooth();//自适应平滑处理
	BOOL NNMedFilter(int);//N*N中值滤波
	BOOL CrossFilter(int);//十字形中值滤波
	BOOL NNMaxFilter(int);//N*N最大值滤波
	BOOL ADDNoise();// 添加噪声

};


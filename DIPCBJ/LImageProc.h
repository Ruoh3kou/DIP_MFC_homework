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
	BOOL GrayEqual(); 		//灰度均衡化
	// 得到直方图（灰级概率Pr)
	void GetHist(float* prR, float* prG, float* prB);
	/* -- 平滑处理 --*/
	BOOL ElimateBlackPot();	//消除黑点
	BOOL ElimateNoise();		//消除噪声
	BOOL AdaptiveSmooth();	//自适应平滑处理
	BOOL NNMedFilter(int);	//N*N中值滤波
	BOOL CrossFilter(int);	//十字形中值滤波
	BOOL NNMaxFilter(int);	//N*N最大值滤波
	BOOL ADDNoise();			//添加噪声
	/* -- 边缘处理 --*/
	BOOL EdgeTwoDir();		//双向微分
	BOOL Threshold();		//门限梯度
	BOOL Robert();			//罗伯特算子
	BOOL Sobel();			//索波尔算子
	//求卷积模板函数
	void Templat(BYTE* m_pdata, int DibWidth, int height, int tempH, int tempW, int tempMX, int tempMY, float* fpArray, float fCoef);
};


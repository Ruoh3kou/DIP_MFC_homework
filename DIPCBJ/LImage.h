class LImage
{
public:
	LImage();
	virtual ~LImage();

	BOOL Create(int a_Width, int a_Height);
	void Destroy();
	BOOL IsValid();
	BOOL CopyFrom(LImage *a_pImg);
	BOOL LoadBmpFile(CString a_Filename);
	BOOL SaveBitmap(CString a_Filename);
	//属性
	int m_Width, m_Height;  //图像的宽度，高度，以象素计
	int m_WidthBytes;  //每行象素所占字节数
	int m_ImageSize;  //象素数据总字节数
	BYTE* m_pBits;  //图像数据块
	CDC m_DC;
	//显示
	BOOL BitBlt(HDC a_DestDC, int a_DestX, int a_DestY, int a_Width, int a_Height,
		int a_SrcX, int a_SrcY, DWORD a_Rop = SRCCOPY);
	BOOL StretchBlt(HDC a_DestDC, int a_DestX, int a_DestY, int a_DestWidth,
		int a_DestHeight, int a_SrcX, int a_SrcY, int a_SrcWidth, int a_SrcHeight,
		DWORD a_Rop = SRCCOPY);
	int FitBlt(HDC a_DestDC, int a_DestX, int a_DestY, int a_DestWidth, int a_DestHeight,
		int a_SrcX, int a_SrcY, int a_SrcWidth, int a_SrcHeight, DWORD a_Rop = SRCCOPY);
protected:
	HBITMAP m_hBitmap;
	HDC m_hMemDC;
private:
	HBITMAP m_hOldBitmap;
};

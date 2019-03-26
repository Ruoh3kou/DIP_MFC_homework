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
	//����
	int m_Width, m_Height;  //ͼ��Ŀ�ȣ��߶ȣ������ؼ�
	int m_WidthBytes;  //ÿ��������ռ�ֽ���
	int m_ImageSize;  //�����������ֽ���
	BYTE* m_pBits;  //ͼ�����ݿ�
	CDC m_DC;
	//��ʾ
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

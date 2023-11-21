#pragma once

#include "DataManager.h"
#include <queue>
#include "Utility/IcrCriticalSection.h"

class CImageItem
{
public:
	HBITMAP_SHARED_PTR m_rawImage;

	int m_controlWidth = 0;

	int m_controlHeight = 0;

	// 等比例缩放显示在控件上的图片，不用的时候需要释放内存
	Gdiplus::Bitmap* m_scaledImage = nullptr;

	// 从m_scaledImage获得的句柄，不用的时候需要释放内存
	HBITMAP m_hScaledImage = NULL;

	// 等比例缩放的比例
	float m_aspectRatio = 1.0f;
};

class CImageHandler : public CWinThread
{
	DECLARE_DYNCREATE(CImageHandler)

public:
	CImageHandler();           // 动态创建所使用的受保护的构造函数
	virtual ~CImageHandler();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	// 增加一张待处理的图片
	void AddImage(const CImageItem& imageItem);

	// 获取一张已处理的图片
	bool GetImage(CImageItem& imageItem);

private:
	void HandleImage();

	void HandleImage(CImageItem& imageItem);

private:
	CCSWrap m_csWrap;

	HANDLE m_hSyncEvent = NULL;

	// 待处理的图片
	std::queue<CImageItem> m_inputImages;

	// 已处理的图片
	std::queue<CImageItem> m_outputImages;

protected:
	DECLARE_MESSAGE_MAP()
};



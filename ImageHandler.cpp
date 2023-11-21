// ImageHandler.cpp: 实现文件
//

#include "pch.h"
#include "record.h"
#include "ImageHandler.h"
#include <atlimage.h>

#define MAX_BUFFER_SIZE  60

IMPLEMENT_DYNCREATE(CImageHandler, CWinThread)

CImageHandler::CImageHandler()
{
	m_hSyncEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_hSyncEvent == NULL)
	{
		LOG_ERROR(L"failed to create event, error is %d", GetLastError());
	}
}

CImageHandler::~CImageHandler()
{
	if (m_hSyncEvent != NULL)
	{
		CloseHandle(m_hSyncEvent);
		m_hSyncEvent = NULL;
	}
}

BOOL CImageHandler::InitInstance()
{
	LOG_DEBUG(L"the image handler is running");
	while (true)
	{
		if (WaitForSingleObject(m_hSyncEvent, INFINITE) == WAIT_OBJECT_0)
		{
			HandleImage();
		}
	}

	return FALSE;
}

void CImageHandler::HandleImage()
{
	while (true)
	{
		CIcrCriticalSection cs1(m_csWrap.GetCS());
		if (m_inputImages.empty())
		{
			ResetEvent(m_hSyncEvent);
			break;
		}

		CImageItem imageItem = m_inputImages.front();
		m_inputImages.pop();
		cs1.Leave();

		HandleImage(imageItem);

		CIcrCriticalSection cs2(m_csWrap.GetCS());
		m_outputImages.push(imageItem);
		if (m_outputImages.size() > MAX_BUFFER_SIZE)
		{
			m_outputImages.pop();
		}
		cs2.Leave();
	}
}

void CImageHandler::AddImage(const CImageItem& imageItem)
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	m_inputImages.push(imageItem);
	if (m_inputImages.size() > MAX_BUFFER_SIZE)
	{
		m_inputImages.pop();
	}
	SetEvent(m_hSyncEvent);
}

bool CImageHandler::GetImage(CImageItem& imageItem)
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	if (m_outputImages.empty())
	{
		return false;
	}

	imageItem = m_outputImages.front();
	m_outputImages.pop();
	return true;
}

void CImageHandler::HandleImage(CImageItem& imageItem)
{
	Gdiplus::Bitmap* originalImage = Gdiplus::Bitmap::FromHBITMAP(imageItem.m_rawImage.get(), NULL);
	if (originalImage == nullptr)
	{
		LOG_ERROR(L"failed to load image from HBITMAP");
		return;
	}

	int imageWidth = originalImage->GetWidth();
	int imageHeight = originalImage->GetHeight();
	int controlWidth = imageItem.m_controlWidth;
	int controlHeight = imageItem.m_controlHeight;
	double widthRatio = controlWidth * 1.0 / imageWidth;
	double heightRatio = controlHeight * 1.0 / imageHeight;
	double aspectRatio = min(widthRatio, heightRatio);
	int displayWidth = (int)(imageWidth * aspectRatio);
	int displayHeight = (int)(imageHeight * aspectRatio);
	Gdiplus::Bitmap* scaledImagePtr = new Gdiplus::Bitmap(displayWidth, displayHeight, originalImage->GetPixelFormat());
	Gdiplus::Bitmap& scaledImage = *scaledImagePtr;
	Gdiplus::Graphics graphics(&scaledImage);
	graphics.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQualityBicubic);
	graphics.DrawImage(originalImage, 0, 0, displayWidth, displayHeight);
	delete originalImage;
	originalImage = nullptr;
	
	imageItem.m_scaledImage = scaledImagePtr;
	imageItem.m_aspectRatio = (float)aspectRatio;	

	HBITMAP hScaledImage = NULL;
	Gdiplus::Status status = scaledImage.GetHBITMAP(Gdiplus::Color::Black, &hScaledImage);
	if (status != Gdiplus::Ok)
	{
		LOG_ERROR(L"failed to call GetHBITMAP, error is %d", status);
		delete scaledImagePtr;
		return;
	}

	imageItem.m_hScaledImage = hScaledImage;
}

int CImageHandler::ExitInstance()
{
	// TODO:    在此执行任意逐线程清理
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CImageHandler, CWinThread)
END_MESSAGE_MAP()


// CImageHandler 消息处理程序

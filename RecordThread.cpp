// RecordThread.cpp: 实现文件
//

#include "pch.h"
#include "record.h"
#include "RecordThread.h"
#include "DataManager.h"
#include "SettingManager.h"

IMPLEMENT_DYNCREATE(CRecordThread, CWinThread)

bool CRecordThread::m_captrueRectChange = true;

CRect CRecordThread::s_captureRect;

CRecordThread::CRecordThread()
{
}

CRecordThread::~CRecordThread()
{
}

BOOL CRecordThread::InitInstance()
{
	LOG_INFO(L"the recording thread runs");
	m_hScreenDC = GetDC(NULL);
	m_hMemDC = CreateCompatibleDC(m_hScreenDC);
	int interval = 1000 / CSettingManager::GetInstance()->GetRecordFrameRate();
	while (true)
	{
		if (m_captrueRectChange)
		{
			m_captureRect = s_captureRect;
			m_captrueRectChange = false;
		}

		ULONGLONG beginTime = GetTickCount64();
		CaptureScreen();
		ULONGLONG elapse = GetTickCount64() - beginTime;
		if (elapse < interval)
		{
			Sleep((DWORD)(interval - elapse));
		}
	}

	return FALSE;
}

void CRecordThread::CaptureScreen()
{
	// Create a compatible bitmap to hold the captured screen
	HBITMAP hBitmap = CreateCompatibleBitmap(m_hScreenDC, m_captureRect.Width(), m_captureRect.Height());

	// Select the bitmap into the memory DC
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(m_hMemDC, hBitmap);

	// Copy the contents of the screen into the bitmap
	BitBlt(m_hMemDC, 0, 0, m_captureRect.Width(), m_captureRect.Height(), m_hScreenDC, 
		m_captureRect.left, m_captureRect.top, SRCCOPY);

	CDataManager::Get()->FrameArrive(HBITMAP_SHARED_PTR(hBitmap));

	// Clean up resources
	SelectObject(m_hMemDC, hOldBitmap);	
}

int CRecordThread::ExitInstance()
{	
	return CWinThread::ExitInstance();
}

void CRecordThread::StartRecord()
{
	static bool bFirst = true;
	if (bFirst)
	{
		bFirst = false;
		CRecordThread* thread = new CRecordThread();
		thread->m_bAutoDelete = TRUE;
		thread->CreateThread();
	}
}

void CRecordThread::SetCaptureRect(CRect captureRect)
{
	s_captureRect = captureRect;
	m_captrueRectChange = true;
}

BEGIN_MESSAGE_MAP(CRecordThread, CWinThread)
END_MESSAGE_MAP()


// CRecordThread 消息处理程序

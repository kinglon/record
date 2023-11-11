#pragma once

class CRecordThread : public CWinThread
{
	DECLARE_DYNCREATE(CRecordThread)

protected:
	CRecordThread();
	virtual ~CRecordThread();

public:
	static void StartRecord();

	static void SetCaptureRect(CRect captureRect);

private:
	void CaptureScreen();

private:
	static bool m_captrueRectChange;

	static CRect s_captureRect;

	CRect m_captureRect;

	HDC m_hScreenDC = NULL;

	HDC m_hMemDC = NULL;

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
};

// CaptureSizeDlg.cpp: 实现文件
//

#include "pch.h"
#include "record.h"
#include "afxdialogex.h"
#include "CaptureSizeDlg.h"
#include "SettingManager.h"
#include "RecordThread.h"

// CCaptureSizeDlg 对话框

IMPLEMENT_DYNAMIC(CCaptureSizeDlg, CDialogEx)

CCaptureSizeDlg::CCaptureSizeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTING_CAPTURE_SIZE, pParent)
{

}

CCaptureSizeDlg::~CCaptureSizeDlg()
{
}

void CCaptureSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCaptureSizeDlg, CDialogEx)
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCaptureSizeDlg 消息处理程序


void CCaptureSizeDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CCaptureSizeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Make the dialog half-transparent
	SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, GetWindowLongPtr(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 100, LWA_ALPHA);	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCaptureSizeDlg::OnDestroy()
{
	GetParent()->ShowWindow(SW_SHOW);

	CDialogEx::OnDestroy();
	
	if (m_bAutoDestroy)
	{
		delete this;
	}
}


BOOL CCaptureSizeDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			DestroyWindow();
			return TRUE;
		}
		else if (pMsg->wParam == VK_RETURN)
		{
			CRect rect;
			GetWindowRect(&rect);
			CRecordThread::SetCaptureRect(rect);

			rect.OffsetRect(m_screenOrigin.x * -1, m_screenOrigin.y * -1);
			CSettingManager::GetInstance()->SetCaptureRect(rect);

			CRecordThread::StartRecord();
			DestroyWindow();
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CCaptureSizeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_DLG)
	{
		static COLORREF bgColor = RGB(0, 0, 255);
		static HBRUSH hBursh = (HBRUSH)CreateSolidBrush(bgColor);
		pDC->SetBkColor(bgColor);
		hbr = hBursh;
	}

	return hbr;
}

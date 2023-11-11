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
END_MESSAGE_MAP()


// CCaptureSizeDlg 消息处理程序


void CCaptureSizeDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_ESCAPE)
	{
		DestroyWindow();
	}
	else if (nChar == VK_RETURN)
	{
		CRect rect;
		GetWindowRect(&rect);
		CRecordThread::SetCaptureRect(rect);

		rect.OffsetRect(m_screenOrigin.x * -1, m_screenOrigin.y * -1);
		CSettingManager::GetInstance()->SetCaptureRect(rect);

		CRecordThread::StartRecord();
		DestroyWindow();
	}

	CDialogEx::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CCaptureSizeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Make the dialog half-transparent
	SetWindowLongPtr(m_hWnd, GWL_EXSTYLE, GetWindowLongPtr(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
	SetLayeredWindowAttributes(0, 50, LWA_ALPHA);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CCaptureSizeDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	
	if (m_bAutoDestroy)
	{
		delete this;
	}
}

#pragma once
#include "afxdialogex.h"


// CCaptureSizeDlg 对话框

class CCaptureSizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCaptureSizeDlg)

public:
	CCaptureSizeDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CCaptureSizeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_CAPTURE_SIZE };
#endif

public:
	void SetAutoDestroy() { m_bAutoDestroy = true; }

	void SetScreenOrgin(CPoint screenOrigin) { m_screenOrigin = screenOrigin; }

private:
	bool m_bAutoDestroy = false;

	CPoint m_screenOrigin;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

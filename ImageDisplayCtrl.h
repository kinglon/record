#pragma once


// CImageDisplayCtrl

class CImageDisplayCtrl : public CStatic
{
	DECLARE_DYNAMIC(CImageDisplayCtrl)

public:
	CImageDisplayCtrl();
	virtual ~CImageDisplayCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};



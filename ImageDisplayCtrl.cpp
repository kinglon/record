// ImageDisplayCtrl.cpp: 实现文件
//

#include "pch.h"
#include "record.h"
#include "ImageDisplayCtrl.h"


// CImageDisplayCtrl

IMPLEMENT_DYNAMIC(CImageDisplayCtrl, CStatic)

CImageDisplayCtrl::CImageDisplayCtrl()
{

}

CImageDisplayCtrl::~CImageDisplayCtrl()
{
}


BEGIN_MESSAGE_MAP(CImageDisplayCtrl, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CImageDisplayCtrl 消息处理程序




BOOL CImageDisplayCtrl::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CImageDisplayCtrl::OnPaint()
{
    HBITMAP bitmap = GetBitmap();
    if (bitmap == NULL)
    {
        return;
    }

	CPaintDC dc(this); // device context for painting	
    CMemDC memDC(dc, this); // Use a memory device context for double buffering
    CDC dcBuffer;
    dcBuffer.CreateCompatibleDC(&memDC.GetDC());

    CRect rect;
    GetClientRect(&rect);
    CBitmap bmpBuffer;
    bmpBuffer.CreateCompatibleBitmap(&memDC.GetDC(), rect.Width(), rect.Height());
    HGDIOBJ oldBitmap = dcBuffer.SelectObject(&bmpBuffer);
    
    dcBuffer.FillSolidRect(&rect, RGB(0, 0, 0));

    CBitmap tmpBitmap;
    tmpBitmap.Attach(bitmap);
    BITMAP bm;
    tmpBitmap.GetBitmap(&bm);
    int x = (rect.Width() - bm.bmWidth) / 2;
    x = max(0, x);
    int y = (rect.Height() - bm.bmHeight) / 2;
    y = max(0, y);
    tmpBitmap.Detach();
    dcBuffer.DrawState(CPoint(x, y), CSize(rect.Width() - 2 * x, rect.Height() - 2 * y), bitmap, DST_BITMAP);

    memDC.GetDC().BitBlt(0, 0, rect.Width(), rect.Height(), &dcBuffer, 0, 0, SRCCOPY);
    dcBuffer.SelectObject(oldBitmap);
}

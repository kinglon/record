
// RecordDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "record.h"
#include "RecordDlg.h"
#include "afxdialogex.h"
#include "SettingManager.h"
#include "DataManager.h"
#include "CaptureSizeDlg.h"
#include <atlimage.h>
#include <ShlObj.h>
#include "RecordThread.h"

using namespace std;

#ifdef _DEBUG
//#define new DEBUG_NEW
#endif


#define WM_SHOW_MAX  (WM_USER+100)

#define IDC_GRID_BASE 5000

#define REFRESH_SCREEN_CTRL_TIMER_ID  1001

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRecordDlg 对话框



CRecordDlg::CRecordDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RECORD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CRecordDlg::~CRecordDlg()
{
	for (auto item : m_grids)
	{
		delete item;
	}
	m_grids.clear();
}

void CRecordDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCREEN_CTRL, m_screenCtrl);
	DDX_Control(pDX, IDC_PLAY_CAPTURE_SPEED_CTRL, m_playCaptureSpeedCtrl);
	DDX_Control(pDX, IDC_PLAY_CACHE_SPEED_CTRL, m_playCacheSpeedCtrl);
	DDX_Control(pDX, IDC_PLAY_PROGRESS_CTRL, m_playProgressCtrl);
	DDX_Control(pDX, IDC_PLAY_CTRL, m_playCtrl);
	DDX_Control(pDX, IDC_STOP_CTRL, m_stopCtrl);
	DDX_Control(pDX, IDC_REVERSE_CTRL, m_reverseCtrl);
	DDX_Control(pDX, IDC_FETCH_IMAGE_CTRL, m_fetchImageCtrl);
	DDX_Control(pDX, IDC_PLAY_CONTENT, m_playContentCtrl);
	DDX_Control(pDX, IDC_PLAY_WINDOW_CTRL, m_playWindowCtrl);
	DDX_Control(pDX, IDC_CACHE1CTRL, m_cache1Ctrl);
	DDX_Control(pDX, IDC_CACHE2CTRL, m_cache2Ctrl);
	DDX_Control(pDX, IDC_ROW_COUNT_CTRL, m_rowCountCtrl);
	DDX_Control(pDX, IDC_COLUMN_COUNT_CTRL, m_columnCountCtrl);
	DDX_Control(pDX, IDC_FRONT_COUNT_CTRL, m_frontCountCtrl);
	DDX_Control(pDX, IDC_BACK_COUNT_CTRL, m_backCountCtrl);
}

BEGIN_MESSAGE_MAP(CRecordDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CAPTURE_CTRL, &CRecordDlg::OnBnClickedCaptureCtrl)
	ON_BN_CLICKED(IDC_CLEAR_CAPTURE_CTRL, &CRecordDlg::OnBnClickedClearCaptureCtrl)
	ON_BN_CLICKED(IDC_OPEN_CAPTURE_WINDOW_CTRL, &CRecordDlg::OnBnClickedOpenCaptureWindowCtrl)
	ON_BN_CLICKED(IDC_CLEAR_CACHE_CTRL, &CRecordDlg::OnBnClickedClearCacheCtrl)
	ON_EN_UPDATE(IDC_ROW_COUNT_CTRL, &CRecordDlg::OnEnUpdateRowCountCtrl)
	ON_EN_UPDATE(IDC_FRONT_COUNT_CTRL, &CRecordDlg::OnEnUpdateFrontCountCtrl)
	ON_EN_UPDATE(IDC_BACK_COUNT_CTRL, &CRecordDlg::OnEnUpdateBackCountCtrl)
	ON_BN_CLICKED(IDC_PLAY_CTRL, &CRecordDlg::OnBnClickedPlayCtrl)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP_CTRL, &CRecordDlg::OnBnClickedStopCtrl)
	ON_BN_CLICKED(IDC_FETCH_IMAGE_CTRL, &CRecordDlg::OnBnClickedFetchImageCtrl)
	ON_WM_MOUSEHWHEEL()
	ON_STN_CLICKED(IDC_PLAY_WINDOW_CTRL, &CRecordDlg::OnStnClickedPlayWindowCtrl)
	ON_EN_UPDATE(IDC_COLUMN_COUNT_CTRL, &CRecordDlg::OnUpdateColumnCountCtrl)
	ON_BN_CLICKED(IDC_REVERSE_CTRL, &CRecordDlg::OnBnClickedReverseCtrl)
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_PLAY_PROGRESS_CTRL, &CRecordDlg::OnTRBNThumbPosChangingPlayProgressCtrl)
	ON_WM_MOUSEWHEEL()
	ON_NOTIFY(TRBN_THUMBPOSCHANGING, IDC_PLAY_CACHE_SPEED_CTRL, &CRecordDlg::OnThumbposchangingPlayCacheSpeedCtrl)
END_MESSAGE_MAP()


// CRecordDlg 消息处理程序

BOOL CRecordDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	CRect wndRect;
	GetWindowRect(&wndRect);
	m_initSizeX = wndRect.Width();
	m_initSizeY = wndRect.Height();

	PostMessage(WM_SHOW_MAX);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRecordDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_RESTORE && lParam != NULL)
	{
		// 禁止最大化后恢复
		return;
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRecordDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRecordDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRecordDlg::InitAllCtrl()
{
	CRect wndRect;
	GetWindowRect(&wndRect);
	int deltaWidth = wndRect.Width() - m_initSizeX;
	int deltaHeight = wndRect.Height() - m_initSizeY;
	if (deltaWidth < 0)
	{
		deltaWidth = 0;
	}
	if (deltaHeight < 0)
	{
		deltaHeight = 0;
	}
	int deltaHalfWidth = deltaWidth / 2;

	// 调整控件位置、宽度、高度
	CtrlAddSize(&m_screenCtrl, deltaHalfWidth, 0);
	CtrlAddSize(&m_playCaptureSpeedCtrl, 0, deltaHeight);
	CtrlAddSize(&m_playCacheSpeedCtrl, 0, deltaHeight);
	CtrlMovePos(&m_playCacheSpeedCtrl, deltaHalfWidth, 0);
	CtrlAddSize(&m_playWindowCtrl, deltaHalfWidth, deltaHeight);	
	CtrlMovePos(&m_playProgressCtrl, 0, deltaHeight);
	CtrlAddSize(&m_playProgressCtrl, deltaHalfWidth, 0);
	CtrlMovePos(&m_playCtrl, 0, deltaHeight);
	CtrlMovePos(&m_stopCtrl, 0, deltaHeight);
	CtrlMovePos(&m_reverseCtrl, 0, deltaHeight);
	CtrlMovePos(&m_fetchImageCtrl, 0, deltaHeight);
	CtrlMovePos(&m_playContentCtrl, 0, deltaHeight);

	// 初始化控件的内容
	CtrlShowBlack(&m_screenCtrl);
	CtrlShowBlack(&m_cache1Ctrl);
	CtrlShowBlack(&m_cache2Ctrl);
	CtrlShowBlack(&m_playWindowCtrl);
	m_rowCountCtrl.SetWindowText(to_wstring(CSettingManager::GetInstance()->GetRowCount()).c_str());
	m_columnCountCtrl.SetWindowText(to_wstring(CSettingManager::GetInstance()->GetColumnCount()).c_str());
	m_frontCountCtrl.SetWindowText(to_wstring(CSettingManager::GetInstance()->GetFrontCount()).c_str());
	m_backCountCtrl.SetWindowText(to_wstring(CSettingManager::GetInstance()->GetBackCount()).c_str());
	m_playCaptureSpeedCtrl.SetRange(0, 200);
	m_playCaptureSpeedCtrl.SetPos(100);
	m_playCacheSpeedCtrl.SetRange(0, 200);
	m_playCacheSpeedCtrl.SetPos(100);

	// 初始化右侧的格子
	InitGrids(CSettingManager::GetInstance()->GetRowCount(), CSettingManager::GetInstance()->GetColumnCount());

	// 启动屏幕控件刷新窗口	
	SetTimer(REFRESH_SCREEN_CTRL_TIMER_ID, 16, NULL);
}

void CRecordDlg::DestroyGrids()
{
	for (auto item : m_grids)
	{
		item->DestroyWindow();
		delete item;
	}
	m_grids.clear();
}

void CRecordDlg::InitGrids(int nRowCount, int nColumnCount)
{
	DestroyGrids();

	CRect wndRect;
	GetClientRect(&wndRect);
	int left = wndRect.Width() / 2;
	int top = 0;
	int rowHeight = wndRect.Height() / nRowCount;
	int columnWidth = wndRect.Width() / 2 / nColumnCount;
	for (int i = 0; i < nRowCount; i++)
	{
		for (int j = 0; j < nColumnCount; j++)
		{
			CStatic* imageCtrl = new CStatic();
			UINT ctrlId = IDC_GRID_BASE + i * nColumnCount + j;
			CRect ctrlRect;
			ctrlRect.left = left + j * columnWidth;
			ctrlRect.top = top + i * rowHeight;
			ctrlRect.right = ctrlRect.left + columnWidth - 5;
			ctrlRect.bottom = ctrlRect.top + rowHeight - 5;
			imageCtrl->Create(_T(""), WS_CHILD | WS_VISIBLE | SS_BITMAP | SS_NOTIFY, ctrlRect, this, ctrlId);
			CtrlShowBlack(imageCtrl);
			m_grids.push_back(imageCtrl);
		}
	}
}

void CRecordDlg::CtrlAddSize(CWnd* ctrl, int deltaWidth, int deltaHeight)
{
	CRect tmpRect;
	ctrl->GetWindowRect(&tmpRect);
	ScreenToClient(&tmpRect);
	ctrl->MoveWindow(tmpRect.left, tmpRect.top, tmpRect.Width() + deltaWidth, tmpRect.Height() + deltaHeight);	
}

void CRecordDlg::CtrlMovePos(CWnd* ctrl, int deltaX, int deltaY)
{
	CRect tmpRect;
	ctrl->GetWindowRect(&tmpRect);
	ScreenToClient(&tmpRect);
	tmpRect.OffsetRect(deltaX, deltaY);
	ctrl->MoveWindow(&tmpRect);
}

void CRecordDlg::CtrlShowBlack(CStatic* ctrl)
{
	CRect wndRect;
	ctrl->GetClientRect(&wndRect);

	CBitmap bitmap;
	bitmap.CreateBitmap(wndRect.Width(), wndRect.Height(), 1, 1, nullptr);
	HBITMAP hOldBmp = ctrl->SetBitmap((HBITMAP)bitmap.Detach());
	if (hOldBmp != NULL)
	{
		DeleteObject(hOldBmp);
	}
}

void CRecordDlg::RefreshGrids()
{
	std::vector<HBITMAP_SHARED_PTR> covers = CDataManager::Get()->GetCaptureCovers();
	for (unsigned int i =0; i<m_grids.size(); i++)
	{
		if (i < covers.size())
		{
			CtrlShowBitmap(m_grids[i], covers[i]);
		}
		else
		{
			CtrlShowBlack(m_grids[i]);
		}
	}
}

void CRecordDlg::RefreshCacheCtrls()
{
	std::vector<HBITMAP_SHARED_PTR> covers = CDataManager::Get()->GetCacheCovers();
	if (covers.size() >= 1)
	{
		CtrlShowBitmap(&m_cache1Ctrl, covers[0]);
	}
	else
	{
		CtrlShowBlack(&m_cache1Ctrl);
	}

	if (covers.size() >= 2)
	{
		CtrlShowBitmap(&m_cache2Ctrl, covers[1]);
	}
	else
	{
		CtrlShowBlack(&m_cache2Ctrl);
	}
}

void CRecordDlg::RefreshPlayContentCtrl()
{
	CString content = L"未选中播放内容";
	CString format = L"选中的播放内容：%s%d";
	if (m_playContent.m_contentType == CONTENT_TYPE_CACHE)
	{
		content.Format(format, L"缓存", m_playContent.m_index+1);
	}
	else if (m_playContent.m_contentType == CONTENT_TYPE_CAPTURE)
	{
		content.Format(format, L"定格抓拍", m_playContent.m_index+1);
	}
	m_playContentCtrl.SetWindowText(content);
}

bool CRecordDlg::OnKeyDown(wchar_t ch)
{
	if (ch == VK_RETURN)
	{
		return true;
	}
	else if (ch == L'Z')
	{
		// 缓存按键
		if (CDataManager::Get()->IsCaching())
		{
			CDataManager::Get()->CacheScreen(false);
		}
		else
		{
			if (!CRecordThread::IsRecording())
			{
				MessageBox(L"请先开启图窗。", L"提示", MB_OK);
				return true;
			}
			if (!CDataManager::Get()->canSave())
			{
				MessageBox(L"可用内存较低，请先清空数据。", L"提示", MB_OK);
				return true;
			}

			CDataManager::Get()->CacheScreen(true);
		}
		RefreshCacheCtrls();
		return true;
	}
	else if (ch == L' ')
	{
		// 空格键定格截图
		OnBnClickedCaptureCtrl();
		return true;
	}
	else if (ch == L'C')
	{
		// 清空抓拍
		ClearCacheAndCapture(false, true);
		return true;
	}
	else if (ch == L'M')
	{
		// 重新开始播放
		StopPlay(true);
		OnBnClickedPlayCtrl();
		return true;
	}
	else if (ch == VK_LEFT)
	{
		if (m_playContent.m_contentType != CONTENT_TYPE_CAPTURE)
		{
			SelectPlayContent(CONTENT_TYPE_CAPTURE, 0);
		}
		else
		{
			if (m_playContent.m_index > 0)
			{
				SelectPlayContent(CONTENT_TYPE_CAPTURE, m_playContent.m_index - 1);
			}
		}
		return true;
	}
	else if (ch == VK_RIGHT)
	{
		if (m_playContent.m_contentType != CONTENT_TYPE_CAPTURE)
		{
			SelectPlayContent(CONTENT_TYPE_CAPTURE, 0);
		}
		else
		{
			if (m_playContent.m_index < m_grids.size() - 1)
			{
				SelectPlayContent(CONTENT_TYPE_CAPTURE, m_playContent.m_index + 1);
			}
		}
		return true;
	}
	else if (ch == VK_UP)
	{
		PlayByFrame(false);
		return true;
	}
	else if (ch == VK_DOWN)
	{
		PlayByFrame(true);
		return true;
	}
	else if (ch == L'B' || ch == L'N')
	{
		if (m_playCtrl.IsWindowEnabled())
		{
			OnBnClickedPlayCtrl();
		}
		else
		{
			StopPlay(false);
		}
		return true;
	}

	return false;
}

bool CRecordDlg::OnLButtonDown(HWND hWnd)
{
	if (hWnd == m_cache1Ctrl.GetSafeHwnd())
	{
		SelectPlayContent(CONTENT_TYPE_CACHE, 0);		
		return true;
	}
	else if (hWnd == m_cache2Ctrl.GetSafeHwnd())
	{
		SelectPlayContent(CONTENT_TYPE_CACHE, 1);
		return true;
	}
	else
	{
		for (unsigned int  i = 0; i < m_grids.size(); i++)
		{
			if (hWnd == m_grids[i]->GetSafeHwnd())
			{
				SelectPlayContent(CONTENT_TYPE_CAPTURE, i);				
				return true;
			}
		}
	}

	return false;
}

void CRecordDlg::ChangeCaptureCount()
{
	int count = CSettingManager::GetInstance()->GetRowCount() * CSettingManager::GetInstance()->GetColumnCount();
	CDataManager::Get()->SetCaptureCount(count);
	if (m_playContent.m_contentType == CONTENT_TYPE_CAPTURE)
	{
		SelectPlayContent(CONTENT_TYPE_CAPTURE, 0);
	}
	InitGrids(CSettingManager::GetInstance()->GetRowCount(), CSettingManager::GetInstance()->GetColumnCount());
}

void CRecordDlg::SetPlayTimer()
{
	float percent = 0.5f;
	if (m_playContent.m_contentType == CONTENT_TYPE_CACHE)
	{
		percent = m_playCacheSpeedCtrl.GetPos() * 1.0f / m_playCacheSpeedCtrl.GetRangeMax();
	}
	else if (m_playContent.m_contentType == CONTENT_TYPE_CAPTURE)
	{
		percent = m_playCaptureSpeedCtrl.GetPos() * 1.0f / m_playCaptureSpeedCtrl.GetRangeMax();
	}
	if (percent < 0.1f)
	{
		percent = 0.1f;
	}

	float playSpeedMultipler = percent * 2.0f;
	int playInterval = 1000 / CSettingManager::GetInstance()->GetRecordFrameRate();  // 正常播放速度
	m_playInterval = int(playInterval / playSpeedMultipler);  // 乘上倍数	
}

void CRecordDlg::CtrlShowBitmap(CStatic* ctrl, HBITMAP_SHARED_PTR bitmap)
{
	Gdiplus::Bitmap* originalImage = Gdiplus::Bitmap::FromHBITMAP(bitmap.get(), NULL);
	if (originalImage == nullptr)
	{
		LOG_ERROR(L"failed to load image from HBITMAP");
		return;
	}

	int imageWidth = originalImage->GetWidth();
	int imageHeight = originalImage->GetHeight();

	CRect rect;
	ctrl->GetClientRect(rect);
	int controlWidth = rect.Width();
	int controlHeight = rect.Height();
	double widthRatio = controlWidth * 1.0 / imageWidth;
	double heightRatio = controlHeight * 1.0 / imageHeight;

	CRect displayRect;
	if (widthRatio < heightRatio)
	{
		int y = (int)((controlHeight - imageHeight * widthRatio) / 2);
		displayRect.SetRect(0, y, controlWidth, y + (int)(imageHeight * widthRatio));
	}
	else
	{
		int x = (int)((controlWidth - imageWidth * heightRatio) / 2);
		displayRect.SetRect(x, 0, x + (int)(imageWidth * heightRatio), controlHeight);
	}

	Gdiplus::Bitmap displayImage(controlWidth, controlHeight, originalImage->GetPixelFormat());
	Gdiplus::Graphics graphics(&displayImage);	
	Gdiplus::SolidBrush blackBrush(Gdiplus::Color::Black);
	graphics.FillRectangle(&blackBrush, 0, 0, displayImage.GetWidth(), displayImage.GetHeight());
	graphics.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQualityBicubic);
	graphics.DrawImage(originalImage, displayRect.left, displayRect.top, displayRect.Width(), displayRect.Height());
	delete originalImage;
	originalImage = nullptr;	

	HBITMAP hBitmap = NULL;
	Gdiplus::Status status = displayImage.GetHBITMAP(Gdiplus::Color::Black, &hBitmap);
	if (status != Gdiplus::Ok)
	{
		LOG_ERROR(L"failed to call GetHBITMAP, error is %d", status);
		return;
	}

	HBITMAP hOldBmp = ctrl->SetBitmap(hBitmap);
	if (hOldBmp != NULL)
	{
		DeleteObject(hOldBmp);
	}
	if (ctrl->GetBitmap() != hBitmap)
	{
		DeleteObject(hBitmap);
	}
}

void CRecordDlg::PlayWindowShowBitmap(HBITMAP_SHARED_PTR bitmap)
{
	Gdiplus::Bitmap* originalImage = Gdiplus::Bitmap::FromHBITMAP(bitmap.get(), NULL);
	if (originalImage == nullptr)
	{
		LOG_ERROR(L"failed to load image from HBITMAP");
		return;
	}

	int imageWidth = originalImage->GetWidth();
	int imageHeight = originalImage->GetHeight();

	CRect rect;
	m_playWindowCtrl.GetClientRect(rect);
	int controlWidth = rect.Width();
	int controlHeight = rect.Height();
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

	m_playImage.Reset();
	m_playImage.m_image = scaledImagePtr;
	m_playImage.m_defaultScaleRatio = (float)aspectRatio;
	m_playImage.m_cropWidth = displayWidth;
	m_playImage.m_cropHeight = displayHeight;
	m_playImage.m_centerPos = CPoint(displayWidth/2, displayHeight/2);

	if (m_playWindowCtrl.GetBitmap() != NULL)
	{
		DeleteObject(m_playWindowCtrl.GetBitmap());
		m_playWindowCtrl.SetBitmap(NULL);
	}

	HBITMAP hScaledImage = NULL;
	Gdiplus::Status status = scaledImage.GetHBITMAP(Gdiplus::Color::Black, &hScaledImage);
	if (status != Gdiplus::Ok)
	{
		LOG_ERROR(L"failed to call GetHBITMAP, error is %d", status);
		return;
	}

	HBITMAP hOldBmp = m_playWindowCtrl.SetBitmap(hScaledImage);
	if (hOldBmp != NULL)
	{
		DeleteObject(hOldBmp);
	}
	if (m_playWindowCtrl.GetBitmap() != hScaledImage)
	{
		DeleteObject(hScaledImage);
	}
}

void CRecordDlg::PlayWindowScaleBitmap(HBITMAP_SHARED_PTR bitmap, float scaleFactor)
{
	if (m_playImage.m_image == nullptr)
	{
		return;
	}

	Gdiplus::Bitmap* originalImage = Gdiplus::Bitmap::FromHBITMAP(bitmap.get(), NULL);
	if (originalImage == nullptr)
	{
		LOG_ERROR(L"failed to load image from HBITMAP");
		return;
	}

	int imageWidth = originalImage->GetWidth();
	int imageHeight = originalImage->GetHeight();
	int displayWidth = (int)(imageWidth * m_playImage.m_defaultScaleRatio * scaleFactor);
	int displayHeight = (int)(imageHeight * m_playImage.m_defaultScaleRatio * scaleFactor);
	Gdiplus::Bitmap* scaledImagePtr = new Gdiplus::Bitmap(displayWidth, displayHeight, originalImage->GetPixelFormat());
	Gdiplus::Bitmap& scaledImage = *scaledImagePtr;
	Gdiplus::Graphics graphics(&scaledImage);
	graphics.SetInterpolationMode(Gdiplus::InterpolationMode::InterpolationModeHighQualityBicubic);
	graphics.DrawImage(originalImage, 0, 0, displayWidth, displayHeight);
	delete originalImage;
	originalImage = nullptr;

	if (m_playImage.m_image)
	{
		delete m_playImage.m_image;
	}
	m_playImage.m_image = scaledImagePtr;
	m_playImage.m_scaleFactor = scaleFactor;
	PlayWindowMoveBitmap(m_playImage.m_centerPos);
}

void CRecordDlg::PlayWindowMoveBitmap(CPoint centerPos)
{	
	if (m_playImage.m_image == nullptr)
	{
		return;
	}

	m_playImage.m_centerPos = centerPos;

	// 计算裁剪区域
	CRect cropRect;
	cropRect.left = (LONG)(centerPos.x * m_playImage.m_scaleFactor - m_playImage.m_cropWidth / 2);
	cropRect.right = (LONG)(centerPos.x * m_playImage.m_scaleFactor + m_playImage.m_cropWidth / 2);
	if (cropRect.left < 0)
	{
		cropRect.left = 0;
		cropRect.right = cropRect.left + m_playImage.m_cropWidth;
	}
	if (cropRect.right > (LONG)m_playImage.m_image->GetWidth())
	{
		cropRect.right = m_playImage.m_image->GetWidth();
		cropRect.left = max(0, cropRect.right - m_playImage.m_cropWidth);
	}
	
	cropRect.top = (LONG)(centerPos.y * m_playImage.m_scaleFactor - m_playImage.m_cropHeight / 2);
	cropRect.bottom = (LONG)(centerPos.y * m_playImage.m_scaleFactor + m_playImage.m_cropHeight / 2);
	if (cropRect.top < 0)
	{
		cropRect.top = 0;
		cropRect.bottom = m_playImage.m_cropHeight;
	}
	if (cropRect.bottom > (LONG)(m_playImage.m_image->GetHeight()))
	{
		cropRect.bottom = m_playImage.m_image->GetHeight();
		cropRect.top = max(0, cropRect.bottom - m_playImage.m_cropHeight);
	}

	// 裁剪图片
	Gdiplus::Bitmap cropImage(cropRect.Width(), cropRect.Height(), m_playImage.m_image->GetPixelFormat());
	Gdiplus::Graphics graphics(&cropImage);
	graphics.DrawImage(m_playImage.m_image, 0, 0, cropRect.left, cropRect.top, cropRect.Width(), cropRect.Height(), Gdiplus::UnitPixel);

	if (m_playWindowCtrl.GetBitmap() != NULL)
	{
		DeleteObject(m_playWindowCtrl.GetBitmap());
		m_playWindowCtrl.SetBitmap(NULL);
	}

	HBITMAP hCropImage = NULL;
	Gdiplus::Status status = cropImage.GetHBITMAP(Gdiplus::Color::Black, &hCropImage);
	if (status != Gdiplus::Ok)
	{
		LOG_ERROR(L"failed to call GetHBITMAP, error is %d", status);
		return;
	}

	HBITMAP hOldBmp = m_playWindowCtrl.SetBitmap(hCropImage);
	if (hOldBmp != NULL)
	{
		DeleteObject(hOldBmp);
	}
	if (m_playWindowCtrl.GetBitmap() != hCropImage)
	{
		DeleteObject(hCropImage);
	}
}

BOOL CRecordDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (OnKeyDown(pMsg->wParam))
		{
			return TRUE;
		}
	}
	else if (pMsg->message == WM_SHOW_MAX)
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		return TRUE;
	}
	else if (pMsg->message == WM_LBUTTONDOWN)
	{		
		if (OnLButtonDown(pMsg->hwnd))
		{
			return TRUE;
		}
	}
	else if (pMsg->message == WM_LBUTTONUP)
	{
		m_movingPlayingImage = false;
	}
	else if (pMsg->message == WM_MOUSEMOVE)
	{
		if (m_movingPlayingImage)
		{
			MovePlayingImage();
			return TRUE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CRecordDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nType == SIZE_MAXIMIZED)
	{
		static bool bFirst = true;
		if (bFirst)
		{
			bFirst = false;
			InitAllCtrl();
		}
	}
}


void CRecordDlg::OnBnClickedCaptureCtrl()
{
	if (!CRecordThread::IsRecording())
	{
		MessageBox(L"请先开启图窗。", L"提示", MB_OK);
		return;
	}
	if (!CDataManager::Get()->canSave())
	{
		MessageBox(L"可用内存较低，请先清空数据。", L"提示", MB_OK);
		return;
	}

	CDataManager::Get()->CaptureScreen();
	RefreshGrids();
}


void CRecordDlg::OnBnClickedClearCaptureCtrl()
{
	ClearCacheAndCapture(false, true);		
}

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor, HDC, LPRECT, LPARAM lParam)
{
	std::vector<HMONITOR>* monitors = reinterpret_cast<std::vector<HMONITOR>*>(lParam);
	monitors->push_back(hMonitor);
	return TRUE;
}

void CRecordDlg::OnBnClickedOpenCaptureWindowCtrl()
{
	bool captureAnotherScreen = false;
	std::vector<HMONITOR> monitors;
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, reinterpret_cast<LPARAM>(&monitors));
	if (monitors.size() > 1)
	{
		if (MessageBox(L"检测到有多个屏幕，是否抓拍录制其他屏幕？", L"提示", MB_YESNO) == IDYES)
		{
			captureAnotherScreen = true;
		}
	}

	CPoint screenTopLeft(0, 0);
	if (captureAnotherScreen)
	{		
		for (auto& item : monitors)
		{
			MONITORINFO monitorInfo;
			monitorInfo.cbSize = sizeof(MONITORINFO);
			if (GetMonitorInfo(item, &monitorInfo) && !(monitorInfo.dwFlags&MONITORINFOF_PRIMARY))
			{
				screenTopLeft.x = monitorInfo.rcMonitor.left;
				screenTopLeft.y = monitorInfo.rcMonitor.top;
				break;
			}
		}
	}

	MessageBox(L"在图窗上按Enter键保存设置，按ESC键退出", L"提示", MB_OK);

	CCaptureSizeDlg* dlg = new CCaptureSizeDlg(this);
	dlg->SetAutoDestroy();
	dlg->SetScreenOrgin(screenTopLeft);
	dlg->Create(IDD_SETTING_CAPTURE_SIZE, NULL);
	CRect captureRect = CSettingManager::GetInstance()->GetCaptureRect();
	captureRect.OffsetRect(screenTopLeft);
	dlg->SetWindowPos(&CWnd::wndTopMost, captureRect.left, captureRect.top, 
		captureRect.Width(), captureRect.Height(), SWP_SHOWWINDOW);

	// 隐藏主窗口
	ShowWindow(SW_HIDE);
}

void CRecordDlg::OnBnClickedClearCacheCtrl()
{
	ClearCacheAndCapture(true, false);
}


void CRecordDlg::OnEnUpdateRowCountCtrl()
{
	CString rowCountString;
	m_rowCountCtrl.GetWindowText(rowCountString);
	if (rowCountString.IsEmpty())
	{
		return;
	}

	int rowCountInt = std::stoi((LPCWSTR)rowCountString);
	if (rowCountInt < 1 || rowCountInt > 15)
	{
		MessageBox(L"定格行数只能在1-15", L"提示", MB_OK);
		return;
	}

	CSettingManager::GetInstance()->SetRowCount(rowCountInt);
	ChangeCaptureCount();
}


//void CRecordDlg::OnEnChangeColumnCountCtrl()
//{
//	CString columnCountString;
//	m_columnCountCtrl.GetWindowText(columnCountString);
//	int columnCountInt = std::stoi((LPCWSTR)columnCountString);
//	if (columnCountInt < 1 || columnCountInt > 4)
//	{
//		MessageBox(L"定格列数只能在1-4", L"提示", MB_OK);
//		return;
//	}
//
//	CSettingManager::GetInstance()->SetColumnCount(columnCountInt);
//	ChangeCaptureCount();
//}


void CRecordDlg::OnEnUpdateFrontCountCtrl()
{
	CString frontCountString;
	m_frontCountCtrl.GetWindowText(frontCountString);
	if (frontCountString.IsEmpty())
	{
		return;
	}
	int frontCountInt = std::stoi((LPCWSTR)frontCountString);
	if (frontCountInt < 1 || frontCountInt > 120)
	{
		MessageBox(L"前抓拍张数只能在1-120", L"提示", MB_OK);
		return;
	}

	CSettingManager::GetInstance()->SetFrontCount(frontCountInt);
	CDataManager::Get()->SetCaptureFrontBackCount(frontCountInt, CSettingManager::GetInstance()->GetBackCount());
}


void CRecordDlg::OnEnUpdateBackCountCtrl()
{
	CString backCountString;
	m_backCountCtrl.GetWindowText(backCountString);
	if (backCountString.IsEmpty())
	{
		return;
	}

	int backCountInt = std::stoi((LPCWSTR)backCountString);
	if (backCountInt < 1 || backCountInt > 120)
	{
		MessageBox(L"后抓拍张数只能在1-120", L"提示", MB_OK);
		return;
	}

	CSettingManager::GetInstance()->SetBackCount(backCountInt);
	CDataManager::Get()->SetCaptureFrontBackCount(CSettingManager::GetInstance()->GetFrontCount(), backCountInt);
}


void CRecordDlg::OnBnClickedPlayCtrl()
{
	if (m_playContent.m_contentType != CONTENT_TYPE_CACHE)
	{
		int count = CDataManager::Get()->GetCacheCount();
		if (count == 0)
		{
			MessageBox(L"没有缓存可以播放", L"提示", MB_OK);
			return;
		}
		else
		{
			SelectPlayContent(CONTENT_TYPE_CACHE, count - 1);
		}
	}

	// 初始化状态	
	m_playCtrl.EnableWindow(FALSE);
	m_reverseCtrl.EnableWindow(FALSE);
	m_stopCtrl.EnableWindow(TRUE);	

	// 设置播放定时器，开始播放
	SetPlayTimer();

	SetFocus();
}

void CRecordDlg::StopPlay(bool complete)
{	
	m_playInterval = 0;
	m_playCtrl.EnableWindow(TRUE);
	m_stopCtrl.EnableWindow(FALSE);
	if (complete)
	{				
		m_reverseCtrl.EnableWindow(TRUE);		
		m_playProgressCtrl.SetPos(0);
		if (m_reverseCtrl.GetCheck() == BST_CHECKED)
		{
			m_playProgressCtrl.SetPos(m_playFrames.size() - 1);
		}
		PlayWindowShowBitmap(m_playFrames[m_playProgressCtrl.GetPos()]);
	}
	else
	{
		m_reverseCtrl.EnableWindow(FALSE);
	}
}

void CRecordDlg::ClearCacheAndCapture(bool cache, bool capture)
{
	if (cache && capture)
	{
		if (MessageBox(L"确定清空截图和缓存吗？", L"提示", MB_OKCANCEL) == IDOK)
		{
			CDataManager::Get()->ClearCacheData();			
			RefreshCacheCtrls();
			CDataManager::Get()->ClearCaptureData();
			RefreshGrids();
		}
	}
	else if (cache)
	{
		if (MessageBox(L"确定清空缓存吗？", L"提示", MB_OKCANCEL) == IDOK)
		{
			CDataManager::Get()->ClearCacheData();
			RefreshCacheCtrls();
		}
	}
	else if (capture)
	{
		if (MessageBox(L"确定清空截图吗？", L"提示", MB_OKCANCEL) == IDOK)
		{
			CDataManager::Get()->ClearCaptureData();
			RefreshGrids();
		}
	}
}

void CRecordDlg::SelectPlayContent(int type, int index)
{
	std::vector<HBITMAP_SHARED_PTR> frames;
	if (type == CONTENT_TYPE_CACHE)
	{
		frames = CDataManager::Get()->GetCacheFrames(index);
	}
	else if (type == CONTENT_TYPE_CAPTURE)
	{
		frames = CDataManager::Get()->GetCaptureFrames(index);
	}
	
	if (frames.size() <= 0)
	{
		return;
	}

	m_playInterval = 0;

	m_playContent.m_contentType = type;
	m_playContent.m_index = index;
	m_playFrames = frames;

	// 初始化控件的状态
	m_playCtrl.EnableWindow(TRUE);
	m_stopCtrl.EnableWindow(FALSE);
	m_reverseCtrl.EnableWindow(TRUE);	
	RefreshPlayContentCtrl();

	m_playProgressCtrl.SetRange(0, m_playFrames.size() - 1);
	if (m_playContent.m_contentType == CONTENT_TYPE_CACHE)
	{		
		m_playProgressCtrl.SetPos(0);
		if (m_reverseCtrl.GetCheck() == BST_CHECKED)
		{
			m_playProgressCtrl.SetPos(m_playFrames.size() - 1);
		}
	}
	else
	{
		int frontCount = CSettingManager::GetInstance()->GetFrontCount();
		m_playProgressCtrl.SetPos(min(frontCount-1, (int)m_playFrames.size()-1));	
	}
	PlayWindowShowBitmap(m_playFrames[m_playProgressCtrl.GetPos()]);
}

void CRecordDlg::PlayByFrame(bool nextFrame)
{
	int currentPlayFrameIndex = m_playProgressCtrl.GetPos();
	if (nextFrame)
	{
		if (m_reverseCtrl.GetCheck() == BST_CHECKED)
		{
			if (currentPlayFrameIndex > 0)
			{
				currentPlayFrameIndex--;
			}
		}
		else
		{
			if (currentPlayFrameIndex < (int)m_playFrames.size() - 1)
			{
				currentPlayFrameIndex++;
			}
		}
	}
	else
	{
		if (m_reverseCtrl.GetCheck() == BST_CHECKED)
		{
			if (currentPlayFrameIndex < (int)m_playFrames.size() - 1)
			{
				currentPlayFrameIndex++;
			}
		}
		else
		{
			if (currentPlayFrameIndex > 0)
			{
				currentPlayFrameIndex--;
			}
		}
	}

	if (currentPlayFrameIndex != m_playProgressCtrl.GetPos())
	{
		m_playProgressCtrl.SetPos(currentPlayFrameIndex);
		PlayWindowShowBitmap(m_playFrames[currentPlayFrameIndex]);
	}
}

bool CRecordDlg::IsPlayFinish()
{
	if (m_reverseCtrl.GetCheck() == BST_CHECKED && m_playProgressCtrl.GetPos() <= 0)
	{
		return true;
	}

	if (m_reverseCtrl.GetCheck() != BST_CHECKED && m_playProgressCtrl.GetPos() >= (int)m_playFrames.size() - 1)
	{
		return true;
	}

	return false;
}

void CRecordDlg::ScalePlayingImage(short delta)
{
	if (m_playImage.m_image == nullptr)
	{
		return;
	}

	float deltaScaleRatio = delta / 120.0f * 0.25f;
	float scaleRatio = m_playImage.m_scaleFactor + deltaScaleRatio;
	if (scaleRatio < 1.0f)
	{
		scaleRatio = 1.0f;
	}
	
	int currentPlayFrameIndex = m_playProgressCtrl.GetPos();
	if (currentPlayFrameIndex >= 0 && currentPlayFrameIndex < (int)m_playFrames.size())
	{
		PlayWindowScaleBitmap(m_playFrames[currentPlayFrameIndex], scaleRatio);
	}
}

void CRecordDlg::MovePlayingImage()
{
	if (m_playImage.m_image == nullptr)
	{
		return;
	}

	POINT pt;
	GetCursorPos(&pt);

	CPoint newCenterPos;
	newCenterPos.x = m_playImage.m_centerPos.x + m_mousePos.x - pt.x;
	if (newCenterPos.x < 0)
	{
		newCenterPos.x = 0;
	}
	else if (newCenterPos.x > (int)m_playImage.m_image->GetWidth())
	{
		newCenterPos.x = m_playImage.m_image->GetWidth();
	}
	newCenterPos.y = m_playImage.m_centerPos.y + m_mousePos.y - pt.y;
	if (newCenterPos.y < 0)
	{
		newCenterPos.y = 0;
	}
	else if (newCenterPos.y > (int)m_playImage.m_image->GetHeight())
	{
		newCenterPos.y = m_playImage.m_image->GetHeight();
	}
	PlayWindowMoveBitmap(newCenterPos);

	m_mousePos = pt;
}

void CRecordDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == REFRESH_SCREEN_CTRL_TIMER_ID)
	{
		static ULONGLONG lastTick = GetTickCount64();
		ULONGLONG nowTick = GetTickCount64();
		int deltaTime = (int)(nowTick - lastTick);
		RefreshScreenCtrl(deltaTime);
		RefreshPlayWindow(deltaTime);
		lastTick = nowTick;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CRecordDlg::RefreshPlayWindow(int deltaTime)
{
	if (m_playInterval == 0)
	{
		return;
	}

	static int totalDelta = 0;
	totalDelta += deltaTime;
	if (totalDelta >= m_playInterval)
	{		
		totalDelta -= m_playInterval;
		PlayByFrame(true);
		if (IsPlayFinish())
		{
			StopPlay(true);
		}
	}
}

void CRecordDlg::RefreshScreenCtrl(int deltaTime)
{
	static int refreshInterval = 1000 / CSettingManager::GetInstance()->GetRecordFrameRate();
	static int totalDelta = 0;
	totalDelta += deltaTime;
	if (totalDelta >= refreshInterval)
	{
		totalDelta -= refreshInterval;
		HBITMAP_SHARED_PTR frame = CDataManager::Get()->GetLatestRecordFrame();
		if (frame.get() != NULL)
		{
			CtrlShowBitmap(&m_screenCtrl, frame);
		}
	}	
}

void CRecordDlg::OnBnClickedStopCtrl()
{
	StopPlay(false);
	SetFocus();
}


void CRecordDlg::OnBnClickedFetchImageCtrl()
{
	if (m_playContent.m_contentType == CONTENT_TYPE_UNKNOWN)
	{
		MessageBox(L"先选择播放内容", L"提示", MB_OK);
		return;
	}

	CString filter = _T("Jpeg Files (*.jpg)|*.jpg||");
	CFileDialog dlg(FALSE, _T(".png"), nullptr, OFN_OVERWRITEPROMPT, filter, this);
	if (dlg.DoModal() == IDOK)
	{
		CString filePath = dlg.GetPathName();
		int currentPlayFrameIndex = m_playProgressCtrl.GetPos();
		if (currentPlayFrameIndex >= 0 && currentPlayFrameIndex < (int)m_playFrames.size())
		{
			CImage image;
			image.Attach(m_playFrames[currentPlayFrameIndex].get());
			HRESULT hr = image.Save(filePath);
			if (FAILED(hr))
			{
				LOG_ERROR(L"failed to save the current image, error is 0x%x", hr);
			}
			image.Detach();
		}		
	}
}


void CRecordDlg::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
{
	CDialogEx::OnMouseHWheel(nFlags, zDelta, pt);
}


void CRecordDlg::OnStnClickedPlayWindowCtrl()
{
	m_playWindowCtrl.SetFocus();
	if (GetKeyState(VK_MENU) < 0)  // ALT键按下
	{
		m_movingPlayingImage = true;
		GetCursorPos(&m_mousePos);
	}	
}


void CRecordDlg::OnUpdateColumnCountCtrl()
{
	CString columnCountString;
	m_columnCountCtrl.GetWindowText(columnCountString);
	if (columnCountString.IsEmpty())
	{
		return;
	}

	int columnCountInt = std::stoi((LPCWSTR)columnCountString);
	if (columnCountInt < 1 || columnCountInt > 15)
	{
		MessageBox(L"定格列数只能在1-15", L"提示", MB_OK);
		return;
	}

	CSettingManager::GetInstance()->SetColumnCount(columnCountInt);
	ChangeCaptureCount();
}


void CRecordDlg::OnBnClickedReverseCtrl()
{
	if (m_playFrames.size() <= 0)
	{
		return;
	}
	
	m_playProgressCtrl.SetPos(0);
	if (m_reverseCtrl.GetCheck() == BST_CHECKED)
	{
		m_playProgressCtrl.SetPos(m_playFrames.size() - 1);
	}
	PlayWindowShowBitmap(m_playFrames[m_playProgressCtrl.GetPos()]);
}


void CRecordDlg::OnTRBNThumbPosChangingPlayProgressCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTRBTHUMBPOSCHANGING* pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING*>(pNMHDR);	
	*pResult = 0;

	if (m_playFrames.size() > 0)
	{
		m_playProgressCtrl.SetPos(pNMTPC->dwPos);
		PlayByFrame(true);
	}
}


BOOL CRecordDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (GetKeyState(VK_MENU) < 0)
	{
		// 按ALT键滚动滑轮，缩放播放图片
		ScalePlayingImage(zDelta);
	}
	else
	{
		// 没按ALT键滚动滑轮，按张播放前后翻张
		if (zDelta < 0)
		{
			PlayByFrame(false);
		}
		else
		{
			PlayByFrame(true);
		}
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CRecordDlg::OnThumbposchangingPlayCacheSpeedCtrl(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTRBTHUMBPOSCHANGING* pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING*>(pNMHDR);	
	*pResult = 0;

	if (!m_playCtrl.IsWindowEnabled())  // 正在播放中
	{
		m_playCacheSpeedCtrl.SetPos(pNMTPC->dwPos);
		SetPlayTimer();
	}
}

#pragma once

#include <vector>
#include "DataManager.h"
#include "ImageDisplayCtrl.h"
#include "ImageHandler.h"

#define CONTENT_TYPE_UNKNOWN   0  //还未指定
#define CONTENT_TYPE_CACHE  1  //缓存
#define CONTENT_TYPE_CAPTURE 2  //定格抓拍

class CPlayContent
{
public:
	int m_contentType = CONTENT_TYPE_UNKNOWN;

	unsigned int m_index = 0;
};

class COperatingImage
{
public:
	void Reset()
	{
		if (m_image)
		{
			delete m_image;
			m_image = nullptr;
		}

		m_defaultScaleRatio = 1.0f;
		m_scaleFactor = 1.0f;
		m_centerPos.x = 0;
		m_centerPos.y = 0;
	}

public:
	// 缩放后图片
	Gdiplus::Bitmap* m_image = nullptr;

	// 完全显示在控件的比例，作为base比例
	float m_defaultScaleRatio = 1.0f;

	// 移动时裁剪的宽度，也就是图片在控件显示的宽度，不一定等于控件的宽度
	int m_cropWidth = 0;

	// 移动时裁剪的高度，也就是图片在控件显示的高度，不一定等于控件的高度
	int m_cropHeight = 0;

	// 在base基础上的缩放因子
	float m_scaleFactor = 1.0f;

	// 在base图片上的中心点位置，裁剪显示时使用
	CPoint m_centerPos;
};

class CRecordDlg : public CDialogEx
{
// 构造
public:
	CRecordDlg(CWnd* pParent = nullptr);	// 标准构造函数
	~CRecordDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RECORD_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

private:
	void InitAllCtrl();

	// 控件大小改变，左上角位置不变
	void CtrlAddSize(CWnd* ctrl, int deltaWidth, int deltaHeight);

	// 控件宽高不变，位置偏移
	void CtrlMovePos(CWnd* ctrl, int deltaX, int deltaY);

	void CtrlShowBlack(CStatic* ctrl);	

	void CtrlShowBitmap(CStatic* ctrl, HBITMAP_SHARED_PTR bitmap);

	void PlayWindowShowBitmap(HBITMAP_SHARED_PTR bitmap);

	void PlayWindowScaleBitmap(HBITMAP_SHARED_PTR bitmap, float scaleFactor);

	void PlayWindowMoveBitmap(CPoint centerPos);

	void InitGrids(int nRowCount, int nColumnCount);

	void DestroyGrids();

	// 刷新定格抓拍的缩略图
	void RefreshGrids();

	// 刷新缓存的缩略图
	void RefreshCacheCtrls();

	// 选择的播放内容说明控件
	void RefreshPlayContentCtrl();

	// 按下快捷键
	bool OnKeyDown(wchar_t ch);

	// 在某个控件上单击，如果消息已处理返回true
	bool OnLButtonDown(HWND hWnd);

	void ChangeCaptureCount();

	void SetPlayTimer();

	void StopPlay(bool complete);

	// 清空定格抓拍和缓存
	void ClearCacheAndCapture(bool cache, bool capture);

	// 选择播放内容
	void SelectPlayContent(int type, int index);

	// 通过按键按张播放
	void PlayByFrame(bool nextFrame);

	bool IsPlayFinish();

	void ScalePlayingImage(short delta);

	void MovePlayingImage();

	void RefreshScreenCtrl(int deltaTime);

	void RefreshPlayWindow(int deltaTime);

	void RefreshCacheStatusCtrl(int deltaTime);

private:
	int m_initSizeX = 0;

	int m_initSizeY = 0;

	std::vector<CStatic*> m_grids;

	CPlayContent m_playContent;

	// 播放帧
	std::vector<HBITMAP_SHARED_PTR> m_playFrames;

	// 显示在播放窗口中的图片
	COperatingImage m_playImage;

	// 播放间隔，根据播放帧率计算，毫秒
	int m_playInterval = 0;

	POINT m_mousePos;

	bool m_movingPlayingImage = false;

	// 实时刷新屏幕的图片处理器
	CImageHandler m_screenImageHandler;

	// 播放窗口的图片处理器
	CImageHandler m_playImageHandler;

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CImageDisplayCtrl m_screenCtrl;
	CSliderCtrl m_playCaptureSpeedCtrl;
	CSliderCtrl m_playCacheSpeedCtrl;
	CSliderCtrl m_playProgressCtrl;
	CButton m_playCtrl;
	CButton m_stopCtrl;
	CButton m_reverseCtrl;
	CButton m_fetchImageCtrl;
	CStatic m_playContentCtrl;
	CImageDisplayCtrl m_playWindowCtrl;
	CStatic m_cache1Ctrl;
	CStatic m_cache2Ctrl;
	CEdit m_rowCountCtrl;
	CEdit m_columnCountCtrl;
	CEdit m_frontCountCtrl;
	CEdit m_backCountCtrl;
	afx_msg void OnBnClickedCaptureCtrl();
	afx_msg void OnBnClickedClearCaptureCtrl();
	afx_msg void OnBnClickedOpenCaptureWindowCtrl();
	afx_msg void OnBnClickedClearCacheCtrl();
	afx_msg void OnEnUpdateRowCountCtrl();
//	afx_msg void OnEnChangeColumnCountCtrl();
	afx_msg void OnEnUpdateFrontCountCtrl();
	afx_msg void OnEnUpdateBackCountCtrl();
	afx_msg void OnBnClickedPlayCtrl();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStopCtrl();
	afx_msg void OnBnClickedFetchImageCtrl();
	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnStnClickedPlayWindowCtrl();
	afx_msg void OnUpdateColumnCountCtrl();
	afx_msg void OnBnClickedReverseCtrl();
	afx_msg void OnTRBNThumbPosChangingPlayProgressCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
//	afx_msg void OnNMCustomdrawPlayCacheSpeedCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnThumbposchangingPlayCacheSpeedCtrl(NMHDR* pNMHDR, LRESULT* pResult);
	CStatic m_cacheStatusCtrl;
};

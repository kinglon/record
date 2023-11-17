#pragma once

#include <memory>
#include <list>
#include <vector>
#include "Utility/IcrCriticalSection.h"

struct HBitmapDeleter
{
    void operator()(HBITMAP hBitmap) const
    {
        ::DeleteObject(hBitmap);
    }
};

// Create a shared_ptr with custom deleter for HBITMAP
typedef std::shared_ptr<std::remove_pointer<HBITMAP>::type> HBITMAP_SHARED_PTR;

class CDataManager
{
public:
    CDataManager();

    static CDataManager* Get();

public:
    void FrameArrive(const HBITMAP_SHARED_PTR& frame);

    // 定格抓拍
    void CaptureScreen();

    // 缓存，isStart true开始缓存，false停止缓存
    void CacheScreen(bool isStart);

    // 是否正在缓存
    bool IsCaching();
    
    // 清空定格抓拍数据
    void ClearCaptureData();

    // 清空缓存的数据
    void ClearCacheData();

    // 获取所有定格抓拍的封面
    std::vector<HBITMAP_SHARED_PTR> GetCaptureCovers();

    // 获取所有缓存的封面
    std::vector<HBITMAP_SHARED_PTR> GetCacheCovers();

    // 获取缓存个数
    int GetCacheCount();

    // 获取指定缓存的帧
    std::vector<HBITMAP_SHARED_PTR> GetCacheFrames(int cacheIndex);

    // 获取指定定格抓拍的帧
    std::vector<HBITMAP_SHARED_PTR> GetCaptureFrames(int captureIndex);

    // 获取最新录制帧
    HBITMAP_SHARED_PTR GetLatestRecordFrame();

    // 设置定格数
    void SetCaptureCount(int captureCount);

    // 设置定格抓拍前后张数
    void SetCaptureFrontBackCount(int frontCount, int backCount);

    bool canSave();

private:
    CCSWrap m_csWrap;

    // 实时录制
    std::list<HBITMAP_SHARED_PTR> m_recordFrames;

    // 定格抓拍
    std::vector<std::vector<HBITMAP_SHARED_PTR>> m_captureFrames;

    // 缓存
    std::vector<std::vector<HBITMAP_SHARED_PTR>> m_cacheFrames;

    bool m_isCaching = false;

    bool m_isCapturing = false;

    ULONGLONG m_capturingBeginTime = 0;

    int m_captureCount = 1;

    int m_frontCount = 20;

    int m_backCount = 20;

    int m_cacheDuration = 60000;

    int m_totalFrameCount = 0;

    ULONGLONG m_statisticStartTime = 0;

    int m_recordFrameRate = 30;

    bool m_canSave = true;
};


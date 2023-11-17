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

    // ����ץ��
    void CaptureScreen();

    // ���棬isStart true��ʼ���棬falseֹͣ����
    void CacheScreen(bool isStart);

    // �Ƿ����ڻ���
    bool IsCaching();
    
    // ��ն���ץ������
    void ClearCaptureData();

    // ��ջ��������
    void ClearCacheData();

    // ��ȡ���ж���ץ�ĵķ���
    std::vector<HBITMAP_SHARED_PTR> GetCaptureCovers();

    // ��ȡ���л���ķ���
    std::vector<HBITMAP_SHARED_PTR> GetCacheCovers();

    // ��ȡ�������
    int GetCacheCount();

    // ��ȡָ�������֡
    std::vector<HBITMAP_SHARED_PTR> GetCacheFrames(int cacheIndex);

    // ��ȡָ������ץ�ĵ�֡
    std::vector<HBITMAP_SHARED_PTR> GetCaptureFrames(int captureIndex);

    // ��ȡ����¼��֡
    HBITMAP_SHARED_PTR GetLatestRecordFrame();

    // ���ö�����
    void SetCaptureCount(int captureCount);

    // ���ö���ץ��ǰ������
    void SetCaptureFrontBackCount(int frontCount, int backCount);

    bool canSave();

private:
    CCSWrap m_csWrap;

    // ʵʱ¼��
    std::list<HBITMAP_SHARED_PTR> m_recordFrames;

    // ����ץ��
    std::vector<std::vector<HBITMAP_SHARED_PTR>> m_captureFrames;

    // ����
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


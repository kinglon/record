#include "pch.h"
#include "DataManager.h"
#include "SettingManager.h"

CDataManager::CDataManager()
{
	m_captureCount = CSettingManager::GetInstance()->GetRowCount() * CSettingManager::GetInstance()->GetColumnCount();
	m_frontCount = CSettingManager::GetInstance()->GetFrontCount();
	m_backCount = CSettingManager::GetInstance()->GetBackCount();
	m_cacheDuration = CSettingManager::GetInstance()->GetCacheDuration() * 1000;
	m_recordFrameRate = CSettingManager::GetInstance()->GetRecordFrameRate();
}

CDataManager* CDataManager::Get()
{
	static CDataManager* instance = new CDataManager();
	return instance;
}

void CDataManager::FrameArrive(const HBITMAP_SHARED_PTR& frame)
{
	CIcrCriticalSection cs(m_csWrap.GetCS());

	m_recordFrames.push_back(frame);
	if ((int)m_recordFrames.size() > m_frontCount)
	{
		m_recordFrames.pop_front();
	}

	// 统计帧率和可用内存
	m_totalFrameCount++;
	if (m_totalFrameCount == 1)
	{
		m_statisticStartTime = GetTickCount64();
	}
	else if (m_totalFrameCount > m_recordFrameRate)
	{
		ULONGLONG totalTime = GetTickCount64() - m_statisticStartTime;
		int frameRate = (int)(1000.0f / (totalTime * 1.0f / m_totalFrameCount));
		LOG_DEBUG(L"the frame rate of recording is %d", frameRate);

		m_totalFrameCount = 0;

		MEMORYSTATUSEX memoryStatus;
		memoryStatus.dwLength = sizeof(memoryStatus);
		if (GlobalMemoryStatusEx(&memoryStatus))
		{
			DWORDLONG availableMemory = memoryStatus.ullAvailPhys;
			float g = availableMemory / 1024.0f / 1024 / 1024;
			LOG_DEBUG(L"the available memory is %fG", g);
			m_canSave = g > 1.0f;
			if (!m_canSave && (m_isCaching || m_isCapturing))
			{
				LOG_INFO(L"stop to capture and caching");
				m_isCaching = false;
				m_isCapturing = false;
			}
		}
	}

	if (m_isCapturing && m_captureFrames.size() > 0)
	{
		auto& lastItem = m_captureFrames[m_captureFrames.size() - 1];
		lastItem.push_back(frame);
		if ((int)lastItem.size() >= m_frontCount + m_backCount)
		{
			m_isCapturing = false;
		}
	}

	if (m_isCaching && m_cacheFrames.size() > 0)
	{
		auto& lastItem = m_cacheFrames[m_cacheFrames.size() - 1];
		lastItem.push_back(frame);
		if (GetTickCount64() - m_capturingBeginTime > m_cacheDuration)
		{
			m_isCaching = false;
		}
	}
}

void CDataManager::CaptureScreen()
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	if (m_isCaching || m_isCapturing)
	{
		LOG_INFO(L"it is caching or capturing");
		return;
	}
	
	if ((int)m_captureFrames.size() >= m_captureCount)
	{
		LOG_INFO(L"can not continue to capture, the count is max.");
		return;
	}

	if (m_recordFrames.size() <= 0)
	{
		LOG_INFO(L"not record any frames.");
		return;
	}

	m_captureFrames.push_back(std::vector<HBITMAP_SHARED_PTR>());
	auto& lastItem = m_captureFrames[m_captureFrames.size() - 1];
	lastItem.insert(lastItem.begin(), m_recordFrames.begin(), m_recordFrames.end());
	m_isCapturing = true;
}

void CDataManager::CacheScreen(bool isStart)
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	if (isStart)
	{
		if (m_isCaching || m_isCapturing)
		{
			LOG_INFO(L"it is caching or capturing");
			return;
		}

		if (m_recordFrames.size() <= 0)
		{
			LOG_INFO(L"not record any frames.");
			return;
		}

		if (m_cacheFrames.size() >= 2)
		{
			m_cacheFrames.erase(m_cacheFrames.begin());
		}
		m_cacheFrames.push_back(std::vector<HBITMAP_SHARED_PTR>());
		m_cacheFrames[m_cacheFrames.size() - 1].push_back(m_recordFrames.back());
		m_isCaching = true;
		m_capturingBeginTime = GetTickCount64();
	}
	else
	{
		m_isCaching = false;
	}
}

bool CDataManager::IsCaching()
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	return m_isCaching;
}

void CDataManager::ClearCaptureData()
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	m_isCapturing = false;
	m_captureFrames.clear();
}

void CDataManager::ClearCacheData()
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	m_isCaching = false;
	m_cacheFrames.clear();
}

std::vector<HBITMAP_SHARED_PTR> CDataManager::GetCaptureCovers()
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	std::vector<HBITMAP_SHARED_PTR> covers;
	for (auto& item : m_captureFrames)
	{
		if (m_frontCount <= (int)item.size())
		{
			covers.push_back(item[m_frontCount -1]);
		}
		else if (item.size() > 0)
		{
			covers.push_back(item[item.size()-1]);
		}
	}

	return covers;
}

std::vector<HBITMAP_SHARED_PTR> CDataManager::GetCacheCovers()
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	std::vector<HBITMAP_SHARED_PTR> covers;
	for (auto& item : m_cacheFrames)
	{
		if (item.size() > 0)
		{
			covers.push_back(item[0]);
		}
	}
	return covers;
}

std::vector<HBITMAP_SHARED_PTR> CDataManager::GetCacheFrames(int cacheIndex)
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	if (cacheIndex < (int)m_cacheFrames.size())
	{
		return m_cacheFrames[cacheIndex];
	}
	return std::vector<HBITMAP_SHARED_PTR>();
}

std::vector<HBITMAP_SHARED_PTR> CDataManager::GetCaptureFrames(int captureIndex)
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	if (captureIndex < (int)m_captureFrames.size())
	{
		return m_captureFrames[captureIndex];
	}
	return std::vector<HBITMAP_SHARED_PTR>();
}

HBITMAP_SHARED_PTR CDataManager::GetLatestRecordFrame()
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	if (m_recordFrames.size() > 0)
	{
		return m_recordFrames.back();
	}
	else
	{
		return HBITMAP_SHARED_PTR();
	}
}

void CDataManager::SetCaptureCount(int captureCount)
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	m_captureCount = captureCount;
	if ((int)m_captureFrames.size() > captureCount)
	{
		m_captureFrames.erase(m_captureFrames.begin(), 
			m_captureFrames.begin() + m_captureFrames.size() - captureCount);
	}
}

void CDataManager::SetCaptureFrontBackCount(int frontCount, int backCount)
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	m_frontCount = frontCount;
	m_backCount = backCount;
	if ((int)m_recordFrames.size() > m_frontCount)
	{
		for (int i = m_recordFrames.size() - m_frontCount; i > 0; i--)
		{
			m_recordFrames.erase(m_recordFrames.begin());
		}
	}
}

bool CDataManager::canSave()
{
	CIcrCriticalSection cs(m_csWrap.GetCS());
	return m_canSave;
}
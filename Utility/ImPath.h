#pragma once

#include <string>
#include <shtypes.h>

#pragma comment(lib, "shlwapi.lib")
#pragma warning(disable:4215)
class CImPath
{
public:
    /**
    @name ��ȡ�����װ·����ִ���ļ����ڵ�·��
    */
    static std::wstring CImPath::GetSoftInstallPath();

    /**
    @name ��ȡӦ������·������б�ܽ�β
    */
	static std::wstring GetDataPath();

    /**
    @name ��ȡ��������·������б�ܽ�β
    */
	static std::wstring GetCachePath();

    /**
    @name ��ȡϵͳ������ʱ�ļ���·��%AppData%\\Local
    */
	static std::wstring GetLocalAppDataPath();

	/**
	@name ��ȡϵͳ������ʱ�ļ���·��%AppData%\\Roaming
	*/
	static std::wstring GetAppDataRoamingPath();

    /**
    @name ��ȡ����·������б�ܽ�β
    */
    static std::wstring GetConfPath();

    /**
    @name ��ȡ��־·������б�ܽ�β
    */
    static std::wstring GetLogPath();

    /**
    @name ��ȡdump·������б�ܽ�β
    */
    static std::wstring GetDumpPath();

private:	
	static std::wstring GetSystemTempPath();
};
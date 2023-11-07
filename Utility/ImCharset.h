#pragma once
#include <string>

class  CImCharset
{
public:
	CImCharset();
	~CImCharset();

public:
 	/**
    @name ANSIתΪUnicode
    @param src ANSI��ʽ�ַ���
    @return wstring Unicode��ʽ�ַ���
    */
 	static std::wstring AnsiToUnicode(const char* src);

    /**
    @name ANSIתΪUTF8
    @param src ANSI��ʽ�ַ���
    @return string UTF8��ʽ�ַ���
    */
    static std::string AnsiToUTF8(const char* src);

    /**
    @name UTF8תΪUNICODE
    @param src UTF8��ʽ�ַ���
    @param srcLength, src�ĳ���
    @return wstring Unicode��ʽ�ַ���
    */
    static std::wstring UTF8ToUnicode(const char* src);

    /**
    @name UNICODEתΪUTF8
    @param src Unicode��ʽ�ַ���
    @return string UTF8��ʽ�ַ���
    */
    static std::string UnicodeToUTF8(const wchar_t* src);

    /**
    @name UNICODEתΪGbk
    @param src Unicode��ʽ�ַ���
    @return string gbk��ʽ�ַ���
    */
    static std::string UnicodeToGbk(const wchar_t* src);

    /**
    @name �ַ����Ƿ�ΪUTF8����
    @param pBuffer �ַ���
    @param size �ַ����ַ�����
    @return bool true��ʾ��UTF8�����ַ�����false��ʾ��UTF8�����ַ���
    */
	static bool IsUTF8(const char* pBuffer, long size);
};
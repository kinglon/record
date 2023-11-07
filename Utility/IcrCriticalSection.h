#pragma once

/***
�ٽ������󣬹��캯���Զ������ٽ��������������Զ��ͷ��ٽ���������֮ǰ���ֶ���Leave��ǰ�뿪�ٽ���
*/
class CIcrCriticalSection
{
public:
    CIcrCriticalSection(CRITICAL_SECTION* pCS);
    ~CIcrCriticalSection();

public:
    /**
    @name �뿪�ٽ���
    */
    void Leave();

private:
    CRITICAL_SECTION* m_pCS = nullptr;
    bool m_bAlreadyLeave = false;
};

/**
���ٽ���������װ�ɶ������ù��캯����ʼ�������������ͷ���Դ
*/
class CCSWrap
{
public:
    CCSWrap()  { InitializeCriticalSection(&m_cs); }
    ~CCSWrap() { DeleteCriticalSection(&m_cs); }

public:
    CRITICAL_SECTION* GetCS() { return &m_cs; }

private:
    CRITICAL_SECTION m_cs;
};
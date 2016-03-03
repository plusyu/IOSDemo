#pragma once
#include <string>
#include <list>
#include <wchar.h>
#include <ObjBase.h>
#include <WinBase.h>
#include <Windows.h>
#include <WinDef.h>
#include <DbgHelp.h>
#include "Smart.Def.h"
namespace Smart
{
    namespace Global
    {
        class CSmart;     //Smart����
        class CLog;       //��־��
        class CCrashDump; //�쳣��
        class CLibManager;//�������
    };
};
//========Smart::Global::CSmart=================================================
class Smart::Global::CSmart
{
public:
    CSmart(void); //���������ܡ������ʼ��
    ~CSmart(void);//���������ܡ������ͷ�
};
//========��־Ŀ������==========================================================
enum LogTargetType
{
    LTT_VSDebugWindow = 0x01,//VS��DebugWindow���
    LTT_DiskFile      = 0x02,//��־�ļ����
    LTT_All           = 0x03
};
//========��־����Ϣ����========================================================
enum LogLineType
{
    LLT_Startup       = 0x01, //��������
    LLT_Shutdown      = 0x02, //�رռ���
    LLT_Info          = 0x04, //��Ϣ����
    LLT_Trace         = 0x08, //���ټ���
    LLT_Warning       = 0x10, //���漶��
    LLT_Error         = 0x20, //���󼶱�
    LLT_Debug         = 0x40, //���Լ���
    LLT_Simple        = 0x3F,
    LLT_All           = 0x7F
};
//========��־���ֶ�����========================================================
enum LogLineFieldType
{
    LLPT_DateTime    = 0x01,//ʱ���ǩ
    LLPT_ThreadID    = 0x02,//�߳�Id
    LLPT_SourceLine  = 0x04,//Դ���к�
    LLPT_InfoType    = 0x08,//��Ϣ����
    LLPT_InfoText    = 0x10,//��Ϣ����
    LLPT_Simple      = 0x1B,
    LLPT_All         = 0x1F
};
//========Smart::Global::CLog===================================================
class Smart::Global::CLog
{
public:
    CLog(void);
    ~CLog(void);
    void SetSource(LPCWSTR File, UINT Line);                   //���������ܡ���������־Դ���ļ����к�
    void Output(UINT LineType, LPCWSTR Fmt, va_list Args);     //���������ܡ��������־
    void OutputError(LPCWSTR Fmt, va_list Args);               //���������ܡ������������־
protected:
    void Initial(void);                                        //���������ܡ�����ʼ����־
    void ClearTimeout(void);                                   //���������ܡ������������־
protected:
    Smart::CSection m_Section;
    std::wstring    m_LogDir;     //��־��·��
    std::wstring    m_AppName;    //��־��������
    WORD            m_CreateDay;  //��־�ļ��������ڣ����ֵ�ǰ�������䲻������������־�ļ�
    Smart::CFile    m_File;       //��־�ļ�
    std::wstring    m_SourceFile; //Դ���ļ�
    UINT            m_SourceLine; //Դ���к�
protected:
    DeclareProperty(UINT32, TargetType);   //��־Ŀ������
    DeclareProperty(UINT32, LineType);     //��־����Ϣ����
    DeclareProperty(UINT32, LineFieldType);//��־���ֶ�����
    DeclareProperty(INT32,  SaveDays);     //��־����������Ĭ��30��
};
//========Smart::Global::CCrashDump=============================================
class Smart::Global::CCrashDump
{
public:
    CCrashDump(void);
    ~CCrashDump(void);
protected:
    void Initial(void);                                                              //���������ܡ�����ʼ���쳣����
    void CreateDump(const std::wstring& DumpPath, PEXCEPTION_POINTERS ExceptionInfo);//���������ܡ�������Dump�ļ�
    BOOL IsDataSectionNeeded(const WCHAR* pModuleName);                              //���������ܡ���This function determines whether we need data sections of the given module 
protected:
    Smart::CSection m_Section;
    std::wstring    m_DumpDir;     //DumpĿ¼
    std::wstring    m_AppName;     //��������
    std::wstring    m_Version;     //����汾
    std::wstring    m_BugReportApp;//Bug�ύ����
    std::wstring    m_BugReportURL;//Bug�ύURL
protected:
    LPTOP_LEVEL_EXCEPTION_FILTER m_ExceptionFilter;
protected:
    static LONG WINAPI UnhandledExceptionFilter(PEXCEPTION_POINTERS ExceptionInfo);                                      //���������ܡ�����ǰExceptionFilter���
    static BOOL WINAPI MiniDump_CBRoutine(PVOID Param, PMINIDUMP_CALLBACK_INPUT Input, PMINIDUMP_CALLBACK_OUTPUT Output);//���������ܡ���A pointer to the MiniDumpCallback callback function. 
    static void __cdecl InvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);
    static void __cdecl PurecallHandler(void);
};
//========Smart::Global::CLibManager============================================
class Smart::Global::CLibManager
{
public:
    CLibManager(void);
    virtual ~CLibManager(void);
    void    Clear(void);                         //���������ܡ��������
    HMODULE LoadDll(const std::wstring& DLLPath);//���������ܡ������ؿ�
    void    UnloadDll(HMODULE Moudle);           //���������ܡ���ж�ؿ�
    LPVOID  CallDllFunction(const std::wstring& DllPath, const std::wstring& FunctionName);
    HRESULT CallDllGetClassObject(const std::wstring& DllPath, REFCLSID rCLSID, REFIID rIID, LPVOID* pVoid);
protected:
    Smart::CSection                 m_Section;
    std::map<HMODULE, std::wstring> m_Modules;
public:
    static LPVOID WINAPI CallFunction(const std::wstring& ModulePath, const std::wstring& FunctionName);//���������ܡ�������ģ�麯��
    static void   WINAPI Clean(void);                                                                   //���������ܡ����������ģ��
};
//========��־�����============================================================
extern Smart::Global::CLog g_Log;
static  void  WINAPI  _OutputLog(UINT LineType, LPCWSTR Fmt, ...)
{
    va_list Args;
    va_start(Args, Fmt);
    g_Log.Output(LineType, Fmt, Args);
    va_end(Args);
};
static  void  WINAPI  _OutputLog_Error(LPCWSTR Fmt, ...)
{
    va_list Args;
    va_start(Args, Fmt);
    g_Log.Output(LLT_Error, Fmt, Args);
    va_end(Args);
};
#define OutputLog         g_Log.SetSource(__WIDEFILE__, __LINE__); _OutputLog
#define OutputLog_Error   g_Log.SetSource(__WIDEFILE__, __LINE__); _OutputLog_Error

#if TRUE
#include "Log.h"
#define LOG                 GenericLog
#define LLT_Info            Info
#else
#define LOG                 OutputLog
#endif
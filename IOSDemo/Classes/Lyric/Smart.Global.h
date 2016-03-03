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
        class CSmart;     //Smart库类
        class CLog;       //日志类
        class CCrashDump; //异常类
        class CLibManager;//库管理类
    };
};
//========Smart::Global::CSmart=================================================
class Smart::Global::CSmart
{
public:
    CSmart(void); //【函数功能】：库初始化
    ~CSmart(void);//【函数功能】：库释放
};
//========日志目标类型==========================================================
enum LogTargetType
{
    LTT_VSDebugWindow = 0x01,//VS的DebugWindow输出
    LTT_DiskFile      = 0x02,//日志文件输出
    LTT_All           = 0x03
};
//========日志行信息类型========================================================
enum LogLineType
{
    LLT_Startup       = 0x01, //启动级别
    LLT_Shutdown      = 0x02, //关闭级别
    LLT_Info          = 0x04, //信息级别
    LLT_Trace         = 0x08, //跟踪级别
    LLT_Warning       = 0x10, //警告级别
    LLT_Error         = 0x20, //错误级别
    LLT_Debug         = 0x40, //调试级别
    LLT_Simple        = 0x3F,
    LLT_All           = 0x7F
};
//========日志行字段类型========================================================
enum LogLineFieldType
{
    LLPT_DateTime    = 0x01,//时间标签
    LLPT_ThreadID    = 0x02,//线程Id
    LLPT_SourceLine  = 0x04,//源码行号
    LLPT_InfoType    = 0x08,//信息类型
    LLPT_InfoText    = 0x10,//信息内容
    LLPT_Simple      = 0x1B,
    LLPT_All         = 0x1F
};
//========Smart::Global::CLog===================================================
class Smart::Global::CLog
{
public:
    CLog(void);
    ~CLog(void);
    void SetSource(LPCWSTR File, UINT Line);                   //【函数功能】：设置日志源码文件和行号
    void Output(UINT LineType, LPCWSTR Fmt, va_list Args);     //【函数功能】：输出日志
    void OutputError(LPCWSTR Fmt, va_list Args);               //【函数功能】：输出错误日志
protected:
    void Initial(void);                                        //【函数功能】：初始化日志
    void ClearTimeout(void);                                   //【函数功能】：清理过期日志
protected:
    Smart::CSection m_Section;
    std::wstring    m_LogDir;     //日志根路径
    std::wstring    m_AppName;    //日志程序名称
    WORD            m_CreateDay;  //日志文件创建日期，发现当前日期与其不符即创建新日志文件
    Smart::CFile    m_File;       //日志文件
    std::wstring    m_SourceFile; //源码文件
    UINT            m_SourceLine; //源码行号
protected:
    DeclareProperty(UINT32, TargetType);   //日志目标类型
    DeclareProperty(UINT32, LineType);     //日志行信息类型
    DeclareProperty(UINT32, LineFieldType);//日志行字段类型
    DeclareProperty(INT32,  SaveDays);     //日志保留天数，默认30天
};
//========Smart::Global::CCrashDump=============================================
class Smart::Global::CCrashDump
{
public:
    CCrashDump(void);
    ~CCrashDump(void);
protected:
    void Initial(void);                                                              //【函数功能】：初始化异常捕获
    void CreateDump(const std::wstring& DumpPath, PEXCEPTION_POINTERS ExceptionInfo);//【函数功能】：创建Dump文件
    BOOL IsDataSectionNeeded(const WCHAR* pModuleName);                              //【函数功能】：This function determines whether we need data sections of the given module 
protected:
    Smart::CSection m_Section;
    std::wstring    m_DumpDir;     //Dump目录
    std::wstring    m_AppName;     //程序名称
    std::wstring    m_Version;     //程序版本
    std::wstring    m_BugReportApp;//Bug提交程序
    std::wstring    m_BugReportURL;//Bug提交URL
protected:
    LPTOP_LEVEL_EXCEPTION_FILTER m_ExceptionFilter;
protected:
    static LONG WINAPI UnhandledExceptionFilter(PEXCEPTION_POINTERS ExceptionInfo);                                      //【函数功能】：当前ExceptionFilter入口
    static BOOL WINAPI MiniDump_CBRoutine(PVOID Param, PMINIDUMP_CALLBACK_INPUT Input, PMINIDUMP_CALLBACK_OUTPUT Output);//【函数功能】：A pointer to the MiniDumpCallback callback function. 
    static void __cdecl InvalidParameterHandler(const wchar_t* expression, const wchar_t* function, const wchar_t* file, unsigned int line, uintptr_t pReserved);
    static void __cdecl PurecallHandler(void);
};
//========Smart::Global::CLibManager============================================
class Smart::Global::CLibManager
{
public:
    CLibManager(void);
    virtual ~CLibManager(void);
    void    Clear(void);                         //【函数功能】：清理库
    HMODULE LoadDll(const std::wstring& DLLPath);//【函数功能】：加载库
    void    UnloadDll(HMODULE Moudle);           //【函数功能】：卸载库
    LPVOID  CallDllFunction(const std::wstring& DllPath, const std::wstring& FunctionName);
    HRESULT CallDllGetClassObject(const std::wstring& DllPath, REFCLSID rCLSID, REFIID rIID, LPVOID* pVoid);
protected:
    Smart::CSection                 m_Section;
    std::map<HMODULE, std::wstring> m_Modules;
public:
    static LPVOID WINAPI CallFunction(const std::wstring& ModulePath, const std::wstring& FunctionName);//【函数功能】：调用模块函数
    static void   WINAPI Clean(void);                                                                   //【函数功能】：清理加载模块
};
//========日志输出宏============================================================
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
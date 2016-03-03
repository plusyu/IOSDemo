#include "StdAfx.h"
#include <direct.h>
#include <Share.h>
#include <list>
#include <WinUser.h>
#include "Smart.Global.h"
#include "Smart.DUI.h"
#include "Smart.Registry.h"
#include "Smart.Shell.h"
//========Smart::Global::CSmart=================================================
Smart::Global::CSmart g_Smart;
Smart::Global::CSmart::CSmart(void)
{
    ::CoInitialize(NULL);
#ifdef Smart_Net_CURL
    Smart::Net::Curl::Initial();
#endif
#ifdef Smart_SQL_SQLite
    Smart::SQLite::Initial();
#endif
#ifdef Smart_SQL_MySQL
    Smart::MySQL::Initial();
#endif
#ifdef Smart_DUI
    DirectUIInitial(TRUE);
#endif
}
Smart::Global::CSmart::~CSmart(void)
{
#ifdef Smart_DUI
    DirectUIFinal();
#endif
#ifdef Smart_SQL_MySQL
    Smart::MySQL::Final();
#endif
#ifdef Smart_SQL_SQLite
    Smart::SQLite::Final();
#endif
#ifdef Smart_Net_CURL
    Smart::Net::Curl::Final();
#endif
    ::CoUninitialize();
}
//========Smart::Global::CLog===================================================
Smart::Global::CLog g_Log;
Smart::Global::CLog::CLog(void)
    : m_Section(L"Global\\{C17F79C1-9E39-474E-B238-718AC911141C}_日志")
    , m_CreateDay(100) //创建日期，初始化为不可能的日期
    , m_SourceLine(0)

    , m_TargetType(LTT_All)
    , m_LineType(LLT_All)
    , m_LineFieldType(LLPT_Simple)
    , m_SaveDays(-365)
{
    SASectionI;
#ifdef IDS_PROGRAM_LOG
    //格式为"Log目录;程序名称"
    wchar_t                   Buffer[1024] = {0};
    std::vector<std::wstring> Array;
    if(::LoadStringW(Smart::Shell::CModule::GetEXECHandle(), IDS_PROGRAM_LOG, Buffer, 1024) != 0)
    {
        Smart::CText::Split(Buffer, ';', Array);
        if(Array.size() != 2)    { this->Initial();}
        else if(Array[0].empty()){ this->Initial();}
        else if(Array[1].empty()){ this->Initial();}
        else
        {
            this->m_LogDir  = Smart::CText::Format(L"%s%s\\Logs\\", Smart::Shell::CExplorer::GetSpecialFolderPath(CSIDL_APPDATA).c_str(), Array[0].c_str());
#ifdef _DEBUG
            this->m_AppName =  Array[1] + L"D";
#else
            this->m_AppName =  Array[1];
#endif
        }
    }
    else
#endif
    {
        this->Initial();
    }
}
Smart::Global::CLog::~CLog(void)
{
    this->m_File.Close();
}
void Smart::Global::CLog::SetSource(LPCWSTR File, UINT Line)
{
    SASectionI;
    this->m_SourceFile = File;
    this->m_SourceLine = Line;
}
void Smart::Global::CLog::Output(UINT LineType, LPCWSTR Fmt, va_list Args)
{
    SASectionI;
    if((this->m_LineType & LineType) == 0){ return;}//忽略此日志

    SYSTEMTIME LT;
    ::GetLocalTime(&LT); 
    std::wstring LineText;

    if(this->m_LineFieldType & LLPT_DateTime)  { LineText.append(Smart::CText::Format(L"[%02u:%02u:%02u.%03u]|", LT.wHour, LT.wMinute, LT.wSecond, LT.wMilliseconds)); }
    if(this->m_LineFieldType & LLPT_ThreadID)  { LineText.append(Smart::CText::Format(L"[TID:%05d]|", ::GetCurrentThreadId())); }
    if(this->m_LineFieldType & LLPT_SourceLine){ LineText.append(Smart::CText::Format(L"[%s:%d]|", this->m_SourceFile.c_str(), this->m_SourceLine)); }
    if(this->m_LineFieldType & LLPT_InfoType)  { LineText.append(Smart::CText::Format(L"[%d]|",    LineType)); }
    if(this->m_LineFieldType & LLPT_InfoText)  { LineText.append(Smart::CText::Format_(Fmt, Args)); }
    LineText.append(L"\r\n");
    if(this->m_TargetType & LTT_VSDebugWindow) { ::OutputDebugStringW(LineText.c_str()); }

//#ifndef _DEBUG
    if(this->m_TargetType & LTT_DiskFile)
    {
        if(LT.wDay != this->m_CreateDay)
        {
            this->m_File.Close();
        }
        if(!this->m_File.IsOpen())
        {
            this->ClearTimeout();
            std::wstring LogPath = Smart::CText::Format(L"%s%s_%04u%02u%02u.log", this->m_LogDir.c_str(), this->m_AppName.c_str(), LT.wYear, LT.wMonth, LT.wDay);
            Smart::CPath::CreateDirFromFilePath(LogPath);
            this->m_File.Open(LogPath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, TRUE);
            this->m_File.IsEmpty() ? this->m_File.Write() : this->m_File.SeekEnd();
            this->m_CreateDay    = LT.wDay;
        }
        if(this->m_File.IsOpen())
        {
            this->m_File.SeekEnd();
            this->m_File.Write(L"%s", LineText.c_str());
            //::_wsetlocale(LC_ALL, L"chs");//chs
            //::fflush(this->m_File);
        }
    }
//#endif
}
void Smart::Global::CLog::OutputError(LPCWSTR Fmt, va_list Args)
{
    this->Output(LLT_Error, Fmt, Args);
}
//------------------------------------------------------------------------------
void Smart::Global::CLog::Initial(void)
{
    this->m_LogDir  = Smart::CText::Format(L"%sSmart\\Logs\\", Smart::Shell::CExplorer::GetSpecialFolderPath(CSIDL_APPDATA).c_str());
#ifdef _DEBUG
    this->m_AppName = Smart::Shell::CModule::GetEXECName() + L"D";
#else
    this->m_AppName = Smart::Shell::CModule::GetEXECName();
#endif
}
void Smart::Global::CLog::ClearTimeout(void)
{
    std::list<std::wstring> FileList;
    std::wstring            FileFilter   = Smart::CText::Format(L"%s*.Log", this->m_LogDir.c_str());
    WIN32_FIND_DATAW        FindFileData = {0};
    HANDLE                  FindHandle   = ::FindFirstFileW(FileFilter.c_str(), &FindFileData);
    if(FindHandle != INVALID_HANDLE_VALUE)
    {
        do
        {
            FileList.push_back(Smart::CText::Format(L"%s%s", this->m_LogDir.c_str(), FindFileData.cFileName));
        }while(::FindNextFileW(FindHandle, &FindFileData));
        ::FindClose(FindHandle);
    }

    if(!FileList.empty())
    {
        SYSTEMTIME ST = {0};
        ::GetSystemTime(&ST);
        Smart::CDateTime::AddDays(ST, this->m_SaveDays);
        std::wstring LogPath = Smart::CText::Format(L"%s%s_%04u%02u%02u.log", this->m_LogDir.c_str(), this->m_AppName.c_str(), ST.wYear, ST.wMonth, ST.wDay);

        for(auto iter = FileList.begin(); iter != FileList.end(); iter++)
        {
            if(*iter < LogPath)
            {
                ::DeleteFileW(iter->c_str());
            }
        }

        FileList.clear();
    }
}
//========Smart::Global::CCrashDump=============================================
#pragma comment(lib , "DbgHelp.lib")
Smart::Global::CCrashDump g_Exception;
Smart::Global::CCrashDump::CCrashDump(void)
    : m_ExceptionFilter(NULL)
{
    SASectionI;
#ifdef IDS_PROGRAM_EXCEPTION
    //格式为"Bug提交程序;Bug提交URL;Dump目录;程序名称;程序版本"
    wchar_t                   Buffer[1024] = {0};
    std::vector<std::wstring> Array;
    if(::LoadStringW(Smart::Shell::CModule::GetEXECHandle(), IDS_PROGRAM_EXCEPTION, Buffer, 1024) != 0)
    {
        Smart::CText::Split(Buffer, ';', Array);
        if(Array.size() != 5)    { this->Initial();}
        else if(Array[2].empty()){ this->Initial();}
        else if(Array[3].empty()){ this->Initial();}
        else
        {
            this->m_BugReportApp = Array[0];
            this->m_BugReportURL = Array[1];
            this->m_DumpDir = Smart::CText::Format(L"%s%s\\Dumps\\", Smart::Shell::CExplorer::GetSpecialFolderPath(CSIDL_APPDATA).c_str(), Array[2].c_str());
            this->m_AppName      = Array[3];
            this->m_Version      = Array[4];
        }
    }
    else
#endif
    {
        this->Initial();
    }
    this->m_ExceptionFilter = ::SetUnhandledExceptionFilter(Smart::Global::CCrashDump::UnhandledExceptionFilter);
    ::_set_invalid_parameter_handler(Smart::Global::CCrashDump::InvalidParameterHandler);
    ::_set_purecall_handler(Smart::Global::CCrashDump::PurecallHandler);
}
Smart::Global::CCrashDump::~CCrashDump(void)
{
    ::SetUnhandledExceptionFilter(this->m_ExceptionFilter);
}
//------------------------------------------------------------------------------
void Smart::Global::CCrashDump::Initial(void)
{
    this->m_BugReportApp.clear();
    this->m_BugReportURL.clear();
    this->m_DumpDir = Smart::CText::Format(L"%sSmart\\Dumps\\", Smart::Shell::CExplorer::GetSpecialFolderPath(CSIDL_APPDATA).c_str());
    this->m_AppName = Smart::Shell::CModule::GetEXECName();
    this->m_Version = L"0.0.0.0";
}
void Smart::Global::CCrashDump::CreateDump(const std::wstring& DumpPath, PEXCEPTION_POINTERS ExceptionInfo)
{
    // 创建Dump文件
    Smart::CPath::CreateDirFromFilePath(DumpPath);
    HANDLE hDumpFile = ::CreateFile(DumpPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(INVALID_HANDLE_VALUE != hDumpFile)
    {
        //Dump类型
        MINIDUMP_TYPE DumpType = (MINIDUMP_TYPE)(MiniDumpWithIndirectlyReferencedMemory | MiniDumpScanMemory);
        //Dump异常信息
        MINIDUMP_EXCEPTION_INFORMATION DumpEI;
        DumpEI.ThreadId          = ::GetCurrentThreadId();
        DumpEI.ExceptionPointers = ExceptionInfo;
        DumpEI.ClientPointers    = FALSE;
        //Dump回调信息
        MINIDUMP_CALLBACK_INFORMATION DumpCI; 
        DumpCI.CallbackRoutine = Smart::Global::CCrashDump::MiniDump_CBRoutine;
        DumpCI.CallbackParam   = NULL; 

        ::MiniDumpWriteDump(::GetCurrentProcess(), ::GetCurrentProcessId(), hDumpFile, DumpType, (ExceptionInfo == NULL) ? NULL : &DumpEI, 0, &DumpCI);

        ::CloseHandle(hDumpFile);
    }
}
BOOL Smart::Global::CCrashDump::IsDataSectionNeeded( const WCHAR* pModuleName ) 
{
    // Check parameters 
    if( pModuleName == NULL ) 
    {
        return FALSE; 
    }

    WCHAR szModule[512];
    GetModuleFileName(NULL, szModule, 512);
    if( _wcsicmp(szModule, pModuleName) == 0)
    {
        return TRUE;
    }

    // Extract the module name 
    WCHAR szFileName[_MAX_FNAME] = L""; 
    _wsplitpath_s( pModuleName, NULL, 0, NULL, 0, szFileName, _MAX_FNAME, NULL, 0 ); 

    // Compare the name with the list of known names and decide 
    if( _wcsicmp( szFileName, L"ntdll" ) == 0 ) 
    {
        return TRUE; 
    }

    // Complete 
    return FALSE; 
}
//------------------------------------------------------------------------------
LONG Smart::Global::CCrashDump::UnhandledExceptionFilter(PEXCEPTION_POINTERS ExceptionInfo)
{
    OutputLog(LLT_Shutdown, L"%s Found UnhandledException!!!\n", g_Exception.m_Version.c_str());

    std::wstring DumpPath = Smart::CText::Format(L"%s%s_%s.dmp", g_Exception.m_DumpDir.c_str(), g_Exception.m_AppName.c_str(), Smart::CDateTime::GetNow().ToString(FALSE).c_str());
    g_Exception.CreateDump(DumpPath, ExceptionInfo);
    if(!g_Exception.m_BugReportApp.empty())
    {
        std::wstring BugReportParam = Smart::CText::Format(L"\"%s\" \"%s\" \"%s\" \"%s\""
            , g_Exception.m_BugReportURL.c_str()
            , g_Exception.m_DumpDir.c_str()
            , g_Exception.m_AppName.c_str()
            , g_Exception.m_Version.c_str());
        Smart::Shell::CModule::Execute(g_Exception.m_BugReportApp, BugReportParam);
    }
    return EXCEPTION_EXECUTE_HANDLER;
}
BOOL Smart::Global::CCrashDump::MiniDump_CBRoutine(PVOID /*pParam*/, PMINIDUMP_CALLBACK_INPUT pInput, PMINIDUMP_CALLBACK_OUTPUT pOutput) 
{
    BOOL bRet = FALSE; 

    // Check parameters 
    if(pInput  == NULL){ return FALSE;}
    if(pOutput == NULL){ return FALSE;}

    // Process the callbacks 
    switch( pInput->CallbackType ) 
    {
    case ModuleCallback: 
        {
            // Are data sections available for this module ? 
            if( pOutput->ModuleWriteFlags & ModuleWriteDataSeg ) 
            {
                // Yes, they are, but do we need them? 
                if( !g_Exception.IsDataSectionNeeded( pInput->Module.FullPath ) ) 
                {
                    //wprintf( L"Excluding module data sections: %s \n", pInput->Module.FullPath ); 
                    pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg); 
                }
            }
            bRet = TRUE; 
        }
        break; 
    case ThreadCallback:        { bRet = TRUE;}  break; // Include all thread information into the minidump 
    case ThreadExCallback:      { bRet = TRUE;}  break; // Include this information 
    case IncludeThreadCallback: { bRet = TRUE;}  break; // Include the thread into the dump 
    case IncludeModuleCallback: { bRet = TRUE;}  break; // Include the module into the dump 
    case MemoryCallback:        { bRet = FALSE;} break; // We do not include any information here -> return FALSE 
    case CancelCallback: break;
    default:             break;
    }
    return bRet; 
}
void Smart::Global::CCrashDump::InvalidParameterHandler(const wchar_t* /* expression */, const wchar_t* /* function */, const wchar_t* /* file */, unsigned int /* line */, uintptr_t /* pReserved */)
{
    OutputLog(LLT_Shutdown, L"%s Found InvalidParameterHandler!!!\n", g_Exception.m_Version.c_str());
}
void Smart::Global::CCrashDump::PurecallHandler(void)
{
    OutputLog(LLT_Shutdown, L"%s Found PurecallHandler!!!\n", g_Exception.m_Version.c_str());
}
//========Smart::Global::CLibManager============================================
Smart::Global::CLibManager g_LibManager;
Smart::Global::CLibManager::CLibManager(void)
{
}
Smart::Global::CLibManager::~CLibManager(void)
{
    this->Clear();
}
void    Smart::Global::CLibManager::Clear(void)
{
    for(auto iter = this->m_Modules.begin(); iter != this->m_Modules.end(); iter++)
    {
        ::FreeLibrary(iter->first);
    }
    this->m_Modules.clear();
}
HMODULE Smart::Global::CLibManager::LoadDll(const std::wstring& DLLPath)
{
    SASectionI;
    HMODULE Module = NULL;
    if((Module = ::LoadLibraryEx(DLLPath.c_str(), NULL, LOAD_WITH_ALTERED_SEARCH_PATH)) != NULL)
    {
        this->m_Modules[Module] = DLLPath;
    }
    else if((Module = ::LoadLibrary(DLLPath.c_str())) != NULL)
    {
        this->m_Modules[Module] = DLLPath;
    }
    return Module;
}
void    Smart::Global::CLibManager::UnloadDll(HMODULE Moudle)
{
    SASectionI;
    for(auto iter = this->m_Modules.begin(); iter != this->m_Modules.end(); iter++)
    {
        if(iter->first == Moudle)
        {
            this->m_Modules.erase(iter);
            break;
        }
    }
}
LPVOID  Smart::Global::CLibManager::CallDllFunction(const std::wstring& DllPath, const std::wstring& FunctionName)
{
    typedef void* (*PROC_DllFunction)();

    SASectionI;
    HMODULE Module = this->LoadDll(DllPath);
    if(Module  == NULL){ return NULL;}
    PROC_DllFunction DllFunc = (PROC_DllFunction)::GetProcAddress(Module, Smart::CText::UTF16_to_ANSI(FunctionName).c_str());
    if(DllFunc == NULL){ return NULL;}

    return DllFunc();
}
extern "C" typedef HRESULT(__stdcall *PROC_DllGetClassObject)(REFCLSID, REFIID, LPVOID*);
HRESULT Smart::Global::CLibManager::CallDllGetClassObject(const std::wstring& DllPath, REFCLSID rCLSID, REFIID rIID, LPVOID* pVoid)
{
    SASectionI;
    HMODULE Module = this->LoadDll(DllPath);
    if(Module  == NULL){ return S_FALSE;}
    PROC_DllGetClassObject DllFunc = (PROC_DllGetClassObject)::GetProcAddress(Module, "DllGetClassObject");
    if(DllFunc == NULL){ return S_FALSE;}
    return DllFunc(rCLSID, rIID, pVoid);
}
//------------------------------------------------------------------------------
LPVOID Smart::Global::CLibManager::CallFunction(const std::wstring& ModulePath, const std::wstring& FunctionName)
{
    return g_LibManager.CallDllFunction(ModulePath, FunctionName);
}
void   Smart::Global::CLibManager::Clean(void)
{
    g_LibManager.Clear();
}

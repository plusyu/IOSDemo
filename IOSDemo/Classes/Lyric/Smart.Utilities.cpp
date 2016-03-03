#include "StdAfx.h"
#include <direct.h>
#include <Share.h>
#include <list>
#include <WinUser.h>
#include "Smart.Registry.h"
#include "Smart.Utilities.h"
//========Smart::UAC::CProcessToken=============================================
DWORD Smart::UAC::CProcessToken::GetTokenInformation(HANDLE TokenHandle, TOKEN_INFORMATION_CLASS TokenInformationClass, LPVOID* TokenInformation)
{
    DWORD TokenInformationLength = 0;
    if (!::GetTokenInformation(TokenHandle, TokenInformationClass, NULL, 0, &TokenInformationLength))
    {
        if (ERROR_INSUFFICIENT_BUFFER != ::GetLastError())                                                                             { return ::GetLastError(); }
    }
    if ((*TokenInformation = ::LocalAlloc(LPTR, TokenInformationLength)) == NULL)                                                      { return ::GetLastError(); }
    if (!::GetTokenInformation(TokenHandle, TokenInformationClass, *TokenInformation, TokenInformationLength, &TokenInformationLength)){ DWORD Error = ::GetLastError(); ::LocalFree(*TokenInformation); return Error; }
    return ERROR_SUCCESS;
}
void  Smart::UAC::CProcessToken::ReleaseTokenInformation(LPVOID TokenInformation)
{
    if(TokenInformation != NULL)
    {
        ::LocalFree(TokenInformation);
    }
}
//========Smart::UAC::CSelfElevation======================================
DWORD Smart::UAC::CSelfElevation::IsUserInAdminGroup(BOOL inGroup)
{
    OSVERSIONINFOW OSVersion                   = { sizeof(OSVERSIONINFOW) };
    HANDLE         TokenHandle                 = NULL;
    HANDLE         LinkedToken                 = NULL;
    BYTE           Data[SECURITY_MAX_SID_SIZE] = {0};
    DWORD          Size                        = sizeof(Data);
    PSID           AdminSID                    = &Data;
    DWORD          Error                       = ERROR_SUCCESS;

    if (!::CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &AdminSID, &Size))             { Error = ::GetLastError(); return Error; }// Create the SID corresponding to the Administrators group.
    if (!::GetVersionExW(&OSVersion))                                                           { Error = ::GetLastError(); return Error; }
    if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, &TokenHandle)){ Error = ::GetLastError(); return Error; }// Open the primary access token of the process for query and duplicate.

    // Determine whether system is running Windows Vista or later operating systems (major version >= 6) because they support linked tokens, but previous versions (major version < 6) do not.
    if (OSVersion.dwMajorVersion >= 6)
    {
        // Determine token type: limited, elevated, or default. 
        PTOKEN_ELEVATION_TYPE ElevationType;
        if((Error = Smart::UAC::CProcessToken::GetTokenInformation(TokenHandle, TokenElevationType, (void**)&ElevationType)) != ERROR_SUCCESS){ SAFE_CLOSEHANDLE(TokenHandle); return Error; }
        // If limited, get the linked elevated token for further check.
        if (TokenElevationTypeLimited == *ElevationType)
        {
            if (!::GetTokenInformation(TokenHandle, TokenLinkedToken, &LinkedToken, sizeof(LinkedToken), &Size))  { Error = ::GetLastError(); SAFE_CLOSEHANDLE(TokenHandle); return Error; }
        }
    }
    // CheckTokenMembership requires an impersonation token. 
    // If we just got a linked token, it already is an impersonation token. 
    // If we did not get a linked token, duplicate the original into an impersonation token for CheckTokenMembership.
    if (!LinkedToken)
    {
        if (!::DuplicateToken(TokenHandle, SecurityIdentification, &LinkedToken))                                 { Error = ::GetLastError(); SAFE_CLOSEHANDLE(TokenHandle); return Error; }
    }

    // Check if the token to be checked contains admin SID.
    // http://msdn.microsoft.com/en-us/library/aa379596(VS.85).aspx:
    // To determine whether a SID is enabled in a token, that is, whether it has the SE_GROUP_ENABLED attribute, call CheckTokenMembership.
    if (!::CheckTokenMembership(LinkedToken, &AdminSID, &inGroup))                 { Error = ::GetLastError(); SAFE_CLOSEHANDLE(LinkedToken); SAFE_CLOSEHANDLE(TokenHandle); return Error; }

    SAFE_CLOSEHANDLE(TokenHandle);
    SAFE_CLOSEHANDLE(LinkedToken);
    return Error;
}
DWORD Smart::UAC::CSelfElevation::IsRunAsAdmin(BOOL isRunAs)
{
    SID_IDENTIFIER_AUTHORITY Authority    = SECURITY_NT_AUTHORITY;
    PSID                     AdminGroup   = NULL;
    DWORD                    Error        = ERROR_SUCCESS;

    // Allocate and initialize a SID of the administrators group.
    if (!::AllocateAndInitializeSid(&Authority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &AdminGroup)){ Error = GetLastError(); return Error; }
    // Determine whether the SID of administrators group is enabled in the primary access token of the process.
    if (!::CheckTokenMembership(NULL, AdminGroup, &isRunAs))                                                  { Error = GetLastError(); SAFE_FREESID(AdminGroup); return Error; }
    SAFE_FREESID(AdminGroup);
    return Error;
}
DWORD Smart::UAC::CSelfElevation::IsProcessElevated(BOOL isElevated)
{
    HANDLE           TokenHandle = NULL;
    PTOKEN_ELEVATION Elevation   = NULL;

    // Open the primary access token of the process with TOKEN_QUERY.
    if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &TokenHandle)){ return ::GetLastError(); }
    // Retrieve token elevation information.
    DWORD Error = Smart::UAC::CProcessToken::GetTokenInformation(TokenHandle, TokenElevation, (void**)&Elevation);
    if (Error == ERROR_SUCCESS)
    {
        // When the process is run on operating systems prior to Windows Vista, GetTokenInformation returns FALSE with the ERROR_INVALID_PARAMETER error code because TokenElevation is not supported on those operating systems.
        isElevated = Elevation->TokenIsElevated;
        Smart::UAC::CProcessToken::ReleaseTokenInformation(Elevation);
    }
    SAFE_CLOSEHANDLE(TokenHandle);
    return Error;
}
DWORD Smart::UAC::CSelfElevation::GetProcessIntegrityLevel(DWORD& Level)
{
    HANDLE                 TokenHandle    = NULL;
    PTOKEN_MANDATORY_LABEL IntegrityLevel = NULL;

    // Open the primary access token of the process with TOKEN_QUERY.
    if (!::OpenProcessToken(::GetCurrentProcess(), TOKEN_QUERY, &TokenHandle)){ return ::GetLastError(); }
    // Query the size of the token integrity level information. 
    // Note that we expect a FALSE result and the last error ERROR_INSUFFICIENT_BUFFER from GetTokenInformation because we have given it a NULL buffer. 
    // On exit cbTokenIL will tell the size of the integrity level information.
    DWORD Error = Smart::UAC::CProcessToken::GetTokenInformation(TokenHandle, TokenIntegrityLevel, (void**)&IntegrityLevel);
    if (Error == ERROR_SUCCESS)
    {
        // Integrity Level SIDs are in the form of S-1-16-0xXXXX. (e.g. S-1-16-0x1000 stands for low integrity level SID). There is one and only one subauthority.
        Level = *::GetSidSubAuthority(IntegrityLevel->Label.Sid, 0);
        Smart::UAC::CProcessToken::ReleaseTokenInformation(IntegrityLevel);
    }
    SAFE_CLOSEHANDLE(TokenHandle);
    return Error;
}
//========Smart::UAC::CAgentClient========================================
Smart::UAC::CAgentClient::CAgentClient(void)
    : m_Pipe(NULL)
{
    this->m_Overlapped.hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}
Smart::UAC::CAgentClient::~CAgentClient(void)
{
    SAFE_CLOSEHANDLE(this->m_Overlapped.hEvent);
}
BOOL Smart::UAC::CAgentClient::Execute(void)
{
    AgentProtocolType       APT;
    TaskProtocolSubType     TPST;
    std::wstring            Text;
    
    if(!::WaitNamedPipe(this->m_PipeName.c_str(), UACAgentTimeOut)) { return FALSE;}
    this->m_Pipe = ::CreateFile(this->m_PipeName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(this->m_Pipe == NULL)                                        { return FALSE;}
    if(this->m_Pipe == INVALID_HANDLE_VALUE)                        { return FALSE;}

    while(this->ReadData(APT, TPST, Text))
    {
        if(APT == APT_Close)            { this->WriteData(APT_Quit); break;}
        else if(APT == APT_RegKeyAQ)    { this->WriteData(APT_RegKeyAA,   this->CreateRegistryKey(TPST, Text)      ? TPST_Success : TPST_Failure, this->m_Message);}
        else if(APT == APT_RegKeyDQ)    { this->WriteData(APT_RegKeyDA,   this->DeleteRegistryKey(TPST, Text)      ? TPST_Success : TPST_Failure, this->m_Message);}
        else if(APT == APT_RegValueAQ)  { this->WriteData(APT_RegValueAA, this->SetRegistryKeyValue(TPST, Text)    ? TPST_Success : TPST_Failure, this->m_Message);}
        else if(APT == APT_RegValueDQ)  { this->WriteData(APT_RegValueDA, this->DeleteRegistryKeyValue(TPST, Text) ? TPST_Success : TPST_Failure, this->m_Message);}
        else if(APT == APT_FileAQ)      { this->WriteData(APT_FileAA,     this->MoveFile(Text)                     ? TPST_Success : TPST_Failure, this->m_Message);}
        else if(APT == APT_FileDQ)      { this->WriteData(APT_FileDA,     this->DeleteFile(Text)                   ? TPST_Success : TPST_Failure, this->m_Message);}
        else if(APT == APT_FolderAQ)    { this->WriteData(APT_FolderAA,   this->MoveFolder(Text)                   ? TPST_Success : TPST_Failure, this->m_Message);}
        else if(APT == APT_FolderDQ)    { this->WriteData(APT_FolderDA,   this->DeleteFolder(Text)                 ? TPST_Success : TPST_Failure, this->m_Message);}
        else                            { this->WriteData(APT_NoSupport);}
    }
    SAFE_CLOSEHANDLE(this->m_Pipe);
    return TRUE;
}
//------------------------------------------------------------------------
BOOL Smart::UAC::CAgentClient::WriteData(AgentProtocolType APT, TaskProtocolSubType TPST, const std::wstring& Text)
{
    BYTE   TaskData[UACAgentMaxSize] = {0};//管道任务数据
    UINT32 TaskSize                  = 0;  //管道任务数据大小

    TaskSize = sizeof(UINT32)*3 + sizeof(wchar_t)*Text.size();
    (PUINT32(TaskData))[0] = TaskSize;
    (PUINT32(TaskData))[1] = UINT32(APT);
    (PUINT32(TaskData))[2] = UINT32(TPST);
    ::memcpy_s(&TaskData[sizeof(UINT32)*3], UACAgentMaxSize - sizeof(UINT32)*3, Text.c_str(), sizeof(wchar_t)*Text.size());
    return ::WriteFile(this->m_Pipe, TaskData, TaskSize, NULL, NULL);
}
BOOL Smart::UAC::CAgentClient::ReadData(AgentProtocolType& APT, TaskProtocolSubType& TPST, std::wstring& Text)
{
    BYTE   TaskData[UACAgentMaxSize] = {0};//管道任务数据
    UINT32 TaskSize                  = 0;  //管道任务数据大小
    DWORD  TempSize                  = 0;

    while(::ReadFile(this->m_Pipe, &TaskData[TaskSize], UACAgentMaxSize - TaskSize, &TempSize, NULL))
    {
        TaskSize += TempSize;
        if(TaskSize               < sizeof(UINT32)*3)     { continue;}
        if((PUINT32(TaskData))[0] > TaskSize)             { continue;}
        if((PUINT32(TaskData))[0] < TaskSize)             { break;}

        APT  = AgentProtocolType((PUINT32(TaskData))[1]);
        TPST = TaskProtocolSubType((PUINT32(TaskData))[2]);
        Text = std::wstring(LPWSTR(&TaskData[sizeof(UINT32)*3]), (TaskSize - sizeof(UINT32)*3)/2);
        return TRUE;
    }
    return FALSE;
}
//------------------------------------------------------------------------
BOOL Smart::UAC::CAgentClient::Analyse(const std::wstring& Text, UINT32& BPos, std::wstring& Item)
{
    if(Text.size() <= BPos)
    {
        return FALSE;
    }
    else
    {
        UINT32 EPos = Text.find('|', BPos);
        if(EPos == std::wstring::npos)
        {
            Item = Text.substr(BPos, Text.size() - BPos);
            BPos = Text.size();
        }
        else
        {
            Item = Text.substr(BPos, EPos - BPos);
            BPos = EPos + 1;
        }
        return TRUE;
    }
}
BOOL Smart::UAC::CAgentClient::Analyse(const std::wstring& Text, UINT32& BPos, DWORD& Item)
{
    if(Text.size() <= BPos)
    {
        return FALSE;
    }
    else
    {
        UINT32 EPos = Text.find('|', BPos);
        if(EPos == std::wstring::npos)
        {
            Item = DWORD(::_wtoi(Text.substr(BPos, Text.size() - BPos).c_str()));
            BPos = Text.size();
        }
        else
        {
            Item = DWORD(::_wtoi(Text.substr(BPos, EPos - BPos).c_str()));
            BPos = EPos + 1;
        }
        return TRUE;
    }
}
BOOL Smart::UAC::CAgentClient::Analyse(const std::wstring& Text, UINT32& BPos, PBYTE Data, UINT32& Size)
{
    if(Text.size() <= BPos)
    {
        return FALSE;
    }
    else
    {
        UINT32 EPos = Text.find('|', BPos);
        if(EPos == std::wstring::npos)
        {
            Smart::CHex::ToByte(Text.substr(BPos, Text.size() - BPos), Data, Size);
            BPos = Text.size();
        }
        else
        {
            Smart::CHex::ToByte(Text.substr(BPos, EPos - BPos), Data, Size);
            BPos = EPos + 1;
        }
        return TRUE;
    }
}
BOOL Smart::UAC::CAgentClient::CreateRegistryKey(TaskProtocolSubType TPST, const std::wstring& Text)
{
    switch (TPST)
    {
    case TPST_Registry_ClassesRoot:     return Smart::Registry::CRegistry::CreateKey(HKEY_CLASSES_ROOT, Text);
    case TPST_Registry_CurrentConfig:   return Smart::Registry::CRegistry::CreateKey(HKEY_CURRENT_CONFIG, Text);
    case TPST_Registry_CurrentUser:     return Smart::Registry::CRegistry::CreateKey(HKEY_CURRENT_USER, Text);
    case TPST_Registry_LocalMachine:    return Smart::Registry::CRegistry::CreateKey(HKEY_LOCAL_MACHINE, Text);
    case TPST_Registry_Users:           return Smart::Registry::CRegistry::CreateKey(HKEY_USERS, Text);
    default:                            return FALSE;
    }
}
BOOL Smart::UAC::CAgentClient::DeleteRegistryKey(TaskProtocolSubType TPST, const std::wstring& Text)
{
    switch (TPST)
    {
    case TPST_Registry_ClassesRoot:     return Smart::Registry::CRegistry::DeleteKey(HKEY_CLASSES_ROOT, Text);
    case TPST_Registry_CurrentConfig:   return Smart::Registry::CRegistry::DeleteKey(HKEY_CURRENT_CONFIG, Text);
    case TPST_Registry_CurrentUser:     return Smart::Registry::CRegistry::DeleteKey(HKEY_CURRENT_USER, Text);
    case TPST_Registry_LocalMachine:    return Smart::Registry::CRegistry::DeleteKey(HKEY_LOCAL_MACHINE, Text);
    case TPST_Registry_Users:           return Smart::Registry::CRegistry::DeleteKey(HKEY_USERS, Text);
    default:                            return FALSE;
    }
}
BOOL Smart::UAC::CAgentClient::SetRegistryKeyValue(TaskProtocolSubType TPST, const std::wstring& Text)
{
    UINT32       BPos = 0;
    std::wstring Path;
    std::wstring Name;
    DWORD        Type;
    BYTE         Value[UACAgentMaxSize] = {0};
    UINT32       Size                   = 0;

    if(!this->Analyse(Text, BPos, Path))        { return FALSE;}
    if(!this->Analyse(Text, BPos, Name))        { return FALSE;}
    if(!this->Analyse(Text, BPos, Type))        { return FALSE;}
    if(!this->Analyse(Text, BPos, Value, Size)) { return FALSE;}

    switch (TPST)
    {
    case TPST_Registry_ClassesRoot:     return Smart::Registry::CRegistry::SetKeyValue(HKEY_CLASSES_ROOT, Path, Name, Type, Value, Size);
    case TPST_Registry_CurrentConfig:   return Smart::Registry::CRegistry::SetKeyValue(HKEY_CURRENT_CONFIG, Path, Name, Type, Value, Size);
    case TPST_Registry_CurrentUser:     return Smart::Registry::CRegistry::SetKeyValue(HKEY_CURRENT_USER, Path, Name, Type, Value, Size);
    case TPST_Registry_LocalMachine:    return Smart::Registry::CRegistry::SetKeyValue(HKEY_LOCAL_MACHINE, Path, Name, Type, Value, Size);
    case TPST_Registry_Users:           return Smart::Registry::CRegistry::SetKeyValue(HKEY_USERS, Path, Name, Type, Value, Size);
    default:                            return FALSE;
    }
}
BOOL Smart::UAC::CAgentClient::DeleteRegistryKeyValue(TaskProtocolSubType TPST, const std::wstring& Text)
{
    UINT32       BPos = 0;
    std::wstring Path;
    std::wstring Name;

    if(!this->Analyse(Text, BPos, Path))        { return FALSE;}
    if(!this->Analyse(Text, BPos, Name))        { return FALSE;}

    switch (TPST)
    {
    case TPST_Registry_ClassesRoot:     return Smart::Registry::CRegistry::DeleteKeyValue(HKEY_CLASSES_ROOT, Path, Name);
    case TPST_Registry_CurrentConfig:   return Smart::Registry::CRegistry::DeleteKeyValue(HKEY_CURRENT_CONFIG, Path, Name);
    case TPST_Registry_CurrentUser:     return Smart::Registry::CRegistry::DeleteKeyValue(HKEY_CURRENT_USER, Path, Name);
    case TPST_Registry_LocalMachine:    return Smart::Registry::CRegistry::DeleteKeyValue(HKEY_LOCAL_MACHINE, Path, Name);
    case TPST_Registry_Users:           return Smart::Registry::CRegistry::DeleteKeyValue(HKEY_USERS, Path, Name);
    default:                            return FALSE;
    }
}
BOOL Smart::UAC::CAgentClient::MoveFile(const std::wstring& Text)
{
    UINT32       BPos = 0;
    std::wstring Sour;
    std::wstring Dest;

    if(!this->Analyse(Text, BPos, Sour))        { return FALSE;}
    if(!this->Analyse(Text, BPos, Dest))        { return FALSE;}

    return ::MoveFileEx(Sour.c_str(), Dest.c_str(), MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
}
BOOL Smart::UAC::CAgentClient::DeleteFile(const std::wstring& Text)
{
    return ::DeleteFile(Text.c_str());
}
BOOL Smart::UAC::CAgentClient::MoveFolder(const std::wstring& Text)
{
    UINT32       BPos = 0;
    std::wstring Sour;
    std::wstring Dest;

    if(!this->Analyse(Text, BPos, Sour))        { return FALSE;}
    if(!this->Analyse(Text, BPos, Dest))        { return FALSE;}

    return ::MoveFileEx(Sour.c_str(), Dest.c_str(), MOVEFILE_COPY_ALLOWED|MOVEFILE_REPLACE_EXISTING);
}
BOOL Smart::UAC::CAgentClient::DeleteFolder(const std::wstring& Text)
{
    return ::RemoveDirectory(Text.c_str());
}
//========Smart::UAC::CAgentServer========================================
Smart::UAC::CAgentServer::CAgentServer(void)
    : m_ClientProcess(NULL)
{
}
Smart::UAC::CAgentServer::~CAgentServer(void)
{
    this->StopServer();
}
void Smart::UAC::CAgentServer::SetProperty(const std::wstring& PipeName, const std::wstring& ClientPath)
{
    this->m_PipeName   = PipeName;
    this->m_ClientPath = ClientPath;
}
//------------------------------------------------------------------------
BOOL Smart::UAC::CAgentServer::StartServer(void)
{
    SASectionI;
    if(this->m_Pipe == NULL)
    {
        if(!this->StartClient())                    {                                                     return FALSE;}
        this->m_Pipe = ::CreateNamedPipe(this->m_PipeName.c_str(), PIPE_ACCESS_DUPLEX | FILE_FLAG_WRITE_THROUGH | FILE_FLAG_OVERLAPPED, PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_NOWAIT, PIPE_UNLIMITED_INSTANCES, UACAgentMaxSize, UACAgentMaxSize, UACAgentTimeOut, NULL);
        if(this->m_Pipe == NULL)                    { SAFE_CLOSEHANDLE(this->m_Pipe); this->StopClient(); return FALSE;}
        if(this->m_Pipe == INVALID_HANDLE_VALUE)    { SAFE_CLOSEHANDLE(this->m_Pipe); this->StopClient(); return FALSE;}
        if(!::ConnectNamedPipe(this->m_Pipe, &this->m_Overlapped))
        {
            switch(::GetLastError())
            {
            case ERROR_IO_PENDING:
            case ERROR_PIPE_LISTENING:
                if(::WaitForSingleObject(this->m_Overlapped.hEvent, UACAgentTimeOut) != WAIT_OBJECT_0)
                {
                    SAFE_CLOSEHANDLE(this->m_Pipe);
                    this->StopClient();
                }
                break;
            case ERROR_PIPE_CONNECTED:
                break;
            default:
                SAFE_CLOSEHANDLE(this->m_Pipe);
                this->StopClient();
                break;
            }

        }
    }
    return this->m_Pipe == NULL;
}
void Smart::UAC::CAgentServer::StopServer(void)
{
    SASectionI;
    if(this->m_Pipe != NULL)
    {
        this->ServerProc(APT_Close);
        SAFE_CLOSEHANDLE(this->m_Pipe);
        this->StopClient();
    }
}
BOOL Smart::UAC::CAgentServer::StartClient(void)
{
    SHELLEXECUTEINFO Info = {0};
    Info.cbSize           = sizeof(SHELLEXECUTEINFO);
    Info.fMask            = SEE_MASK_NOCLOSEPROCESS;
    Info.lpVerb           = L"runas";
    Info.lpFile           = this->m_ClientPath.c_str();
    Info.lpParameters     = this->m_PipeName.c_str();
    Info.lpDirectory      = NULL;
    Info.nShow            = SW_HIDE;
    if(::ShellExecuteEx(&Info))
    {
        this->m_ClientProcess = Info.hProcess;
        return TRUE;
    }
    else
    { 
        return FALSE;
    }
}
void Smart::UAC::CAgentServer::StopClient(void)
{
    ::WaitForSingleObject(this->m_ClientProcess, INFINITE);
    SAFE_CLOSEHANDLE(this->m_ClientProcess);
}
BOOL Smart::UAC::CAgentServer::ServerProc(AgentProtocolType APTQ, TaskProtocolSubType TPST, const std::wstring& Text)
{
    AgentProtocolType APTA;
    SASectionI;
    if(!this->StartServer())                        { this->m_Message = L"UAC代理启动失败";  return FALSE;}
    if(!this->WriteData(APTQ, TPST, Text))          { this->m_Message = L"写入管道数据失败"; return FALSE;}
    if(!this->ReadData(APTA, TPST, this->m_Message)){ this->m_Message = L"读取管道数据失败"; return FALSE;}
    if(APTA == APT_NoSupport)                       { this->m_Message = L"管道指令不支持";   return FALSE;}
    if(UINT32(APTQ) + 1 != UINT32(APTA))            { this->m_Message = L"错误管道应答指令"; return FALSE;}
    if(TPST != TPST_Success)                        { return FALSE;}
    return TRUE;
}
//------------------------------------------------------------------------
BOOL Smart::UAC::CAgentServer::CreateRegistryKey(HKEY Key, const std::wstring& SubPath)
{
    switch(UINT64(Key))
    {
    case HKEY_CLASSES_ROOT:   return this->ServerProc(APT_RegKeyAQ, TPST_Registry_ClassesRoot, SubPath);
    case HKEY_CURRENT_CONFIG: return this->ServerProc(APT_RegKeyAQ, TPST_Registry_CurrentConfig, SubPath);
    case HKEY_CURRENT_USER:   return this->ServerProc(APT_RegKeyAQ, TPST_Registry_CurrentUser, SubPath);
    case HKEY_LOCAL_MACHINE:  return this->ServerProc(APT_RegKeyAQ, TPST_Registry_LocalMachine, SubPath);
    case HKEY_USERS:          return this->ServerProc(APT_RegKeyAQ, TPST_Registry_Users, SubPath);
    default: this->m_Message = L"不支持主键类型"; return FALSE;
    }
}
BOOL Smart::UAC::CAgentServer::DeleteRegistryKey(HKEY Key, const std::wstring& SubPath)
{
    switch(UINT64(Key))
    {
    case HKEY_CLASSES_ROOT:   return this->ServerProc(APT_RegKeyDQ, TPST_Registry_ClassesRoot, SubPath);
    case HKEY_CURRENT_CONFIG: return this->ServerProc(APT_RegKeyDQ, TPST_Registry_CurrentConfig, SubPath);
    case HKEY_CURRENT_USER:   return this->ServerProc(APT_RegKeyDQ, TPST_Registry_CurrentUser, SubPath);
    case HKEY_LOCAL_MACHINE:  return this->ServerProc(APT_RegKeyDQ, TPST_Registry_LocalMachine, SubPath);
    case HKEY_USERS:          return this->ServerProc(APT_RegKeyDQ, TPST_Registry_Users, SubPath);
    default: this->m_Message = L"不支持主键类型"; return FALSE;
    }
}
BOOL Smart::UAC::CAgentServer::SetRegistryKeyValue(HKEY Key, const std::wstring& SubPath, const std::wstring& Name, DWORD Type, PBYTE Value, DWORD Size)
{
    switch(UINT64(Key))
    {
    case HKEY_CLASSES_ROOT:   return this->ServerProc(APT_RegValueAQ, TPST_Registry_ClassesRoot, Smart::CText::Format(L"%s|%s|%d|%s", SubPath.c_str(), Name.c_str(), Type, Smart::CHex::ToHexW(Value, Size).c_str()));
    case HKEY_CURRENT_CONFIG: return this->ServerProc(APT_RegValueAQ, TPST_Registry_CurrentConfig, Smart::CText::Format(L"%s|%s|%d|%s", SubPath.c_str(), Name.c_str(), Type, Smart::CHex::ToHexW(Value, Size).c_str()));
    case HKEY_CURRENT_USER:   return this->ServerProc(APT_RegValueAQ, TPST_Registry_CurrentUser, Smart::CText::Format(L"%s|%s|%d|%s", SubPath.c_str(), Name.c_str(), Type, Smart::CHex::ToHexW(Value, Size).c_str()));
    case HKEY_LOCAL_MACHINE:  return this->ServerProc(APT_RegValueAQ, TPST_Registry_LocalMachine, Smart::CText::Format(L"%s|%s|%d|%s", SubPath.c_str(), Name.c_str(), Type, Smart::CHex::ToHexW(Value, Size).c_str()));
    case HKEY_USERS:          return this->ServerProc(APT_RegValueAQ, TPST_Registry_Users, Smart::CText::Format(L"%s|%s|%d|%s", SubPath.c_str(), Name.c_str(), Type, Smart::CHex::ToHexW(Value, Size).c_str()));
    default: this->m_Message = L"不支持主键类型"; return FALSE;
    }
}
BOOL Smart::UAC::CAgentServer::DeleteRegistryKeyValue(HKEY Key, const std::wstring& SubPath, const std::wstring& Name)
{
    switch(UINT64(Key))
    {
    case HKEY_CLASSES_ROOT:   return this->ServerProc(APT_RegValueDQ, TPST_Registry_ClassesRoot, Smart::CText::Format(L"%s|%s", SubPath.c_str(), Name.c_str()));
    case HKEY_CURRENT_CONFIG: return this->ServerProc(APT_RegValueDQ, TPST_Registry_CurrentConfig, Smart::CText::Format(L"%s|%s", SubPath.c_str(), Name.c_str()));
    case HKEY_CURRENT_USER:   return this->ServerProc(APT_RegValueDQ, TPST_Registry_CurrentUser, Smart::CText::Format(L"%s|%s", SubPath.c_str(), Name.c_str()));
    case HKEY_LOCAL_MACHINE:  return this->ServerProc(APT_RegValueDQ, TPST_Registry_LocalMachine, Smart::CText::Format(L"%s|%s", SubPath.c_str(), Name.c_str()));
    case HKEY_USERS:          return this->ServerProc(APT_RegValueDQ, TPST_Registry_Users, Smart::CText::Format(L"%s|%s", SubPath.c_str(), Name.c_str()));
    default: this->m_Message = L"不支持主键类型"; return FALSE;
    }
}
BOOL Smart::UAC::CAgentServer::MoveFile(const std::wstring& SourPath, const std::wstring& DestPath)
{
    return this->ServerProc(APT_FileAQ, TPST_Undefined, Smart::CText::Format(L"%s|%s", SourPath.c_str(), DestPath.c_str()));
}
BOOL Smart::UAC::CAgentServer::DeleteFile(const std::wstring& Path)
{
    return this->ServerProc(APT_FileDQ, TPST_Undefined, Path);
}
BOOL Smart::UAC::CAgentServer::MoveFolder(const std::wstring& SourPath, const std::wstring& DestPath)
{
    return this->ServerProc(APT_FolderAQ, TPST_Undefined, Smart::CText::Format(L"%s|%s", SourPath.c_str(), DestPath.c_str()));
}
BOOL Smart::UAC::CAgentServer::DeleteFolder(const std::wstring& Path)
{
    return this->ServerProc(APT_FolderDQ, TPST_Undefined, Path);
}
//========Smart::Singleton::CApp================================================
Smart::Singleton::CApp::CApp(const std::wstring& GUID)
    : m_GUID(GUID)
    , m_Mutex(NULL)
    , m_Running(FALSE)
{
    this->m_Mutex = ::CreateMutex(NULL, TRUE, this->m_GUID.c_str());
    if(this->m_Mutex == NULL)                        { this->m_Running = FALSE;}
    else if(::GetLastError() != ERROR_ALREADY_EXISTS){ this->m_Running = FALSE;}
    else                                             { this->m_Running = TRUE;}
}
Smart::Singleton::CApp::~CApp(void)
{
    SAFE_CLOSEHANDLE(this->m_Mutex);
}
BOOL Smart::Singleton::CApp::IsRunning(void)
{
    return this->m_Running;
}
void Smart::Singleton::CApp::SetWnd(HWND hWnd)
{
    ::SetProp(hWnd, this->m_GUID.c_str(), HANDLE(TRUE));
}
void Smart::Singleton::CApp::Show(void)
{
    HWND   hWnd    = ::GetWindow(::GetDesktopWindow(), GW_CHILD); 
    HANDLE hHandle = NULL;
    while(::IsWindow(hWnd)) 
    {
        if((hHandle = ::GetProp(hWnd, this->m_GUID.c_str())) != NULL)
        {
            ::PostMessage(hWnd, WM_USER_SINGLETON, 0, 0);
            break;
        }
        hWnd = ::GetWindow(hWnd, GW_HWNDNEXT); 
    } 
}
#ifdef Smart_Json
//========Smart::CJson==========================================================
BOOL Smart::CJson::TextToJson(const std::string& Text, Json::Value& Object)
{
    Json::Reader Reader;
    return Reader.parse(Text, Object);
}
BOOL Smart::CJson::JsonToText(const Json::Value& Object, std::string&  Text)
{
    if(Object.isNull()){ return FALSE;}
    else
    {
        Json::FastWriter Writer;
        Text = Writer.write(Object);
        return TRUE;
    }
}
BOOL Smart::CJson::JsonToText(const Json::Value& Object, std::wstring& Text)
{
    if(Object.isNull()){ return FALSE;}
    else
    {
        Json::FastWriter Writer;
        Text = Smart::CText::UTF8_to_UTF16(Writer.write(Object));
        return TRUE;
    }
}
//------------------------------------------------------------------------------
std::wstring Smart::CJson::JsonToText(const Json::Value& Object)
{
    std::wstring Text;
    if(Smart::CJson::JsonToText(Object, Text))
    {
        return Text;
    }
    else
    {
        return L"";
    }
}
//------------------------------------------------------------------------------
BOOL Smart::CJson::GetValue(const Json::Value& Object, const std::string& Name, bool&         Value, bool                Default)
{
    if(Object.isNull())            { Value = Default; return FALSE;}
    else if(!Object.isMember(Name)){ Value = Default; return FALSE;}
    else if(!Object[Name].isBool()){ Value = Default; return FALSE;}
    else
    {
        Value = Object[Name].asBool();
        return TRUE;
    }
}
BOOL Smart::CJson::GetValue(const Json::Value& Object, const std::string& Name, INT&          Value, INT                 Default)
{
    if(Object.isNull())            { Value = Default; return FALSE;}
    else if(!Object.isMember(Name)){ Value = Default; return FALSE;}
    else if(!Object[Name].isInt()) { Value = Default; return FALSE;}
    else
    {
        Value = Object[Name].asInt();
        return TRUE;
    }
}
BOOL Smart::CJson::GetValue(const Json::Value& Object, const std::string& Name, UINT&         Value, UINT                Default)
{
    if(Object.isNull())            { Value = Default; return FALSE;}
    else if(!Object.isMember(Name)){ Value = Default; return FALSE;}
    else if(!Object[Name].isUInt()){ Value = Default; return FALSE;}
    else
    {
        Value = Object[Name].asUInt();
        return TRUE;
    }
}
BOOL Smart::CJson::GetValue(const Json::Value& Object, const std::string& Name, DOUBLE&       Value, DOUBLE              Default)
{
    if(Object.isNull())              { Value = Default; return FALSE;}
    else if(!Object.isMember(Name))  { Value = Default; return FALSE;}
    else if(!Object[Name].isDouble()){ Value = Default; return FALSE;}
    else
    {
        Value = Object[Name].asDouble();
        return TRUE;
    }
}
BOOL Smart::CJson::GetValue(const Json::Value& Object, const std::string& Name, std::string&  Value, const std::string&  Default)
{
    if(Object.isNull())              { Value = Default; return FALSE;}
    else if(!Object.isMember(Name))  { Value = Default; return FALSE;}
    else if(!Object[Name].isString()){ Value = Default; return FALSE;}
    else
    {
        Value = Object[Name].asString();
        return TRUE;
    }
}
BOOL Smart::CJson::GetValue(const Json::Value& Object, const std::string& Name, std::wstring& Value, const std::wstring& Default)
{
    if(Object.isNull())              { Value = Default; return FALSE;}
    else if(!Object.isMember(Name))  { Value = Default; return FALSE;}
    else if(!Object[Name].isString()){ Value = Default; return FALSE;}
    else
    {
        Value = Smart::CText::UTF8_to_UTF16(Object[Name].asString());
        return TRUE;
    }
}
#endif
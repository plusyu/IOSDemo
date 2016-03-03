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
#include "Smart.Shell.h"
namespace Smart
{
    namespace UAC
    {
        class CProcessToken; //
        class CSelfElevation;//权限提升
        class CAgentClient;  //任务代理客户端
        class CAgentServer;  //任务代理服务器
    };
};
//========Smart::UAC::CProcessToken=============================================
class Smart::UAC::CProcessToken
{
public:
    static DWORD WINAPI GetTokenInformation(HANDLE TokenHandle, TOKEN_INFORMATION_CLASS TokenInformationClass, LPVOID* TokenInformation);
    static void  WINAPI ReleaseTokenInformation(LPVOID TokenInformation);
};
//========Smart::UAC::CSelfElevation============================================
class Smart::UAC::CSelfElevation
{
public:
    static DWORD WINAPI IsUserInAdminGroup(BOOL inGroup);      //【函数功能】：当前用户属于管理员组，成功返回ERROR_SUCCESS，失败返回错误代码
    static DWORD WINAPI IsRunAsAdmin(BOOL isRunAs);            //【函数功能】：当前进程是否以管理员运行，成功返回ERROR_SUCCESS，失败返回错误代码
    static DWORD WINAPI IsProcessElevated(BOOL isElevated);    //【函数功能】：当前进程是否已经提升权限，成功返回ERROR_SUCCESS，失败返回错误代码
    static DWORD WINAPI GetProcessIntegrityLevel(DWORD& Level);//【函数功能】：获取当前进程权限等级，成功返回ERROR_SUCCESS，失败返回错误代码
};
//========Smart::UAC::CAgentClient==============================================
class Smart::UAC::CAgentClient
{
public:
    #define UACAgentTimeOut                     5000
    #define UACAgentMaxSize                     4096
    enum AgentProtocolType
    {
        APT_Close = 0x00,//结束指令（服务器指令）
        APT_Quit  = 0x01,//退出指令（客户端指令）

        APT_RegKeyAQ   = 0x10,//注册表创建键请求（服务器指令）
        APT_RegKeyAA   = 0x11,//注册表创建键应答（客户端指令）
        APT_RegKeyDQ   = 0x12,//注册表删除键请求（服务器指令）
        APT_RegKeyDA   = 0x13,//注册表删除键应答（客户端指令）
        APT_RegValueAQ = 0x14,//注册表创建键请求（服务器指令）
        APT_RegValueAA = 0x15,//注册表创建键应答（客户端指令）
        APT_RegValueDQ = 0x16,//注册表删除键请求（服务器指令）
        APT_RegValueDA = 0x17,//注册表删除键应答（客户端指令）

        APT_FileAQ   = 0x20,//文件添加请求（服务器指令）
        APT_FileAA   = 0x21,//文件添加应答（客户端指令）
        APT_FileDQ   = 0x22,//文件删除请求（服务器指令）
        APT_FileDA   = 0x23,//文件删除应答（客户端指令）
        APT_FolderAQ = 0x24,//文件夹移动请求（服务器指令）
        APT_FolderAA = 0x25,//文件夹移动应答（客户端指令）
        APT_FolderDQ = 0x26,//文件夹删除请求（服务器指令）
        APT_FolderDA = 0x27,//文件夹删除应答（客户端指令）

        APT_NoSupport = 0xFF//非法指令应答（客户端指令）
    };
    enum TaskProtocolSubType
    {
        TPST_Undefined = 0x00,
        TPST_Success   = 0x01,
        TPST_Failure   = 0x02,

        TPST_Registry_ClassesRoot   = 0x03,
        TPST_Registry_CurrentConfig = 0x04,
        TPST_Registry_CurrentUser   = 0x05,
        TPST_Registry_LocalMachine  = 0x06,
        TPST_Registry_Users         = 0x07
    };
public:
    CAgentClient(void);
    ~CAgentClient(void);
    BOOL Execute(void);//【函数功能】：执行代理任务
protected:
    BOOL WriteData(AgentProtocolType APT, TaskProtocolSubType TPST = TPST_Undefined, const std::wstring& Text = L"");//【函数功能】：向管道写数据
    BOOL ReadData(AgentProtocolType& APT, TaskProtocolSubType& TPST, std::wstring& Text);                            //【函数功能】：从管道读数据
protected:
    BOOL Analyse(const std::wstring& Text, UINT32& BPos, std::wstring& Data);
    BOOL Analyse(const std::wstring& Text, UINT32& BPos, DWORD& Data);
    BOOL Analyse(const std::wstring& Text, UINT32& BPos, PBYTE Data, UINT32& Size);
    BOOL CreateRegistryKey(TaskProtocolSubType TPST, const std::wstring& Text);     //【函数功能】：创建注册表键
    BOOL DeleteRegistryKey(TaskProtocolSubType TPST, const std::wstring& Text);     //【函数功能】：删除注册表键
    BOOL SetRegistryKeyValue(TaskProtocolSubType TPST, const std::wstring& Text);   //【函数功能】：设置注册表键值
    BOOL DeleteRegistryKeyValue(TaskProtocolSubType TPST, const std::wstring& Text);//【函数功能】：删除注册表键值
    BOOL MoveFile(const std::wstring& Text);                                        //【函数功能】：移动文件
    BOOL DeleteFile(const std::wstring& Text);                                      //【函数功能】：删除文件
    BOOL MoveFolder(const std::wstring& Text);                                      //【函数功能】：移动文件夹
    BOOL DeleteFolder(const std::wstring& Text);                                    //【函数功能】：删除文件夹
protected:
    HANDLE          m_Pipe;                    //管道句柄
    OVERLAPPED      m_Overlapped;              //管道OVERLAPPED
    DeclareProperty(std::wstring,   PipeName);//管道名称
    DeclareProperty_R(std::wstring, Message); //管道任务消息
};
//========Smart::UAC::CAgentServer==============================================
class Smart::UAC::CAgentServer
    : protected Smart::UAC::CAgentClient
{
public:
    CAgentServer(void);
    ~CAgentServer(void);
    void SetProperty(const std::wstring& PipeName, const std::wstring& ClientPath);
    std::wstring GetMessage(void){ return CAgentClient::GetMessage();};
protected:
    BOOL StartServer(void);//【函数功能】：启动任务代理服务器
    void StopServer(void); //【函数功能】：停止任务代理服务器
    BOOL StartClient(void);//【函数功能】：启动任务代理客户端
    void StopClient(void); //【函数功能】：停止任务代理客户端
    BOOL ServerProc(AgentProtocolType APTQ, TaskProtocolSubType TPST = TPST_Undefined, const std::wstring& Text = L"");//【函数功能】：服务器指令执行流程
public:
    BOOL CreateRegistryKey(HKEY Key, const std::wstring& SubPath);                                                                 //【函数功能】：创建注册表键
    BOOL DeleteRegistryKey(HKEY Key, const std::wstring& SubPath);                                                                 //【函数功能】：删除注册表键
    BOOL SetRegistryKeyValue(HKEY Key, const std::wstring& SubPath, const std::wstring& Name, DWORD Type, PBYTE Value, DWORD Size);//【函数功能】：设置注册表键值
    BOOL DeleteRegistryKeyValue(HKEY Key, const std::wstring& SubPath, const std::wstring& Name);                                  //【函数功能】：删除注册表键值
    BOOL MoveFile(const std::wstring& SourPath, const std::wstring& DestPath); //【函数功能】：移动文件
    BOOL DeleteFile(const std::wstring& Path);                                 //【函数功能】：删除文件
    BOOL MoveFolder(const std::wstring&SourPath, const std::wstring& DestPath);//【函数功能】：移动文件夹
    BOOL DeleteFolder(const std::wstring& Path);                               //【函数功能】：删除文件夹
protected:
    Smart::CSection m_Section;
    std::wstring    m_ClientPath;                   //管道客户端路径
    HANDLE          m_ClientProcess;                //管道客户端进程
};

namespace Smart
{
    template<typename T>class CSingleton : public T
    {
    public:
        static T*   Get(void)
        {
            if(s_Instance == NULL)
            {
                s_Section.Enter();
                if(s_Instance == NULL)
                {
                    s_Instance = new T;
                }
                s_Section.Leave();
            }
            return s_Instance;
        };
        static void Release(void)
        {
            if(s_Instance != NULL)
            {
                delete s_Instance;
                s_Instance = NULL;
            }
        };
        static BOOL IsNull(void)
        {
            return s_Instance == NULL;
        };
    private:
        CSingleton(void);
        CSingleton(const CSingleton& Singleton);
        CSingleton& operator=(const CSingleton& Singleton);
        ~CSingleton(void);
    private:
        static Smart::CSection s_Section;//
        static T*              s_Instance;
    };
    namespace Singleton
    {
        class CApp;//单实例程序
    };
};
//========Smart::Singleton::CApp================================================
class Smart::Singleton::CApp
{
public:
    CApp(const std::wstring& GUID = L"C58D7FC1-B295-4C8C-97F5-E06A06F9BD81");
    ~CApp(void);
    BOOL IsRunning(void);  //【函数功能】：判断单例程序是否正在运行
    void SetWnd(HWND hWnd);//【函数功能】：设置单例程序接收消息窗口
    void Show(void);       //【函数功能】：显示单例程序
protected:
    std::wstring m_GUID;   //程序GUID
    HANDLE       m_Mutex;  //程序单例锁
    BOOL         m_Running;//程序运行状态
};

#include ".\Deelx\deelx.h"
namespace Smart
{
    class CRegex;//正则操作类
};
//========Smart::CRegex=========================================================
class Smart::CRegex
{
public:
    static BOOL GetGroupMatchGroup(const std::wstring& Text, const MatchResult& Match, INT32 GID, const std::wstring& Regex, INT32 SGID, std::wstring& Group)
    {
        Group.clear();

        std::wstring _Group;
        if(!Smart::CRegex::GetGroup(Text, Match, GID, _Group))
        {
            return FALSE;
        }
        else
        {
            return Smart::CRegex::GetMatchGroup(_Group, Regex, SGID, Group);
        }
    };
    static BOOL GetGroupMatchGroups(const std::wstring& Text, const MatchResult& Match, INT32 GID, const std::wstring& Regex, INT32 SGID, std::wstring& Groups)
    {
        Groups.clear();

        std::wstring _Group;
        if(!Smart::CRegex::GetGroup(Text, Match, GID, _Group))
        {
            return FALSE;
        }
        else
        {
            return Smart::CRegex::GetMatchGroups(_Group, Regex, SGID, Groups);
        }
    };

    static std::wstring GetMatchGroups(const std::wstring& Text, const std::wstring& Regex, INT32 GID = 0)
    {
        std::wstring Groups;
        Smart::CRegex::GetMatchGroups(Text, Regex, GID, Groups);
        return Groups;
    };
    static BOOL         GetMatchGroups(const std::wstring& Text, const std::wstring& Regex, INT32 GID, std::wstring& Groups)
    {
        Groups.clear();

        std::list<MatchResult> Matches;
        if(!Smart::CRegex::Match(Text, Regex, Matches)){ return FALSE; }
        auto iter = Matches.begin();
        if(GID < 0)                                    { return FALSE; }
        if(GID > iter->MaxGroupNumber())               { return FALSE; }
        if(iter->GetGroupStart(GID) >= 0){ Groups = Text.substr(iter->GetGroupStart(GID), iter->GetGroupEnd(GID) - iter->GetGroupStart(GID)); }

        for(iter++; iter != Matches.end(); iter++)
        {
            Groups.append(L";");
            if(iter->GetGroupStart(GID) >= 0){ Groups.append(Text.substr(iter->GetGroupStart(GID), iter->GetGroupEnd(GID) - iter->GetGroupStart(GID))); }
        }
        return TRUE;
    };//【函数功能】：匹配字符串，当匹配项超过1个，以符号;分隔

    static std::wstring GetMatchGroup(const std::wstring& Text, const std::wstring& Regex, INT32 GID = 0)
    {
        std::wstring Group;
        Smart::CRegex::GetMatchGroup(Text, Regex, GID, Group);
        return Group;
    };//【函数功能】：匹配字符串，当匹配项超过1个，返回错误
    static BOOL         GetMatchGroup(const std::wstring& Text, const std::wstring& Regex, INT32 GID, std::wstring& Group)
    {
        Group.clear();

        std::list<MatchResult> Matches;
        if(!Smart::CRegex::Match(Text, Regex, Matches)){ return FALSE; }
        if(Matches.size() > 1)                         { return FALSE; }
        auto iter = Matches.begin();
        if(GID < 0)                                    { return FALSE; }
        if(GID > iter->MaxGroupNumber())               { return FALSE; }
        if(iter->GetGroupStart(GID) >= 0){ Group = Text.substr(iter->GetGroupStart(GID), iter->GetGroupEnd(GID) - iter->GetGroupStart(GID)); }
        return TRUE;
    };//【函数功能】：匹配字符串，当匹配项超过1个，返回错误
    static BOOL         GetMatchGroup(const std::wstring& Text, const std::wstring& Regex, INT32 GID, INT32& Group)
    {
        std::wstring _Group;
        if(!Smart::CRegex::GetMatchGroup(Text, Regex, GID, _Group)){ return FALSE; }
        Group = ::_wtoi(_Group.c_str());
        return TRUE;
    };//【函数功能】：匹配字符串，当匹配项超过1个，返回错误
    static BOOL         GetMatchGroup(const std::wstring& Text, const std::wstring& Regex, INT32 GID, UINT32& Group)
    {
        std::wstring _Group;
        if(!Smart::CRegex::GetMatchGroup(Text, Regex, GID, _Group)){ return FALSE; }
        Group = (UINT32)::_wtoi(_Group.c_str());
        return TRUE;
    };//【函数功能】：匹配字符串，当匹配项超过1个，返回错误
    static BOOL         GetMatchGroup(const std::wstring& Text, const std::wstring& Regex, INT32 GID, DOUBLE& Group)
    {
        std::wstring _Group;
        if(!Smart::CRegex::GetMatchGroup(Text, Regex, GID, _Group)){ return FALSE; }
        Group = ::_wtof(_Group.c_str());
        return TRUE;
    };//【函数功能】：匹配字符串，当匹配项超过1个，返回错误
    
    static std::wstring GetGroup(const std::wstring& Text, const MatchResult& Match, INT32 GID = 0)
    {
        std::wstring Group;
        Smart::CRegex::GetGroup(Text, Match, GID, Group);
        return Group;
    }
    static BOOL         GetGroup(const std::wstring& Text, const MatchResult& Match, INT32 GID, std::wstring& Group)
    {
        Group.clear();
        if(GID < 0)                      { return FALSE; }
        if(GID > Match.MaxGroupNumber()) { return FALSE; }
        if(Match.GetGroupStart(GID) >= 0)
        {
            Group = Text.substr(
                Match.GetGroupStart(GID), 
                Match.GetGroupEnd(GID) - Match.GetGroupStart(GID));
        }
        return TRUE;
    }
    static BOOL         GetGroup(const std::wstring& Text, const MatchResult& Match, INT32 GID, INT32& Group)
    {
        std::wstring _Group;
        if(!Smart::CRegex::GetGroup(Text, Match, GID, _Group)){ return FALSE; }
        Group = ::_wtoi(_Group.c_str());
        return TRUE;
    }
    static BOOL         GetGroup(const std::wstring& Text, const MatchResult& Match, INT32 GID, UINT32& Group)
    {
        return Smart::CRegex::GetGroup(Text, Match, GID, *((PINT)&Group));
    }
    static BOOL         GetGroup(const std::wstring& Text, const MatchResult& Match, INT32 GID, DOUBLE& Group)
    {
        std::wstring _Group;
        if(!Smart::CRegex::GetGroup(Text, Match, GID, _Group)){ return FALSE; }
        Group = ::_wtof(_Group.c_str());
        return TRUE;
    }
    
    static BOOL Match(const std::wstring& Text, const std::wstring& Regex)                                 { return Smart::CRegex::Match(Text, Regex, SINGLELINE|IGNORECASE); }            //【函数功能】：匹配字符串
    static BOOL Match(const std::wstring& Text, const std::wstring& Regex, MatchResult& Match)             { return Smart::CRegex::Match(Text, Regex, SINGLELINE|IGNORECASE, Match); }     //【函数功能】：匹配字符串
    static BOOL Match(const std::wstring& Text, const std::wstring& Regex, std::list<MatchResult>& Matches){ return Smart::CRegex::Match(Text, Regex, SINGLELINE|IGNORECASE, Matches); }   //【函数功能】：匹配字符串

    static BOOL Match(const std::wstring& Text, const std::wstring& Regex, int Flag)
    {
        MatchResult Match;
        return Smart::CRegex::Match(Text, Regex, Flag, Match);
    };//【函数功能】：匹配字符串
    static BOOL Match(const std::wstring& Text, const std::wstring& Regex, int Flag, MatchResult& Match)
    {
        CRegexpT<wchar_t> RegexT(Regex.c_str(), Flag);
        Match = RegexT.Match(Text.c_str());
        return Match.IsMatched();
    }//【函数功能】：匹配字符串
    static BOOL Match(const std::wstring& Text, const std::wstring& Regex, int Flag, std::list<MatchResult>& Matches)
    {
        //CRegexp RegexT(PUINT16(Regex.c_str()), int(Regex.size()), NO_FLAG);
        CRegexpT<wchar_t> RegexT(Regex.c_str(), Flag);
        CContext*         Context = RegexT.PrepareMatch(Text.c_str());
        MatchResult       Match;
        Matches.clear();
        while((Match = RegexT.Match(Context)).IsMatched())
        {
            Matches.push_back(Match);
        }
        RegexT.ReleaseContext(Context);
        return !Matches.empty();
    };//【函数功能】：匹配字符串

//#include <Regex>
//    class Smart::CRegex
//    {
//    public:
//        static BOOL WINAPI Test(const std::wstring& Source, const std::wstring& Regex);                                                                      //【函数功能】：匹配字符串
//        static BOOL WINAPI Match(const std::wstring& Source, const std::wstring& Regex, std::match_results<LPCWSTR>& Match);                                 //【函数功能】：匹配字符串
//        static BOOL WINAPI Matches(const std::wstring& Source, const std::wstring& Regex, std::vector<std::wstring>& Results);                               //【函数功能】：匹配字符串
//        static BOOL WINAPI Matches(const std::wstring& Source, const std::wstring& Regex, int iSubx, std::vector<std::wstring>& Results);                    //【函数功能】：匹配字符串
//        static BOOL WINAPI Matches(const std::wstring& Source, const std::wstring& Regex, const std::vector<int>& _Subx, std::vector<std::wstring>& Results);//【函数功能】：匹配字符串
//    };
//    //========Smart::CRegex===================================================
//    BOOL Smart::CRegex::Test(const std::wstring& Source, const std::wstring& Regex)
//    {
//        std::match_results<LPCWSTR> Match;
//        return Smart::CRegex::Match(Source, Regex, Match);
//    }
//    BOOL Smart::CRegex::Match(const std::wstring& Source, const std::wstring& Regex, std::match_results<LPCWSTR>& Match)
//    {
//        LPCWSTR First = Source.c_str();
//        LPCWSTR Last  = Source.c_str() + Source.length();
//        return std::regex_search(First, Last, Match, std::wregex(Regex, std::regex_constants::icase));
//    }
//    BOOL Smart::CRegex::Matches(const std::wstring& Source, const std::wstring& Regex, std::vector<std::wstring>& Result)
//    {
//        Result.clear();
//
//        LPCWSTR First = Source.c_str();
//        LPCWSTR Last  = Source.c_str() + Source.length();
//        std::regex_token_iterator<LPCWSTR>::regex_type Rx(Regex, std::regex_constants::icase);
//        for(std::regex_token_iterator<LPCWSTR> iter(First, Last, Rx), end; iter != end ; iter++)
//        {
//            Result.push_back(*iter);
//        }
//        return Result.size() > 0;
//    }
//    BOOL Smart::CRegex::Matches(const std::wstring& Source, const std::wstring& Regex, int iSubx, std::vector<std::wstring>& Result)
//    {
//        std::vector<int> _Subx;
//        _Subx.push_back(iSubx);
//        Result.clear();
//
//        LPCWSTR First = Source.c_str();
//        LPCWSTR Last  = Source.c_str() + Source.length();
//        std::regex_token_iterator<LPCWSTR>::regex_type Rx(Regex, std::regex_constants::icase);
//        for(std::regex_token_iterator<LPCWSTR> iter(First, Last, Rx, _Subx), end; iter != end ; iter++)
//        {
//            if(std::find(Result.begin(), Result.end(), iter->str()) == Result.end())
//            {
//                Result.push_back(iter->str());
//            }
//        }
//        return Result.size() > 0;
//    }
//    BOOL Smart::CRegex::Matches(const std::wstring& Source, const std::wstring& Regex, const std::vector<int>& _Subx, std::vector<std::wstring>& Result)
//    {
//        Result.clear();
//
//        LPCWSTR First = Source.c_str();
//        LPCWSTR Last  = Source.c_str() + Source.length();
//        std::regex_token_iterator<LPCWSTR>::regex_type Rx(Regex, std::regex_constants::icase);
//        for(std::regex_token_iterator<LPCWSTR> iter(First, Last, Rx, _Subx), end; iter != end ; iter++)
//        {
//            if(std::find(Result.begin(), Result.end(), iter->str()) == Result.end())
//            {
//                Result.push_back(iter->str());
//            }
//        }
//        return Result.size() > 0;
//    }
};

#ifdef Smart_Json
#include ".\Smart.Expand\Jsoncpp\include\json.h"
namespace Smart
{
    class CJson;
};
//========Smart::CJson==========================================================
class Smart::CJson
{
public:
    static BOOL WINAPI TextToJson(const std::string& Text, Json::Value& Object);                  //【函数功能】：将文本转化为JSON对象
    static BOOL WINAPI JsonToText(const Json::Value& Object, std::string&  Text);                 //【函数功能】：将JSON转化为文本
    static BOOL WINAPI JsonToText(const Json::Value& Object, std::wstring& Text);                 //【函数功能】：将JSON转化为文本
public:
    static std::wstring WINAPI JsonToText(const Json::Value& Object);//【函数功能】：将JSON转化为文本
public:
    static BOOL WINAPI GetValue(const Json::Value& Object, const std::string& Name, bool&         Value, bool                Default = false);//【函数功能】：获取JSON对象的某一项值，返回值表示是否使用默认值，【TRUE：没有使用默认值】【FALSE：使用默认值】
    static BOOL WINAPI GetValue(const Json::Value& Object, const std::string& Name, INT&          Value, INT                 Default = 0);    //【函数功能】：获取JSON对象的某一项值，返回值表示是否使用默认值，【TRUE：没有使用默认值】【FALSE：使用默认值】
    static BOOL WINAPI GetValue(const Json::Value& Object, const std::string& Name, UINT&         Value, UINT                Default = 0);    //【函数功能】：获取JSON对象的某一项值，返回值表示是否使用默认值，【TRUE：没有使用默认值】【FALSE：使用默认值】
    static BOOL WINAPI GetValue(const Json::Value& Object, const std::string& Name, DOUBLE&       Value, DOUBLE              Default = 0);    //【函数功能】：获取JSON对象的某一项值，返回值表示是否使用默认值，【TRUE：没有使用默认值】【FALSE：使用默认值】
    static BOOL WINAPI GetValue(const Json::Value& Object, const std::string& Name, std::string&  Value, const std::string&  Default = "");   //【函数功能】：获取JSON对象的某一项值，返回值表示是否使用默认值，【TRUE：没有使用默认值】【FALSE：使用默认值】
    static BOOL WINAPI GetValue(const Json::Value& Object, const std::string& Name, std::wstring& Value, const std::wstring& Default = L"");  //【函数功能】：获取JSON对象的某一项值，返回值表示是否使用默认值，【TRUE：没有使用默认值】【FALSE：使用默认值】
};
#endif

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
        class CSelfElevation;//Ȩ������
        class CAgentClient;  //�������ͻ���
        class CAgentServer;  //������������
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
    static DWORD WINAPI IsUserInAdminGroup(BOOL inGroup);      //���������ܡ�����ǰ�û����ڹ���Ա�飬�ɹ�����ERROR_SUCCESS��ʧ�ܷ��ش������
    static DWORD WINAPI IsRunAsAdmin(BOOL isRunAs);            //���������ܡ�����ǰ�����Ƿ��Թ���Ա���У��ɹ�����ERROR_SUCCESS��ʧ�ܷ��ش������
    static DWORD WINAPI IsProcessElevated(BOOL isElevated);    //���������ܡ�����ǰ�����Ƿ��Ѿ�����Ȩ�ޣ��ɹ�����ERROR_SUCCESS��ʧ�ܷ��ش������
    static DWORD WINAPI GetProcessIntegrityLevel(DWORD& Level);//���������ܡ�����ȡ��ǰ����Ȩ�޵ȼ����ɹ�����ERROR_SUCCESS��ʧ�ܷ��ش������
};
//========Smart::UAC::CAgentClient==============================================
class Smart::UAC::CAgentClient
{
public:
    #define UACAgentTimeOut                     5000
    #define UACAgentMaxSize                     4096
    enum AgentProtocolType
    {
        APT_Close = 0x00,//����ָ�������ָ�
        APT_Quit  = 0x01,//�˳�ָ��ͻ���ָ�

        APT_RegKeyAQ   = 0x10,//ע����������󣨷�����ָ�
        APT_RegKeyAA   = 0x11,//ע�������Ӧ�𣨿ͻ���ָ�
        APT_RegKeyDQ   = 0x12,//ע���ɾ�������󣨷�����ָ�
        APT_RegKeyDA   = 0x13,//ע���ɾ����Ӧ�𣨿ͻ���ָ�
        APT_RegValueAQ = 0x14,//ע����������󣨷�����ָ�
        APT_RegValueAA = 0x15,//ע�������Ӧ�𣨿ͻ���ָ�
        APT_RegValueDQ = 0x16,//ע���ɾ�������󣨷�����ָ�
        APT_RegValueDA = 0x17,//ע���ɾ����Ӧ�𣨿ͻ���ָ�

        APT_FileAQ   = 0x20,//�ļ�������󣨷�����ָ�
        APT_FileAA   = 0x21,//�ļ����Ӧ�𣨿ͻ���ָ�
        APT_FileDQ   = 0x22,//�ļ�ɾ�����󣨷�����ָ�
        APT_FileDA   = 0x23,//�ļ�ɾ��Ӧ�𣨿ͻ���ָ�
        APT_FolderAQ = 0x24,//�ļ����ƶ����󣨷�����ָ�
        APT_FolderAA = 0x25,//�ļ����ƶ�Ӧ�𣨿ͻ���ָ�
        APT_FolderDQ = 0x26,//�ļ���ɾ�����󣨷�����ָ�
        APT_FolderDA = 0x27,//�ļ���ɾ��Ӧ�𣨿ͻ���ָ�

        APT_NoSupport = 0xFF//�Ƿ�ָ��Ӧ�𣨿ͻ���ָ�
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
    BOOL Execute(void);//���������ܡ���ִ�д�������
protected:
    BOOL WriteData(AgentProtocolType APT, TaskProtocolSubType TPST = TPST_Undefined, const std::wstring& Text = L"");//���������ܡ�����ܵ�д����
    BOOL ReadData(AgentProtocolType& APT, TaskProtocolSubType& TPST, std::wstring& Text);                            //���������ܡ����ӹܵ�������
protected:
    BOOL Analyse(const std::wstring& Text, UINT32& BPos, std::wstring& Data);
    BOOL Analyse(const std::wstring& Text, UINT32& BPos, DWORD& Data);
    BOOL Analyse(const std::wstring& Text, UINT32& BPos, PBYTE Data, UINT32& Size);
    BOOL CreateRegistryKey(TaskProtocolSubType TPST, const std::wstring& Text);     //���������ܡ�������ע����
    BOOL DeleteRegistryKey(TaskProtocolSubType TPST, const std::wstring& Text);     //���������ܡ���ɾ��ע����
    BOOL SetRegistryKeyValue(TaskProtocolSubType TPST, const std::wstring& Text);   //���������ܡ�������ע����ֵ
    BOOL DeleteRegistryKeyValue(TaskProtocolSubType TPST, const std::wstring& Text);//���������ܡ���ɾ��ע����ֵ
    BOOL MoveFile(const std::wstring& Text);                                        //���������ܡ����ƶ��ļ�
    BOOL DeleteFile(const std::wstring& Text);                                      //���������ܡ���ɾ���ļ�
    BOOL MoveFolder(const std::wstring& Text);                                      //���������ܡ����ƶ��ļ���
    BOOL DeleteFolder(const std::wstring& Text);                                    //���������ܡ���ɾ���ļ���
protected:
    HANDLE          m_Pipe;                    //�ܵ����
    OVERLAPPED      m_Overlapped;              //�ܵ�OVERLAPPED
    DeclareProperty(std::wstring,   PipeName);//�ܵ�����
    DeclareProperty_R(std::wstring, Message); //�ܵ�������Ϣ
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
    BOOL StartServer(void);//���������ܡ�������������������
    void StopServer(void); //���������ܡ���ֹͣ������������
    BOOL StartClient(void);//���������ܡ��������������ͻ���
    void StopClient(void); //���������ܡ���ֹͣ�������ͻ���
    BOOL ServerProc(AgentProtocolType APTQ, TaskProtocolSubType TPST = TPST_Undefined, const std::wstring& Text = L"");//���������ܡ���������ָ��ִ������
public:
    BOOL CreateRegistryKey(HKEY Key, const std::wstring& SubPath);                                                                 //���������ܡ�������ע����
    BOOL DeleteRegistryKey(HKEY Key, const std::wstring& SubPath);                                                                 //���������ܡ���ɾ��ע����
    BOOL SetRegistryKeyValue(HKEY Key, const std::wstring& SubPath, const std::wstring& Name, DWORD Type, PBYTE Value, DWORD Size);//���������ܡ�������ע����ֵ
    BOOL DeleteRegistryKeyValue(HKEY Key, const std::wstring& SubPath, const std::wstring& Name);                                  //���������ܡ���ɾ��ע����ֵ
    BOOL MoveFile(const std::wstring& SourPath, const std::wstring& DestPath); //���������ܡ����ƶ��ļ�
    BOOL DeleteFile(const std::wstring& Path);                                 //���������ܡ���ɾ���ļ�
    BOOL MoveFolder(const std::wstring&SourPath, const std::wstring& DestPath);//���������ܡ����ƶ��ļ���
    BOOL DeleteFolder(const std::wstring& Path);                               //���������ܡ���ɾ���ļ���
protected:
    Smart::CSection m_Section;
    std::wstring    m_ClientPath;                   //�ܵ��ͻ���·��
    HANDLE          m_ClientProcess;                //�ܵ��ͻ��˽���
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
        class CApp;//��ʵ������
    };
};
//========Smart::Singleton::CApp================================================
class Smart::Singleton::CApp
{
public:
    CApp(const std::wstring& GUID = L"C58D7FC1-B295-4C8C-97F5-E06A06F9BD81");
    ~CApp(void);
    BOOL IsRunning(void);  //���������ܡ����жϵ��������Ƿ���������
    void SetWnd(HWND hWnd);//���������ܡ������õ������������Ϣ����
    void Show(void);       //���������ܡ�����ʾ��������
protected:
    std::wstring m_GUID;   //����GUID
    HANDLE       m_Mutex;  //��������
    BOOL         m_Running;//��������״̬
};

#include ".\Deelx\deelx.h"
namespace Smart
{
    class CRegex;//���������
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
    };//���������ܡ���ƥ���ַ�������ƥ�����1�����Է���;�ָ�

    static std::wstring GetMatchGroup(const std::wstring& Text, const std::wstring& Regex, INT32 GID = 0)
    {
        std::wstring Group;
        Smart::CRegex::GetMatchGroup(Text, Regex, GID, Group);
        return Group;
    };//���������ܡ���ƥ���ַ�������ƥ�����1�������ش���
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
    };//���������ܡ���ƥ���ַ�������ƥ�����1�������ش���
    static BOOL         GetMatchGroup(const std::wstring& Text, const std::wstring& Regex, INT32 GID, INT32& Group)
    {
        std::wstring _Group;
        if(!Smart::CRegex::GetMatchGroup(Text, Regex, GID, _Group)){ return FALSE; }
        Group = ::_wtoi(_Group.c_str());
        return TRUE;
    };//���������ܡ���ƥ���ַ�������ƥ�����1�������ش���
    static BOOL         GetMatchGroup(const std::wstring& Text, const std::wstring& Regex, INT32 GID, UINT32& Group)
    {
        std::wstring _Group;
        if(!Smart::CRegex::GetMatchGroup(Text, Regex, GID, _Group)){ return FALSE; }
        Group = (UINT32)::_wtoi(_Group.c_str());
        return TRUE;
    };//���������ܡ���ƥ���ַ�������ƥ�����1�������ش���
    static BOOL         GetMatchGroup(const std::wstring& Text, const std::wstring& Regex, INT32 GID, DOUBLE& Group)
    {
        std::wstring _Group;
        if(!Smart::CRegex::GetMatchGroup(Text, Regex, GID, _Group)){ return FALSE; }
        Group = ::_wtof(_Group.c_str());
        return TRUE;
    };//���������ܡ���ƥ���ַ�������ƥ�����1�������ش���
    
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
    
    static BOOL Match(const std::wstring& Text, const std::wstring& Regex)                                 { return Smart::CRegex::Match(Text, Regex, SINGLELINE|IGNORECASE); }            //���������ܡ���ƥ���ַ���
    static BOOL Match(const std::wstring& Text, const std::wstring& Regex, MatchResult& Match)             { return Smart::CRegex::Match(Text, Regex, SINGLELINE|IGNORECASE, Match); }     //���������ܡ���ƥ���ַ���
    static BOOL Match(const std::wstring& Text, const std::wstring& Regex, std::list<MatchResult>& Matches){ return Smart::CRegex::Match(Text, Regex, SINGLELINE|IGNORECASE, Matches); }   //���������ܡ���ƥ���ַ���

    static BOOL Match(const std::wstring& Text, const std::wstring& Regex, int Flag)
    {
        MatchResult Match;
        return Smart::CRegex::Match(Text, Regex, Flag, Match);
    };//���������ܡ���ƥ���ַ���
    static BOOL Match(const std::wstring& Text, const std::wstring& Regex, int Flag, MatchResult& Match)
    {
        CRegexpT<wchar_t> RegexT(Regex.c_str(), Flag);
        Match = RegexT.Match(Text.c_str());
        return Match.IsMatched();
    }//���������ܡ���ƥ���ַ���
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
    };//���������ܡ���ƥ���ַ���

//#include <Regex>
//    class Smart::CRegex
//    {
//    public:
//        static BOOL WINAPI Test(const std::wstring& Source, const std::wstring& Regex);                                                                      //���������ܡ���ƥ���ַ���
//        static BOOL WINAPI Match(const std::wstring& Source, const std::wstring& Regex, std::match_results<LPCWSTR>& Match);                                 //���������ܡ���ƥ���ַ���
//        static BOOL WINAPI Matches(const std::wstring& Source, const std::wstring& Regex, std::vector<std::wstring>& Results);                               //���������ܡ���ƥ���ַ���
//        static BOOL WINAPI Matches(const std::wstring& Source, const std::wstring& Regex, int iSubx, std::vector<std::wstring>& Results);                    //���������ܡ���ƥ���ַ���
//        static BOOL WINAPI Matches(const std::wstring& Source, const std::wstring& Regex, const std::vector<int>& _Subx, std::vector<std::wstring>& Results);//���������ܡ���ƥ���ַ���
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
    static BOOL WINAPI TextToJson(const std::string& Text, Json::Value& Object);                  //���������ܡ������ı�ת��ΪJSON����
    static BOOL WINAPI JsonToText(const Json::Value& Object, std::string&  Text);                 //���������ܡ�����JSONת��Ϊ�ı�
    static BOOL WINAPI JsonToText(const Json::Value& Object, std::wstring& Text);                 //���������ܡ�����JSONת��Ϊ�ı�
public:
    static std::wstring WINAPI JsonToText(const Json::Value& Object);//���������ܡ�����JSONת��Ϊ�ı�
public:
    static BOOL WINAPI GetValue(const Json::Value& Object, const std::string& Name, bool&         Value, bool                Default = false);//���������ܡ�����ȡJSON�����ĳһ��ֵ������ֵ��ʾ�Ƿ�ʹ��Ĭ��ֵ����TRUE��û��ʹ��Ĭ��ֵ����FALSE��ʹ��Ĭ��ֵ��
    static BOOL WINAPI GetValue(const Json::Value& Object, const std::string& Name, INT&          Value, INT                 Default = 0);    //���������ܡ�����ȡJSON�����ĳһ��ֵ������ֵ��ʾ�Ƿ�ʹ��Ĭ��ֵ����TRUE��û��ʹ��Ĭ��ֵ����FALSE��ʹ��Ĭ��ֵ��
    static BOOL WINAPI GetValue(const Json::Value& Object, const std::string& Name, UINT&         Value, UINT                Default = 0);    //���������ܡ�����ȡJSON�����ĳһ��ֵ������ֵ��ʾ�Ƿ�ʹ��Ĭ��ֵ����TRUE��û��ʹ��Ĭ��ֵ����FALSE��ʹ��Ĭ��ֵ��
    static BOOL WINAPI GetValue(const Json::Value& Object, const std::string& Name, DOUBLE&       Value, DOUBLE              Default = 0);    //���������ܡ�����ȡJSON�����ĳһ��ֵ������ֵ��ʾ�Ƿ�ʹ��Ĭ��ֵ����TRUE��û��ʹ��Ĭ��ֵ����FALSE��ʹ��Ĭ��ֵ��
    static BOOL WINAPI GetValue(const Json::Value& Object, const std::string& Name, std::string&  Value, const std::string&  Default = "");   //���������ܡ�����ȡJSON�����ĳһ��ֵ������ֵ��ʾ�Ƿ�ʹ��Ĭ��ֵ����TRUE��û��ʹ��Ĭ��ֵ����FALSE��ʹ��Ĭ��ֵ��
    static BOOL WINAPI GetValue(const Json::Value& Object, const std::string& Name, std::wstring& Value, const std::wstring& Default = L"");  //���������ܡ�����ȡJSON�����ĳһ��ֵ������ֵ��ʾ�Ƿ�ʹ��Ĭ��ֵ����TRUE��û��ʹ��Ĭ��ֵ����FALSE��ʹ��Ĭ��ֵ��
};
#endif

#include "stdafx.h"
#include <assert.h>
#include <CommDlg.h>
#include <ShlObj.h>
#include <sys\stat.h>
#include "Smart.Def.h"
#include "Smart.Global.h"
//==============================================================================
#pragma comment(lib, "Version.lib")
#pragma comment(lib, "winmm.lib")
//========Smart::CHex===========================================================
BYTE         Smart::CHex::xtoi(char c)
{
    switch(c)
    {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'a': case 'A': return 10;
    case 'b': case 'B': return 11;
    case 'c': case 'C': return 12;
    case 'd': case 'D': return 13;
    case 'e': case 'E': return 14;
    case 'f': case 'F': return 15;
    default:  return 0;
    }
}
BYTE         Smart::CHex::xtoi(wchar_t c)
{
    switch(c)
    {
    case '0': return 0;
    case '1': return 1;
    case '2': return 2;
    case '3': return 3;
    case '4': return 4;
    case '5': return 5;
    case '6': return 6;
    case '7': return 7;
    case '8': return 8;
    case '9': return 9;
    case 'a': case 'A': return 10;
    case 'b': case 'B': return 11;
    case 'c': case 'C': return 12;
    case 'd': case 'D': return 13;
    case 'e': case 'E': return 14;
    case 'f': case 'F': return 15;
    default:  return 0;
    }
}
BOOL         Smart::CHex::ToByte(const std::string& HexA, PBYTE Data, UINT32& Size)
{
    if(HexA.empty())
    {
        Size = 0;
    }
    else
    {
        Size = HexA.size()/2;
        for(UINT32 i = 0, j = 0; i < Size; i += 1, j += 2)
        {
            Data[i] = Smart::CHex::xtoi(HexA[j])*0x10 + Smart::CHex::xtoi(HexA[j + 1]);
        }

        if(HexA.size()%2 == 1)
        {
            Data[Size] = Smart::CHex::xtoi(HexA[HexA.size() - 1])*0x10;
            Size += 1;
        }
    }
    return TRUE;
}
BOOL         Smart::CHex::ToByte(const std::wstring& HexW, PBYTE Data, UINT32& Size)
{
    if(HexW.empty())
    {
        Size = 0;
    }
    else
    {
        Size = HexW.size()/2;
        for(UINT32 i = 0, j = 0; i < Size; i += 1, j += 2)
        {
            Data[i] = Smart::CHex::xtoi(HexW[j])*0x10 + Smart::CHex::xtoi(HexW[j + 1]);
        }

        if(HexW.size()%2 == 1)
        {
            Data[Size] = Smart::CHex::xtoi(HexW[HexW.size() - 1])*0x10;
            Size += 1;
        }
    }
    return TRUE;
}
std::string  Smart::CHex::ToHexA(PBYTE Data, UINT32 Size, BOOL Upper)
{
    std::string Hex;
    LPCSTR      Format  = Upper ? "%02X" : "%02x";
    char        Code[3] = {0};

    for(UINT i = 0; i < Size; i++)
    {
        ::sprintf_s(Code, 3, Format, Data[i]);
        Hex.append(Code);
    }

    return Hex;
}
std::wstring Smart::CHex::ToHexW(PBYTE Data, UINT32 Size, BOOL Upper)
{
    std::wstring Hex;
    LPCWSTR      Format  = Upper ? L"%02X" : L"%02x";
    wchar_t      Code[3] = {0};

    for(UINT i = 0; i < Size; i++)
    {
        ::swprintf_s(Code, 3, Format, Data[i]);
        Hex.append(Code);
    }

    return Hex;
}
//========Smart::CURI===========================================================
std::string  Smart::CURI::UrlEncode(const std::string& Text)
{
    std::string UrlCode;
    char        Code[4] = {0};
    for(auto iter = Text.begin(); iter != Text.end(); iter++)
    {
        ::memset(Code, 0, 4);

        if(::isalnum(BYTE(*iter)) != 0)
        {
            Code[0] = char(*iter);
        }
        else
        {
            ::sprintf_s(Code, "%%%02X", BYTE(*iter));
        }
        UrlCode.append(Code);
    }
    return UrlCode;
}
std::string  Smart::CURI::UrlDecode(const std::string& Text)
{
    std::string AnsiCode;
    char        Code[2] = {0};
    for(UINT i = 0, l = Text.size(); i < l;)
    {
        if(Text[i] == '+')     {                                                                               AnsiCode.append(" ");  i += 1;}
        else if(Text[i] != '%'){ Code[0] = Text[i];                                                            AnsiCode.append(Code); i += 1;}
        else if(i + 2 < l)     { Code[0] = Smart::CHex::xtoi(Text[i + 1])*16 + Smart::CHex::xtoi(Text[i + 2]); AnsiCode.append(Code); i += 3;}
        else                   {                                                                                                      i += 1;}
    }
    return AnsiCode;
}
//------------------------------------------------------------------------------
std::wstring Smart::CURI::UrlEncode_ANSI(const std::wstring& Text)
{
    return Smart::CText::ANSI_to_UTF16(Smart::CURI::UrlEncode(Smart::CText::UTF16_to_ANSI(Text)));
}
std::wstring Smart::CURI::UrlEncode_GBK(const std::wstring& Text)
{
    return Smart::CText::GBK_to_UTF16(Smart::CURI::UrlEncode(Smart::CText::UTF16_to_GBK(Text)));
}
std::wstring Smart::CURI::UrlEncode_UTF8(const std::wstring& Text)
{
    return Smart::CText::UTF8_to_UTF16(Smart::CURI::UrlEncode(Smart::CText::UTF16_to_UTF8(Text)));
}
std::wstring Smart::CURI::UrlDecode_ANSI(const std::wstring& Text)
{
    return Smart::CText::ANSI_to_UTF16(Smart::CURI::UrlDecode(Smart::CText::UTF16_to_ANSI(Text)));
}
std::wstring Smart::CURI::UrlDecode_GBK(const std::wstring& Text)
{
    return Smart::CText::GBK_to_UTF16(Smart::CURI::UrlDecode(Smart::CText::UTF16_to_GBK(Text)));
}
std::wstring Smart::CURI::UrlDecode_UTF8(const std::wstring& Text)
{
    return Smart::CText::UTF8_to_UTF16(Smart::CURI::UrlDecode(Smart::CText::UTF16_to_UTF8(Text)));
}
//========Smart::CDateTime======================================================
Smart::CDateTime::CDateTime(void)
    : m_Year(0)
    , m_Month(0)
    , m_Day(0)
    , m_Hour(0)
    , m_Minute(0)
    , m_Second(0)
    , m_Millisecond(0)
    , m_DayOfWeek(0)
    , m_TimeZoneID(TIME_ZONE_ID_INVALID)
{
    ::memset(&this->m_TimeZone, 0, sizeof(TIME_ZONE_INFORMATION));
}
Smart::CDateTime::~CDateTime(void)
{
}
UINT64       Smart::CDateTime::GetTimeStamp(BOOL Second)
{
    if(this->m_Year < 1970)
    {
        return 0;
    }
    else
    {
        UINT64 Stamp = 0;
        for(UINT32 Year = 1970; Year < this->m_Year; Year++)
        {
            Stamp += Smart::CDateTime::GetDaysOfYear(Year)*24*3600;
        }
        Stamp += (Smart::CDateTime::GetDayOfYear(this->m_Year, this->m_Month, this->m_Day) - 1)*24*3600 + this->m_Hour*3600 + this->m_Minute*60 + this->m_Second;

        if(this->m_TimeZoneID == TIME_ZONE_ID_UNKNOWN || this->m_TimeZoneID == TIME_ZONE_ID_STANDARD)
        {
            Stamp += this->m_TimeZone.Bias*60;
        }



        if(Second)
        {
            return Stamp;
        }
        else
        {
            return Stamp*1000 + this->m_Millisecond;
        }
    }
}
std::wstring Smart::CDateTime::ToString(BOOL Format)
{
    if(Format)
    {
        return Smart::CText::Format(L"%04d-%02d-%02d %02d:%02d:%02d.%03d", this->m_Year, this->m_Month, this->m_Day, this->m_Hour, this->m_Minute, this->m_Second, this->m_Millisecond);
    }
    else
    {
        return Smart::CText::Format(L"%04d%02d%02d%02d%02d%02d%03d", this->m_Year, this->m_Month, this->m_Day, this->m_Hour, this->m_Minute, this->m_Second, this->m_Millisecond);
    }
}
//------------------------------------------------------------------------------
BOOL             Smart::CDateTime::IsLeapYear(UINT32 Year)
{
    if(Year%4   != 0){ return FALSE;}
    if(Year%400 == 0){ return TRUE;}
    if(Year%100 == 0){ return FALSE;}
    return TRUE;
}
BOOL             Smart::CDateTime::IsIllegal(UINT32 Year, UINT32 Month, UINT32 Day)
{
    switch(Month)
    {
    case  1:
    case  3:
    case  5:
    case  7:
    case  8:
    case 10:
    case 12:
        return Day <= 0 || Day > 31;
    case 2:
        return Smart::CDateTime::IsLeapYear(Year) ? (Day <= 0 || Day > 29) : (Day <= 0 || Day > 28);
    case  4:
    case  6:
    case  9:
    case 11:
        return Day <= 0 || Day > 30;
    default:
        return TRUE;
    }
}
UINT32           Smart::CDateTime::GetDaysOfYear(UINT32 Year)
{
    return Smart::CDateTime::IsLeapYear(Year) ? 366 : 365;
}
UINT32           Smart::CDateTime::GetDayOfYear(UINT32 Year, UINT32 Month, UINT32 Day)
{
    if(Smart::CDateTime::IsIllegal(Year, Month, Day)){ return UINT(-1); }
    UINT32 Days = 0;
    switch(Month)
    {
    case  1: Days = Day; break;
    case  2: Days = 31 + Day; break;
    case  3: Days = 31 + 28 + Day; break;
    case  4: Days = 31 + 28 + 31 + Day; break;
    case  5: Days = 31 + 28 + 31 + 30 + Day; break;
    case  6: Days = 31 + 28 + 31 + 30 + 31 + Day; break;
    case  7: Days = 31 + 28 + 31 + 30 + 31 + 30 + Day; break;
    case  8: Days = 31 + 28 + 31 + 30 + 31 + 30 + 31 + Day; break;
    case  9: Days = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + Day; break;
    case 10: Days = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + Day; break;
    case 11: Days = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + Day; break;
    case 12: Days = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + Day; break;
    default: return UINT(-1);
    }
    if(Month > 2 && Smart::CDateTime::IsLeapYear(Year))
    {
        Days += 1;
    }
    return Days;
}
Smart::CDateTime Smart::CDateTime::GetNow(void)
{
    SYSTEMTIME LT = {0};
    ::GetLocalTime(&LT);
    Smart::CDateTime DateTime = Smart::CDateTime::FromSYSTEMTIME(LT);
    DateTime.m_TimeZoneID = ::GetTimeZoneInformation(&DateTime.m_TimeZone);
    return DateTime;
}
Smart::CDateTime Smart::CDateTime::FromSYSTEMTIME(const SYSTEMTIME& ST)
{
    Smart::CDateTime DateTime;
    DateTime.m_Year        = ST.wYear;
    DateTime.m_Month       = UINT8(ST.wMonth);
    DateTime.m_Day         = UINT8(ST.wDay);
    DateTime.m_Hour        = UINT8(ST.wHour);
    DateTime.m_Minute      = UINT8(ST.wMinute);
    DateTime.m_Second      = UINT8(ST.wSecond);
    DateTime.m_Millisecond = ST.wMilliseconds;
    DateTime.m_DayOfWeek   = UINT8(ST.wDayOfWeek);
    return DateTime;
}
Smart::CDateTime Smart::CDateTime::FromFILETIME(const FILETIME& FT)
{
    SYSTEMTIME ST;
    ::FileTimeToSystemTime(&FT, &ST);
    return Smart::CDateTime::FromSYSTEMTIME(ST);
}
//------------------------------------------------------------------------------
void Smart::CDateTime::AddDays(SYSTEMTIME& ST, INT32 Days)
{
    UINT64 FT = {0};
    ::SystemTimeToFileTime(&ST, (PFILETIME)&FT);
    FT += Days*24*3600*1000*1000*10;
    ::FileTimeToSystemTime((PFILETIME)&FT, &ST);
}
//========Smart::CParam=========================================================
Smart::CParam::CParam(UINT Type, UINT Content)
    : m_Type(Type)
    , m_uContent(Content)
    , m_TimeStamp(Smart::CDateTime::GetNow().GetTimeStamp())
{
}
Smart::CParam::CParam(UINT Type, const std::wstring& Content)
    : m_Type(Type)
    , m_strContent(Content)
    , m_TimeStamp(Smart::CDateTime::GetNow().GetTimeStamp())
{
}
Smart::CParam::CParam(UINT Type, LPVOID Content)
    : m_Type(Type)
    , m_pContent(Content)
    , m_TimeStamp(Smart::CDateTime::GetNow().GetTimeStamp())
{
}
Smart::CParam::~CParam(void)
{
    if(this->m_Type >= WM_USER_SERIALIZE_EXPAND_MIN && this->m_Type <= WM_USER_SERIALIZE_EXPAND_MAX)
    {
        delete this->m_pContent;
    }
}
//========Smart::CTool==========================================================
void* Smart::CTool::MallocMemory(void** Mem, DWORD Size)
{
    assert(Mem);
    *Mem = ::malloc(Size);
    return *Mem;
}
void* Smart::CTool::MergerMemory(void* Data1, DWORD Size1, void* Data2, DWORD Size2)
{
    if(Size1 == 0 && Size2 == 0){ return NULL;}
    char* Memory = (char*)::malloc(Size1 + Size2);
    if(Memory == NULL)          { return NULL;}

    if(Data1 != NULL){ ::memcpy_s(Memory,           Size1, Data1, Size1); ::free(Data1);}
    if(Data2 != NULL){ ::memcpy_s(&(Memory[Size1]), Size2, Data2, Size2); ::free(Data2);}

    return Memory;
}
BOOL  Smart::CTool::MergerData(void* pDestData, DWORD dwDestDataSize, void* Data0, DWORD dwData0Size, void* Data1, DWORD dwData1Size)
{
    if(NULL == pDestData)
    {
        return FALSE;
    }
    else if(dwDestDataSize < dwData0Size + dwData1Size)
    {
        return FALSE;
    }
    else
    {
        ::memset(pDestData, 0, dwDestDataSize);
        if(Data0 != NULL){ ::memcpy_s(pDestData, dwDestDataSize, Data0, dwData0Size);}
        if(Data1 != NULL){ ::memcpy_s(&(((PBYTE)pDestData)[dwData0Size]), dwDestDataSize - dwData0Size, Data1, dwData1Size);}
        return TRUE;
    }
}
void  Smart::CTool::FreeMemory(void** Mem)
{
    if(Mem != NULL)
    {
        ::free(*Mem);
        *Mem = NULL;
    }
}
//------------------------------------------------------------------------------
RECT Smart::CTool::ScreenRect(void)
{
    RECT Rect = { 0, 0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN)};
    return Rect;
}
RECT Smart::CTool::WorkArea(void)
{
    RECT WorkRect;
    if(::SystemParametersInfo(SPI_GETWORKAREA, NULL, &WorkRect, NULL))
    {
        RECT Rect = { WorkRect.left, WorkRect.top, WorkRect.right - WorkRect.left, WorkRect.bottom - WorkRect.top };
        return Rect;
    }
    else
    {
        return Smart::CTool::ScreenRect();
        //SM_CXMAXIMIZED并不等于屏幕的宽，SM_CXMAXIMIZED等于屏幕的宽加上窗口边框的宽*2
        //同样SM_CYMAXIMIZED也不是屏幕的高,
        //且不能获取工作区域的Left和Top
        //return Gdiplus::Rect(0, 0, ::GetSystemMetrics(SM_CXMAXIMIZED), ::GetSystemMetrics(SM_CYMAXIMIZED));
        //下面使用的是全屏窗口减去任务栏的做法，在标准系统中没有问题，
        //但是在非标准系统里，别的程序将任务栏移动的窗口外区域时，再通过下面的计算就会有问题,
        //当然也是可以计算的，不过有点麻烦
        /*Gdiplus::Rect Rect     = Smart::DUI::CWindow::ScreenRect();
        Gdiplus::Rect TrayRect = Smart::DUI::CWindow::TrayRect();

        if(TrayRect.X == 0 && TrayRect.Y == 0)
        {
        if(TrayRect.Width == Rect.Width)
        {
        Rect.Y       = TrayRect.Height;
        Rect.Height -= TrayRect.Height;
        }
        else
        {
        Rect.X      = TrayRect.Width;
        Rect.Width -= TrayRect.Width;
        }
        }
        else
        {
        if(TrayRect.X == 0)
        {
        Rect.Height -= TrayRect.Height;
        }
        else
        {
        Rect.Width -= TrayRect.Width;
        }
        }
        return Rect;*/
    }
}
RECT Smart::CTool::TrayRect(void)
{
    RECT TrayRect;
    HWND TrayWnd = ::FindWindow(L"Shell_TrayWnd", NULL);
    if(TrayWnd == NULL)                          { RECT Rect = { 0, 0, 0, 0}; return Rect;}
    else if(!::GetWindowRect(TrayWnd, &TrayRect)){ RECT Rect = { 0, 0, 0, 0}; return Rect;}
    else                                         { RECT Rect = { TrayRect.left, TrayRect.top, TrayRect.right - TrayRect.left, TrayRect.bottom - TrayRect.top}; return Rect;}
}
//------------------------------------------------------------------------------
POINT Smart::CTool::GetCursorPos(void)
{
    POINT Point = {0, 0};
    ::GetCursorPos(&Point);
    return Point;
}
//------------------------------------------------------------------------------
BOOL Smart::CTool::SelectFileDialog(HWND Owner, const std::wstring& Title, LPCWSTR Filter, std::wstring& FilePath)
{
    wchar_t Path[MAX_PATH] = {0};

    OPENFILENAMEW OFN       = {0};
    OFN.lStructSize         = sizeof(OPENFILENAME);
    OFN.hwndOwner           = Owner;
    OFN.hInstance           = NULL;
    OFN.lpstrFilter         = Filter;
    OFN.lpstrCustomFilter   = NULL;
    OFN.nMaxCustFilter      = 0;
    OFN.nFilterIndex        = 1;
    OFN.lpstrFile           = Path;
    OFN.nMaxFile            = MAX_PATH;
    OFN.lpstrFileTitle      = NULL;
    OFN.nMaxFileTitle       = 0;
    OFN.lpstrInitialDir     = NULL;
    OFN.lpstrTitle          = Title.c_str();
    OFN.Flags               = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if(::GetOpenFileNameW(&OFN))
    {
        FilePath = Path;
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
BOOL Smart::CTool::SelectFolderDialog(HWND Owner, const std::wstring& Title, std::wstring& FolderPath)
{
    wchar_t Path[MAX_PATH] = {0};

    BROWSEINFOW BI      = {0};
    BI.hwndOwner        = Owner;
    BI.pidlRoot         = NULL;
    BI.pszDisplayName   = Path;
    BI.lpszTitle        = Title.c_str();
    BI.ulFlags          = BIF_RETURNONLYFSDIRS;
    BI.lpfn             = NULL;
    BI.lParam           = 0;
    BI.iImage           = 0;

    try
    {
        LPITEMIDLIST pIIDL = ::SHBrowseForFolderW(&BI);
        if(pIIDL == NULL)                       { return FALSE;}
        if(!::SHGetPathFromIDListW(pIIDL, Path)){ return FALSE;}
        FolderPath = Path;
        if(FolderPath[FolderPath.size() - 1] != '\\' && FolderPath[FolderPath.size() - 1] != '/')
        {
            FolderPath.append(L"\\");
        }
        return TRUE;
    }
    catch(int)  { }
    catch(void*){ }
    return FALSE;
}
//========Smart::CText==========================================================
void         Smart::CText::Split(const std::wstring& Text, wchar_t             Separator, std::vector<std::wstring>& Array)
{
    Array.clear();
    UINT b = 0;
    for(UINT i = 0, l = Text.size(); i < l; i++)
    {
        if(wchar_t(Text[i]) == Separator)
        {
            if(b == i)
            {
                Array.push_back(L"");
            }
            else
            {
                Array.push_back(Text.substr(b, i - b));
            }
            b = i;
            b++;
        }
    }
    Array.push_back(Text.substr(b));
}
void         Smart::CText::Split(const std::wstring& Text, const std::wstring& Separator, std::vector<std::wstring>& Array)
{
    Array.clear();
    if(Separator.empty()){ Array.push_back(Text); return;}

    UINT b = 0;
    for(UINT i = 0, l = Text.size() - Separator.size() + 1; i < l; i++)
    {
        if(::wcsncmp(&(Text.c_str()[i]), Separator.c_str(), Separator.size()) == 0)
        {
            if(b == i)
            {
                Array.push_back(L"");
            }
            else
            {
                Array.push_back(Text.substr(b, i - b));
            }
            b = i;
            b += Separator.size();
        }
    }
    Array.push_back(Text.substr(b));
}
void         Smart::CText::Split(LPCWSTR Text, WCHAR Separator, LPWSTR Array[], UINT Length)
{
    if(Text == NULL)                 { }
    else if(Array == NULL)           { }
    else if(Length == 0)             { }
    else
    {
        ::memset(Array, 0, sizeof(LPWSTR)*Length);
        LPCWSTR  Find = Text;
        for(UINT i = 0; i < Length; i++)
        {
            LPCWSTR Mark = ::wcschr(Find, Separator);
            if(Mark == NULL)
            {
                UINT Size = ::wcslen(Text) - (Find - Text) + 1;
                Array[i] = (LPWSTR)::malloc(sizeof(wchar_t)*Size);
                ::wcscpy_s(Array[i], Size, Find);
                break;
            }
            else
            {
                UINT Size = Mark - Find + 1;
                Array[i]  = (LPWSTR)::malloc(sizeof(wchar_t)*Size);
                ::wcsncpy_s(Array[i], Size, Find, Mark - Find);
                Find = ++Mark;
            }
        }
    }
}
void         Smart::CText::Split(LPCWSTR Text, LPCWSTR Separator, LPWSTR Array[], UINT Length)
{
    if(Text == NULL)                 { }
    else if(Separator == NULL)       { }
    else if(::wcslen(Separator) == 0){ }
    else if(Array == NULL)           { }
    else if(Length == 0)             { }
    else
    {
        ::memset(Array, 0, sizeof(LPWSTR)*Length);
        LPCWSTR  Find = Text;
        for(UINT i = 0; i < Length; i++)
        {
            LPCWSTR Mark = ::wcsstr(Find, Separator);
            if(Mark == NULL)
            {
                UINT Size = ::wcslen(Text) - (Find - Text) + 1;
                Array[i] = (LPWSTR)::malloc(sizeof(wchar_t)*Size);
                ::wcscpy_s(Array[i], Size, Find);
                break;
            }
            else
            {
                UINT Size = Mark - Find + 1;
                Array[i]  = (LPWSTR)::malloc(sizeof(wchar_t)*Size);
                ::wcsncpy_s(Array[i], Size, Find, Mark - Find);
                Find = Mark + ::wcslen(Separator);
            }
        }
    }
}
void         Smart::CText::FreeSplitArray(LPWSTR Array[], UINT Length)
{
    if(Array != NULL)
    {
        for(UINT i = 0; i < Length; i++)
        {
            if(Array[0] != NULL){ ::free(Array[0]); Array[0] = NULL;}
        }
    }
}
LPWSTR       Smart::CText::Trim(LPWSTR Text, size_t Size)
{
    if(Text == NULL)  { return NULL;}
    else if(Size == 0){ return NULL;}
    else
    {
        ::wcscpy_s(Text, Size, Smart::CText::Trim(std::wstring(Text)).c_str());
        return Text;
    }
}
LPWSTR       Smart::CText::Trim(LPWSTR Text, size_t Size, LPCWSTR Prefix, LPCWSTR Suffix)
{
    if(Text == NULL)  { return NULL;}
    else if(Size == 0){ return NULL;}
    else
    {
        ::wcscpy_s(Text, Size, Smart::CText::Trim(std::wstring(Text), Prefix, Suffix).c_str());
        return Text;
    }
}
std::wstring Smart::CText::Trim(const std::wstring& Text)
{
    if(Text.empty())
    {
        return std::wstring();
    }
    else
    {
        size_t beg = Text.find_first_not_of(L" \a\b\f\n\r\t\v");
        size_t end = Text.find_last_not_of(L" \a\b\f\n\r\t\v");
        if(beg == Text.npos)
        {
            return std::wstring();
        }
        else
        {
            return Text.substr(beg, end - beg +1);//std::wstring(Text, beg, end - beg + 1);
        }
    }
}
std::wstring Smart::CText::Trim(const std::wstring& Text, LPCWSTR Prefix, LPCWSTR Suffix)
{
    std::wstring::size_type beg = Text.find(Prefix) + ::wcslen(Prefix);
    std::wstring::size_type end = Text.rfind(Suffix);
    return Smart::CText::Trim(Text.substr(beg, end - beg));
}
std::string  Smart::CText::Format(LPCSTR Format, ...)
{
    std::string Text;
    if(Format != NULL)
    {
        va_list  ArgList = NULL;
        va_start(ArgList, Format);

        int   Size = ::_vscprintf(Format, ArgList) + 1;
        char* Data = (char*)::malloc(Size);
        ::vsprintf_s(Data, Size, Format, ArgList);
        Text = Data;
        ::free(Data);

        va_end(ArgList);
    }
    return Text;
}
std::wstring Smart::CText::Format(LPCWSTR Format, ...)
{
    std::wstring Text;
    if(Format != NULL)
    {
        va_list  ArgList = NULL;
        va_start(ArgList, Format);

        int      Size = ::_vscwprintf(Format, ArgList) + 1;
        wchar_t* Data = (wchar_t*)::malloc(Size*2);
        ::_vswprintf_s_l(Data, Size, Format, NULL, ArgList);
        Text = Data;
        ::free(Data);

        va_end(ArgList);
    }
    return Text;
}
std::wstring Smart::CText::Format_(LPCWSTR Format, va_list Args)
{
    std::wstring Text;
    if(Format != NULL)
    {
        int      Size = ::_vscwprintf(Format, Args) + 1;
        wchar_t* Data = (wchar_t*)::malloc(Size*2);
        ::_vswprintf_s_l(Data, Size, Format, NULL, Args);
        Text = Data;
        ::free(Data);
    }
    return Text;
}
std::string  Smart::CText::Replace(const std::string&  Text, const std::string&  OldStr, const std::string&  NewStr)
{
    std::string            Str       = Text;
    size_t                 Pos       = 0;
    std::string::size_type NewStrLen = NewStr.length();
    std::string::size_type OldStrLen = OldStr.length();
    while(TRUE)
    {
        Pos = Str.find(OldStr, Pos);
        if(Pos == std::string::npos){ break;}
        Str.replace(Pos, OldStrLen, NewStr);
        Pos += NewStrLen;
    }
    return Str;
}
std::wstring Smart::CText::Replace(const std::wstring& Text, const std::wstring& OldStr, const std::wstring& NewStr)
{
    std::wstring            Str       = Text;
    size_t                  Pos       = 0;
    std::wstring::size_type NewStrLen = NewStr.length();
    std::wstring::size_type OldStrLen = OldStr.length();
    while(TRUE)
    {
        Pos = Str.find(OldStr, Pos);
        if(Pos == std::wstring::npos){ break;}
        Str.replace(Pos, OldStrLen, NewStr);
        Pos += NewStrLen;
    }
    return Str;
}
//------------------------------------------------------------------------------
BOOL         Smart::CText::WideCharToMultiByte(UINT CodePage, DWORD Flags, LPCWSTR WideByte, int WSize, LPSTR MultiByte, int MSize)
{
    if(MultiByte == NULL)                                                           { return FALSE;}
    if(MSize      == 0)                                                             { return FALSE;}
    ::memset(MultiByte, 0, MSize);
    if(WideByte  == NULL)                                                           { return FALSE;}
    if(WSize     == 0)                                                              { return FALSE;}

    int MinSize = ::WideCharToMultiByte(CodePage, Flags, WideByte, WSize, NULL, 0, NULL, NULL);
    if(MinSize <= 0 || MinSize > MSize)                                             { return FALSE;}
    ::WideCharToMultiByte(CodePage, Flags, WideByte, WSize, MultiByte, MSize, NULL, NULL);

    return TRUE;
}
BOOL         Smart::CText::WideCharToMultiByte(UINT CodePage, DWORD Flags, const std::wstring& WideByteStr, std::string& MultiByteStr)
{
    if (WideByteStr.empty()) { MultiByteStr.clear(); return TRUE;}

    LPSTR Data = NULL;
    int   Size = ::WideCharToMultiByte(CodePage, Flags, WideByteStr.c_str(), WideByteStr.size(), NULL, 0, NULL, NULL);
    if(Size == 0)                                                                   { return FALSE;}
    Data  = LPSTR(::calloc(Size, sizeof(char)));
    ::WideCharToMultiByte(CodePage, Flags, WideByteStr.c_str(), WideByteStr.size(), Data, Size, NULL, NULL);
    MultiByteStr = std::string(Data, Size);
    ::free(Data);
    return TRUE;
}
std::string  Smart::CText::WideCharToMultiByte(UINT CodePage, DWORD Flags, const std::wstring& WideByteStr)
{
    std::string MultiByteStr;
    WideCharToMultiByte(CodePage, Flags, WideByteStr, MultiByteStr);
    return MultiByteStr;
}
BOOL         Smart::CText::MultiByteToWideChar(UINT CodePage, DWORD Flags, LPCSTR MultiByte, int MSize, LPWSTR WideByte, int WSize)
{
    if(WideByte  == NULL)                                                   { return FALSE;}
    if(WSize     == 0)                                                      { return FALSE;}
    ::memset(WideByte, 0, WSize*sizeof(wchar_t));
    if(MultiByte == NULL)                                                   { return FALSE;}
    if(MSize      == 0)                                                     { return FALSE;}

    int MinSize = ::MultiByteToWideChar(CodePage, Flags, MultiByte, MSize, NULL, 0);
    if(MinSize <= 0 || MinSize > MSize)                                     { return FALSE;}
    ::MultiByteToWideChar(CodePage, Flags, MultiByte, MSize, WideByte, WSize);
    return TRUE;
}
BOOL         Smart::CText::MultiByteToWideChar(UINT CodePage, DWORD Flags, const std::string& MultiByteStr, std::wstring& WideByteStr)
{
    if (MultiByteStr.empty()) { WideByteStr.clear(); return TRUE;}

    LPWSTR Data = NULL;
    int    Size = ::MultiByteToWideChar(CodePage, Flags, MultiByteStr.c_str(), MultiByteStr.size(), NULL, 0);
    if(Size == 0)                                                                  { return FALSE;}
    Data  = LPWSTR(::calloc(Size, sizeof(wchar_t)));
    ::MultiByteToWideChar(CodePage, Flags, MultiByteStr.c_str(), MultiByteStr.size(), Data, Size);
    WideByteStr = std::wstring(Data, Size);
    ::free(Data);
    return TRUE;
}
std::wstring Smart::CText::MultiByteToWideChar(UINT CodePage, DWORD Flags, const std::string& MultiByteStr)
{
    std::wstring WideByteStr;
    MultiByteToWideChar(CodePage, Flags, MultiByteStr, WideByteStr);
    return WideByteStr;
}
//------------------------------------------------------------------------------
void Smart::CText::BigEndianToLittleEndian(const std::wstring& Sour, std::wstring& Dest)
{
    //std::locale::global(std::locale("chs"));
    //std::wstring_convert<std::codecvt_utf16<wchar_t>, wchar_t> final_try;
    //FaceName = final_try.from_bytes(lpszNameBuf);
    Dest.clear();
    for(auto iter = Sour.begin(); iter != Sour.end(); iter++)
    {
        Dest.push_back(wchar_t(CONVERTBL16(*iter)));
    }
}
void Smart::CText::LittleEndianToBigEndian(const std::wstring& Sour, std::wstring& Dest)
{
    Smart::CText::BigEndianToLittleEndian(Sour, Dest);
}
BOOL Smart::CText::ToUnicode(const std::string& MText, std::wstring& WText)
{
    //GetACP:           Retrieves the current Windows ANSI code page identifier for the operating system.
    //IsValidCodePage:  Determines if a specified code page is valid.
    if (Smart::CText::IsUTF8(MText)){ return Smart::CText::MultiByteToWideChar(CP_UTF8, 0, MText, WText);}//UTF8编码
    BOOL Big5 = Smart::CText::IsBig5(MText);
    BOOL GBK  = Smart::CText::IsGBK(MText);
    if ( Big5 && !GBK)              { return Smart::CText::MultiByteToWideChar(950,    0, MText, WText);}//BIG5编码
    if (!Big5 &&  GBK)              { return Smart::CText::MultiByteToWideChar(936,    0, MText, WText);}//GBK编码
    if (!Big5 && !GBK)              { return Smart::CText::MultiByteToWideChar(CP_ACP, 0, MText, WText);}//ANSI编码
    if ( Big5 &&  GBK)
    {
        switch(::GetSystemDefaultLangID())
        {
        case 0x0404:
        case 0x0C04:
        case 0x1404: return Smart::CText::MultiByteToWideChar(950, 0, MText, WText);   //BIG5编码
        case 0x0804:
        case 0x1004: return Smart::CText::MultiByteToWideChar(936, 0, MText, WText);   //GBK编码
        default:     return Smart::CText::MultiByteToWideChar(CP_ACP, 0, MText, WText);//ANSI编码
        }
    }
    return FALSE;
}
BOOL Smart::CText::ToUnicodeEx(const std::string& MText, std::wstring& WText)
{
    if (MText.size() < 2)
    {
        return ToUnicode(MText, WText);
    }
    if (WORD(*(WORD*)(&MText[0])) == 0xFFFE)
    {
        BigEndianToLittleEndian(std::wstring(LPWSTR(&MText.c_str()[2]), (MText.size() - 2)/2), WText);//(MText.size() - 2)/2自动去除长度不符的多余字符
        return TRUE;
    }
    if (WORD(*(WORD*)(&MText[0])) == 0xFEFF)
    {
        WText = std::wstring(LPWSTR(&MText.c_str()[2]), (MText.size() - 2)/2);                        //(MText.size() - 2)/2自动去除长度不符的多余字符
        return TRUE;
    }
    return ToUnicode(MText, WText);
}
//------------------------------------------------------------------------------
#define UTF8_ONE                        0x00    // binary 0xxxxxxx >> 7
#define UTF8_SPECIAL                    0x02    // binary 10xxxxxx >> 6
#define UTF8_TWO                        0X06    // binary 110xxxxx >> 5
#define UTF8_THREE                      0X0E    // binary 1110xxxx >> 4  
#define UTF8_FOUR                       0X1E    // binary 11110xxx >> 3
#define UTF8_FIVE                       0X3E    // binary 111110xx >> 2
#define UTF8_SIX                        0X7E    // binary 1111110x >> 1
BOOL Smart::CText::IsUTF8SpecialByte(BYTE b)
{
    return (b >> 6) == UTF8_SPECIAL; 
}
BOOL Smart::CText::IsUTF8(const std::string& Text)
{
    BYTE c = 0;    
    for (UINT i = 0, l = Text.size(); i < l; )
    {
        c = (unsigned char)Text[i];

        if (c>>7 == UTF8_ONE)                     {                                           i += 1; continue;}
        else if (c>>5 == UTF8_TWO   && (i+1) < l) { if (IsUTF8SpecialByte(BYTE(Text[i+1]))) { i += 2; continue;} return FALSE;}
        else if (c>>4 == UTF8_THREE && (i+2) < l) { if (IsUTF8SpecialByte(BYTE(Text[i+1])) &&
            IsUTF8SpecialByte(BYTE(Text[i+2]))) { i += 3; continue;} return FALSE;}
        else if (c>>3 == UTF8_FOUR  && (i+3) < l) { if (IsUTF8SpecialByte(BYTE(Text[i+1])) &&
            IsUTF8SpecialByte(BYTE(Text[i+2])) &&
            IsUTF8SpecialByte(BYTE(Text[i+3]))) { i += 4; continue;} return FALSE;}
        else if (c>>3 == UTF8_FIVE  && (i+4) < l) { if (IsUTF8SpecialByte(BYTE(Text[i+1])) &&
            IsUTF8SpecialByte(BYTE(Text[i+2])) &&
            IsUTF8SpecialByte(BYTE(Text[i+3])) &&
            IsUTF8SpecialByte(BYTE(Text[i+4]))) { i += 5; continue;} return FALSE;}
        else if (c>>3 == UTF8_SIX   && (i+5) < l) { if (IsUTF8SpecialByte(BYTE(Text[i+1])) &&
            IsUTF8SpecialByte(BYTE(Text[i+2])) &&
            IsUTF8SpecialByte(BYTE(Text[i+3])) &&
            IsUTF8SpecialByte(BYTE(Text[i+4])) &&
            IsUTF8SpecialByte(BYTE(Text[i+5]))) { i += 6; continue;} return FALSE;}
        else                                      {                                                              return FALSE;}
    }
    return TRUE;
}
#define GB2312_ONE                      UTF8_ONE
#define GB2312_H_B                      0xA1
#define GB2312_H_E                      0xF7
#define GB2312_L_B                      0xA1
#define GB2312_L_E                      0xFE
BOOL Smart::CText::IsGb2312(const std::string& Text)
{
    //空位D7FA-D7FE，未做处理
    for (UINT i = 0, l = Text.size(); i < l; )
    {
        BYTE c = BYTE(Text[i]);
        if (c>>7 == GB2312_ONE)                                   {                                                                    i += 1; continue;}
        else if (c >= GB2312_H_B && c <= GB2312_H_E && (i+1) < l) { BYTE k = BYTE(Text[i+1]); if (k >= GB2312_L_B && k <= GB2312_L_E){ i += 2; continue;} return FALSE;}
        else                                                      {                                                                                       return FALSE;}
    }
    return TRUE;
}
#define BIG5_ONE                        UTF8_ONE
#define BIG5_H_B                        0xA1
#define BIG5_H_E                        0xF9
#define BIG5_L_B1                       0x40
#define BIG5_L_E1                       0x7F
#define BIG5_L_B2                       0xA1
#define BIG5_L_E2                       0xFE
BOOL Smart::CText::IsBig5(const std::string& Text)
{
    for (UINT i = 0, l = Text.size(); i < l; )
    {
        BYTE c = BYTE(Text[i]);
        if (c>>7 == BIG5_ONE)                                 {                                                                   i += 1; continue;}
        else if (c >= BIG5_H_B && c <= BIG5_H_E && (i+1) < l) { BYTE k = BYTE(Text[i+1]); if (k >= BIG5_L_B1 && k <= BIG5_L_E1 ||
            k >= BIG5_L_B2 && k <= BIG5_L_E2) { i += 2; continue;} return FALSE;}
        else                                                  {                                                                                      return FALSE;}
    }
    return TRUE;
}
#define GBK_ONE                         UTF8_ONE
#define GBK_H_B                         0x81
#define GBK_H_E                         0xFE
#define GBK_L_B                         0x40
#define GBK_L_E                         0xFE
BOOL Smart::CText::IsGBK(const std::string& Text)
{
    for (UINT i = 0, l = Text.size(); i < l; )
    {
        BYTE c = BYTE(Text[i]);
        if (c>>7 == GBK_ONE)                                {                                                               i += 1; continue;}
        else if (c >= GBK_H_B && c <= GBK_H_E && (i+1) < l) { BYTE k = BYTE(Text[i+1]); if (k >= GBK_L_B && k <= GBK_L_E) { i += 2; continue;} return FALSE;}
        else                                                {                                                                                  return FALSE;}
    }
    return TRUE;
}
//========Smart::CBlock=========================================================
Smart::CBlock::CBlock(UINT Size)
    : m_Data(NULL)
    , m_Size(0)
    , m_ApplySize(0)
{
    this->Adjust(Size);
}
Smart::CBlock::CBlock(const CBlock& Block)
    : m_Data(NULL)
    , m_Size(0)
    , m_ApplySize(0)
{
    *this = Block;
}
Smart::CBlock::~CBlock(void)
{
    SAFE_FREE(this->m_Data);
    this->m_Size      = 0;
    this->m_ApplySize = 0;
}
void Smart::CBlock::operator= (const CBlock& Block)
{
    this->Clear();
    this->Append(Block.GetData(), Block.GetSize());
}
//------------------------------------------------------------------------------
void Smart::CBlock::Append(PBYTE Data, UINT Size)
{
    this->Apply(this->m_Size + Size);
    ::memcpy_s(&this->m_Data[this->m_Size], Size, Data, Size);
    this->m_Size += Size;
}
void Smart::CBlock::Adjust(UINT Size)
{
    this->Apply(Size);
    this->m_Size = Size;
}
void Smart::CBlock::Apply(UINT Size)
{
    if(Size <= this->m_ApplySize)
    {
    }
    else if(this->m_ApplySize == 0)
    {
        this->m_ApplySize = Size/0x400*0x400 + 0x400;
        this->m_Data      = PBYTE(::calloc(this->m_ApplySize, sizeof(BYTE)));
        assert(this->m_Data);
    }
    else if(this->m_Size == 0)
    {
        SAFE_FREE(this->m_Data);
        this->m_ApplySize = Size/0x400*0x400 + 0x400;
        this->m_Data      = PBYTE(::calloc(this->m_ApplySize, sizeof(BYTE)));
        assert(this->m_Data);
    }
    else
    {
        PBYTE   Data      = this->m_Data;
        this->m_ApplySize = Size/0x400*0x400 + 0x400;
        this->m_Data      = PBYTE(::calloc(this->m_ApplySize, sizeof(BYTE)));
        assert(this->m_Data);
        ::memcpy_s(this->m_Data, this->m_ApplySize, Data, this->m_Size);
        SAFE_FREE(Data);
    }
}
void Smart::CBlock::Clear(void)
{
    this->m_Size = 0;
}
//------------------------------------------------------------------------------
std::string  Smart::CBlock::ToHexA(void)
{
    return Smart::CHex::ToHexA(this->m_Data, this->m_Size);
}
std::wstring Smart::CBlock::ToHexW(void)
{
    return Smart::CHex::ToHexW(this->m_Data, this->m_Size);
}
void         Smart::CBlock::FromHexA(const std::string& Hex)
{
    UINT32 Size = Hex.size()/2 + Hex.size()%2;
    this->Adjust(Size);
    Smart::CHex::ToByte(Hex, this->m_Data, Size);
}
void         Smart::CBlock::FromHexW(const std::wstring& Hex)
{
    UINT32 Size = Hex.size()/2 + Hex.size()%2;
    this->Adjust(Size);
    Smart::CHex::ToByte(Hex, this->m_Data, Size);
}
//========Smart::CTextBlock=====================================================
void Smart::CTextBlock::AppendFormat(LPCSTR Format, ...)
{
    if(Format != NULL)
    {
        va_list  ArgList = NULL;
        va_start(ArgList, Format);

        int Size = ::_vscprintf(Format, ArgList);
        this->Apply(this->m_Size + Size + 1);
        ::vsprintf_s((char*)(&this->m_Data[this->m_Size]), Size + 1, Format, ArgList);
        this->m_Size += Size;

        va_end(ArgList);
    }
}
void Smart::CTextBlock::AppendFormat(LPCWSTR Format, ...)
{
    if(Format != NULL)
    {
        va_list  ArgList = NULL;
        va_start(ArgList, Format);

        int Size = ::_vscwprintf(Format, ArgList);
        this->Apply(this->m_Size + Size*2 + 2);
        ::_vswprintf_s_l((wchar_t*)(&this->m_Data[this->m_Size]), Size + 1, Format, NULL, ArgList);
        this->m_Size += Size*2;

        va_end(ArgList);
    }
}
//========Smart::CThread========================================================
Smart::CThread::CThread(void)
    : m_Cancel(FALSE)
    , m_TimeStamp(0)
    , m_ID(0)
    , m_StartAddress(NULL)
    , m_Times(0)
    , m_Period(0)
    , m_Delay(0)

    , m_Arg(NULL)
{
    this->m_Handle[0] = NULL;
    this->m_Handle[1] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    this->m_Handle[2] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    this->m_Handle[3] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
}
Smart::CThread::~CThread(void)
{
    this->Shutdown();

    SAFE_CLOSEHANDLE(this->m_Handle[0]);
    SAFE_CLOSEHANDLE(this->m_Handle[1]);
    SAFE_CLOSEHANDLE(this->m_Handle[2]);
    SAFE_CLOSEHANDLE(this->m_Handle[3]);
}
BOOL Smart::CThread::Startup(StartAddress Address, LPVOID Arg, UINT64 Times, UINT32 Period, UINT32 Delay)
{
    this->Shutdown();
    Smart::CSection::CAuto Section(this->m_Section);
    if(this->m_Cancel)           { return FALSE;}//线程正在关闭
    if(Address           == NULL){ return FALSE;}//函数地址为空
    if(this->m_Handle[0] != NULL){ return FALSE;}//线程已经启动

    this->m_TimeStamp       = Smart::CDateTime::GetNow().GetTimeStamp(FALSE);
    this->m_StartAddress    = Address;
    this->m_Arg             = Arg;
    this->m_Times           = max(Times, 1);
    this->m_Period          = Period;
    this->m_Delay           = Delay;
    this->m_Handle[0]       = (HANDLE)::_beginthreadex(NULL, 0, Smart::CThread::__ThreadProcess, this, 0, &this->m_ID);//线程启动
    if(this->m_Handle[0] == NULL)                                                                      { return FALSE;}//线程启动失败
    if(this->m_Handle[0] == INVALID_HANDLE_VALUE)                                                      { return FALSE;}//线程启动失败
    SASectionL;

    if(::WaitForMultipleObjects(3, this->m_Handle, FALSE, INFINITE) - WAIT_OBJECT_0 != 1)//线程启动失败
    {
        SAFE_CLOSEHANDLE(this->m_Handle[0]);
        this->m_ID        = 0;
        return FALSE;
    }
    else                                                                                 //线程启动成功
    {
        return TRUE;
    }
}
void Smart::CThread::Shutdown(BOOL Wait)
{
    SASectionI;
    if(this->m_Handle[0] == NULL){ return;}         //线程未执行，或已经结束执行
    if(this->m_Cancel)           { return;}         //现在取消正在执行
    this->m_Cancel = TRUE;
    ::SetEvent(this->m_Handle[3]);                  //发送延迟结束时间
    SASectionL;
    if(Wait)
    {
        if(::WaitForSingleObject(this->m_Handle[0], 10000) != WAIT_OBJECT_0)
        {
            ::TerminateThread(this->m_Handle[0], 0);
            ::WaitForSingleObject(this->m_Handle[0], INFINITE);
        }

        SASectionE;
        this->m_Cancel = FALSE;
        SAFE_CLOSEHANDLE(this->m_Handle[0]);
        ::ResetEvent(this->m_Handle[1]);
        ::ResetEvent(this->m_Handle[2]);
        ::ResetEvent(this->m_Handle[3]);
        this->m_ID = 0;
    }
}
void Smart::CThread::WaitforComplete(void)
{
    SASectionI;
    if(this->m_Handle[0] == NULL){ return;}
    SASectionL;

    ::WaitForSingleObject(this->m_Handle[0], INFINITE);

    SASectionE;
    this->m_Cancel = FALSE;
    SAFE_CLOSEHANDLE(this->m_Handle[0]);
    ::ResetEvent(this->m_Handle[1]);
    ::ResetEvent(this->m_Handle[2]);
    ::ResetEvent(this->m_Handle[3]);
    this->m_ID = 0;
}
//------------------------------------------------------------------------------
BOOL     Smart::CThread::IsCancel(void)
{
    SASectionI;
    return this->m_Cancel;
}
BOOL     Smart::CThread::IsBusy(void)
{
    SASectionI;
    if(this->m_Handle[0] == NULL){ return FALSE;}
    SASectionL;
    return ::WaitForSingleObject(this->m_Handle[0], 100) != WAIT_OBJECT_0;
}
BOOL     Smart::CThread::IsIdle(void)
{
    return !this->IsBusy();
}
UINT     Smart::CThread::__ThreadProcess(LPVOID Thread)
{
    OutputLog(LLT_Trace, L"线程启动！");
    if(Thread == NULL)               { OutputLog(LLT_Trace, L"线程退出(1)！"); ::_endthreadex(0); return 0;}//线程传入参数为空
    Smart::CThread* pThis = static_cast<Smart::CThread*>(Thread);
    if(pThis == NULL)                { OutputLog(LLT_Trace, L"线程退出(2)！"); ::_endthreadex(0); return 0;}//线程传入参数错误
    if(pThis->m_StartAddress == NULL){ OutputLog(LLT_Trace, L"线程退出(3)！"); ::_endthreadex(0); return 0;}//线程执行地址为空
    ::SetEvent(pThis->m_Handle[1]);                                                                         //发送成功事件

    if(pThis->m_Delay == 0 || WAIT_OBJECT_0 != ::WaitForSingleObject(pThis->m_Handle[3], pThis->m_Delay))
    {
        SASectionI_(pThis);
        while(!pThis->m_Cancel && pThis->m_Times-- > 0)
        {
            DWORD Begin = ::timeGetTime();
            SASectionL;

            pThis->m_StartAddress(pThis);

            SASectionE;
#if FALSE
            //执行周期Sleep，能够精确执行周期时间，但是取消线程时需要等待时间最大为周期时间
            DWORD End   = ::timeGetTime();
            if(End < Begin)                        { }                                                                                    //计数器超过最大计数
            else if(End - Begin >= pThis->m_Period){ }                                                                                    //实际执行时间大于执行周期
            else                                   { DWORD Dura = pThis->m_Period - End + Begin;  SASectionL; ::Sleep(Dura); SASectionE; }//执行周期暂停
#else
            //执行短Sleep，周期精度取决于短Sleep时间，时间越小精度越高，资源占用越高，取消线程需要等待时间最大为短Sleep时间
            while(!pThis->m_Cancel)
            {
                DWORD End = ::timeGetTime();
                if(End < Begin)                       { break; }                              //计数器超过最大计数
                else if(End - Begin > pThis->m_Period){ break; }                              //实际执行时间大于执行周期
                else                                  { SASectionL; ::Sleep(40); SASectionE; }//执行短暂停
            }
#endif
        }
    }

    OutputLog(LLT_Trace, L"线程退出(0)！");
    ::_endthreadex(0);
    return 0;
}
//------------------------------------------------------------------------------
BOOL Smart::CThread::StartupGroup(INT32 Size, StartAddress Address, LPVOID Arg)
{
    std::list<Smart::CThread*> List;
    for(INT32 i = 0; i < Size; i++)
    {
        Smart::CThread* pThread = new Smart::CThread();
        List.push_back(pThread);
        pThread->Startup(Address, Arg);
    }
    while(!List.empty())
    {
        List.back()->WaitforComplete();
        delete List.back();
        List.pop_back();
    }
    return TRUE;
}
//========Smart::CPath==========================================================
BOOL Smart::CPath::FormatPath(std::wstring& Path)
{
    Path = Smart::CText::Trim(Path);
    if(Path.empty()){ return FALSE;}
    while(Path.find(L"/")    != std::wstring::npos){ Path = Smart::CText::Replace(Path, L"/", L"\\");}
    while(Path.find(L"\\\\") != std::wstring::npos){ Path = Smart::CText::Replace(Path, L"\\\\", L"\\");}  
    return TRUE;
}
BOOL Smart::CPath::PathIsDrive(const std::wstring& Path)
{
    //DWORD A = ::GetLogicalDrives();
    //DWORD B = ::GetLogicalDriveStringsW(MAX_PATH, Drive);
    if(Path.size() != 3)                   { return FALSE; }
    if(Path[1] != L':')                    { return FALSE; }
    if(Path[2] != L'\\' && Path[2] != L'/'){ return FALSE; }
    DWORD Drive = 1 << ((Path[0] >= 'a') ? (Path[0] - 'a') : (Path[0] - 'A'));
    return ::GetLogicalDrives() & Drive;
}
BOOL Smart::CPath::PathIsDirectory(const std::wstring& Path)
{
    if(Smart::CPath::PathIsDrive(Path)){ return TRUE; }
    WIN32_FIND_DATAW FindFileData = {0};
    HANDLE           FindHandle   = ::FindFirstFileW(Path.c_str(), &FindFileData);
    if(FindHandle != INVALID_HANDLE_VALUE)
    {
        if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            ::FindClose(FindHandle);
            return TRUE;
        }
        ::FindClose(FindHandle);
    }
    return FALSE;
}
BOOL Smart::CPath::PathIsFile(const std::wstring& Path)
{
    WIN32_FIND_DATAW FindFileData = {0};
    HANDLE           FindHandle   = ::FindFirstFileW(Path.c_str(), &FindFileData);
    if(FindHandle != INVALID_HANDLE_VALUE)
    {
        if(!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
        {
            ::FindClose(FindHandle);
            return TRUE;
        }
        ::FindClose(FindHandle);
    }
    return FALSE;
}
BOOL Smart::CPath::PathIsExists(const std::wstring& Path)
{
    if(::_waccess(Path.c_str(), 0) != 0)
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
BOOL Smart::CPath::PathIsExists(const std::wstring& Path, UINT32 FileSize, BOOL& FSCheck)
{
    if(::_waccess(Path.c_str(), 0) != 0)
    {
        return FALSE;
    }
    else
    {
        long Size = 0;
        FSCheck = FALSE;
        if(Smart::CFile::GetFileSize(Path, Size)){ if(FileSize == UINT(Size)){ FSCheck = TRUE;}}
        return TRUE;
    }
}
//------------------------------------------------------------------------------
BOOL Smart::CPath::GetFileDir(const std::wstring& Path, std::wstring& Dir)
{
    std::wstring Folder = Path;
    if(Folder.empty())                               { return FALSE;}
    while(Folder.find(L"/")    != std::wstring::npos){ Folder = Smart::CText::Replace(Folder, L"/", L"\\");}
    while(Folder.find(L"\\\\") != std::wstring::npos){ Folder = Smart::CText::Replace(Folder, L"\\\\", L"\\");}
    if(Folder[Folder.size() - 1] == '\\')            { Folder = Folder.substr(0, Folder.rfind(L"\\"));}

    std::wstring::size_type Pos = Folder.rfind(L"\\");
    if(Pos == std::wstring::npos)
    {
        return FALSE;
    }
    else
    {
        Dir = Folder.substr(0, Pos + 1);
        return TRUE;
    }
}
BOOL Smart::CPath::GetFileName(const std::wstring& Path, std::wstring& FileName)
{
    std::wstring Folder = Path;
    if(Folder.empty())                               { return FALSE; }
    while(Folder.find(L"/")    != std::wstring::npos){ Folder = Smart::CText::Replace(Folder, L"/", L"\\"); }
    while(Folder.find(L"\\\\") != std::wstring::npos){ Folder = Smart::CText::Replace(Folder, L"\\\\", L"\\"); }
    if(Smart::CPath::PathIsDrive(Folder))            { return FALSE; }//路径为驱动器路径
    if(Folder[Folder.size() - 1] == '\\')            { Folder = Folder.substr(0, Folder.rfind(L"\\")); }

    std::wstring::size_type Pos = Folder.rfind(L"\\");
    if(Pos == std::wstring::npos)
    {
        return FALSE;
        //FileName = Folder.substr(0, 1);
    }
    else
    {
        FileName = Folder.substr(Pos + 1, Folder.size() - Pos - 1);
        return TRUE;
    }
}
BOOL Smart::CPath::GetFileNameNoExtension(const std::wstring& Path, std::wstring& FileName)
{
    if(!Smart::CPath::GetFileName(Path, FileName))
    {
        return FALSE;
    }
    else
    {
        std::wstring::size_type Pos = FileName.rfind(L".");
        if(Pos != std::wstring::npos)
        {
            FileName = FileName.substr(0, Pos);
        }
        return TRUE;
    }
}
BOOL Smart::CPath::GetFileExtension(const std::wstring& Path, std::wstring& FileExtension)
{
    if(!Smart::CPath::GetFileName(Path, FileExtension))
    {
        return FALSE;
    }
    else
    {
        std::wstring::size_type Pos = FileExtension.rfind(L".");
        if(Pos == std::wstring::npos)
        {
            FileExtension = L"";
        }
        else
        {
            FileExtension = FileExtension.substr(Pos + 1);
        }
        return TRUE;
    }
}
BOOL Smart::CPath::PathHasExtension(const std::wstring& Path, const std::wstring& Extension)
{
    std::wstring FileExtension;
    if(!Smart::CPath::GetFileExtension(Path, FileExtension))     { return FALSE; }
    if(::_wcsicmp(FileExtension.c_str(), Extension.c_str()) != 0){ return FALSE; }
    return TRUE;
}
//------------------------------------------------------------------------------
BOOL Smart::CPath::CreateDir(const std::string& Path)
{
    return Smart::CPath::CreateDir(Smart::CText::ANSI_to_UTF16(Path));
}
BOOL Smart::CPath::CreateDir(const std::wstring& Path)
{
    std::wstring Dir = Path;
    if(Dir.empty())                               { return FALSE;}
    while(Dir.find(L"/")    != std::wstring::npos){ Dir = Smart::CText::Replace(Dir, L"/", L"\\");}
    while(Dir.find(L"\\\\") != std::wstring::npos){ Dir = Smart::CText::Replace(Dir, L"\\\\", L"\\");}
    if(Dir[Dir.size() - 1] != '\\')               { Dir.append(L"\\");}

    std::vector<std::wstring> vPath;  //存放每一层目录字符串
    for(int i = int(Dir.size()) - 1; i >= 0; i--)
    {
        if(Dir[i] == '\\')
        {
            std::wstring tPath = Dir.substr(0, i);
            if(Smart::CPath::PathIsExists(tPath)){ break;}
            else
            {
                vPath.insert(vPath.begin(), tPath);
            }
        }
    }

    for(auto iter = vPath.begin(); iter != vPath.end(); iter++) 
    {
        if(Smart::CPath::PathIsExists(*iter))          { }
        else if(::CreateDirectory(iter->c_str(), NULL)){ }
        else                                           { return FALSE;}
    }
    return TRUE;
}
BOOL Smart::CPath::CreateDirFromFilePath(const std::string& Path)
{
    return Smart::CPath::CreateDirFromFilePath(Smart::CText::ANSI_to_UTF16(Path));
}
BOOL Smart::CPath::CreateDirFromFilePath(const std::wstring& Path)
{
    size_t Pos1 = Path.rfind('\\');
    size_t Pos2 = Path.rfind('/');

    if(Pos1 == std::wstring::npos && Pos2 == std::wstring::npos){ return FALSE;}
    if(Pos1 == std::wstring::npos){ return Smart::CPath::CreateDir(Path.substr(0, Pos2));}
    if(Pos2 == std::wstring::npos){ return Smart::CPath::CreateDir(Path.substr(0, Pos1));}
    return Smart::CPath::CreateDir(Path.substr(0, max(Pos1, Pos2)));
}
//------------------------------------------------------------------------------
BOOL Smart::CPath::MoveFile(const std::wstring& Sour, const std::wstring& Dest)
{
    return ::MoveFileEx(Sour.c_str(), Dest.c_str(), MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING);
}
BOOL Smart::CPath::DeleteFile(const std::wstring& Dest)
{
    return ::DeleteFile(Dest.c_str());
}
//------------------------------------------------------------------------------
std::wstring Smart::CPath::FilterIllegal(const std::wstring& FileName)
{
    std::wstring Name = std::wstring(FileName.c_str());
    Name = Smart::CText::Replace(Name, L"\\", L"");
    Name = Smart::CText::Replace(Name, L"/",  L"");
    Name = Smart::CText::Replace(Name, L":",  L"");
    Name = Smart::CText::Replace(Name, L"*",  L"");
    Name = Smart::CText::Replace(Name, L"?",  L"");
    Name = Smart::CText::Replace(Name, L"\"", L"");
    Name = Smart::CText::Replace(Name, L"<",  L"");
    Name = Smart::CText::Replace(Name, L">",  L"");
    Name = Smart::CText::Replace(Name, L"|",  L"");
    return Name;
}
std::wstring Smart::CPath::GetFilePathWithoutSame(std::wstring& FilePath)
{
    if(::_waccess(FilePath.c_str(), 0) != 0){ return FilePath;}

    wchar_t      Name[MAX_PATH]      = {0};
    std::wstring FileExtension;
    if(Smart::CPath::GetFileExtension(FilePath, FileExtension))
    {
        ::wcsncpy_s(Name, MAX_PATH, FilePath.c_str(), FilePath.length() - FileExtension.size() - 1);
        for(int i = 1; i < 0xFFFF; i++)
        {
            FilePath.clear();
            FilePath.append(Smart::CText::Format(L"%s(%d).%s", Name, i, FileExtension.c_str()));
            if(::_waccess(FilePath.c_str(), 0) != 0){ return FilePath;}
        }
    }
    else
    {
        ::wcscpy_s(Name, MAX_PATH, FilePath.c_str());
        for(int i = 1; i < 0xFFFF; i++)
        {
            FilePath.clear();
            FilePath.append(Smart::CText::Format(L"%s(%d)", Name, i));
            if(::_waccess(FilePath.c_str(), 0) != 0){ return FilePath;}
        }
    }
    return std::wstring(L"");
}
std::wstring Smart::CPath::GetFilePathNorepeat(const std::wstring& FilePath)
{
    std::wstring TempPath = FilePath;
    if(Smart::CPath::PathIsExists(TempPath))
    {
        std::wstring FileName, Extension;
        if((Extension = Smart::CPath::GetFileExtension(TempPath)).empty())
        {
            FileName = TempPath;
            for(int i = 1; i < 0xFFFF; i++)
            {
                TempPath = Smart::CText::Format(L"%s(%d)", FileName.c_str(), i);
                if(!Smart::CPath::PathIsExists(TempPath)){ break;}
            }
        }
        else
        {
            FileName = std::wstring(TempPath.c_str(), TempPath.size() - Extension.size() - 1);
            for(int i = 1; i < 0xFFFF; i++)
            {
                TempPath = Smart::CText::Format(L"%s(%d).%s", FileName.c_str(), i, Extension.c_str());
                if(!Smart::CPath::PathIsExists(TempPath)){ break;}
            }
        }
    }
    return TempPath;
}
//========Smart::CFile==========================================================
Smart::CFile::CFile(void)
    : m_File(NULL)
    , m_Pos(UINT(-1))
{
}
Smart::CFile::~CFile(void)
{
    SAFE_CLOSEHANDLE(this->m_File);
}
BOOL Smart::CFile::IsOpen(void)
{
    return this->m_File != NULL;
}
BOOL Smart::CFile::Open(const std::wstring& FilePath, DWORD DesiredAccess, DWORD ShareMode, BOOL Create)
{
    if(::_waccess(FilePath.c_str(), 0) == 0)
    {
        this->m_File = ::CreateFileW(FilePath.c_str(), DesiredAccess, ShareMode, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    else if(Create)
    {
        this->m_File = ::CreateFileW(FilePath.c_str(), DesiredAccess, ShareMode, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    }
    else
    {
        return FALSE;
    }
    if(this->m_File == INVALID_HANDLE_VALUE)
    {
        this->m_File = NULL;
        return FALSE;
    }
    this->m_Pos = UINT(-1);
    return TRUE;
}
void Smart::CFile::Close(void)
{
    SAFE_CLOSEHANDLE(this->m_File);
    this->m_Pos = UINT(-1);
}
BOOL Smart::CFile::IsEmpty(void)
{
    return ::GetFileSize(this->m_File, NULL) == 0;
}
BOOL Smart::CFile::Read(PVOID Data, UINT Size, PUINT RSize)
{
    DWORD ReadSize = 0;
    BOOL  ReadRe   = ::ReadFile(this->m_File, Data, Size, &ReadSize, NULL);
    if(RSize != NULL){ *RSize = ReadSize; }
    return ReadRe;
}
void Smart::CFile::Clear(void)
{
    ::SetFilePointer(this->m_File, 0, NULL, FILE_BEGIN);
    ::SetEndOfFile(this->m_File);
}
BOOL Smart::CFile::Write(BOMType BOMType)
{
    switch(BOMType)
    {
    case BOMType_NONE:    return TRUE;
    case BOMType_UTF8:    return this->Write((PBYTE)TEXT_BOM_UTF8, 3);
    case BOMType_UTF16LE: return this->Write((PBYTE)TEXT_BOM_UTF16LE, 2);
    case BOMType_UTF16BE: return this->Write((PBYTE)TEXT_BOM_UTF16BE, 2);
    case BOMType_UTF32LE: return this->Write((PBYTE)TEXT_BOM_UTF32LE, 4);
    case BOMType_UTF32BE: return this->Write((PBYTE)TEXT_BOM_UTF32BE, 4);
    default:              return FALSE;
    }
}
BOOL Smart::CFile::Write(const PBYTE Data, UINT Size)
{
    DWORD  WriteSize = 0;
    if(this->m_File == NULL)                                    { return FALSE;}
    if(this->m_File == INVALID_HANDLE_VALUE)                    { return FALSE;}
    if(Data == NULL || Size == 0)                               { return FALSE;}
    if(!::WriteFile(this->m_File, Data, Size, &WriteSize, NULL)){ return FALSE;}
    if(Size != WriteSize)                                       { return FALSE;}
    return TRUE;
}
BOOL Smart::CFile::Write(LPCWSTR Format, ...)
{
    if(Format != NULL)
    {
        va_list  ArgList = NULL;
        va_start(ArgList, Format);

        int      Size   = ::_vscwprintf(Format, ArgList) + 1;
        wchar_t* Data   = (wchar_t*)::malloc(Size*2);
        ::_vswprintf_s_l(Data, Size, Format, NULL, ArgList);
        BOOL     Status = this->Write((PBYTE)Data, (Size - 1)*2);
        ::free(Data);

        va_end(ArgList);
        return Status;
    }
    return FALSE;
}
//------------------------------------------------------------------------------
void Smart::CFile::SavePos(void)
{
    this->m_Pos = this->GetPos();
}
void Smart::CFile::RestorePos(void)
{
    if(this->m_Pos != UINT(-1))
    {
        this->Seek(FILE_BEGIN, this->m_Pos);
    }
}
UINT Smart::CFile::GetPos(void)
{
    return ::SetFilePointer(this->m_File, 0, NULL, FILE_CURRENT);
}
void Smart::CFile::Seek(DWORD MoveMethod, long Move)
{
    ::SetFilePointer(this->m_File, Move, NULL, MoveMethod);
}
void Smart::CFile::SeekBegin(void)
{
    this->Seek(FILE_BEGIN, 0);
}
void Smart::CFile::SeekEnd(void)
{
    this->Seek(FILE_END, 0);
}
//------------------------------------------------------------------------------
BOOL Smart::CFile::FindFile(const std::wstring& FileFilter, std::list<std::wstring>& FileList, BOOL /*Sub*/)
{
    std::wstring     RootDir      = Smart::CPath::GetFileDir(FileFilter);
    WIN32_FIND_DATAW FindFileData = {0};
    HANDLE           FindHandle   = ::FindFirstFileW(FileFilter.c_str(), &FindFileData);
    if(FindHandle != INVALID_HANDLE_VALUE)
    {
        do
        {
            if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
            }
            else
            {
                FileList.push_back(Smart::CText::Format(L"%s%s", RootDir.c_str(), FindFileData.cFileName));
            }
        }while(::FindNextFileW(FindHandle, &FindFileData));
        ::FindClose(FindHandle);
    }
    return TRUE;
}
BOOL Smart::CFile::GetData(const std::wstring& FilePath, BYTE** Data, DWORD* Size)
{
    assert(Data);
    assert(Size);
    *Data  = NULL;
    *Size = 0;

    PBYTE _Data = NULL;
    DWORD _Size = 0;

    HANDLE File = ::CreateFile(FilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (File == NULL)                                            { return FALSE;}                                    //打开文件失败
    if (File == INVALID_HANDLE_VALUE)                            { return FALSE;}                                    //打开文件失败
    if ((_Size = ::GetFileSize(File, NULL)) == INVALID_FILE_SIZE){ ::CloseHandle(File); return FALSE;}               //获取文件大小失败
    if ((_Data  = PBYTE(::malloc(_Size))) == NULL)               { ::CloseHandle(File); return FALSE;}               //申请内存失败
    if (!::ReadFile(File, _Data, _Size, &_Size, NULL))           { ::free(Data); ::CloseHandle(File); return FALSE;} //读文件失败
    ::CloseHandle(File);

    *Data  = _Data;
    *Size = _Size;
    return TRUE;
}
void Smart::CFile::ReleaseData(BYTE** Data)
{
    if(Data != NULL)
    {
        SAFE_FREE(*Data);
    }
}
//------------------------------------------------------------------------------
BOOL Smart::CFile::ReadFile(const std::wstring& Path, Smart::CBlock* Block)
{
    Block->Clear();

    LARGE_INTEGER FileSize = {0};
    HANDLE File = ::CreateFile(Path.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if(File == NULL)                                                                  { return FALSE;}
    if(File == INVALID_HANDLE_VALUE)                                                  { return FALSE;}
    if(!::GetFileSizeEx(File, &FileSize))                                             { SAFE_CLOSEHANDLE(File); return FALSE;}
    if(FileSize.HighPart > 0)                                                         { SAFE_CLOSEHANDLE(File); return FALSE;}
    Block->Adjust(FileSize.LowPart);
    if(!::ReadFile(File, Block->GetData(), Block->GetSize(), &FileSize.LowPart, NULL)){ SAFE_CLOSEHANDLE(File); return FALSE;}

    SAFE_CLOSEHANDLE(File);
    return TRUE;
}
BOOL Smart::CFile::ReadFile(const std::wstring& Path, std::wstring&  Text)
{
    Smart::CTextBlock Block;
    if(!Smart::CFile::ReadFile(Path, &Block))             { return FALSE;}
    if(!Smart::CText::ToUnicodeEx(Block.GetTextA(), Text)){ return FALSE;}
    return TRUE;
}
BOOL Smart::CFile::WriteFile(const std::wstring& Path, const Smart::CBlock& Block)
{
    Smart::CPath::CreateDirFromFilePath(Path);

    DWORD  Size = 0;
    HANDLE File = ::CreateFile(Path.c_str(), GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(File == NULL)                                                            { return FALSE;}
    if(File == INVALID_HANDLE_VALUE)                                            { return FALSE;}
    if(!::WriteFile(File, Block.GetData(), Block.GetSize(), &Size, NULL))       { SAFE_CLOSEHANDLE(File); return FALSE;}
    if(Block.GetSize() != Size)                                                 { SAFE_CLOSEHANDLE(File); return FALSE;}

    ::CloseHandle(File);
    return TRUE;
}
BOOL Smart::CFile::WriteFile(const std::wstring& Path, const std::wstring&  Text)
{
    Smart::CPath::CreateDirFromFilePath(Path);

    DWORD  Size = 0;
    HANDLE File = ::CreateFile(Path.c_str(), GENERIC_READ|GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(File == NULL)                                                { return FALSE;}
    if(File == INVALID_HANDLE_VALUE)                                { return FALSE;}
    if(!::WriteFile(File, TEXT_BOM_UTF16LE, 2, &Size, NULL))        { SAFE_CLOSEHANDLE(File); return FALSE;}
    if(!::WriteFile(File, Text.c_str(), Text.size()*2, &Size, NULL)){ SAFE_CLOSEHANDLE(File); return FALSE;}
    if(Text.size()*2 != Size)                                       { SAFE_CLOSEHANDLE(File); return FALSE;}

    ::CloseHandle(File);
    return TRUE;
}
//------------------------------------------------------------------------------
BOOL Smart::CFile::WriteFile(LPCWSTR FilePath, void* Data, DWORD Size)
{
    if(Data == NULL || Size == 0){ return FALSE;}

    DWORD  WriteSize = 0;
    HANDLE File = ::CreateFile(FilePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(File == INVALID_HANDLE_VALUE)                                { return FALSE;}
    else if(::SetFilePointer(File, 0, NULL, FILE_END) == 0xFFFFFFFF){ ::CloseHandle(File); return FALSE;}
    else if(!::WriteFile(File, Data, Size, &WriteSize, NULL))       { ::CloseHandle(File); return FALSE;}
    else if(Size != WriteSize)                                      { ::CloseHandle(File); return FALSE;}
    else                                                            { ::CloseHandle(File); return TRUE; }
}
BOOL Smart::CFile::WriteFile(LPCWSTR FilePath, LPCWSTR Data)
{
    return Smart::CFile::WriteFile(FilePath, Data, ::wcslen(Data));
}
BOOL Smart::CFile::WriteFile(LPCWSTR FilePath, LPCWSTR Data, DWORD Size)
{
    HANDLE File = ::CreateFile(FilePath, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(File == INVALID_HANDLE_VALUE){ return FALSE;}
    else
    {
        DWORD Pos = ::SetFilePointer(File, 0, NULL, FILE_END);
        if(Pos == 0xFFFFFFFF){                                                                   ::CloseHandle(File); return FALSE;}
        else if(Pos == 0)    { if(!Smart::CFile::WriteFile(File, TEXT_BOM_UTF16LE, 2))         { ::CloseHandle(File); return FALSE;}}
        else                 { if(!Smart::CFile::WriteFile(File, L"\r\n", ::wcslen(L"\r\n")*2)){ ::CloseHandle(File); return FALSE;}}

        std::wstring SystemTime = Smart::CDateTime::GetNow().ToString();
        Smart::CFile::WriteFile(File, SystemTime.c_str(), SystemTime.size()*2);
        Smart::CFile::WriteFile(File, L"\t", ::wcslen(L"\t")*2);
        Smart::CFile::WriteFile(File, Data, Size*2);
        ::CloseHandle(File);
        return TRUE;
    }
}
BOOL Smart::CFile::WriteFile(HANDLE File, const void* Data, DWORD Size)
{
    DWORD  WriteSize = 0;
    if(File == INVALID_HANDLE_VALUE)                    { return FALSE;}
    else if(Data == NULL || Size == 0)                       { return FALSE;}
    else if(!::WriteFile(File, Data, Size, &WriteSize, NULL)){ return FALSE;}
    else if(Size != WriteSize)                               { return FALSE;}
    else                                                     { return TRUE; }
}
BOOL Smart::CFile::WriteFile(HANDLE File, const wchar_t* Data)
{
    return Smart::CFile::WriteFile(File, Data, ::wcslen(Data)*sizeof(wchar_t));
}
//-----------------------------------------------------------------------------
BOOL   Smart::CFile::GetFileSize(const std::string& FilePath, long& FileSize)
{
    struct _stat Info;
    if(::_stat(FilePath.c_str() ,&Info) != 0)
    {
        return FALSE;
    }
    else
    {
        FileSize = Info.st_size;
        return TRUE;
    }
}
BOOL   Smart::CFile::GetFileSize(const std::wstring& FilePath, long& FileSize)
{
    struct _stat Info;
    if(::_wstat(FilePath.c_str(), &Info) != 0)
    {
        return FALSE;
    }
    else
    {
        FileSize = Info.st_size;
        return TRUE;
    }
}
UINT32 Smart::CFile::GetFileSize(const std::wstring& FilePath)
{
    struct _stat Info;
    if(::_wstat(FilePath.c_str(), &Info) != 0)
    {
        return 0;
    }
    else
    {
        return UINT32(Info.st_size);
    }
}
//========Smart::Processor::CProcessor==========================================
DWORD Smart::Processor::CProcessor::GetProcessorNumber(void)
{
    SYSTEM_INFO SystemInfo;
    ::GetSystemInfo(&SystemInfo);
    return SystemInfo.dwNumberOfProcessors;
}
DWORD Smart::Processor::CProcessor::SetThreadAffinityMask(HANDLE Thread, DWORD_PTR ThreadAffinityMask)
{
    DWORD ProcessorNumber = Smart::Processor::CProcessor::GetProcessorNumber();
    if(ThreadAffinityMask <= ProcessorNumber)
    {
        return ::SetThreadAffinityMask(Thread, ThreadAffinityMask);
    }
    else
    {
        return ::SetThreadAffinityMask(Thread, ProcessorNumber);
    }
}
BOOL  Smart::Processor::CProcessor::SetThreadPriority(HANDLE Thread, int Priority)
{
    return ::SetThreadPriority(Thread, Priority);
}
//========Smart::Processor::CPerformance========================================
Smart::Processor::CPerformance::CPerformance(void)
{
    this->m_Frequency.QuadPart = 0;
    ::QueryPerformanceFrequency(&this->m_Frequency); 
}
Smart::Processor::CPerformance::~CPerformance(void)
{
    this->m_Counter.clear();
}
void     Smart::Processor::CPerformance::Reset(void)
{
    this->m_Counter.clear();
}
BOOL     Smart::Processor::CPerformance::Tick(void)
{
    LARGE_INTEGER Count = {0};
    if(!::QueryPerformanceCounter(&Count))
    {
        return FALSE;
    }
    else
    {
        this->m_Counter.push_back(Count);
        return TRUE;
    }
}
LONGLONG Smart::Processor::CPerformance::Time(DWORD i)
{
    if(i == 0)
    {
        return 0;
    }
    else if(this->m_Counter.size() <= 1)
    {
        return 0;
    }
    else if(i >= this->m_Counter.size())
    {
        return 0;
    }
    else if(this->m_Frequency.QuadPart == 0)
    {
        return 0;
    }
    else
    {
        return ((this->m_Counter[i].QuadPart - this->m_Counter[i - 1].QuadPart)*1000)/this->m_Frequency.QuadPart;
    }
}

#pragma once
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <ObjBase.h>
#include <WinBase.h>
#include <Windows.h>
#include <wchar.h>
#include <MMSystem.h>
#include <process.h>
#pragma region 基本定义
//namespace Smart
//{
//#define pi 3.14159265
//#define e  2.718281828
//#define FloatPrecision  0.000001
//#define DoublePrecision 0.00000001
//};
#define MAKEUINT16(l, h)    ((UINT16)(((UINT16)(((UINT8)(l)) & 0xff)) | ((UINT16)(((UINT8)(h)) & 0xff)) << 8))
#define MAKEUINT32(l, h)    ((UINT32)(((UINT32)(((UINT16)(l)) & 0xffff)) | ((UINT32)(((UINT16)(h)) & 0xffff)) << 16))
#define MAKEUINT64(l, h)    ((UINT64)(((UINT64)(((UINT32)(l)) & 0xffffffff)) | ((UINT64)(((UINT32)(h)) & 0xffffffff)) << 32))
#define LO8(U16)            ((UINT8)(((UINT16)(U16)) & 0xff))
#define HI8(U16)            ((UINT8)((((UINT16)(U16)) >> 8) & 0xff))
#define LO16(U32)           ((UINT16)(((UINT32)(U32)) & 0xffff))
#define HI16(U32)           ((UINT16)((((UINT32)(U32)) >> 16) & 0xffff))
#define LO32(U64)           ((UINT32)(((UINT64)(U64)) & 0xffffffff))
#define HI32(U64)           ((UINT32)((((UINT64)(U64)) >> 32) & 0xffffffff))
#define CONVERTBL16(U16)    ((UINT16)((LO8(U16) << 8) + HI8(U16)))
#define CONVERTBL32(U32)    ((UINT32)((CONVERTBL16(LO16(U32)) << 16) + CONVERTBL16(HI16(U32))))
#define CONVERTBL64(U64)    ((UINT64)((CONVERTBL32(LO32(U64)) << 32) + CONVERTBL32(HI32(U64))))

static RECT WINAPI MakeRect(int l, int t, int r, int b){ RECT Rect = { l, t, r, b}; return Rect; };
#define RECTWIDTH(rt)                   (rt.right - rt.left)
#define RECTHEIGHT(rt)                  (rt.bottom - rt.top)
#define MAKERECT(l, t, r, b)            MakeRect(l, t, r, b)

#define SMART_STATIC_FUNC(func)         static void WINAPI func
#define SMART_STATIC_FUNC_(type, func)  static type WINAPI func

#ifndef WIDET2
#define WIDET2(x)   L ## x
#endif//WIDEN2
#ifndef WIDET
#define WIDET(x)    WIDET2(x)
#endif//WIDEN
#ifndef __WIDEFILE__
#define __WIDEFILE__   WIDET(__FILE__)
#endif//__WFILE__

#ifndef MAKELPARAMFROMPOINT
#define MAKELPARAMFROMPOINT(p) p.x + p.y * 0x10000
#endif

#define HASCODE(SET, ELE)               (SET & ELE) == ELE
#define NOHASCODE(SET, ELE)             (SET & ELE) != ELE
#pragma endregion
#pragma region 安全释放
#ifndef SAFE_CLEAR_STL
#define SAFE_CLEAR_VECTOR(x)            { while(!x.empty()) { delete x.back();  x.pop_back(); } }
#define SAFE_CLEAR_VECTOR_(x)           { while(!x->empty()){ delete x->back(); x->pop_back(); } }
#define SAFE_CLEAR_LIST(x)                SAFE_CLEAR_VECTOR(x)
#define SAFE_CLEAR_LIST_(x)               SAFE_CLEAR_VECTOR_(x)
#define SAFE_CLEAR_MAP(x)               { while(!x.empty()) { if(x.begin()->second  != NULL){ delete x.begin()->second; }  x.erase(x.begin()); } }
#define SAFE_CLEAR_MAP_(x)              { while(!x->empty()){ if(x->begin()->second != NULL){ delete x->begin()->second; } x->erase(x->begin()); } }
#endif
#ifndef SAFE_DELETE_GDI
#define SAFE_DELETEDC(x)                { if(x != NULL){ ::DeleteDC(x);     x = NULL; } }
#define SAFE_DELETEOBJECT(x)            { if(x != NULL){ ::DeleteObject(x); x = NULL; } }
#endif
#ifndef SAFE_CLOSE_OBJECT
#define SAFE_CLOSEFILE(x)              { if(x != NULL){ ::fclose(x); x = NULL; } }
#define SAFE_CLOSEHANDLE(x)            { if(x == INVALID_HANDLE_VALUE){ x = NULL; }else if(x != NULL){ ::CloseHandle(x); x = NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x)                 { if(x  != NULL){ x->Release();         x  = NULL; } }
#define SAFE_RELEASEDC(Wnd, Dc)         { if(Dc != NULL){ ::ReleaseDC(Wnd, Dc); Dc = NULL; } }
#endif
#ifndef SAFE_DELETE
#define SAFE_DELETE(x)                  { if(x != NULL){ delete x;          x = NULL; } }
#define SAFE_DELETE_(x)                 { if(x != NULL){ delete[] x;        x = NULL; } }
#endif
#ifndef SAFE_FREE
#define SAFE_FREE(x)                    { if(x != NULL){ ::free(x);         x = NULL; } }
#define SAFE_FREESID(x)                 { if(x != NULL){ ::FreeSid(x);      x = NULL; } }
#endif
#pragma endregion
#pragma region 线程同步
#define TFunc                           static UINT WINAPI
#define TFuncDC(FName)                  static UINT WINAPI FName(Smart::CThread* Thread);
#define TFuncDEB(CName, FName)          UINT CName::FName(Smart::CThread* Thread) \
                                        {
#define TFuncDEE                        }
#define SASectionI                      Smart::CSection::CAuto Section(this->m_Section);
#define SASectionI_(Obj)                Smart::CSection::CAuto Section(Obj->m_Section);
#define SASectionI__(Obj)               Smart::CSection::CAuto Section(Obj);
#define SASectionE                      Section.Enter();
#define SASectionL                      Section.Leave();
#pragma endregion
#pragma region 属性声明
#ifndef DeclareMember
#define DeclareMember(type, name)                                               \
protected:                                                                      \
    type m_##name;
#define DeclareMemberEx(type, name)                                             \
public:                                                                         \
    type m_##name;


#define DeclareMemberMethod_R(type, name, method)                               \
public:                                                                         \
    type Get##method(void)       { return this->m_##name.Get##method(); }
#define DeclareMemberMethod_O(type, name, method)                               \
public:                                                                         \
    void Set##method(type method){ this->m_##name->Set##method(method); }       \
    type Get##method(void)       { return this->m_##name->Get##method(); }
#define DeclareMemberMethod_OR(type, name, method)                              \
public:                                                                         \
    type Get##method(void)       { return this->m_##name->Get##method(); }
#endif
#ifndef DeclareProperty                //Simple Property
#define DeclareProperty(type, name)                                             \
public:                                                                         \
    void Set##name(type name)  { this->m_##name = name; }                       \
    type Get##name(void) const { return this->m_##name; }                       \
protected:                                                                      \
    type m_##name;                                                              //Simple Property
#define DeclareProperty_R(type, name)                                           \
public:                                                                         \
    type Get##name(void) const { return this->m_##name; }                       \
protected:                                                                      \
    type m_##name;                                                              //Simple Property ReadOnly
#define DeclareProperty_S(type, name)                                           \
public:                                                                         \
    void Set##name(type name){ SASectionI; this->m_##name = name; }             \
    type Get##name(void)     { SASectionI; return this->m_##name; }             \
protected:                                                                      \
    type m_##name;                                                              //Simple Property ThreadSafe
#define DeclareProperty_RS(type, name)                                          \
public:                                                                         \
    type Get##name(void)     { SASectionI; return this->m_##name; }             \
protected:                                                                      \
    type m_##name;                                                              //Simple Property ReadOnly ThreadSafe
#endif
#ifndef DeclareProperty_O              //Object Property
#define DeclareProperty_O(type, name)                                           \
public:                                                                         \
    void  Set##name(const type& name){ this->m_##name = name; }                 \
    type* Get##name(void)            { return &this->m_##name; }                \
protected:                                                                      \
    type m_##name;                                                              //Object Property
#define DeclareProperty_OR(type, name)                                          \
public:                                                                         \
    type* Get##name(void)            { return &this->m_##name; }                \
protected:                                                                      \
    type m_##name;                                                              //Object Property ReadOnly
#define DeclareProperty_OS(type, name)                                          \
public:                                                                         \
    void  Set##name(type name){ SASectionI; this->m_##name = name; }            \
    type* Get##name(void)     { SASectionI; return &this->m_##name; }           \
protected:                                                                      \
    type m_##name;                                                              //Object Property ThreadSafe
#define DeclareProperty_ORS(type, name)                                         \
public:                                                                         \
    type* Get##name(void)     { SASectionI; return &this->m_##name; }           \
protected:                                                                      \
    type m_##name;                                                              //Object Property ReadOnly ThreadSafe
#endif
#pragma endregion
//========窗口消息==============================================================
#define WM_USER_BASE                    WM_USER      + 0x5000
#define WM_USER_SERIALIZE               WM_USER_BASE + 0x0001   //窗口线程序列化消息
#define WM_USER_TRAYNOTIFY              WM_USER_BASE + 0x0003   //窗口系统托盘消息
#define WM_USER_SINGLETON               WM_USER_BASE + 0x0004   //窗口程序单实例消息，参数未定义，接收消息窗口显示
#define WM_USER_APPBAR                  WM_USER_BASE + 0x0005   //APPBAR消息
//========窗口序列化消息WPARAM参数==============================================
#define WM_USER_SERIALIZE_COMMAND_MIN       0x0000 //LPARAM为UINT
#define WM_USER_SERIALIZE_COMMAND_REFRESH   0x1000
#define WM_USER_SERIALIZE_COMMAND_AUTOHIDE  0x1001
#define WM_USER_SERIALIZE_COMMAND_MAX       0x1FFF //LPARAM为UINT
#define WM_USER_SERIALIZE_GENERAL_MIN       0x2000 //LPARAM为std::wstring
#define WM_USER_SERIALIZE_GENERAL_MAX       0x3FFF //LPARAM为std::wstring
#define WM_USER_SERIALIZE_EXPAND_MIN        0x4000 //LPARAM为CParam*，Content自动释放（是new出来的一段内存）
#define WM_USER_SERIALIZE_EXPAND_MAX        0x5FFF //LPARAM为CParam*，Content自动释放（是new出来的一段内存）
#define WM_USER_SERIALIZE_SPECIAL_MIN       0x6000 //LPARAM为CParam*，Content手动释放
#define WM_USER_SERIALIZE_SPECIAL_MAX       0x7FFF //LPARAM为CParam*，Content手动释放
//========文本BOM===============================================================
enum BOMType
{
    BOMType_NONE    = 0,
    BOMType_UTF8    = 1,
    BOMType_UTF16LE = 2,
    BOMType_UTF16BE = 3,
    BOMType_UTF32LE = 4,
    BOMType_UTF32BE = 5
};
const BYTE TEXT_BOM_UTF8[3]    = { 0xEF, 0xBB, 0xBF };
const BYTE TEXT_BOM_UTF16LE[2] = { 0xFF, 0xFE };
const BYTE TEXT_BOM_UTF16BE[2] = { 0xFE, 0xFF };
const BYTE TEXT_BOM_UTF32LE[4] = { 0xFF, 0xFE, 0x00, 0x00 };
const BYTE TEXT_BOM_UTF32BE[4] = { 0x00, 0x00, 0xFE, 0xFF };
namespace Smart
{
    class CMargin;   //外边框类
    class CPadding;  //内边框类
    class CRectangle;//矩形类
    class CNVT;      //名值类型类



    template<class T> inline int Convert(T t)
    {
        union 
        {
            T   t;
            int a;
        }ut;

        ut.t = t;
        return ut.a;
    };
    class Delegate0
    {
    public:
        template<typename T> Delegate0(LPVOID Obj, T MemFunc) : m_Obj(int(Obj)), m_MemFunc(Smart::Convert(MemFunc)){ }
        void Invoke(void)
        {
            int _Self = this->m_Obj;
            int _Func = this->m_MemFunc;
            _asm
            {
                push eax;
                push ecx;
                mov  ecx, _Self;
                mov  eax, _Func;
                call eax;
                pop  ecx;
                pop  eax;
            }
        };
    protected:
        int m_Obj;
        int m_MemFunc;
    };
    class Delegate1
    {
    public:
        template<typename T> Delegate1(LPVOID Obj, T MemFunc, LPVOID Arg) : m_Obj(int(Obj)), m_MemFunc(Smart::Convert(MemFunc)), m_Arg(int(Arg)){ }
        void Invoke(void)
        {
            int _Self = this->m_Obj;
            int _Func = this->m_MemFunc;
            int _Arg  = this->m_Arg;
            _asm
            {
                push eax;
                push ecx;
                push _Arg;
                mov  ecx, _Self;
                mov  eax, _Func;
                call eax;
                pop  ecx;
                pop  eax;
            }
        };
    protected:
        int m_Obj;
        int m_MemFunc;
        int m_Arg;
    };
    template<class T> class CAutoDelete
    {
    public:
        CAutoDelete(T* t) : m_T(t){ }
        ~CAutoDelete(void)        { delete this->m_T; }
        T* operator->(void)       { return this->m_T; }
    protected:
        T* m_T;
    };

    class CHex;     //Hex类
    class CURI;     //URI类
    class CDateTime;//DateTime类
    class CParam;   //参数类

    class CTool;     //常用操作类
    class CText;     //文本操作类
    //class CMemory;  //内存操作类
    class CBlock;    //数据操作类
    class CTextBlock;//文本数据操作类

    class CSection;     //线程数据临界类
    class CThread;      //线程类

    class CPath;
    class CFile;

    namespace Processor
    {
        class CProcessor;  //处理器类
        class CPerformance;//处理器性能类
    };
};
//========Smart::CMargin========================================================
class Smart::CMargin
{
public:
    CMargin(void)                                     : m_Left(0), m_Top(0), m_Right(0), m_Bottom(0)              { };
    CMargin(int Left, int Top, int Right, int Bottom) : m_Left(Left), m_Top(Top), m_Right(Right), m_Bottom(Bottom){ };
    virtual ~CMargin(void)                                                                                        { };
protected:
    DeclareProperty(int, Left);  //左
    DeclareProperty(int, Right); //右
    DeclareProperty(int, Top);   //上
    DeclareProperty(int, Bottom);//下
};
//========Smart::CPadding=======================================================
class Smart::CPadding : public Smart::CMargin
{
public:
    CPadding(void)                                     : CMargin()                        { };
    CPadding(int Left, int Top, int Right, int Bottom) : CMargin(Left, Top, Right, Bottom){ };
    virtual ~CPadding(void)                                                               { };
};
//========Smart::CRectangle=====================================================
class Smart::CRectangle
{
public:
    CRectangle(void)                       : m_X(0), m_Y(0), m_W(0), m_H(0){ }
    CRectangle(int X, int Y, int W, int H) : m_X(X), m_Y(Y), m_W(W), m_H(H){ }
    virtual ~CRectangle(void)                                              { }
protected:
    DeclareProperty(int, X);//位置X坐标
    DeclareProperty(int, Y);//位置Y坐标
    DeclareProperty(int, W);//大小宽度
    DeclareProperty(int, H);//大小高度
};
//========Smart::CNVT===========================================================
class Smart::CNVT
{
public:
    CNVT(const std::wstring& Name = L"", const std::wstring& Value = L"", INT32 Type = 0) : m_Name(Name), m_Value(Value), m_Type(Type){ };
protected:
    DeclareMemberEx(std::wstring, Name); //名
    DeclareMemberEx(std::wstring, Value);//值
    DeclareMemberEx(UINT32,       Type); //类型
};

//========Smart::CHex===========================================================
class Smart::CHex
{
public:
    static BYTE         WINAPI xtoi(char c);                                              //【函数功能】：Hex字符转BYTE
    static BYTE         WINAPI xtoi(wchar_t c);                                           //【函数功能】：Hex字符转BYTE
    static BOOL         WINAPI ToByte(const std::string& HexA, PBYTE Data, UINT32& Size); //【函数功能】：将Hex字符串转换成字节数据
    static BOOL         WINAPI ToByte(const std::wstring& HexW, PBYTE Data, UINT32& Size);//【函数功能】：将Hex字符串转换成字节数据
    static std::string  WINAPI ToHexA(PBYTE Data, UINT32 Size, BOOL Upper = TRUE);        //【函数功能】：将字节数据转换成Hex字符串
    static std::wstring WINAPI ToHexW(PBYTE Data, UINT32 Size, BOOL Upper = TRUE);        //【函数功能】：将字节数据转换成Hex字符串
};
//========Smart::CURI===========================================================
class Smart::CURI
{
public:
    static std::string WINAPI UrlEncode(const std::string& Text); //【函数功能】：URL编码字符串
    static std::string WINAPI UrlDecode(const std::string& Text); //【函数功能】：URL解码字符串
    static std::wstring WINAPI UrlEncode_ANSI(const std::wstring& Text);//【函数功能】：URL编码字符串，以ANSI为基础
    static std::wstring WINAPI UrlEncode_GBK(const std::wstring& Text); //【函数功能】：URL编码字符串，以GBK为基础
    static std::wstring WINAPI UrlEncode_UTF8(const std::wstring& Text);//【函数功能】：URL编码字符串，以UTF8为基础
    static std::wstring WINAPI UrlDecode_ANSI(const std::wstring& Text);//【函数功能】：URL解码字符串，以ANSI为基础
    static std::wstring WINAPI UrlDecode_GBK(const std::wstring& Text); //【函数功能】：URL解码字符串，以GBK为基础
    static std::wstring WINAPI UrlDecode_UTF8(const std::wstring& Text);//【函数功能】：URL解码字符串，以UTF8为基础
};
//========Smart::CDateTime======================================================
class Smart::CDateTime
{
public:
    CDateTime(void);
    virtual ~CDateTime(void);
    UINT64       GetTimeStamp(BOOL Second = TRUE);//【函数功能】：获取时间戳，起始时间19700101000000000【TRUE：精度到秒】【FALSE：精度到毫秒】
    std::wstring ToString(BOOL Format = TRUE);    //【函数功能】：格式系统时间信息，【TRUE：格式yyyy-MM-DD HH:mm:ss.ms】【FALSE：格式yyyyMMDDHHmmssms】
protected:
    DeclareProperty(UINT16,             Year);       //年
    DeclareProperty(UINT8,              Month);      //月
    DeclareProperty(UINT8,              Day);        //日
    DeclareProperty(UINT8,              Hour);       //时
    DeclareProperty(UINT8,              Minute);     //分
    DeclareProperty(UINT8,              Second);     //秒
    DeclareProperty(UINT16,             Millisecond);//毫秒
    DeclareProperty(UINT8,              DayOfWeek);  //星期
    DeclareProperty(UINT32,             TimeZoneID); //时区类型
    DeclareMember(TIME_ZONE_INFORMATION, TimeZone);   //时区
public:
    static BOOL      IsLeapYear(UINT32 Year);                            //【函数功能】：验证闰年
    static BOOL      IsIllegal(UINT32 Year, UINT32 Month, UINT32 Day);   //【函数功能】：验证非法日期
    static UINT32    GetDaysOfYear(UINT32 Year);                         //【函数功能】：获取指定年份的天数
    static UINT32    GetDayOfYear(UINT32 Year, UINT32 Month, UINT32 Day);//【函数功能】：获取指定日期是一年中的第几天，从1开始计数
    static CDateTime GetNow(void);                                       //【函数功能】：获取当前日期时间
    static CDateTime FromSYSTEMTIME(const SYSTEMTIME& ST);               //【函数功能】：获取时间从SYSTEMTIME
    static CDateTime FromFILETIME(const FILETIME& FT);                   //【函数功能】：获取时间从FILETIME
public:
    static void AddDays(SYSTEMTIME& ST, INT32 Days);//【函数功能】：系统时间增加天数
};
//========Smart::CParam=========================================================
class Smart::CParam
{
public:
    CParam(UINT Type, UINT Content);
    CParam(UINT Type, const std::wstring& Content);
    CParam(UINT Type, LPVOID Content);
    ~CParam(void);
public:
    UINT         m_Type;
    UINT         m_uContent;
    std::wstring m_strContent;
    LPVOID       m_pContent;
    LONGLONG     m_TimeStamp;//时间戳
};
//========Smart::CTool==========================================================
class Smart::CTool
{
public:
    static void* WINAPI MallocMemory(void** Mem, DWORD Size);
    static void* WINAPI MergerMemory(void* Data1, DWORD Size1, void* Data2, DWORD Size2);
    static BOOL  WINAPI MergerData(void* pDestData, DWORD dwDestDataSize, void* Data0, DWORD dwData0Size, void* Data1, DWORD dwData1Size);
    static void  WINAPI FreeMemory(void** Mem);
public:
    static RECT WINAPI ScreenRect(void);           //【函数功能】：获取屏幕区域范围
    static RECT WINAPI WorkArea(void);             //【函数功能】：获取屏幕工作区域（不包含任务栏），区域范围为[left, right)、[top, bottom)
    static RECT WINAPI TrayRect(void);             //【函数功能】：获取任务栏区域范围
public:
    static POINT WINAPI GetCursorPos(void);//【函数功能】：获取当前光标位置
public:
    static BOOL WINAPI SelectFileDialog(HWND Owner, const std::wstring& Title, LPCWSTR Filter, std::wstring& FilePath); //【函数功能】：选择文件对话框
    static BOOL WINAPI SelectFolderDialog(HWND Owner, const std::wstring& Title, std::wstring& FolderPath);             //【函数功能】：选择文件夹对话框
};
//========Smart::CText==========================================================
class Smart::CText
{
public:
    static void         WINAPI Split(const std::wstring& Text, wchar_t             Separator, std::vector<std::wstring>& Array);//【函数功能】：分割字符串
    static void         WINAPI Split(const std::wstring& Text, const std::wstring& Separator, std::vector<std::wstring>& Array);//【函数功能】：分割字符串
    static void         WINAPI Split(LPCWSTR Text, WCHAR Separator, LPWSTR Array[], UINT Length);  //【函数功能】：分割字符串，使用后用FreeSplitArray释放字符串数组资源
    static void         WINAPI Split(LPCWSTR Text, LPCWSTR Separator, LPWSTR Array[], UINT Length);//【函数功能】：分割字符串，使用后用FreeSplitArray释放字符串数组资源
    static void         WINAPI FreeSplitArray(LPWSTR Array[], UINT Length);                        //【函数功能】：释放分割字符串产生的字符串数组
    static LPWSTR       WINAPI Trim(LPWSTR Text, size_t Size);                                //【函数功能】：去除字符串前后空白字符
    static LPWSTR       WINAPI Trim(LPWSTR Text, size_t Size, LPCWSTR Prefix, LPCWSTR Suffix);//【函数功能】：去除字符串前缀后缀后，再去除前后空白字符
    static std::wstring WINAPI Trim(const std::wstring& Text);                                //【函数功能】：去除字符串前后空白字符
    static std::wstring WINAPI Trim(const std::wstring& Text, LPCWSTR Prefix, LPCWSTR Suffix);//【函数功能】：去除字符串前缀后缀后，再去除前后空白字符
    static std::string  WINAPI Format(LPCSTR Format, ...);                                    //【函数功能】：格式化字符串
    static std::wstring WINAPI Format(LPCWSTR Format, ...);                                   //【函数功能】：格式化字符串
    static std::wstring WINAPI Format_(LPCWSTR Format, va_list Args);                         //【函数功能】：格式化字符串
    static std::string  WINAPI Replace(const std::string&  Text, const std::string&  OldStr, const std::string&  NewStr);//【函数功能】：字符串替换    
    static std::wstring WINAPI Replace(const std::wstring& Text, const std::wstring& OldStr, const std::wstring& NewStr);//【函数功能】：字符串替换
public:
    static BOOL         WINAPI WideCharToMultiByte(UINT CodePage, DWORD Flags, LPCWSTR WideByte, int WSize, LPSTR MultiByte, int MSize);   //【函数功能】：宽字节转多字节，目标字符串数组小于需要的字符串大小时返回NULL
    static BOOL         WINAPI WideCharToMultiByte(UINT CodePage, DWORD Flags, const std::wstring& WideByteStr, std::string& MultiByteStr);//【函数功能】：宽字节转多字节
    static std::string  WINAPI WideCharToMultiByte(UINT CodePage, DWORD Flags, const std::wstring& WideCharStr);                           //【函数功能】：宽字节转多字节
    static BOOL         WINAPI MultiByteToWideChar(UINT CodePage, DWORD Flags, LPCSTR MultiByte, int MSize, LPWSTR WideByte, int WSize);   //【函数功能】：多字节转宽字节，目标字符串数组小于需要的字符串大小时返回NULL
    static BOOL         WINAPI MultiByteToWideChar(UINT CodePage, DWORD Flags, const std::string& MultiByteStr, std::wstring& WideByteStr);//【函数功能】：多字节转宽字节
    static std::wstring WINAPI MultiByteToWideChar(UINT CodePage, DWORD Flags, const std::string& MultiByteStr);                           //【函数功能】：多字节转宽字节
public:
    inline static BOOL         WINAPI UTF16_to_ANSI(LPCWSTR WideChar, int WSize, LPSTR MultiByte, int Size){ return Smart::CText::WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, WideChar, WSize, MultiByte, Size);};//【函数功能】：UTF16转ANSI，目标字符串数组小于需要的字符串大小时返回NULL
    inline static std::string  WINAPI UTF16_to_ANSI(const std::wstring& WideCharStr)                       { return Smart::CText::WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, WideCharStr);};                     //【函数功能】：UTF16转ANSI
    inline static BOOL         WINAPI ANSI_to_UTF16(LPCSTR MultiByte, int Size, LPWSTR WideChar, int WSize){ return Smart::CText::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, MultiByte, Size, WideChar, WSize);};      //【函数功能】：ANSI转UTF16，目标字符串数组小于需要的字符串大小时返回NULL
    inline static std::wstring WINAPI ANSI_to_UTF16(const std::string& MultiByteStr)                       { return Smart::CText::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, MultiByteStr);};                          //【函数功能】：ANSI转UTF16
public:
    inline static BOOL         WINAPI UTF16_to_GBK(LPCWSTR WideChar, int WSize, LPSTR MultiByte, int Size){ return Smart::CText::WideCharToMultiByte(936, WC_NO_BEST_FIT_CHARS, WideChar, WSize, MultiByte, Size);};//【函数功能】：UTF16转GBK，目标字符串数组小于需要的字符串大小时返回NULL
    inline static std::string  WINAPI UTF16_to_GBK(const std::wstring& WideCharStr)                       { return Smart::CText::WideCharToMultiByte(936, WC_NO_BEST_FIT_CHARS, WideCharStr);};                     //【函数功能】：UTF16转GBK
    inline static BOOL         WINAPI GBK_to_UTF16(LPCSTR MultiByte, int Size, LPWSTR WideChar, int WSize){ return Smart::CText::MultiByteToWideChar(936, MB_PRECOMPOSED, MultiByte, Size, WideChar, WSize);};      //【函数功能】：GBK转UTF16，目标字符串数组小于需要的字符串大小时返回NULL
    inline static std::wstring WINAPI GBK_to_UTF16(const std::string& MultiByteStr)                       { return Smart::CText::MultiByteToWideChar(936, MB_PRECOMPOSED, MultiByteStr);};                          //【函数功能】：GBK转UTF16
public:
    inline static BOOL         WINAPI UTF16_to_BIG5(LPCWSTR WideChar, int WSize, LPSTR MultiByte, int Size){ return Smart::CText::WideCharToMultiByte(950, WC_NO_BEST_FIT_CHARS, WideChar, WSize, MultiByte, Size);};//【函数功能】：UTF16转BIG5，目标字符串数组小于需要的字符串大小时返回NULL
    inline static std::string  WINAPI UTF16_to_BIG5(const std::wstring& WideCharStr)                       { return Smart::CText::WideCharToMultiByte(950, WC_NO_BEST_FIT_CHARS, WideCharStr);};                     //【函数功能】：UTF16转BIG5
    inline static BOOL         WINAPI BIG5_to_UTF16(LPCSTR MultiByte, int Size, LPWSTR WideChar, int WSize){ return Smart::CText::MultiByteToWideChar(950, MB_PRECOMPOSED, MultiByte, Size, WideChar, WSize);};      //【函数功能】：BIG5转UTF16，目标字符串数组小于需要的字符串大小时返回NULL
    inline static std::wstring WINAPI BIG5_to_UTF16(const std::string& MultiByteStr)                       { return Smart::CText::MultiByteToWideChar(950, MB_PRECOMPOSED, MultiByteStr);};                          //【函数功能】：BIG5转UTF16
public:
    inline static BOOL         WINAPI UTF16_to_UTF8(LPCWSTR WideChar, int WSize, LPSTR MultiByte, int MSize){ return Smart::CText::WideCharToMultiByte(CP_UTF8, 0, WideChar, WSize, MultiByte, MSize);};//【函数功能】：UTF16转UTF8，目标字符串数组小于需要的字符串大小时返回NULL
    inline static std::string  WINAPI UTF16_to_UTF8(const std::wstring& WideCharStr)                        { return Smart::CText::WideCharToMultiByte(CP_UTF8, 0, WideCharStr);};                      //【函数功能】：UTF16转UTF8
    inline static BOOL         WINAPI UTF8_to_UTF16(LPCSTR MultiByte, int MSize, LPWSTR WideChar, int WSize){ return Smart::CText::MultiByteToWideChar(CP_UTF8, 0, MultiByte, MSize, WideChar, WSize);};//【函数功能】：UTF8转UTF16，目标字符串数组小于需要的字符串大小时返回NULL
    inline static std::wstring WINAPI UTF8_to_UTF16(const std::string& MultiByteStr)                        { return Smart::CText::MultiByteToWideChar(CP_UTF8, 0, MultiByteStr);};                     //【函数功能】：UTF8转UTF16
public:
    static std::wstring WINAPI LittleEndianToBigEndian(const std::wstring& Sour){ std::wstring Dest; Smart::CText::LittleEndianToBigEndian(Sour, Dest); return Dest; }//【函数功能】：大小端转换
    static void         WINAPI LittleEndianToBigEndian(const std::wstring& Sour, std::wstring& Dest);                                                                 //【函数功能】：大小端转换
    static std::wstring WINAPI BigEndianToLittleEndian(const std::wstring& Sour){ std::wstring Dest; Smart::CText::BigEndianToLittleEndian(Sour, Dest); return Dest; }//【函数功能】：大小端转换
    static void         WINAPI BigEndianToLittleEndian(const std::wstring& Sour, std::wstring& Dest);                                                                 //【函数功能】：大小端转换
    static BOOL         WINAPI ToUnicode(const std::string& MText, std::wstring& WText);             //【函数功能】：多字节转宽字节，支持Gb2312、Big5、GBK、UTF8
    static BOOL         WINAPI ToUnicodeEx(const std::string& MText, std::wstring& WText);           //【函数功能】：多字节转宽字节，支持Gb2312、Big5、GBK、UTF8、UTF16LE、UTF16BE
public:
    static BOOL WINAPI IsUTF8SpecialByte(BYTE b);                 //【函数功能】：判断字符是否是UTF8的非起始字符
    static BOOL WINAPI IsUTF8(const std::string& Text);           //【函数功能】：判断文本是否是UTF8编码
    static BOOL WINAPI IsGb2312(const std::string& Text);         //【函数功能】：判断文本是否是Gb2312编码
    static BOOL WINAPI IsBig5(const std::string& Text);           //【函数功能】：判断文本是否是Big5编码
    static BOOL WINAPI IsGBK(const std::string& Text);            //【函数功能】：判断文本是否是GBK编码
};
//========Smart::CBlock=========================================================
class Smart::CBlock
{
public:
    CBlock(UINT Size = 0);
    CBlock(const CBlock& Block);
    ~CBlock(void);
    void operator= (const CBlock& Block);                                                                 //【函数功能】：赋值数据
    BOOL IsEmpty(void) const                   { return this->m_Size == 0; }                              //【函数功能】：判断数据块为空
    template<typename T> T* GetData(UINT& Size){ Size = this->m_Size%sizeof(T); return (T*)this->m_Data; }//【函数功能】：获取数据块指针
public:
    void Append(const PBYTE Data, UINT Size); //【函数功能】：添加数据
    void Adjust(UINT Size);                   //【函数功能】：调整数据大小
    void Apply(UINT Size);                    //【函数功能】：申请数据大小，只影响m_ApplySize的值
    void Clear(void);                         //【函数功能】：清理数据
public:
    std::string  ToHexA(void);                     //【函数功能】：将数据转换成HEX字符串
    std::wstring ToHexW(void);                     //【函数功能】：将数据转换成HEX字符串
    void         FromHexA(const std::string& Hex); //【函数功能】：将HEX字符串转换成数据
    void         FromHexW(const std::wstring& Hex);//【函数功能】：将HEX字符串转换成数据
protected:
    DeclareProperty_R(PBYTE, Data);//数据指针
    DeclareProperty_R(UINT, Size); //数据大小
    DeclareMember(UINT, ApplySize); //申请大小
};
//========Smart::CTextBlock=====================================================
class Smart::CTextBlock
    : public Smart::CBlock
{
public:
    CTextBlock(void)
    {
    }
    CTextBlock(const std::string& Text)
    {
        this->Adjust(Text.size());
        ::memcpy_s(this->m_Data, this->m_Size, Text.c_str(), Text.size());
    }
    CTextBlock(const std::wstring& Text)
    {
        this->Adjust(Text.size()*2);
        ::memcpy_s(this->m_Data, this->m_Size, Text.c_str(), Text.size()*2);
    }
public:
    void Append(PBYTE Data, UINT Size)   { Smart::CBlock::Append(Data, Size); }  //【函数功能】：添加数据
    void Append(const std::string& Text) { Smart::CBlock::Append((const PBYTE)Text.c_str(), Text.size()); }  //【函数功能】：添加数据
    void Append(const std::wstring& Text){ Smart::CBlock::Append((const PBYTE)Text.c_str(), Text.size()*2); }//【函数功能】：添加数据
    void AppendFormat(LPCSTR Format, ...);                                                                   //【函数功能】：添加数据
    void AppendFormat(LPCWSTR Format, ...);                                                                  //【函数功能】：添加数据
public:
    std::string  GetTextA(void)       { return std::string((char*)this->m_Data, this->m_Size); }                             //【函数功能】：将数据转换成文本
    std::wstring GetTextW(void)       { return std::wstring((wchar_t*)this->m_Data, this->m_Size/2); }                       //【函数功能】：将数据转换成文本
    std::wstring GetTextFromANSI(void){ return Smart::CText::ANSI_to_UTF16(std::string(LPSTR(this->m_Data), this->m_Size)); }//【函数功能】：获取字符串数据
    std::wstring GetTextFromUTF8(void){ return Smart::CText::UTF8_to_UTF16(std::string(LPSTR(this->m_Data), this->m_Size)); }//【函数功能】：获取字符串数据
};
//========Smart::CSection=======================================================
class Smart::CSection
{
public:
    class CAuto
    {
    public:
        CAuto(Smart::CSection& Section) : m_Section(&Section), m_Status(FALSE){ this->Enter(); }
        CAuto(Smart::CSection* Section) : m_Section(Section),  m_Status(FALSE){ this->Enter(); }
        ~CAuto(void)                                                          { this->Leave(); }
        void Enter(void){ if(this->m_Section != NULL && !this->m_Status){ this->m_Status = TRUE;  this->m_Section->Enter(); } }
        void Leave(void){ if(this->m_Section != NULL &&  this->m_Status){ this->m_Status = FALSE; this->m_Section->Leave(); } }
    protected:
        BOOL             m_Status;
        Smart::CSection* m_Section;
    };
public:
    CSection(const std::wstring& Name = L"")
        : m_Mutex(NULL)
    {
        if(!Name.empty())
        {
            this->m_Mutex = ::CreateMutexW(NULL, FALSE, Name.c_str());
            DWORD A = ::GetLastError();
            DWORD B = A;
        }
        if(this->m_Mutex == NULL)
        {
            ::InitializeCriticalSection(&this->m_Section);
        }
    }
    ~CSection(void) { this->m_Mutex != NULL ? ::CloseHandle(this->m_Mutex)                   : ::DeleteCriticalSection(&this->m_Section); }
    void Enter(void){ this->m_Mutex != NULL ? ::WaitForSingleObject(this->m_Mutex, INFINITE) : ::EnterCriticalSection(&this->m_Section); }
    void Leave(void){ this->m_Mutex != NULL ? ::ReleaseMutex(this->m_Mutex)                  : ::LeaveCriticalSection(&this->m_Section); }
protected:
    HANDLE               m_Mutex;
    RTL_CRITICAL_SECTION m_Section;
};
//========Smart::CThread========================================================
class Smart::CThread
{
public:
    typedef unsigned int (__stdcall * StartAddress)(Smart::CThread* Thread);
public:
    CThread(void);
    virtual ~CThread(void);
    virtual BOOL Startup(StartAddress Address, LPVOID Arg = NULL, UINT64 Times = 1, UINT32 Period = 40, UINT32 Delay = 0);//【函数功能】：启动线程【Times：执行次数】【Period：执行周期】【Delay：首次执行延迟】，当线程有任务在运行时，结束任务，执行新任务
    virtual void Shutdown(BOOL Wait = TRUE);                                                                              //【函数功能】：结束线程
    virtual void WaitforComplete(void);                                                                                   //【函数功能】：等待线程自动完成
public:
    virtual BOOL     IsCancel(void);                                         //【函数功能】：判断线程是否取消
    virtual BOOL     IsBusy(void);                                           //【函数功能】：判断线程是否忙碌
    virtual BOOL     IsIdle(void);                                           //【函数功能】：判断线程是否空闲
protected:
    static UINT WINAPI __ThreadProcess(LPVOID Thread);//【函数功能】:线程入口函数
protected:
    Smart::CSection m_Section;
    BOOL            m_Cancel;      //线程使能状态
    UINT            m_ID;          //线程ID
    HANDLE          m_Handle[4];   //线程句柄及其事件【0：线程句柄】【1：线程启动完成事件句柄】【2：未定义】【3：线程延时事件句柄】
    StartAddress    m_StartAddress;//线程任务入口地址
    ULONGLONG       m_Times;       //线程任务执行次数
    DWORD           m_Period;      //线程任务执行周期
    DWORD           m_Delay;       //线程任务延时时间
public:
    DeclareProperty_RS(LPVOID,             Arg);      //线程参数
    DeclareProperty_RS(UINT64,             TimeStamp);//线程启动时间戳
public:
    static BOOL StartupGroup(INT32 Size, StartAddress Address, LPVOID Arg = NULL);//【函数功能】：启动一组线程执行任务，线程退出后返回
};
//========Smart::CPath==========================================================
class Smart::CPath
{
public:
    static BOOL WINAPI FormatPath(std::wstring& Path);                                        //【函数功能】：格式化路径，将路径中的'/'替换成'\\'
    static BOOL WINAPI PathIsDrive(const std::wstring& Path);                                 //【函数功能】：判断路径是否为驱动器
    static BOOL WINAPI PathIsDirectory(const std::wstring& Path);                             //【函数功能】：判断路径是否为目录
    static BOOL WINAPI PathIsFile(const std::wstring& Path);                                  //【函数功能】：判断路径是否为文件
    static BOOL WINAPI PathIsExists(const std::wstring& Path);                                //【函数功能】：判断路径是否存在
    static BOOL WINAPI PathIsExists(const std::wstring& Path, UINT32 FileSize, BOOL& FSCheck);//【函数功能】：判断路径是否存在
public:
    static BOOL WINAPI GetFileDir(const std::wstring& Path, std::wstring& Dir);                                      //【函数功能】：获取文件目录，不能获取根目录的路径，比如"C:\"
    static BOOL WINAPI GetFileName(const std::wstring& Path, std::wstring& FileName);                                //【函数功能】：获取文件名称，包含扩展名（文件夹是一种特殊文件，对文件夹同样适用）
    static BOOL WINAPI GetFileNameNoExtension(const std::wstring& Path, std::wstring& FileName);                     //【函数功能】：获取文件名称，不包含扩展名，只适用于文件
    static BOOL WINAPI GetFileExtension(const std::wstring& Path, std::wstring& FileExtension);                      //【函数功能】：获取文件扩展名，只适用于文件
    static BOOL WINAPI PathHasExtension(const std::wstring& Path, const std::wstring& Extension);                    //【函数功能】：判断路径拥有指定扩展名
public:
    static std::wstring WINAPI GetFileDir(const std::wstring& Path)                                    { std::wstring Text; if(GetFileDir(Path, Text))            { return Text;} return std::wstring(); }   //【函数功能】：获取文件目录，不能获取根目录的路径，比如"C:\"
    static std::wstring WINAPI GetFileName(const std::wstring& Path)                                   { std::wstring Text; if(GetFileName(Path, Text))           { return Text;} return std::wstring(); }   //【函数功能】：获取文件名称，包含扩展名（文件夹是一种特殊文件，对文件夹同样适用）
    static std::wstring WINAPI GetFileNameNoExtension(const std::wstring& Path)                        { std::wstring Text; if(GetFileNameNoExtension(Path, Text)){ return Text;} return std::wstring(); }   //【函数功能】：获取文件名称，不包含扩展名，只适用于文件
    static std::wstring WINAPI GetFileExtension(const std::wstring& Path)                              { std::wstring Text; if(GetFileExtension(Path, Text))      { return Text;} return std::wstring(); }   //【函数功能】：获取文件扩展名，只适用于文件
    static std::wstring WINAPI CombineFileName(const std::wstring& Main, const std::wstring& Extension){ return Extension.empty() ? Main : Smart::CText::Format(L"%s.%s", Main.c_str(), Extension.c_str()); }//【函数功能】：组合文件名通过主名和扩展名
public:
    static BOOL WINAPI CreateDir(const std::string& DirPath);              //【函数功能】：创建目录通过目录路径
    static BOOL WINAPI CreateDir(const std::wstring& DirPath);             //【函数功能】：创建目录通过目录路径
    static BOOL WINAPI CreateDirFromFilePath(const std::string& FilePath); //【函数功能】：创建目录通过文件路径
    static BOOL WINAPI CreateDirFromFilePath(const std::wstring& FilePath);//【函数功能】：创建目录通过文件路径
public:
    static BOOL WINAPI MoveFile(const std::wstring& Sour, const std::wstring& Dest);//【函数功能】：移动文件
    static BOOL WINAPI DeleteFile(const std::wstring& Sour);                        //【函数功能】：删除文件
public:
    static std::wstring WINAPI FilterIllegal(const std::wstring& FileName);//【函数功能】：去除文件名中非法字符
    static std::wstring WINAPI GetFilePathWithoutSame(std::wstring& FilePath);                                //【函数功能】：获取文件路径，进行路径文件检查，存在就返回新路径
    static std::wstring WINAPI GetFilePathNorepeat(const std::wstring& FilePath);//【函数功能】：获取不重复文件路径
};
//========Smart::CFile==========================================================
class Smart::CFile
{
public:
    CFile(void);
    ~CFile(void);
    BOOL IsOpen(void);                                                                                                                  //【函数功能】：判断文件已经打开
    BOOL Open(const std::wstring& FilePath, DWORD DesiredAccess = GENERIC_READ, DWORD ShareMode = FILE_SHARE_READ, BOOL Create = FALSE);//【函数功能】：打开文件【Create：没有文件是否创建】
    void Close(void);                                                                                                                   //【函数功能】：关闭文件
    BOOL IsEmpty(void);                                                                                                                 //【函数功能】：判断文件是否为空
    BOOL Read(PVOID Data, UINT Size, PUINT RSize = NULL);                                                                               //【函数功能】：读数据
    void Clear(void);                                                                                                                   //【函数功能】：清除数据
    BOOL Write(BOMType BOMType = BOMType_UTF16LE);                                                                                      //【函数功能】：写BOM数据
    BOOL Write(const PBYTE Data, UINT Size);                                                                                            //【函数功能】：写数据
    BOOL Write(LPCWSTR Format, ...);                                                                                                    //【函数功能】：写数据
public:
    void SavePos(void);                    //【函数功能】：保存文件读写位置
    void RestorePos(void);                 //【函数功能】：恢复文件读写位置
    UINT GetPos(void);                     //【函数功能】：获取文件读写位置
    void Seek(DWORD MoveMethod, long Move);//【函数功能】：定位文件读写位置
    void SeekBegin(void);                  //【函数功能】：定位文件起始位置
    void SeekEnd(void);                    //【函数功能】：定位文件结尾位置
protected:
    HANDLE m_File;//文件句柄
    UINT   m_Pos; //文件读写位置
public:
    static BOOL WINAPI FindFile(const std::wstring& FileFilter, std::list<std::wstring>& FileList, BOOL Sub = FALSE);//【函数功能】：查找文件
public:    
    static BOOL WINAPI GetData(const std::wstring& FilePath, BYTE** Data, DWORD* Size); //【函数功能】：获取文件数据，用完后通过ReleaseData释放
    static void WINAPI ReleaseData(BYTE** Data);                                        //【函数功能】：释放文件数据
public:
    static BOOL WINAPI ReadFile(const std::wstring& Path, Smart::CBlock* Block);//【函数功能】：读取文件数据
    static BOOL WINAPI ReadFile(const std::wstring& Path, std::wstring&  Text); //【函数功能】：读取文本文件数据
    static BOOL WINAPI WriteFile(const std::wstring& Path, const Smart::CBlock& Block);//【函数功能】：写入文件数据，覆盖现有文件内容
    static BOOL WINAPI WriteFile(const std::wstring& Path, const std::wstring&  Text); //【函数功能】：写入文件数据，覆盖现有文件内容
public:
    static BOOL WINAPI WriteFile(LPCWSTR FilePath, void* Data, DWORD Size);  //【函数功能】:写入文件数据
    static BOOL WINAPI WriteFile(LPCWSTR FilePath, LPCWSTR Data, DWORD Size);//【函数功能】:写入文件数据
    static BOOL WINAPI WriteFile(LPCWSTR FilePath, LPCWSTR Data);            //【函数功能】:写入文件数据
    static BOOL WINAPI WriteFile(HANDLE File, const void* Data, DWORD Size);       //【函数功能】:向文件写数据
    static BOOL WINAPI WriteFile(HANDLE File, const wchar_t* Data);                //【函数功能】:向文件写数据
public:
    static BOOL   WINAPI GetFileSize(const std::string& FilePath, long& FileSize); //【函数功能】：获取文件大小
    static BOOL   WINAPI GetFileSize(const std::wstring& FilePath, long& FileSize);//【函数功能】：获取文件大小
    static UINT32 WINAPI GetFileSize(const std::wstring& FilePath);                //【函数功能】：获取文件大小
};
//========Smart::Processor::CProcessor==========================================
class Smart::Processor::CProcessor
{
public:
    static DWORD WINAPI GetProcessorNumber(void);
    static DWORD WINAPI SetThreadAffinityMask(HANDLE Thread, DWORD_PTR ThreadAffinityMask);
    static BOOL  WINAPI SetThreadPriority(HANDLE Thread, int Priority = THREAD_PRIORITY_NORMAL);
};
//========Smart::Processor::CPerformance========================================
class Smart::Processor::CPerformance
{
public:
    CPerformance(void);
    ~CPerformance(void);
    void     Reset(void);
    BOOL     Tick(void);
    LONGLONG Time(DWORD i);
protected:
    LARGE_INTEGER              m_Frequency;
    std::vector<LARGE_INTEGER> m_Counter;
};

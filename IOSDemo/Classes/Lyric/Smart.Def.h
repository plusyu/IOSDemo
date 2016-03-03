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
#pragma region ��������
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
#pragma region ��ȫ�ͷ�
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
#pragma region �߳�ͬ��
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
#pragma region ��������
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
//========������Ϣ==============================================================
#define WM_USER_BASE                    WM_USER      + 0x5000
#define WM_USER_SERIALIZE               WM_USER_BASE + 0x0001   //�����߳����л���Ϣ
#define WM_USER_TRAYNOTIFY              WM_USER_BASE + 0x0003   //����ϵͳ������Ϣ
#define WM_USER_SINGLETON               WM_USER_BASE + 0x0004   //���ڳ���ʵ����Ϣ������δ���壬������Ϣ������ʾ
#define WM_USER_APPBAR                  WM_USER_BASE + 0x0005   //APPBAR��Ϣ
//========�������л���ϢWPARAM����==============================================
#define WM_USER_SERIALIZE_COMMAND_MIN       0x0000 //LPARAMΪUINT
#define WM_USER_SERIALIZE_COMMAND_REFRESH   0x1000
#define WM_USER_SERIALIZE_COMMAND_AUTOHIDE  0x1001
#define WM_USER_SERIALIZE_COMMAND_MAX       0x1FFF //LPARAMΪUINT
#define WM_USER_SERIALIZE_GENERAL_MIN       0x2000 //LPARAMΪstd::wstring
#define WM_USER_SERIALIZE_GENERAL_MAX       0x3FFF //LPARAMΪstd::wstring
#define WM_USER_SERIALIZE_EXPAND_MIN        0x4000 //LPARAMΪCParam*��Content�Զ��ͷţ���new������һ���ڴ棩
#define WM_USER_SERIALIZE_EXPAND_MAX        0x5FFF //LPARAMΪCParam*��Content�Զ��ͷţ���new������һ���ڴ棩
#define WM_USER_SERIALIZE_SPECIAL_MIN       0x6000 //LPARAMΪCParam*��Content�ֶ��ͷ�
#define WM_USER_SERIALIZE_SPECIAL_MAX       0x7FFF //LPARAMΪCParam*��Content�ֶ��ͷ�
//========�ı�BOM===============================================================
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
    class CMargin;   //��߿���
    class CPadding;  //�ڱ߿���
    class CRectangle;//������
    class CNVT;      //��ֵ������



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

    class CHex;     //Hex��
    class CURI;     //URI��
    class CDateTime;//DateTime��
    class CParam;   //������

    class CTool;     //���ò�����
    class CText;     //�ı�������
    //class CMemory;  //�ڴ������
    class CBlock;    //���ݲ�����
    class CTextBlock;//�ı����ݲ�����

    class CSection;     //�߳������ٽ���
    class CThread;      //�߳���

    class CPath;
    class CFile;

    namespace Processor
    {
        class CProcessor;  //��������
        class CPerformance;//������������
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
    DeclareProperty(int, Left);  //��
    DeclareProperty(int, Right); //��
    DeclareProperty(int, Top);   //��
    DeclareProperty(int, Bottom);//��
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
    DeclareProperty(int, X);//λ��X����
    DeclareProperty(int, Y);//λ��Y����
    DeclareProperty(int, W);//��С���
    DeclareProperty(int, H);//��С�߶�
};
//========Smart::CNVT===========================================================
class Smart::CNVT
{
public:
    CNVT(const std::wstring& Name = L"", const std::wstring& Value = L"", INT32 Type = 0) : m_Name(Name), m_Value(Value), m_Type(Type){ };
protected:
    DeclareMemberEx(std::wstring, Name); //��
    DeclareMemberEx(std::wstring, Value);//ֵ
    DeclareMemberEx(UINT32,       Type); //����
};

//========Smart::CHex===========================================================
class Smart::CHex
{
public:
    static BYTE         WINAPI xtoi(char c);                                              //���������ܡ���Hex�ַ�תBYTE
    static BYTE         WINAPI xtoi(wchar_t c);                                           //���������ܡ���Hex�ַ�תBYTE
    static BOOL         WINAPI ToByte(const std::string& HexA, PBYTE Data, UINT32& Size); //���������ܡ�����Hex�ַ���ת�����ֽ�����
    static BOOL         WINAPI ToByte(const std::wstring& HexW, PBYTE Data, UINT32& Size);//���������ܡ�����Hex�ַ���ת�����ֽ�����
    static std::string  WINAPI ToHexA(PBYTE Data, UINT32 Size, BOOL Upper = TRUE);        //���������ܡ������ֽ�����ת����Hex�ַ���
    static std::wstring WINAPI ToHexW(PBYTE Data, UINT32 Size, BOOL Upper = TRUE);        //���������ܡ������ֽ�����ת����Hex�ַ���
};
//========Smart::CURI===========================================================
class Smart::CURI
{
public:
    static std::string WINAPI UrlEncode(const std::string& Text); //���������ܡ���URL�����ַ���
    static std::string WINAPI UrlDecode(const std::string& Text); //���������ܡ���URL�����ַ���
    static std::wstring WINAPI UrlEncode_ANSI(const std::wstring& Text);//���������ܡ���URL�����ַ�������ANSIΪ����
    static std::wstring WINAPI UrlEncode_GBK(const std::wstring& Text); //���������ܡ���URL�����ַ�������GBKΪ����
    static std::wstring WINAPI UrlEncode_UTF8(const std::wstring& Text);//���������ܡ���URL�����ַ�������UTF8Ϊ����
    static std::wstring WINAPI UrlDecode_ANSI(const std::wstring& Text);//���������ܡ���URL�����ַ�������ANSIΪ����
    static std::wstring WINAPI UrlDecode_GBK(const std::wstring& Text); //���������ܡ���URL�����ַ�������GBKΪ����
    static std::wstring WINAPI UrlDecode_UTF8(const std::wstring& Text);//���������ܡ���URL�����ַ�������UTF8Ϊ����
};
//========Smart::CDateTime======================================================
class Smart::CDateTime
{
public:
    CDateTime(void);
    virtual ~CDateTime(void);
    UINT64       GetTimeStamp(BOOL Second = TRUE);//���������ܡ�����ȡʱ�������ʼʱ��19700101000000000��TRUE�����ȵ��롿��FALSE�����ȵ����롿
    std::wstring ToString(BOOL Format = TRUE);    //���������ܡ�����ʽϵͳʱ����Ϣ����TRUE����ʽyyyy-MM-DD HH:mm:ss.ms����FALSE����ʽyyyyMMDDHHmmssms��
protected:
    DeclareProperty(UINT16,             Year);       //��
    DeclareProperty(UINT8,              Month);      //��
    DeclareProperty(UINT8,              Day);        //��
    DeclareProperty(UINT8,              Hour);       //ʱ
    DeclareProperty(UINT8,              Minute);     //��
    DeclareProperty(UINT8,              Second);     //��
    DeclareProperty(UINT16,             Millisecond);//����
    DeclareProperty(UINT8,              DayOfWeek);  //����
    DeclareProperty(UINT32,             TimeZoneID); //ʱ������
    DeclareMember(TIME_ZONE_INFORMATION, TimeZone);   //ʱ��
public:
    static BOOL      IsLeapYear(UINT32 Year);                            //���������ܡ�����֤����
    static BOOL      IsIllegal(UINT32 Year, UINT32 Month, UINT32 Day);   //���������ܡ�����֤�Ƿ�����
    static UINT32    GetDaysOfYear(UINT32 Year);                         //���������ܡ�����ȡָ����ݵ�����
    static UINT32    GetDayOfYear(UINT32 Year, UINT32 Month, UINT32 Day);//���������ܡ�����ȡָ��������һ���еĵڼ��죬��1��ʼ����
    static CDateTime GetNow(void);                                       //���������ܡ�����ȡ��ǰ����ʱ��
    static CDateTime FromSYSTEMTIME(const SYSTEMTIME& ST);               //���������ܡ�����ȡʱ���SYSTEMTIME
    static CDateTime FromFILETIME(const FILETIME& FT);                   //���������ܡ�����ȡʱ���FILETIME
public:
    static void AddDays(SYSTEMTIME& ST, INT32 Days);//���������ܡ���ϵͳʱ����������
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
    LONGLONG     m_TimeStamp;//ʱ���
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
    static RECT WINAPI ScreenRect(void);           //���������ܡ�����ȡ��Ļ����Χ
    static RECT WINAPI WorkArea(void);             //���������ܡ�����ȡ��Ļ�������򣨲�������������������ΧΪ[left, right)��[top, bottom)
    static RECT WINAPI TrayRect(void);             //���������ܡ�����ȡ����������Χ
public:
    static POINT WINAPI GetCursorPos(void);//���������ܡ�����ȡ��ǰ���λ��
public:
    static BOOL WINAPI SelectFileDialog(HWND Owner, const std::wstring& Title, LPCWSTR Filter, std::wstring& FilePath); //���������ܡ���ѡ���ļ��Ի���
    static BOOL WINAPI SelectFolderDialog(HWND Owner, const std::wstring& Title, std::wstring& FolderPath);             //���������ܡ���ѡ���ļ��жԻ���
};
//========Smart::CText==========================================================
class Smart::CText
{
public:
    static void         WINAPI Split(const std::wstring& Text, wchar_t             Separator, std::vector<std::wstring>& Array);//���������ܡ����ָ��ַ���
    static void         WINAPI Split(const std::wstring& Text, const std::wstring& Separator, std::vector<std::wstring>& Array);//���������ܡ����ָ��ַ���
    static void         WINAPI Split(LPCWSTR Text, WCHAR Separator, LPWSTR Array[], UINT Length);  //���������ܡ����ָ��ַ�����ʹ�ú���FreeSplitArray�ͷ��ַ���������Դ
    static void         WINAPI Split(LPCWSTR Text, LPCWSTR Separator, LPWSTR Array[], UINT Length);//���������ܡ����ָ��ַ�����ʹ�ú���FreeSplitArray�ͷ��ַ���������Դ
    static void         WINAPI FreeSplitArray(LPWSTR Array[], UINT Length);                        //���������ܡ����ͷŷָ��ַ����������ַ�������
    static LPWSTR       WINAPI Trim(LPWSTR Text, size_t Size);                                //���������ܡ���ȥ���ַ���ǰ��հ��ַ�
    static LPWSTR       WINAPI Trim(LPWSTR Text, size_t Size, LPCWSTR Prefix, LPCWSTR Suffix);//���������ܡ���ȥ���ַ���ǰ׺��׺����ȥ��ǰ��հ��ַ�
    static std::wstring WINAPI Trim(const std::wstring& Text);                                //���������ܡ���ȥ���ַ���ǰ��հ��ַ�
    static std::wstring WINAPI Trim(const std::wstring& Text, LPCWSTR Prefix, LPCWSTR Suffix);//���������ܡ���ȥ���ַ���ǰ׺��׺����ȥ��ǰ��հ��ַ�
    static std::string  WINAPI Format(LPCSTR Format, ...);                                    //���������ܡ�����ʽ���ַ���
    static std::wstring WINAPI Format(LPCWSTR Format, ...);                                   //���������ܡ�����ʽ���ַ���
    static std::wstring WINAPI Format_(LPCWSTR Format, va_list Args);                         //���������ܡ�����ʽ���ַ���
    static std::string  WINAPI Replace(const std::string&  Text, const std::string&  OldStr, const std::string&  NewStr);//���������ܡ����ַ����滻    
    static std::wstring WINAPI Replace(const std::wstring& Text, const std::wstring& OldStr, const std::wstring& NewStr);//���������ܡ����ַ����滻
public:
    static BOOL         WINAPI WideCharToMultiByte(UINT CodePage, DWORD Flags, LPCWSTR WideByte, int WSize, LPSTR MultiByte, int MSize);   //���������ܡ������ֽ�ת���ֽڣ�Ŀ���ַ�������С����Ҫ���ַ�����Сʱ����NULL
    static BOOL         WINAPI WideCharToMultiByte(UINT CodePage, DWORD Flags, const std::wstring& WideByteStr, std::string& MultiByteStr);//���������ܡ������ֽ�ת���ֽ�
    static std::string  WINAPI WideCharToMultiByte(UINT CodePage, DWORD Flags, const std::wstring& WideCharStr);                           //���������ܡ������ֽ�ת���ֽ�
    static BOOL         WINAPI MultiByteToWideChar(UINT CodePage, DWORD Flags, LPCSTR MultiByte, int MSize, LPWSTR WideByte, int WSize);   //���������ܡ������ֽ�ת���ֽڣ�Ŀ���ַ�������С����Ҫ���ַ�����Сʱ����NULL
    static BOOL         WINAPI MultiByteToWideChar(UINT CodePage, DWORD Flags, const std::string& MultiByteStr, std::wstring& WideByteStr);//���������ܡ������ֽ�ת���ֽ�
    static std::wstring WINAPI MultiByteToWideChar(UINT CodePage, DWORD Flags, const std::string& MultiByteStr);                           //���������ܡ������ֽ�ת���ֽ�
public:
    inline static BOOL         WINAPI UTF16_to_ANSI(LPCWSTR WideChar, int WSize, LPSTR MultiByte, int Size){ return Smart::CText::WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, WideChar, WSize, MultiByte, Size);};//���������ܡ���UTF16תANSI��Ŀ���ַ�������С����Ҫ���ַ�����Сʱ����NULL
    inline static std::string  WINAPI UTF16_to_ANSI(const std::wstring& WideCharStr)                       { return Smart::CText::WideCharToMultiByte(CP_ACP, WC_NO_BEST_FIT_CHARS, WideCharStr);};                     //���������ܡ���UTF16תANSI
    inline static BOOL         WINAPI ANSI_to_UTF16(LPCSTR MultiByte, int Size, LPWSTR WideChar, int WSize){ return Smart::CText::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, MultiByte, Size, WideChar, WSize);};      //���������ܡ���ANSIתUTF16��Ŀ���ַ�������С����Ҫ���ַ�����Сʱ����NULL
    inline static std::wstring WINAPI ANSI_to_UTF16(const std::string& MultiByteStr)                       { return Smart::CText::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, MultiByteStr);};                          //���������ܡ���ANSIתUTF16
public:
    inline static BOOL         WINAPI UTF16_to_GBK(LPCWSTR WideChar, int WSize, LPSTR MultiByte, int Size){ return Smart::CText::WideCharToMultiByte(936, WC_NO_BEST_FIT_CHARS, WideChar, WSize, MultiByte, Size);};//���������ܡ���UTF16תGBK��Ŀ���ַ�������С����Ҫ���ַ�����Сʱ����NULL
    inline static std::string  WINAPI UTF16_to_GBK(const std::wstring& WideCharStr)                       { return Smart::CText::WideCharToMultiByte(936, WC_NO_BEST_FIT_CHARS, WideCharStr);};                     //���������ܡ���UTF16תGBK
    inline static BOOL         WINAPI GBK_to_UTF16(LPCSTR MultiByte, int Size, LPWSTR WideChar, int WSize){ return Smart::CText::MultiByteToWideChar(936, MB_PRECOMPOSED, MultiByte, Size, WideChar, WSize);};      //���������ܡ���GBKתUTF16��Ŀ���ַ�������С����Ҫ���ַ�����Сʱ����NULL
    inline static std::wstring WINAPI GBK_to_UTF16(const std::string& MultiByteStr)                       { return Smart::CText::MultiByteToWideChar(936, MB_PRECOMPOSED, MultiByteStr);};                          //���������ܡ���GBKתUTF16
public:
    inline static BOOL         WINAPI UTF16_to_BIG5(LPCWSTR WideChar, int WSize, LPSTR MultiByte, int Size){ return Smart::CText::WideCharToMultiByte(950, WC_NO_BEST_FIT_CHARS, WideChar, WSize, MultiByte, Size);};//���������ܡ���UTF16תBIG5��Ŀ���ַ�������С����Ҫ���ַ�����Сʱ����NULL
    inline static std::string  WINAPI UTF16_to_BIG5(const std::wstring& WideCharStr)                       { return Smart::CText::WideCharToMultiByte(950, WC_NO_BEST_FIT_CHARS, WideCharStr);};                     //���������ܡ���UTF16תBIG5
    inline static BOOL         WINAPI BIG5_to_UTF16(LPCSTR MultiByte, int Size, LPWSTR WideChar, int WSize){ return Smart::CText::MultiByteToWideChar(950, MB_PRECOMPOSED, MultiByte, Size, WideChar, WSize);};      //���������ܡ���BIG5תUTF16��Ŀ���ַ�������С����Ҫ���ַ�����Сʱ����NULL
    inline static std::wstring WINAPI BIG5_to_UTF16(const std::string& MultiByteStr)                       { return Smart::CText::MultiByteToWideChar(950, MB_PRECOMPOSED, MultiByteStr);};                          //���������ܡ���BIG5תUTF16
public:
    inline static BOOL         WINAPI UTF16_to_UTF8(LPCWSTR WideChar, int WSize, LPSTR MultiByte, int MSize){ return Smart::CText::WideCharToMultiByte(CP_UTF8, 0, WideChar, WSize, MultiByte, MSize);};//���������ܡ���UTF16תUTF8��Ŀ���ַ�������С����Ҫ���ַ�����Сʱ����NULL
    inline static std::string  WINAPI UTF16_to_UTF8(const std::wstring& WideCharStr)                        { return Smart::CText::WideCharToMultiByte(CP_UTF8, 0, WideCharStr);};                      //���������ܡ���UTF16תUTF8
    inline static BOOL         WINAPI UTF8_to_UTF16(LPCSTR MultiByte, int MSize, LPWSTR WideChar, int WSize){ return Smart::CText::MultiByteToWideChar(CP_UTF8, 0, MultiByte, MSize, WideChar, WSize);};//���������ܡ���UTF8תUTF16��Ŀ���ַ�������С����Ҫ���ַ�����Сʱ����NULL
    inline static std::wstring WINAPI UTF8_to_UTF16(const std::string& MultiByteStr)                        { return Smart::CText::MultiByteToWideChar(CP_UTF8, 0, MultiByteStr);};                     //���������ܡ���UTF8תUTF16
public:
    static std::wstring WINAPI LittleEndianToBigEndian(const std::wstring& Sour){ std::wstring Dest; Smart::CText::LittleEndianToBigEndian(Sour, Dest); return Dest; }//���������ܡ�����С��ת��
    static void         WINAPI LittleEndianToBigEndian(const std::wstring& Sour, std::wstring& Dest);                                                                 //���������ܡ�����С��ת��
    static std::wstring WINAPI BigEndianToLittleEndian(const std::wstring& Sour){ std::wstring Dest; Smart::CText::BigEndianToLittleEndian(Sour, Dest); return Dest; }//���������ܡ�����С��ת��
    static void         WINAPI BigEndianToLittleEndian(const std::wstring& Sour, std::wstring& Dest);                                                                 //���������ܡ�����С��ת��
    static BOOL         WINAPI ToUnicode(const std::string& MText, std::wstring& WText);             //���������ܡ������ֽ�ת���ֽڣ�֧��Gb2312��Big5��GBK��UTF8
    static BOOL         WINAPI ToUnicodeEx(const std::string& MText, std::wstring& WText);           //���������ܡ������ֽ�ת���ֽڣ�֧��Gb2312��Big5��GBK��UTF8��UTF16LE��UTF16BE
public:
    static BOOL WINAPI IsUTF8SpecialByte(BYTE b);                 //���������ܡ����ж��ַ��Ƿ���UTF8�ķ���ʼ�ַ�
    static BOOL WINAPI IsUTF8(const std::string& Text);           //���������ܡ����ж��ı��Ƿ���UTF8����
    static BOOL WINAPI IsGb2312(const std::string& Text);         //���������ܡ����ж��ı��Ƿ���Gb2312����
    static BOOL WINAPI IsBig5(const std::string& Text);           //���������ܡ����ж��ı��Ƿ���Big5����
    static BOOL WINAPI IsGBK(const std::string& Text);            //���������ܡ����ж��ı��Ƿ���GBK����
};
//========Smart::CBlock=========================================================
class Smart::CBlock
{
public:
    CBlock(UINT Size = 0);
    CBlock(const CBlock& Block);
    ~CBlock(void);
    void operator= (const CBlock& Block);                                                                 //���������ܡ�����ֵ����
    BOOL IsEmpty(void) const                   { return this->m_Size == 0; }                              //���������ܡ����ж����ݿ�Ϊ��
    template<typename T> T* GetData(UINT& Size){ Size = this->m_Size%sizeof(T); return (T*)this->m_Data; }//���������ܡ�����ȡ���ݿ�ָ��
public:
    void Append(const PBYTE Data, UINT Size); //���������ܡ����������
    void Adjust(UINT Size);                   //���������ܡ����������ݴ�С
    void Apply(UINT Size);                    //���������ܡ����������ݴ�С��ֻӰ��m_ApplySize��ֵ
    void Clear(void);                         //���������ܡ�����������
public:
    std::string  ToHexA(void);                     //���������ܡ���������ת����HEX�ַ���
    std::wstring ToHexW(void);                     //���������ܡ���������ת����HEX�ַ���
    void         FromHexA(const std::string& Hex); //���������ܡ�����HEX�ַ���ת��������
    void         FromHexW(const std::wstring& Hex);//���������ܡ�����HEX�ַ���ת��������
protected:
    DeclareProperty_R(PBYTE, Data);//����ָ��
    DeclareProperty_R(UINT, Size); //���ݴ�С
    DeclareMember(UINT, ApplySize); //�����С
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
    void Append(PBYTE Data, UINT Size)   { Smart::CBlock::Append(Data, Size); }  //���������ܡ����������
    void Append(const std::string& Text) { Smart::CBlock::Append((const PBYTE)Text.c_str(), Text.size()); }  //���������ܡ����������
    void Append(const std::wstring& Text){ Smart::CBlock::Append((const PBYTE)Text.c_str(), Text.size()*2); }//���������ܡ����������
    void AppendFormat(LPCSTR Format, ...);                                                                   //���������ܡ����������
    void AppendFormat(LPCWSTR Format, ...);                                                                  //���������ܡ����������
public:
    std::string  GetTextA(void)       { return std::string((char*)this->m_Data, this->m_Size); }                             //���������ܡ���������ת�����ı�
    std::wstring GetTextW(void)       { return std::wstring((wchar_t*)this->m_Data, this->m_Size/2); }                       //���������ܡ���������ת�����ı�
    std::wstring GetTextFromANSI(void){ return Smart::CText::ANSI_to_UTF16(std::string(LPSTR(this->m_Data), this->m_Size)); }//���������ܡ�����ȡ�ַ�������
    std::wstring GetTextFromUTF8(void){ return Smart::CText::UTF8_to_UTF16(std::string(LPSTR(this->m_Data), this->m_Size)); }//���������ܡ�����ȡ�ַ�������
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
    virtual BOOL Startup(StartAddress Address, LPVOID Arg = NULL, UINT64 Times = 1, UINT32 Period = 40, UINT32 Delay = 0);//���������ܡ��������̡߳�Times��ִ�д�������Period��ִ�����ڡ���Delay���״�ִ���ӳ١������߳�������������ʱ����������ִ��������
    virtual void Shutdown(BOOL Wait = TRUE);                                                                              //���������ܡ��������߳�
    virtual void WaitforComplete(void);                                                                                   //���������ܡ����ȴ��߳��Զ����
public:
    virtual BOOL     IsCancel(void);                                         //���������ܡ����ж��߳��Ƿ�ȡ��
    virtual BOOL     IsBusy(void);                                           //���������ܡ����ж��߳��Ƿ�æµ
    virtual BOOL     IsIdle(void);                                           //���������ܡ����ж��߳��Ƿ����
protected:
    static UINT WINAPI __ThreadProcess(LPVOID Thread);//���������ܡ�:�߳���ں���
protected:
    Smart::CSection m_Section;
    BOOL            m_Cancel;      //�߳�ʹ��״̬
    UINT            m_ID;          //�߳�ID
    HANDLE          m_Handle[4];   //�߳̾�������¼���0���߳̾������1���߳���������¼��������2��δ���塿��3���߳���ʱ�¼������
    StartAddress    m_StartAddress;//�߳�������ڵ�ַ
    ULONGLONG       m_Times;       //�߳�����ִ�д���
    DWORD           m_Period;      //�߳�����ִ������
    DWORD           m_Delay;       //�߳�������ʱʱ��
public:
    DeclareProperty_RS(LPVOID,             Arg);      //�̲߳���
    DeclareProperty_RS(UINT64,             TimeStamp);//�߳�����ʱ���
public:
    static BOOL StartupGroup(INT32 Size, StartAddress Address, LPVOID Arg = NULL);//���������ܡ�������һ���߳�ִ�������߳��˳��󷵻�
};
//========Smart::CPath==========================================================
class Smart::CPath
{
public:
    static BOOL WINAPI FormatPath(std::wstring& Path);                                        //���������ܡ�����ʽ��·������·���е�'/'�滻��'\\'
    static BOOL WINAPI PathIsDrive(const std::wstring& Path);                                 //���������ܡ����ж�·���Ƿ�Ϊ������
    static BOOL WINAPI PathIsDirectory(const std::wstring& Path);                             //���������ܡ����ж�·���Ƿ�ΪĿ¼
    static BOOL WINAPI PathIsFile(const std::wstring& Path);                                  //���������ܡ����ж�·���Ƿ�Ϊ�ļ�
    static BOOL WINAPI PathIsExists(const std::wstring& Path);                                //���������ܡ����ж�·���Ƿ����
    static BOOL WINAPI PathIsExists(const std::wstring& Path, UINT32 FileSize, BOOL& FSCheck);//���������ܡ����ж�·���Ƿ����
public:
    static BOOL WINAPI GetFileDir(const std::wstring& Path, std::wstring& Dir);                                      //���������ܡ�����ȡ�ļ�Ŀ¼�����ܻ�ȡ��Ŀ¼��·��������"C:\"
    static BOOL WINAPI GetFileName(const std::wstring& Path, std::wstring& FileName);                                //���������ܡ�����ȡ�ļ����ƣ�������չ�����ļ�����һ�������ļ������ļ���ͬ�����ã�
    static BOOL WINAPI GetFileNameNoExtension(const std::wstring& Path, std::wstring& FileName);                     //���������ܡ�����ȡ�ļ����ƣ���������չ����ֻ�������ļ�
    static BOOL WINAPI GetFileExtension(const std::wstring& Path, std::wstring& FileExtension);                      //���������ܡ�����ȡ�ļ���չ����ֻ�������ļ�
    static BOOL WINAPI PathHasExtension(const std::wstring& Path, const std::wstring& Extension);                    //���������ܡ����ж�·��ӵ��ָ����չ��
public:
    static std::wstring WINAPI GetFileDir(const std::wstring& Path)                                    { std::wstring Text; if(GetFileDir(Path, Text))            { return Text;} return std::wstring(); }   //���������ܡ�����ȡ�ļ�Ŀ¼�����ܻ�ȡ��Ŀ¼��·��������"C:\"
    static std::wstring WINAPI GetFileName(const std::wstring& Path)                                   { std::wstring Text; if(GetFileName(Path, Text))           { return Text;} return std::wstring(); }   //���������ܡ�����ȡ�ļ����ƣ�������չ�����ļ�����һ�������ļ������ļ���ͬ�����ã�
    static std::wstring WINAPI GetFileNameNoExtension(const std::wstring& Path)                        { std::wstring Text; if(GetFileNameNoExtension(Path, Text)){ return Text;} return std::wstring(); }   //���������ܡ�����ȡ�ļ����ƣ���������չ����ֻ�������ļ�
    static std::wstring WINAPI GetFileExtension(const std::wstring& Path)                              { std::wstring Text; if(GetFileExtension(Path, Text))      { return Text;} return std::wstring(); }   //���������ܡ�����ȡ�ļ���չ����ֻ�������ļ�
    static std::wstring WINAPI CombineFileName(const std::wstring& Main, const std::wstring& Extension){ return Extension.empty() ? Main : Smart::CText::Format(L"%s.%s", Main.c_str(), Extension.c_str()); }//���������ܡ�������ļ���ͨ����������չ��
public:
    static BOOL WINAPI CreateDir(const std::string& DirPath);              //���������ܡ�������Ŀ¼ͨ��Ŀ¼·��
    static BOOL WINAPI CreateDir(const std::wstring& DirPath);             //���������ܡ�������Ŀ¼ͨ��Ŀ¼·��
    static BOOL WINAPI CreateDirFromFilePath(const std::string& FilePath); //���������ܡ�������Ŀ¼ͨ���ļ�·��
    static BOOL WINAPI CreateDirFromFilePath(const std::wstring& FilePath);//���������ܡ�������Ŀ¼ͨ���ļ�·��
public:
    static BOOL WINAPI MoveFile(const std::wstring& Sour, const std::wstring& Dest);//���������ܡ����ƶ��ļ�
    static BOOL WINAPI DeleteFile(const std::wstring& Sour);                        //���������ܡ���ɾ���ļ�
public:
    static std::wstring WINAPI FilterIllegal(const std::wstring& FileName);//���������ܡ���ȥ���ļ����зǷ��ַ�
    static std::wstring WINAPI GetFilePathWithoutSame(std::wstring& FilePath);                                //���������ܡ�����ȡ�ļ�·��������·���ļ���飬���ھͷ�����·��
    static std::wstring WINAPI GetFilePathNorepeat(const std::wstring& FilePath);//���������ܡ�����ȡ���ظ��ļ�·��
};
//========Smart::CFile==========================================================
class Smart::CFile
{
public:
    CFile(void);
    ~CFile(void);
    BOOL IsOpen(void);                                                                                                                  //���������ܡ����ж��ļ��Ѿ���
    BOOL Open(const std::wstring& FilePath, DWORD DesiredAccess = GENERIC_READ, DWORD ShareMode = FILE_SHARE_READ, BOOL Create = FALSE);//���������ܡ������ļ���Create��û���ļ��Ƿ񴴽���
    void Close(void);                                                                                                                   //���������ܡ����ر��ļ�
    BOOL IsEmpty(void);                                                                                                                 //���������ܡ����ж��ļ��Ƿ�Ϊ��
    BOOL Read(PVOID Data, UINT Size, PUINT RSize = NULL);                                                                               //���������ܡ���������
    void Clear(void);                                                                                                                   //���������ܡ����������
    BOOL Write(BOMType BOMType = BOMType_UTF16LE);                                                                                      //���������ܡ���дBOM����
    BOOL Write(const PBYTE Data, UINT Size);                                                                                            //���������ܡ���д����
    BOOL Write(LPCWSTR Format, ...);                                                                                                    //���������ܡ���д����
public:
    void SavePos(void);                    //���������ܡ��������ļ���дλ��
    void RestorePos(void);                 //���������ܡ����ָ��ļ���дλ��
    UINT GetPos(void);                     //���������ܡ�����ȡ�ļ���дλ��
    void Seek(DWORD MoveMethod, long Move);//���������ܡ�����λ�ļ���дλ��
    void SeekBegin(void);                  //���������ܡ�����λ�ļ���ʼλ��
    void SeekEnd(void);                    //���������ܡ�����λ�ļ���βλ��
protected:
    HANDLE m_File;//�ļ����
    UINT   m_Pos; //�ļ���дλ��
public:
    static BOOL WINAPI FindFile(const std::wstring& FileFilter, std::list<std::wstring>& FileList, BOOL Sub = FALSE);//���������ܡ��������ļ�
public:    
    static BOOL WINAPI GetData(const std::wstring& FilePath, BYTE** Data, DWORD* Size); //���������ܡ�����ȡ�ļ����ݣ������ͨ��ReleaseData�ͷ�
    static void WINAPI ReleaseData(BYTE** Data);                                        //���������ܡ����ͷ��ļ�����
public:
    static BOOL WINAPI ReadFile(const std::wstring& Path, Smart::CBlock* Block);//���������ܡ�����ȡ�ļ�����
    static BOOL WINAPI ReadFile(const std::wstring& Path, std::wstring&  Text); //���������ܡ�����ȡ�ı��ļ�����
    static BOOL WINAPI WriteFile(const std::wstring& Path, const Smart::CBlock& Block);//���������ܡ���д���ļ����ݣ����������ļ�����
    static BOOL WINAPI WriteFile(const std::wstring& Path, const std::wstring&  Text); //���������ܡ���д���ļ����ݣ����������ļ�����
public:
    static BOOL WINAPI WriteFile(LPCWSTR FilePath, void* Data, DWORD Size);  //���������ܡ�:д���ļ�����
    static BOOL WINAPI WriteFile(LPCWSTR FilePath, LPCWSTR Data, DWORD Size);//���������ܡ�:д���ļ�����
    static BOOL WINAPI WriteFile(LPCWSTR FilePath, LPCWSTR Data);            //���������ܡ�:д���ļ�����
    static BOOL WINAPI WriteFile(HANDLE File, const void* Data, DWORD Size);       //���������ܡ�:���ļ�д����
    static BOOL WINAPI WriteFile(HANDLE File, const wchar_t* Data);                //���������ܡ�:���ļ�д����
public:
    static BOOL   WINAPI GetFileSize(const std::string& FilePath, long& FileSize); //���������ܡ�����ȡ�ļ���С
    static BOOL   WINAPI GetFileSize(const std::wstring& FilePath, long& FileSize);//���������ܡ�����ȡ�ļ���С
    static UINT32 WINAPI GetFileSize(const std::wstring& FilePath);                //���������ܡ�����ȡ�ļ���С
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

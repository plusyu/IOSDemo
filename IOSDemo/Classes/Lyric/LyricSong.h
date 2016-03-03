#pragma once
#include ".\..\Header\ISixLyric.h"
#include <string>
#include <vector>
#include "Smart.h"
#include ".\..\ThridLib\FreeICTCLAS\ICTCLAS\Result\Result.h"
#include ".\..\ThridLib\FreeICTCLAS\ICTCLAS\Utility\Utility.h"
//==============================================================================
#ifdef _DEBUG 
//#pragma comment(lib, ".\\..\\ThridLib\\FreeICTCLAS\\ICTCLAS\\Debug\\ICTCLAS.lib")
#pragma comment(lib, "ICTCLAS.lib")
#else
#pragma comment(lib, ".\\..\\ThridLib\\FreeICTCLAS\\ICTCLAS\\Release\\ICTCLAS.lib")
#endif
//==============================================================================
static std::wstring s_Punctuation = L"。！？：；…、，（）“”‘’!?:;@#￥$%…^&*{}[]【】|—_+-*/=.";
//==============================================================================
class CLyricSong;
//========CLyricWord============================================================
class CLyricWord
{
public:
    CLyricWord(const std::wstring& Word, int Time = 0, int Undefine = 0);
    ~CLyricWord(void);
protected:
    DeclareProperty(std::wstring, Word);    //单词文本
    DeclareProperty(int,          Time);    //单词时间
    DeclareProperty(int,          Undefine);//未定义【1：关键词】
};
//========CLyricLine============================================================
template<class T>class CLyricLineT
{
    friend CLyricSong;
protected:
    CLyricLineT(void);
    CLyricLineT(CResult* ICTCLAS, Lyric_LT LLT, const std::wstring& Text);                                                                        //纯歌词行
    CLyricLineT(CResult* ICTCLAS, int BTime, const std::wstring& Text);                                                                           //LRC歌词行
    CLyricLineT(CResult* ICTCLAS, int BTime, int ETime, const std::wstring& Text, std::vector<UINT32>& Times);                                    //KSC歌词行
    CLyricLineT(CResult* ICTCLAS, int BTime, int ETime, std::vector<std::wstring>& Words, std::vector<UINT32>& Times);                            //KRC歌词行
    CLyricLineT(CResult* ICTCLAS, Lyric_LT LLT, Lyric_LV LLV, int BTime, int ETime, std::vector<std::wstring>& Words, std::vector<UINT32>& Times);//KEC歌词行
    ~CLyricLineT(void);
    void SplitText(const std::wstring& Text);//【函数功能】：拆分歌词文本
    BOOL IsBlankLine(void);                  //【函数功能】：判断歌词行为空行，当行文本全为空白字符，即判定为空行
    void Particle(void);                     //【函数功能】：继续将Words中的拆分
    void FindKey(void);                      //【函数功能】：查找歌词行关键词
public:
    void ParticleText(const std::wstring& Text);//【函数功能】：精分歌词文本
    T*   GetWord(int index);                    //【函数功能】：获取指定索引的单词
protected:
    DeclareProperty(CResult*,           ICTCLAS);
    DeclareProperty(Lyric_LT,           LL_Type); //歌词行信息类型标记
    DeclareProperty(Lyric_LV,           LL_Voice);//歌词行信息声音标记
    DeclareProperty(BOOL,               First);   //歌词行信息段首标记
    DeclareProperty(int,                BTime);   //歌词行信息起始时间，单位毫秒
    DeclareProperty(int,                ETime);   //歌词行信息结束时间，单位毫秒
    DeclareProperty_R(std::wstring,     Text);    //歌词行文本
    DeclareProperty_OR(std::vector<T*>, Words);   //歌词行单词
};
//------------------------------------------------------------------------------
template<class T>CLyricLineT<T>::CLyricLineT(void)
    : m_ICTCLAS(NULL)
    , m_LL_Type(LL_Type_Content_Lyric)
    , m_LL_Voice(LL_Voice_None)
    , m_First(FALSE)
    , m_BTime(0)
    , m_ETime(0)
{
}
template<class T>CLyricLineT<T>::CLyricLineT(CResult* ICTCLAS, Lyric_LT LLT, const std::wstring& Text)
    : m_ICTCLAS(ICTCLAS)
    , m_LL_Type(LLT)
    , m_LL_Voice(LL_Voice_None)
    , m_First(FALSE)
    , m_BTime(0)
    , m_ETime(0)
{
    this->ParticleText(Text);
}
template<class T>CLyricLineT<T>::CLyricLineT(CResult* ICTCLAS, int BTime, const std::wstring& Text)
    : m_ICTCLAS(ICTCLAS)
    , m_LL_Type(LL_Type_Content_Lyric)
    , m_LL_Voice(LL_Voice_None)
    , m_First(FALSE)
    , m_BTime(BTime)
    , m_ETime(0)
{
    this->ParticleText(Text);
}
template<class T>CLyricLineT<T>::CLyricLineT(CResult* ICTCLAS, int BTime, int ETime, const std::wstring& Text, std::vector<UINT32>& Times)
    : m_ICTCLAS(ICTCLAS)
    , m_LL_Type(LL_Type_Content_Lyric)
    , m_LL_Voice(LL_Voice_None)
    , m_First(FALSE)
    , m_BTime(BTime)
    , m_ETime(ETime)
{
    this->SplitText(Text);

    for(UINT i = 0, length = this->m_Words.size(); i < length; i++)
    {
        if(i < Times.size())
        {
            this->m_Words[i]->SetTime(Times[i]);
        }
        else
        {
            break;
        }
    }
}
template<class T>CLyricLineT<T>::CLyricLineT(CResult* ICTCLAS, int BTime, int ETime, std::vector<std::wstring>& Words, std::vector<UINT32>& Times)
    : m_ICTCLAS(ICTCLAS)
    , m_LL_Type(LL_Type_Content_Lyric)
    , m_LL_Voice(LL_Voice_None)
    , m_First(FALSE)
    , m_BTime(BTime)
    , m_ETime(ETime)
{
    auto Word = Words.begin();
    auto Time = Times.begin();
    for(; Word != Words.end(); Word++, Time++)
    {
        this->m_Text.append(*Word);
        this->m_Words.push_back(new T(*Word, *Time));
    }
}
template<class T>CLyricLineT<T>::CLyricLineT(CResult* ICTCLAS, Lyric_LT LLT, Lyric_LV LLV, int BTime, int ETime, std::vector<std::wstring>& Words, std::vector<UINT32>& Times)
    : m_ICTCLAS(ICTCLAS)
    , m_LL_Type(LLT)
    , m_LL_Voice(LLV)
    , m_First(FALSE)
    , m_BTime(BTime)
    , m_ETime(ETime)
{
    auto Word = Words.begin();
    auto Time = Times.begin();
    for(; Word != Words.end(); Word++, Time++)
    {
        this->m_Text.append(*Word);
        this->m_Words.push_back(new T(*Word, *Time));
    }
}
template<class T>CLyricLineT<T>::~CLyricLineT(void)
{
    SAFE_CLEAR_VECTOR(this->m_Words);
}
template<class T>void CLyricLineT<T>::SplitText(const std::wstring& Text)
{
#pragma region 清理已有歌词
    this->m_Text.clear();
    SAFE_CLEAR_VECTOR(this->m_Words);
#pragma endregion

#pragma region 过滤非法字符
    for(auto iter = Text.begin(); iter != Text.end(); iter++)
    {
        switch(*iter)
        {
        case 0x0000://'\0':结束符
        case 0x000A://'\n':换行符
        case 0x000D://'\r':回车符
        case 0x0009://'\t':水平制表符
        case 0x000B://'\v':纵向制表符
            break;
        case 0x0020:              this->m_Text.append(L" ");     break;//' ' 单字节空格
        case 0x3000: case 0xA1A1: this->m_Text.append(L"  ");    break;//'　'双字节空格
        default:                  this->m_Text.push_back(*iter); break;
        }
    }
#pragma endregion

#pragma region 拆分歌词单词
    DWORD WType = 0;//当前单词类型【0：未分类】  【2：宽字节单词】【3：单字节单词】
    DWORD CType = 0;//前一字符类型【0：空白字符】【2：宽字节字符】【3：单字节字符】
    DWORD Begin = 0, Length = this->m_Text.size();
    for(DWORD i = 0; i < Length; i++)
    {
        if(this->m_Text[i] == ' ')        { CType = 0; }//空格字符
        else if(this->m_Text[i] >= 0x0080)              //宽字节字符
        {
            if(WType == 0)                                                                            { WType = 2; CType = 2; }
            else               { this->m_Words.push_back(new T(this->m_Text.substr(Begin, i - Begin))); WType = 2; CType = 2; Begin = i; }
        }
        else                                            //单字节字符
        {
            if(WType == 0)                                                                            { WType = 3; CType = 3; }
            else if(WType != 3){ this->m_Words.push_back(new T(this->m_Text.substr(Begin, i - Begin))); WType = 3; CType = 3; Begin = i; }
            else if(CType == 0){ this->m_Words.push_back(new T(this->m_Text.substr(Begin, i - Begin))); WType = 3; CType = 3; Begin = i; }
        }
    }
    this->m_Words.push_back(new T(this->m_Text.substr(Begin, Length - Begin)));
#pragma endregion
}
template<class T>BOOL CLyricLineT<T>::IsBlankLine(void)
{
    if(this->m_Text.empty()){ return TRUE; }
    for(auto iter = this->m_Text.begin(); iter != this->m_Text.end(); iter++)
    {
        switch(*iter)
        {
        case 0x0000://'\0':结束符
        case 0x000A://'\n':换行符
        case 0x000D://'\r':回车符
        case 0x0009://'\t':水平制表符
        case 0x000B://'\v':纵向制表符
        case 0x0020:             //' ' 单字节空格
        case 0x3000: case 0xA1A1://'　'双字节空格
            break;
        default: return FALSE;
        }
    }
    return TRUE;
}
template<class T>void CLyricLineT<T>::Particle(void)
{
    for(auto Word = this->m_Words.begin(); Word != this->m_Words.end(); )
    {
        std::wstring              Text = (*Word)->GetWord();
        int                       Time = (*Word)->GetTime();
        std::vector<std::wstring> Words;
#pragma region 拆分歌词单词
        DWORD                     WType = 0;//当前单词类型【0：未分类】  【1：宽字节单词】【2：单字节单词】【3：标点符号】
        DWORD                     CType = 0;//前一字符类型【0：空白字符】【1：中文字符】  【2：英文字符】  【3：标点符号】
        DWORD                     Begin = 0, Length = Text.size();
        for(DWORD i = 0; i < Length; i++)
        {
            if(Text[i] == ' ')                              { CType = 0; }//空格字符
            else if(s_Punctuation.find(Text[i]) != std::wstring::npos)    //标点符号
            {
                if(WType == 0)     {                                                 WType = 3; CType = 3; }
                else               { Words.push_back(Text.substr(Begin, i - Begin)); WType = 3; CType = 3; Begin = i; }
            }
            else if(Text[i] >= 0x0080)                                    //宽字节字符
            {
                if(WType == 0)     {                                                 WType = 1; CType = 1; }
                else               { Words.push_back(Text.substr(Begin, i - Begin)); WType = 1; CType = 1; Begin = i; }
            }
            else                                                          //单字节字符
            {
                if(WType == 0)     {                                                 WType = 2; CType = 2; }
                else if(WType != 2){ Words.push_back(Text.substr(Begin, i - Begin)); WType = 2; CType = 2; Begin = i; }
                else if(CType == 0){ Words.push_back(Text.substr(Begin, i - Begin)); WType = 2; CType = 2; Begin = i; }
            }
        }
        Words.push_back(Text.substr(Begin, Length - Begin));
#pragma endregion
        if(Words.size() == 1)
        {
            Word++;
        }
        else
        {
            for(auto iter = Words.begin(); iter != Words.end(); iter++, Word++)
            {
                Word = this->m_Words.insert(Word, new CLyricWord(*iter, Time/Words.size()));
            }
            Word = this->m_Words.erase(Word);
        }
    }
}
template<class T>void CLyricLineT<T>::FindKey(void)
{
    CParagraphUnit Paragraph(Smart::CText::UTF16_to_ANSI(this->m_Text));
    this->m_ICTCLAS->ParagraphProcessing(&Paragraph);

    auto Key = Paragraph.m_Words.begin();
    for(auto iter = Key; iter != Paragraph.m_Words.end(); iter++)
    {
        if((*iter)->Word.size() > (*Key)->Word.size())
        {
            Key = iter;
        }
        else if((*iter)->Word.size() > 3)
        {
            if((*iter)->POS == "n")      { Key = iter; }
            else if((*iter)->POS == "v") { Key = iter; }
            else if((*iter)->POS == "vn"){ Key = iter; }
            //else                         { Key = iter; }
        }
        else if((*Key)->Word.size() < 3)
        {
            if((*iter)->POS == "r"){ Key = iter; }
            //if((*iter)->POS == "r")      { if((*Key)->POS != "v" && (*Key)->POS != "vn" && (*Key)->POS != "n" && (*Key)->POS != "r"){ Key = iter; } }
            else if((*iter)->POS == "n") { if((*Key)->POS != "v" && (*Key)->POS != "vn" && (*Key)->POS != "n")                      { Key = iter; } }
            else if((*iter)->POS == "v") { if((*Key)->POS != "v" && (*Key)->POS != "vn")                                            { Key = iter; } }
            else if((*iter)->POS == "vn"){ if((*Key)->POS != "v" && (*Key)->POS != "vn")                                            { Key = iter; } }
        }
    }

    auto Beg = this->m_Words.begin();
    for(auto Word = Paragraph.m_Words.begin(); Word != Paragraph.m_Words.end(); Word++)
    {
        std::wstring Word1 = Smart::CText::ANSI_to_UTF16((*Word)->Word);
        std::wstring Word2;
        for(; Beg != this->m_Words.end(); Beg++)
        {
            Word2.append((*Beg)->GetWord());
            if(Word == Key)                       { (*Beg)->SetUndefine(1); }
            if(Smart::CText::Trim(Word2) == Word1){ Beg++; break; }
        }
        if(Word == Key){ break; }
    }
}
//------------------------------------------------------------------------------
template<class T>void CLyricLineT<T>::ParticleText(const std::wstring& Text)
{
#pragma region 清理已有歌词
    this->m_Text.clear();
    SAFE_CLEAR_VECTOR(this->m_Words);
#pragma endregion

#pragma region 过滤非法字符
    for(auto iter = Text.begin(); iter != Text.end(); iter++)
    {
        switch(*iter)
        {
        case 0x0000://'\0':结束符
        case 0x000A://'\n':换行符
        case 0x000D://'\r':回车符
        case 0x0009://'\t':水平制表符
        case 0x000B://'\v':纵向制表符
            break;
        case 0x0020:              this->m_Text.append(L" ");     break;//' ' 单字节空格
        case 0x3000: case 0xA1A1: this->m_Text.append(L"  ");    break;//'　'双字节空格
        default:                  this->m_Text.push_back(*iter); break;
        }
    }
#pragma endregion

#pragma region 精分歌词单词
    DWORD WType = 0;//当前单词类型【0：未分类】  【1：标点符号】【2：宽字节单词】【3：单字节单词】
    DWORD CType = 0;//前一字符类型【0：空白字符】【1：标点符号】【2：宽字节字符】【3：单字节字符】
    DWORD Begin = 0, Length = this->m_Text.size();
    for(DWORD i = 0; i < Length; i++)
    {
        if(Text[i] == ' ')                                  { CType = 0; }//空白字符
        else if(s_Punctuation.find(Text[i]) != std::wstring::npos)        //标点符号
        {
            if(WType == 0)                                                                    { WType = 1; CType = 1; }
            else               { this->m_Words.push_back(new T(Text.substr(Begin, i - Begin))); WType = 1; CType = 1; Begin = i; }
        }
        else if(Text[i] >= 0x0080)                                        //宽字节字符
        {
            if(WType == 0)                                                                    { WType = 2; CType = 2; }
            else               { this->m_Words.push_back(new T(Text.substr(Begin, i - Begin))); WType = 2; CType = 2; Begin = i; }
        }
        else                                                              //单字节字符
        {
            if(WType == 0)                                                                    { WType = 3; CType = 3; }
            else if(WType != 3){ this->m_Words.push_back(new T(Text.substr(Begin, i - Begin))); WType = 3; CType = 3; Begin = i; }
            else if(CType == 0){ this->m_Words.push_back(new T(Text.substr(Begin, i - Begin))); WType = 3; CType = 3; Begin = i; }
        }
    }
    this->m_Words.push_back(new T(this->m_Text.substr(Begin, Length - Begin), 0));
#pragma endregion
}
template<class T>T*   CLyricLineT<T>::GetWord(int index)
{
    if(index < 0)                              { return FALSE; }
    else if(index >= int(this->m_Words.size())){ return FALSE; }
    else                                       { return this->m_Words[index]; }
}
//------------------------------------------------------------------------------
typedef CLyricLineT<CLyricWord>  CLyricLine;
//========CLyricSong============================================================
class CLyricSong
{
public:
    CLyricSong(void);
    ~CLyricSong(void);
    BOOL Load(const std::wstring& Path);  //【函数功能】：加载歌词文件，支持LRC、KSC、KRC(酷狗)、KEC(六间房)
    BOOL Load(Smart::CTextBlock& Text);   //【函数功能】：加载歌词文件，支持LRC、KSC、KRC(酷狗)、KEC(六间房)
    void Unload(void);                    //【函数功能】：卸载歌词文件
    void FindKey(void);                   //【函数功能】：查找歌词行关键词
public:
    CLyricLine* GetLine(int index);       //【函数功能】：获取指定索引歌词行
    CLyricLine* GetLineFromTime(int Time);//【函数功能】：获取指定时间歌词行
    CLyricLine* GetPrev(CLyricLine* Line);//【函数功能】：获取指定歌词行的前一行
    CLyricLine* GetNext(CLyricLine* Line);//【函数功能】：获取指定歌词行的后一行
public:
    BOOL HasUnique(void);  //【函数功能】：已经设置唯一标识
    BOOL HasSinger(void);  //【函数功能】：已经设置歌手名称
    BOOL HasSongName(void);//【函数功能】：已经设置歌曲名称
protected:
    DeclareMember(CResult*,       ICTCLAS);//分词系统
    DeclareProperty(UINT,         Unique); //歌曲标识
    DeclareProperty(std::wstring, Status); //歌曲状态
protected:
    DeclareProperty(BOOL,                        Success); //歌词状态【TRUE：时间信息完整】【FALSE：信息不完整】
    DeclareProperty(std::wstring,                SongName);//歌曲名称
    DeclareProperty(std::wstring,                Singer);  //歌手名称
    DeclareProperty(std::wstring,                Album);   //歌曲专辑名称
    DeclareProperty(std::wstring,                Editor);  //歌词编辑名称
    DeclareProperty(int,                         Offset);  //歌词时间偏移，单位毫秒
    DeclareProperty_OR(std::vector<CLyricLine*>, Lines);   //歌词数组
public:
    void         SetSimple(const std::wstring& Lyric);                  //【函数功能】：设置歌词文本，歌词文本纯歌词，不带任何格式
    std::wstring GetSimple(void);                                       //【函数功能】：获取歌词文本，每句之间通过\r\n分隔
    BOOL         SaveLRC(const std::wstring& Path);                     //【函数功能】：保存LRC格式歌词
    BOOL         SaveKSC(const std::wstring& Path);                     //【函数功能】：保存KSC格式歌词
    BOOL         SaveKRC(const std::wstring& Path);                     //【函数功能】：保存KRC格式歌词
    BOOL         SaveKEC(const std::wstring& Path, BOOL Encrypt = TRUE);//【函数功能】：保存KEC格式歌词
protected:
    BOOL AnalyzeLRC(const std::wstring& Lyric);//【函数功能】：解析LRC格式歌词
    BOOL AnalyzeKSC(const std::wstring& Lyric);//【函数功能】：解析KSC格式歌词
    BOOL AnalyzeKRC(const std::string&  Lyric);//【函数功能】：解析KRC格式歌词
    BOOL AnalyzeKEC(const std::string&  Lyric);//【函数功能】：解析KEC格式歌词
    void Sort(void);                           //【函数功能】：歌词行排序
    void Filter(void);                         //【函数功能】：歌词行过滤，清除空行，拆分长单词
};

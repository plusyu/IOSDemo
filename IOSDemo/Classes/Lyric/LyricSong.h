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
static std::wstring s_Punctuation = L"����������������������������!?:;@#��$%��^&*{}[]����|��_+-*/=.";
//==============================================================================
class CLyricSong;
//========CLyricWord============================================================
class CLyricWord
{
public:
    CLyricWord(const std::wstring& Word, int Time = 0, int Undefine = 0);
    ~CLyricWord(void);
protected:
    DeclareProperty(std::wstring, Word);    //�����ı�
    DeclareProperty(int,          Time);    //����ʱ��
    DeclareProperty(int,          Undefine);//δ���塾1���ؼ��ʡ�
};
//========CLyricLine============================================================
template<class T>class CLyricLineT
{
    friend CLyricSong;
protected:
    CLyricLineT(void);
    CLyricLineT(CResult* ICTCLAS, Lyric_LT LLT, const std::wstring& Text);                                                                        //�������
    CLyricLineT(CResult* ICTCLAS, int BTime, const std::wstring& Text);                                                                           //LRC�����
    CLyricLineT(CResult* ICTCLAS, int BTime, int ETime, const std::wstring& Text, std::vector<UINT32>& Times);                                    //KSC�����
    CLyricLineT(CResult* ICTCLAS, int BTime, int ETime, std::vector<std::wstring>& Words, std::vector<UINT32>& Times);                            //KRC�����
    CLyricLineT(CResult* ICTCLAS, Lyric_LT LLT, Lyric_LV LLV, int BTime, int ETime, std::vector<std::wstring>& Words, std::vector<UINT32>& Times);//KEC�����
    ~CLyricLineT(void);
    void SplitText(const std::wstring& Text);//���������ܡ�����ָ���ı�
    BOOL IsBlankLine(void);                  //���������ܡ����жϸ����Ϊ���У������ı�ȫΪ�հ��ַ������ж�Ϊ����
    void Particle(void);                     //���������ܡ���������Words�еĲ��
    void FindKey(void);                      //���������ܡ������Ҹ���йؼ���
public:
    void ParticleText(const std::wstring& Text);//���������ܡ������ָ���ı�
    T*   GetWord(int index);                    //���������ܡ�����ȡָ�������ĵ���
protected:
    DeclareProperty(CResult*,           ICTCLAS);
    DeclareProperty(Lyric_LT,           LL_Type); //�������Ϣ���ͱ��
    DeclareProperty(Lyric_LV,           LL_Voice);//�������Ϣ�������
    DeclareProperty(BOOL,               First);   //�������Ϣ���ױ��
    DeclareProperty(int,                BTime);   //�������Ϣ��ʼʱ�䣬��λ����
    DeclareProperty(int,                ETime);   //�������Ϣ����ʱ�䣬��λ����
    DeclareProperty_R(std::wstring,     Text);    //������ı�
    DeclareProperty_OR(std::vector<T*>, Words);   //����е���
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
#pragma region �������и��
    this->m_Text.clear();
    SAFE_CLEAR_VECTOR(this->m_Words);
#pragma endregion

#pragma region ���˷Ƿ��ַ�
    for(auto iter = Text.begin(); iter != Text.end(); iter++)
    {
        switch(*iter)
        {
        case 0x0000://'\0':������
        case 0x000A://'\n':���з�
        case 0x000D://'\r':�س���
        case 0x0009://'\t':ˮƽ�Ʊ��
        case 0x000B://'\v':�����Ʊ��
            break;
        case 0x0020:              this->m_Text.append(L" ");     break;//' ' ���ֽڿո�
        case 0x3000: case 0xA1A1: this->m_Text.append(L"  ");    break;//'��'˫�ֽڿո�
        default:                  this->m_Text.push_back(*iter); break;
        }
    }
#pragma endregion

#pragma region ��ָ�ʵ���
    DWORD WType = 0;//��ǰ�������͡�0��δ���ࡿ  ��2�����ֽڵ��ʡ���3�����ֽڵ��ʡ�
    DWORD CType = 0;//ǰһ�ַ����͡�0���հ��ַ�����2�����ֽ��ַ�����3�����ֽ��ַ���
    DWORD Begin = 0, Length = this->m_Text.size();
    for(DWORD i = 0; i < Length; i++)
    {
        if(this->m_Text[i] == ' ')        { CType = 0; }//�ո��ַ�
        else if(this->m_Text[i] >= 0x0080)              //���ֽ��ַ�
        {
            if(WType == 0)                                                                            { WType = 2; CType = 2; }
            else               { this->m_Words.push_back(new T(this->m_Text.substr(Begin, i - Begin))); WType = 2; CType = 2; Begin = i; }
        }
        else                                            //���ֽ��ַ�
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
        case 0x0000://'\0':������
        case 0x000A://'\n':���з�
        case 0x000D://'\r':�س���
        case 0x0009://'\t':ˮƽ�Ʊ��
        case 0x000B://'\v':�����Ʊ��
        case 0x0020:             //' ' ���ֽڿո�
        case 0x3000: case 0xA1A1://'��'˫�ֽڿո�
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
#pragma region ��ָ�ʵ���
        DWORD                     WType = 0;//��ǰ�������͡�0��δ���ࡿ  ��1�����ֽڵ��ʡ���2�����ֽڵ��ʡ���3�������š�
        DWORD                     CType = 0;//ǰһ�ַ����͡�0���հ��ַ�����1�������ַ���  ��2��Ӣ���ַ���  ��3�������š�
        DWORD                     Begin = 0, Length = Text.size();
        for(DWORD i = 0; i < Length; i++)
        {
            if(Text[i] == ' ')                              { CType = 0; }//�ո��ַ�
            else if(s_Punctuation.find(Text[i]) != std::wstring::npos)    //������
            {
                if(WType == 0)     {                                                 WType = 3; CType = 3; }
                else               { Words.push_back(Text.substr(Begin, i - Begin)); WType = 3; CType = 3; Begin = i; }
            }
            else if(Text[i] >= 0x0080)                                    //���ֽ��ַ�
            {
                if(WType == 0)     {                                                 WType = 1; CType = 1; }
                else               { Words.push_back(Text.substr(Begin, i - Begin)); WType = 1; CType = 1; Begin = i; }
            }
            else                                                          //���ֽ��ַ�
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
#pragma region �������и��
    this->m_Text.clear();
    SAFE_CLEAR_VECTOR(this->m_Words);
#pragma endregion

#pragma region ���˷Ƿ��ַ�
    for(auto iter = Text.begin(); iter != Text.end(); iter++)
    {
        switch(*iter)
        {
        case 0x0000://'\0':������
        case 0x000A://'\n':���з�
        case 0x000D://'\r':�س���
        case 0x0009://'\t':ˮƽ�Ʊ��
        case 0x000B://'\v':�����Ʊ��
            break;
        case 0x0020:              this->m_Text.append(L" ");     break;//' ' ���ֽڿո�
        case 0x3000: case 0xA1A1: this->m_Text.append(L"  ");    break;//'��'˫�ֽڿո�
        default:                  this->m_Text.push_back(*iter); break;
        }
    }
#pragma endregion

#pragma region ���ָ�ʵ���
    DWORD WType = 0;//��ǰ�������͡�0��δ���ࡿ  ��1�������š���2�����ֽڵ��ʡ���3�����ֽڵ��ʡ�
    DWORD CType = 0;//ǰһ�ַ����͡�0���հ��ַ�����1�������š���2�����ֽ��ַ�����3�����ֽ��ַ���
    DWORD Begin = 0, Length = this->m_Text.size();
    for(DWORD i = 0; i < Length; i++)
    {
        if(Text[i] == ' ')                                  { CType = 0; }//�հ��ַ�
        else if(s_Punctuation.find(Text[i]) != std::wstring::npos)        //������
        {
            if(WType == 0)                                                                    { WType = 1; CType = 1; }
            else               { this->m_Words.push_back(new T(Text.substr(Begin, i - Begin))); WType = 1; CType = 1; Begin = i; }
        }
        else if(Text[i] >= 0x0080)                                        //���ֽ��ַ�
        {
            if(WType == 0)                                                                    { WType = 2; CType = 2; }
            else               { this->m_Words.push_back(new T(Text.substr(Begin, i - Begin))); WType = 2; CType = 2; Begin = i; }
        }
        else                                                              //���ֽ��ַ�
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
    BOOL Load(const std::wstring& Path);  //���������ܡ������ظ���ļ���֧��LRC��KSC��KRC(�ṷ)��KEC(���䷿)
    BOOL Load(Smart::CTextBlock& Text);   //���������ܡ������ظ���ļ���֧��LRC��KSC��KRC(�ṷ)��KEC(���䷿)
    void Unload(void);                    //���������ܡ���ж�ظ���ļ�
    void FindKey(void);                   //���������ܡ������Ҹ���йؼ���
public:
    CLyricLine* GetLine(int index);       //���������ܡ�����ȡָ�����������
    CLyricLine* GetLineFromTime(int Time);//���������ܡ�����ȡָ��ʱ������
    CLyricLine* GetPrev(CLyricLine* Line);//���������ܡ�����ȡָ������е�ǰһ��
    CLyricLine* GetNext(CLyricLine* Line);//���������ܡ�����ȡָ������еĺ�һ��
public:
    BOOL HasUnique(void);  //���������ܡ����Ѿ�����Ψһ��ʶ
    BOOL HasSinger(void);  //���������ܡ����Ѿ����ø�������
    BOOL HasSongName(void);//���������ܡ����Ѿ����ø�������
protected:
    DeclareMember(CResult*,       ICTCLAS);//�ִ�ϵͳ
    DeclareProperty(UINT,         Unique); //������ʶ
    DeclareProperty(std::wstring, Status); //����״̬
protected:
    DeclareProperty(BOOL,                        Success); //���״̬��TRUE��ʱ����Ϣ��������FALSE����Ϣ��������
    DeclareProperty(std::wstring,                SongName);//��������
    DeclareProperty(std::wstring,                Singer);  //��������
    DeclareProperty(std::wstring,                Album);   //����ר������
    DeclareProperty(std::wstring,                Editor);  //��ʱ༭����
    DeclareProperty(int,                         Offset);  //���ʱ��ƫ�ƣ���λ����
    DeclareProperty_OR(std::vector<CLyricLine*>, Lines);   //�������
public:
    void         SetSimple(const std::wstring& Lyric);                  //���������ܡ������ø���ı�������ı�����ʣ������κθ�ʽ
    std::wstring GetSimple(void);                                       //���������ܡ�����ȡ����ı���ÿ��֮��ͨ��\r\n�ָ�
    BOOL         SaveLRC(const std::wstring& Path);                     //���������ܡ�������LRC��ʽ���
    BOOL         SaveKSC(const std::wstring& Path);                     //���������ܡ�������KSC��ʽ���
    BOOL         SaveKRC(const std::wstring& Path);                     //���������ܡ�������KRC��ʽ���
    BOOL         SaveKEC(const std::wstring& Path, BOOL Encrypt = TRUE);//���������ܡ�������KEC��ʽ���
protected:
    BOOL AnalyzeLRC(const std::wstring& Lyric);//���������ܡ�������LRC��ʽ���
    BOOL AnalyzeKSC(const std::wstring& Lyric);//���������ܡ�������KSC��ʽ���
    BOOL AnalyzeKRC(const std::string&  Lyric);//���������ܡ�������KRC��ʽ���
    BOOL AnalyzeKEC(const std::string&  Lyric);//���������ܡ�������KEC��ʽ���
    void Sort(void);                           //���������ܡ������������
    void Filter(void);                         //���������ܡ�������й��ˣ�������У���ֳ�����
};

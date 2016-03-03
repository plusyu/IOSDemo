
#include "LyricSong.h"
//========CLyricWord============================================================
CLyricWord::CLyricWord(const std::wstring& Word, int Time, int Undefine)
    : m_Word(Word)
    , m_Time(Time)
    , m_Undefine(Undefine)
{
}
CLyricWord::~CLyricWord(void)
{
}
//========CLyricSong============================================================
CLyricSong::CLyricSong(void)
    : m_ICTCLAS(NULL)
    , m_Unique(0)
    , m_Success(FALSE)
    , m_Offset(0)
{
    this->m_ICTCLAS = new CResult(Smart::CText::UTF16_to_ANSI(Smart::CText::Format(L"%sData\\", Smart::Shell::CModule::GetEXECDir().c_str())).c_str());
}
CLyricSong::~CLyricSong(void)
{
    this->Unload();
    SAFE_DELETE(this->m_ICTCLAS);
}
BOOL CLyricSong::Load(const std::wstring& Path)
{
    this->Unload();
    Smart::CTextBlock Data;
    if(!Smart::CFile::ReadFile(Path, &Data))
    {
        return FALSE;
    }
    else
    {
        return this->Load(Data);
    }
}
BOOL CLyricSong::Load(Smart::CTextBlock& Text)
{
    this->Unload();
    std::string  MText = Text.GetTextA();
    std::wstring WText;

    if(MText.find("krc") == 0)     { if(this->AnalyzeKRC(MText)){ goto SUCCESS; }else{ return FALSE; } }
    if(MText.find("kec") == 0)     { if(this->AnalyzeKEC(MText)){ goto SUCCESS; }else{ return FALSE; } }
    if(!Smart::CText::ToUnicodeEx(MText, WText))                                     { return FALSE; }
    if(WText.find(L"karaoke") == 0){ if(this->AnalyzeKSC(WText)){ goto SUCCESS; }else{ return FALSE; } }
    if(WText.find(L"[")       == 0){ if(this->AnalyzeLRC(WText)){ goto SUCCESS; }else{ return FALSE; } }

SUCCESS:
    this->m_Success   = TRUE;
    return TRUE;
}
void CLyricSong::Unload(void)
{
    this->m_Success = FALSE;
    this->m_SongName.clear();
    this->m_Singer.clear();
    this->m_Album.clear();
    this->m_Editor.clear();
    this->m_Offset  = 0;
    SAFE_CLEAR_VECTOR(this->m_Lines);
}
void CLyricSong::FindKey(void)
{
    for(auto iter = this->m_Lines.begin(); iter != this->m_Lines.end(); iter++)
    {
        (*iter)->FindKey();
    }
}
//------------------------------------------------------------------------------
CLyricLine* CLyricSong::GetLine(int index)
{
    if(index < 0)                              { return NULL; }
    else if(index >= int(this->m_Lines.size())){ return NULL; }
    else                                       { return this->m_Lines[index]; }
}
CLyricLine* CLyricSong::GetLineFromTime(int Time)
{
    for(UINT i = 0, length = this->m_Lines.size(); i < length; i++)
    {
        CLyricLine* Curr = this->m_Lines[i];
        if(Time <= Curr->GetBTime())
        {
            if(i == 0)                                           { return Curr; }
            CLyricLine* Prev = this->m_Lines[i - 1];
            if(Curr->GetBTime() - Prev->GetETime() <=  1000)     { return Curr; }
            else if(Curr->GetBTime() - Prev->GetETime() <=  3000){ return (Time <= Curr->GetBTime() - 1000) ? Prev : Curr; }
            else if(Curr->GetBTime() - Prev->GetETime() <=  5000){ return (Time <= Prev->GetETime() + 2000) ? Prev : Curr; }
            else if(Curr->GetBTime() - Prev->GetETime() <=  6000){ return (Time <= Curr->GetBTime() - 3000) ? Prev : Curr; }
            else                                                 { return (Time <= Prev->GetETime() + 3000) ? Prev : Curr; }
        }
        else if(Time <= Curr->GetETime())                        { return Curr; }
    }
    return this->m_Lines.empty() ? NULL : this->m_Lines.back();
}
CLyricLine* CLyricSong::GetPrev(CLyricLine* Line)
{
    for(auto iter = this->m_Lines.begin(); iter != this->m_Lines.end(); iter++)
    {
        if(*iter == Line)
        {
            return iter == this->m_Lines.begin() ? NULL : *--iter;
        }
    }
    return NULL;
}
CLyricLine* CLyricSong::GetNext(CLyricLine* Line)
{
    for(auto iter = this->m_Lines.begin(); iter != this->m_Lines.end(); iter++)
    {
        if(*iter == Line)
        {
            return ++iter == this->m_Lines.end() ? NULL : *iter;
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------
BOOL CLyricSong::HasUnique(void)
{
    return this->m_Unique != 0;
}
BOOL CLyricSong::HasSinger(void)
{
    return this->m_Singer.empty() ? FALSE : TRUE;
}
BOOL CLyricSong::HasSongName(void)
{
    return this->m_SongName.empty() ? FALSE : TRUE;
}
//------------------------------------------------------------------------------
void         CLyricSong::SetSimple(const std::wstring& Lyric)
{
    this->m_Success = FALSE;
    this->m_Offset  = 0;
    SAFE_CLEAR_VECTOR(this->m_Lines);

    for(UINT i = 0, begin = 0, length = Lyric.size(); i <= length; i++)
    {
        if(Lyric[i] != '\r' && Lyric[i] != '\n' && Lyric[i] != '\0'){ }         //正常字符
        else if(i == begin)                                         { begin++; }//起始字符不是有效字符
        else
        {
            this->m_Lines.push_back(new CLyricLine(this->m_ICTCLAS, LL_Type_Content_Lyric, Lyric.substr(begin, i - begin)));
            begin = i + 1;
            while(begin <= length && (Lyric[begin] == '\r' || Lyric[begin] == '\n' || Lyric[begin] == '\0'))
            {
                i++;
                begin++;
            }
        }
    }
}
std::wstring CLyricSong::GetSimple(void)
{
    std::wstring Lyric;
    for(auto iter = this->m_Lines.begin(); iter != this->m_Lines.end(); iter++)
    {
        Lyric.append((*iter)->GetText());
        Lyric.append(L"\r\n");
    }
    return Lyric;
}
BOOL         CLyricSong::SaveLRC(const std::wstring& Path)
{
    Smart::CFile File;
    if(!File.Open(Path, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, TRUE)){ return FALSE; }
    File.Clear();
    File.Write(BOMType_UTF16LE);
    File.Write(L"[ti:%s]\r\n", this->m_SongName.c_str());
    File.Write(L"[ar:%s]\r\n", this->m_Singer.c_str());
    File.Write(L"[al:%s]\r\n", this->m_Album.c_str());
    File.Write(L"[by:%s]\r\n", this->m_Editor.c_str());
    File.Write(L"[offset:%d]\r\n", this->m_Offset);
    for(auto Line = this->m_Lines.begin(); Line != this->m_Lines.end(); Line++)
    {
        File.Write(L"[%02d:%02d.%03d]%s\r\n", (*Line)->GetBTime()/1000/60, (*Line)->GetBTime()/1000%60, (*Line)->GetBTime()%1000, (*Line)->GetText().c_str());
    }
    return TRUE;
}
BOOL         CLyricSong::SaveKSC(const std::wstring& Path)
{
    Smart::CFile File;
    if(!File.Open(Path, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, TRUE)){ return FALSE; }
    File.Clear();
    File.Write(BOMType_UTF16LE);
    File.Write(L"karaoke := CreateKaraokeObject;\r\n");
    File.Write(L"karaoke.rows := 2;\r\n");
    File.Write(L"karaoke.clear;\r\n");
    File.Write(L"karaoke.songname := '%s';\r\n", this->m_SongName.c_str());
    File.Write(L"karaoke.singer := '%s';\r\n", this->m_Singer.c_str());
    for(auto Line = this->m_Lines.begin(); Line != this->m_Lines.end(); Line++)
    {
        std::wstring Text,Time;
        for(auto iterWord = (*Line)->m_Words.begin(); iterWord != (*Line)->m_Words.end(); iterWord++)
        {
            Text.append((*iterWord)->GetWord());
            Time.append(Smart::CText::Format(L"%s%d", Time.empty() ? L"" : L",", (*iterWord)->GetTime()));
        }
        if(!Text.empty()){ File.Write(Smart::CText::Format(L"karaoke.add('%03d:%02d.%03d','%03d:%02d.%03d','%s','%s');\r\n",
            (*Line)->GetBTime()/60000, (*Line)->GetBTime()%60000/1000, (*Line)->GetBTime()%1000,
            (*Line)->GetETime()/60000, (*Line)->GetETime()%60000/1000, (*Line)->GetETime()%1000,
            Text.c_str(),
            Time.c_str()).c_str());}
    }
    return TRUE;
}
BOOL         CLyricSong::SaveKRC(const std::wstring& Path)
{
    FILE* File = NULL;
    if(!Smart::CPath::CreateDirFromFilePath(Path))  { return FALSE;}//路径格式不正确或创建路径失败
    if(::_wfopen_s(&File, Path.c_str(), L"wb") != 0){ return FALSE;}//打开文件失败
    ::fclose(File);
    return FALSE;
}
BOOL         CLyricSong::SaveKEC(const std::wstring& Path, BOOL Encrypt)
{
    FILE* File = NULL;
    if(!Smart::CPath::CreateDirFromFilePath(Path))  { return FALSE;}//路径格式不正确或创建路径失败
    if(::_wfopen_s(&File, Path.c_str(), L"wb") != 0){ return FALSE;}//打开文件失败

    Smart::CTextBlock Text;
    Text.AppendFormat(L"[ti:%s]\n", this->m_SongName.c_str());
    Text.AppendFormat(L"[al:%s]\n", this->m_Album.c_str());
    Text.AppendFormat(L"[ar:%s]\n", this->m_Singer.c_str());
    Text.AppendFormat(L"[by:%s]\n", this->m_Editor.c_str());
    Text.AppendFormat(L"[vn:六间房歌词编辑器1.0]\n");
    for(auto iterLine = this->m_Lines.begin(); iterLine != this->m_Lines.end(); iterLine++)
    {
        Text.AppendFormat(L"[%d,%d,%d,%d]", (*iterLine)->GetBTime(), (*iterLine)->GetETime(), (*iterLine)->GetLL_Type(), (*iterLine)->GetLL_Voice());
        for(auto iterWord = (*iterLine)->m_Words.begin(); iterWord != (*iterLine)->m_Words.end(); iterWord++)
        {
            Text.AppendFormat(L"<%d,%d>%s", (*iterWord)->GetTime(), (*iterWord)->GetUndefine(), (*iterWord)->GetWord().c_str());
        }
        Text.AppendFormat(L"\n");
    }
    if(Encrypt)
    {
        Smart::CBlock Sour;
        Smart::CBlock Dest;
        Smart::Compress::ZLIB::Compress(Text, Sour);
        BYTE Key[8] = { '6', 'r', 'o', 'o', 'm', 's', 'z', 'l' };
        Smart::Security::DES8::Encode(Key, &Sour, &Dest);

        ::fwrite("kec0", 4, 1, File);
        ::fwrite(Dest.GetData(), Dest.GetSize(), 1, File);
    }
    else
    {
        ::fwrite(TEXT_BOM_UTF16LE, 2, 1, File);
        ::fwrite(Text.GetData(), Text.GetSize(), 1, File);
    }
    ::fclose(File);
    return TRUE;
}
//------------------------------------------------------------------------------
BOOL CLyricSong::AnalyzeLRC(const std::wstring& Lyric)
{
    std::list<MatchResult> LineMatches;
    std::list<MatchResult> TimeMatches;
    if(!Smart::CRegex::Match(Lyric, L"((?:\\[(?:\\d+:)?\\d+(?:[\\.:]\\d+)?\\])+)([^\\r\\n]*)", LineMatches)){ return FALSE; }
    Smart::CRegex::GetMatchGroup(Lyric, L"\\[ti:(.*?)\\][\r\n]",     1, this->m_SongName);
    Smart::CRegex::GetMatchGroup(Lyric, L"\\[ar:(.*?)\\][\r\n]",     1, this->m_Singer);
    Smart::CRegex::GetMatchGroup(Lyric, L"\\[al:(.*?)\\][\r\n]",     1, this->m_Album);
    Smart::CRegex::GetMatchGroup(Lyric, L"\\[by:(.*?)\\][\r\n]",     1, this->m_Editor);
    Smart::CRegex::GetMatchGroup(Lyric, L"\\[offset:(.*?)\\][\r\n]", 1, this->m_Offset);
    for(auto iter = LineMatches.begin(); iter != LineMatches.end(); iter++)
    {
        std::wstring Time = Smart::CRegex::GetGroup(Lyric, *iter, 1);
        std::wstring Text = Smart::CRegex::GetGroup(Lyric, *iter, 2);
        Smart::CRegex::Match(Time, L"\\[(?:(\\d+):)?(\\d+)(?:[\\.:](\\d+))?\\]", TimeMatches);
        for(auto j = TimeMatches.begin(); j != TimeMatches.end(); j++)
        {
            int A = 0, B = 0, C = 0;
            if(!Smart::CRegex::GetGroup(Time, *j, 1, A)){ A = 0; }
            if(!Smart::CRegex::GetGroup(Time, *j, 2, B)){ B = 0; }
            if(!Smart::CRegex::GetGroup(Time, *j, 3, C)){ C = 0; }
            this->m_Lines.push_back(new CLyricLine(this->m_ICTCLAS, A*60*1000 + B*1000 + C, Text));
        }
    }

    this->Sort();
    int ETime = 0x0FFFFFFF, Interal = 0;
    std::vector<CLyricWord*>* Words = NULL;
    for(auto iter = this->m_Lines.rbegin(); iter != this->m_Lines.rend(); iter++)
    {
        (*iter)->SetETime(max((*iter)->GetBTime(), min((*iter)->GetBTime() + 30000, ETime)));//当结束时间小于起始时间，将结束时间设置为开始时间，当结束时间与开始时间的时间差大于30000时，结束时间设置为开始时间加30000
        Words   =  (*iter)->GetWords();
        Interal = ((*iter)->GetETime() - (*iter)->GetBTime())/Words->size();
        for(auto Word = Words->begin(); Word != Words->end(); Word++)
        {
            (*Word)->SetTime(Interal);
        }
        ETime = (*iter)->GetBTime();
    }
    this->Filter();

    return TRUE;
}
BOOL CLyricSong::AnalyzeKSC(const std::wstring& Lyric)
{
    std::list<MatchResult> LineMatches;
    if(!Smart::CRegex::Match(Lyric, L"karaoke\\.add\\( *\' *(\\d+):(\\d+).(\\d+) *\' *, *\' *(\\d+):(\\d+).(\\d+) *\' *, *\' *(.*?)\' *, *\'([0-9, ]+)\' *\\) *;", LineMatches)){ return FALSE; }

    Smart::CRegex::GetMatchGroup(Lyric, L"karaoke\\.songname *:= *'(.*?)' *;", 1, this->m_SongName);
    Smart::CRegex::GetMatchGroup(Lyric, L"karaoke\\.singer *:= *'(.*?)' *;",   1, this->m_Singer);
    for(auto iter = LineMatches.begin(); iter != LineMatches.end(); iter++)
    {
        int A = 0, B = 0, C = 0, D = 0, E = 0, F = 0;
        std::wstring           TLine;
        std::wstring           TTime;
        std::list<MatchResult> TimeMatches;
        std::vector<UINT32>    TimeWords;

        Smart::CRegex::GetGroup(Lyric, *iter, 1, A);
        Smart::CRegex::GetGroup(Lyric, *iter, 2, B);
        Smart::CRegex::GetGroup(Lyric, *iter, 3, C);
        Smart::CRegex::GetGroup(Lyric, *iter, 4, D);
        Smart::CRegex::GetGroup(Lyric, *iter, 5, E);
        Smart::CRegex::GetGroup(Lyric, *iter, 6, F);
        Smart::CRegex::GetGroup(Lyric, *iter, 7, TLine);
        Smart::CRegex::GetGroup(Lyric, *iter, 8, TTime);
        if(Smart::CRegex::Match(TTime, L"\\d+", TimeMatches))
        {
            for(auto iter = TimeMatches.begin(); iter != TimeMatches.end(); iter++)
            {
                UINT32 Time = 0;
                Smart::CRegex::GetGroup(TTime, *iter, 0, Time);
                TimeWords.push_back(Time);
            }
        }

        this->m_Lines.push_back(new CLyricLine(this->m_ICTCLAS, A*60*1000 + B*1000 + C, D*60*1000 + E*1000 + F, TLine, TimeWords));
    }

    this->Sort();
    this->Filter();
    return TRUE;
}
BOOL CLyricSong::AnalyzeKRC(const std::string&  Lyric)
{
    //BYTE         Bytes[] = { 64, 71, 97, 119, 94, 50, 116, 71, 81, 54, 49, 45, 206, 210, 110, 105 };
    BYTE         Bytes[] = { 0x40, 0x47, 0x61, 0x77, 0x5E, 0x32, 0x74, 0x47, 0x51, 0x36, 0x31, 0x2D, 0xCE, 0xD2, 0x6E, 0x69 };
    std::string  Text = Lyric.substr(4);
    for(UINT32 i = 0; i < Text.size(); i++)
    {
        Text[i] ^= Bytes[i % sizeof(Bytes)];
    }


    Smart::CTextBlock Sour(Text);
    Smart::CTextBlock Dest;
    if(!Smart::Compress::ZLIB::Uncompress(Sour, Dest))                           { return FALSE; }
    std::wstring Text1 = Dest.GetTextFromUTF8();

    std::list<MatchResult> LineMatches;
    std::list<MatchResult> WordMatches;
    if(!Smart::CRegex::Match(Text1, L"\\[(\\d+),(\\d+)\\](.+?)[\r\n]", LineMatches)){ return FALSE; }
    Smart::CRegex::GetMatchGroup(Text1, L"\\[ti:(.*?)\\][\r\n]",     1, this->m_SongName);
    Smart::CRegex::GetMatchGroup(Text1, L"\\[ar:(.*?)\\][\r\n]",     1, this->m_Singer);
    Smart::CRegex::GetMatchGroup(Text1, L"\\[al:(.*?)\\][\r\n]",     1, this->m_Album);
    Smart::CRegex::GetMatchGroup(Text1, L"\\[by:(.*?)\\][\r\n]",     1, this->m_Editor);
    Smart::CRegex::GetMatchGroup(Text1, L"\\[offset:(.*?)\\][\r\n]", 1, this->m_Offset);
    for(auto iter = LineMatches.begin(); iter != LineMatches.end(); iter++)
    {
        std::wstring BTime = Smart::CRegex::GetGroup(Text1, *iter, 1);
        std::wstring ETime = Smart::CRegex::GetGroup(Text1, *iter, 2);
        std::wstring LText = Smart::CRegex::GetGroup(Text1, *iter, 3);
        Smart::CRegex::Match(LText, L"<(\\d+),(\\d+),(\\d+)>([\\s\\S]*?)(?=<(?:\\d+),(?:\\d+),(?:\\d+)>|$)", WordMatches);

        std::vector<std::wstring> TextWords;
        std::vector<UINT32>       TimeWords;
        for(auto iterWord = WordMatches.begin(); iterWord != WordMatches.end(); iterWord++)
        {
            UINT32 Time = 0;
            Smart::CRegex::GetGroup(LText, *iterWord, 2, Time);
            TimeWords.push_back(Time);
            TextWords.push_back(Smart::CRegex::GetGroup(LText, *iterWord, 4));
        }
        this->m_Lines.push_back(new CLyricLine(this->m_ICTCLAS, ::_wtoi(BTime.c_str()), ::_wtoi(BTime.c_str()) + ::_wtoi(ETime.c_str()), TextWords, TimeWords));
    }

    this->Sort();
    this->Filter();
    return TRUE;
}
BOOL CLyricSong::AnalyzeKEC(const std::string&  Lyric)
{
    std::wstring Text;
    if(Lyric[3] == '0')
    {
        BYTE Key[8] = { '6', 'r', 'o', 'o', 'm', 's', 'z', 'l' };
        Smart::CTextBlock Sour(Lyric.substr(4));
        Smart::CTextBlock Dest;
        Smart::CTextBlock Data;
        Smart::Security::DES8::Decode(Key, &Sour, &Dest);
        Smart::Compress::ZLIB::Uncompress(Dest, Data);
        Text = Data.GetTextW();
    }

    std::list<MatchResult> LineMatches;
    std::list<MatchResult> WordMatches;
    if(!Smart::CRegex::Match(Text, L"\\[(\\d+),(\\d+),(\\d+),(\\d+)\\](.+?)(?:[\\r\\n]|$)", LineMatches)){ return FALSE; }
    Smart::CRegex::GetMatchGroup(Text, L"\\[ti:(.*?)\\][\r\n]",     1, this->m_SongName);
    Smart::CRegex::GetMatchGroup(Text, L"\\[ar:(.*?)\\][\r\n]",     1, this->m_Singer);
    Smart::CRegex::GetMatchGroup(Text, L"\\[al:(.*?)\\][\r\n]",     1, this->m_Album);
    Smart::CRegex::GetMatchGroup(Text, L"\\[by:(.*?)\\][\r\n]",     1, this->m_Editor);
    Smart::CRegex::GetMatchGroup(Text, L"\\[offset:(.*?)\\][\r\n]", 1, this->m_Offset);
    for(auto iter = LineMatches.begin(); iter != LineMatches.end(); iter++)
    {
        Lyric_LT      LLT;
        Lyric_LV      LLV;
        std::wstring BTime = Smart::CRegex::GetGroup(Text, *iter, 1);
        std::wstring ETime = Smart::CRegex::GetGroup(Text, *iter, 2);
        Smart::CRegex::GetGroup(Text, *iter, 3, *((int*)&LLT));
        Smart::CRegex::GetGroup(Text, *iter, 4, *((int*)&LLV));
        std::wstring LText = Smart::CRegex::GetGroup(Text, *iter, 5);
        Smart::CRegex::Match(LText, L"<(\\d+),(\\d+)>([\\s\\S]*?)(?=<(?:\\d+),(?:\\d+>|$)", WordMatches);

        std::vector<std::wstring> TextWords;
        std::vector<UINT32>       TimeWords;
        for(auto iterWord = WordMatches.begin(); iterWord != WordMatches.end(); iterWord++)
        {
            UINT32 Time = 0;
            Smart::CRegex::GetGroup(LText, *iterWord, 1, Time);
            TimeWords.push_back(Time);
            TextWords.push_back(Smart::CRegex::GetGroup(LText, *iterWord, 3));
        }
        this->m_Lines.push_back(new CLyricLine(this->m_ICTCLAS, LLT, LLV, ::_wtoi(BTime.c_str()), ::_wtoi(BTime.c_str()) + ::_wtoi(ETime.c_str()), TextWords, TimeWords));
    }

    this->Sort();
    this->Filter();
    return TRUE;
}
void CLyricSong::Sort(void)
{
    if(!this->m_Lines.empty())
    {
        for(UINT i = 0, x = this->m_Lines.size(), y = x - 1; i < y; i++)
        {
            CLyricLine* Curr = this->m_Lines[i];
            for(UINT j = i + 1; j < x; j++)
            {
                CLyricLine* Next = this->m_Lines[j];
                if(Curr->GetBTime() >= Next->GetBTime())
                {
                    this->m_Lines[i] = Next;
                    this->m_Lines[j] = Curr;
                    Curr = Next;
                }
            }
        }
    }
}
void CLyricSong::Filter(void)
{
    for(auto iter = this->m_Lines.begin(); iter != this->m_Lines.end();)
    {
        if((*iter)->IsBlankLine())
        {
            delete *iter;
            iter = this->m_Lines.erase(iter);
        }
        else
        {
            iter++;
        }
    }

    int PrevE = 0;
    for(auto iter = this->m_Lines.begin(); iter != this->m_Lines.end(); iter++)
    {
        (*iter)->SetFirst(((*iter)->GetBTime() >= (PrevE + Lyric_FirstLine_Enactment)));
        PrevE = max(PrevE, (*iter)->GetETime());
        (*iter)->Particle();
    }
}


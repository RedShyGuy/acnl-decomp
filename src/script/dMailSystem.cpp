#include "script/dMailSystem.h"
#include "script/dIMailRecept.h"
#include "script/dMailInsert.h"
#include "script/dMailMeasure.h"
#include "script/dMailPhrase.h"
#include "script/dMailSeq.h"
#include "script/dMgr.h"
#include "script/dTalkSystem.h"
#include "Other/dRegion.h"
#include "sead/seadStringFormatter.h"
#include "time/time_Api.h"

#include <new>

namespace script {
// (inline)
inline MailInsert::MailInsert(MailSystem* pOwner) : Insert(pOwner->m_pMeasure), m_pOwner(pOwner) {}

// (inline)
inline bool MailSystem::LoadFile()
{
    return m_File.m_Loader.Load(m_File.m_Path, Mgr::s_pMailHeap, 0) && m_File.m_Loader.m_IsLoaded;
}

// (inline)
inline bool MailSystem::MakeText(s32 label, bool (MailSeq::*func)(const char16* const* text))
{
    FormatLabelNumber(&m_Label, label);
    const char16* text = static_cast<const char16*>(m_Message.GetTextByLabel(m_Label.getCString()));
    return text != NULL && (m_pSeq->*func)(&text);
}

// 0x00600F20 (name is ours)
void MailSystem::State34()
{
    if (MakeText(m_pRecept->m_Labels[1], &MailSeq::RunText)) {
        m_pRecept->m_pBodyWord->Copy(m_pSeq->m_Text);
    } else {
        m_IsSuccess = 0;
    }
    m_State = 35;
}

// 0x00600FAC (name is ours)
void MailSystem::State32()
{
    // (the label of the part: a debug log of the original, removed)
    m_File.Set(m_pRecept->m_BodyFile, "Bbs");
    m_State = 33;
}

// 0x00600FF0 (name is ours)
void MailSystem::State33()
{
    if (LoadFile()) {
        SetupMessage();
        m_State = 34;
    } else {
        m_IsSuccess = 0;
        m_State = 35;
    }
}

// 0x0060104C (name is ours)
void MailSystem::State35()
{
    ReleaseFile();
    m_State = 36;
}

// 0x006010B8 (name is ours)
void MailSystem::State14()
{
    if (MakeText(m_pRecept->m_Labels[0], &MailSeq::RunTo)) {
        m_pRecept->m_pToWord->Copy(m_pSeq->m_Text);
        *m_pRecept->m_pToResult = m_pSeq->m_FirstLineLength;
    } else {
        m_IsSuccess = 0;
    }
    m_State = 15;
}

// 0x00601158 (name is ours)
void MailSystem::State12()
{
    // (the label of the part: a debug log of the original, removed)
    m_File.Set(m_pRecept->m_ToFile, "MailR/To");
    m_State = 13;
}

// 0x006011AC (name is ours)
void MailSystem::State13()
{
    if (LoadFile()) {
        SetupMessage();
        m_State = 14;
    } else {
        m_IsSuccess = 0;
        m_State = 15;
    }
}

// 0x00601208 (name is ours)
void MailSystem::State30()
{
    if (MakeText(m_pRecept->m_Labels[4], &MailSeq::RunText)) {
        m_pRecept->m_pFromWord->Copy(m_pSeq->m_Text);
    } else {
        m_IsSuccess = 0;
    }
    m_State = 31;
}

// 0x00601294 (name is ours)
void MailSystem::State28()
{
    // (the label of the part: a debug log of the original, removed)
    m_File.Set(m_pRecept->m_FromFile, "MailR/From");
    m_State = 29;
}

// 0x006012EC (name is ours)
void MailSystem::State29()
{
    if (LoadFile()) {
        SetupMessage();
        m_State = 30;
    } else {
        m_IsSuccess = 0;
        m_State = 31;
    }
}

// 0x00601348 (name is ours)
void MailSystem::State18()
{
    if (MakeText(m_pRecept->m_Labels[1], &MailSeq::RunText)) {
        m_pRecept->m_pBodyWord->Copy(m_pSeq->m_Text);
    } else {
        m_IsSuccess = 0;
    }
    m_State = 19;
}

// 0x006013D4 (name is ours)
void MailSystem::State16()
{
    // (the label of the part: a debug log of the original, removed)
    m_File.Set(m_pRecept->m_BodyFile, "MailR/MsgA");
    m_State = 17;
}

// 0x0060142C (name is ours)
void MailSystem::State17()
{
    if (LoadFile()) {
        SetupMessage();
        m_State = 18;
    } else {
        m_IsSuccess = 0;
        m_State = 19;
    }
}

// 0x00601488 (name is ours)
void MailSystem::State22()
{
    if (MakeText(m_pRecept->m_Labels[2], &MailSeq::RunText)) {
        m_pRecept->m_pBodyWord->Copy(m_pSeq->m_Text);
    } else {
        m_IsSuccess = 0;
    }
    m_State = 23;
}

// 0x00601514 (name is ours)
void MailSystem::State20()
{
    // (the label of the part: a debug log of the original, removed)
    m_File.Set(m_pRecept->m_BodyFile, "MailR/MsgB");
    m_State = 21;
}

// 0x0060156C (name is ours)
void MailSystem::State21()
{
    if (LoadFile()) {
        SetupMessage();
        m_State = 22;
    } else {
        m_IsSuccess = 0;
        m_State = 23;
    }
}

// 0x006015C8 (name is ours)
void MailSystem::State26()
{
    if (MakeText(m_pRecept->m_Labels[3], &MailSeq::RunText)) {
        m_pRecept->m_pBodyWord->Copy(m_pSeq->m_Text);
    } else {
        m_IsSuccess = 0;
    }
    m_State = 27;
}

// 0x00601654 (name is ours)
void MailSystem::State24()
{
    // (the label of the part: a debug log of the original, removed)
    m_File.Set(m_pRecept->m_BodyFile, "MailR/MsgC");
    m_State = 25;
}

// 0x006016AC (name is ours)
void MailSystem::State25()
{
    if (LoadFile()) {
        SetupMessage();
        m_State = 26;
    } else {
        m_IsSuccess = 0;
        m_State = 27;
    }
}

// 0x00601708 (name is ours)
void MailSystem::State15()
{
    ReleaseFile();
    m_State = 16;
}

// 0x00601774 (name is ours)
void MailSystem::State2()
{
    if (MakeText(m_pRecept->m_Labels[0], &MailSeq::RunTo)) {
        m_pRecept->m_pToWord->Copy(m_pSeq->m_Text);
        *m_pRecept->m_pToResult = m_pSeq->m_FirstLineLength;
    } else {
        m_IsSuccess = 0;
    }
    m_State = 3;
}

// 0x00601814 (name is ours)
void MailSystem::State0()
{
    // (the label of the part: a debug log of the original, removed)
    m_File.Set(m_pRecept->m_ToFile, "MailN/To");
    m_State = 1;
}

// 0x00601864 (name is ours)
void MailSystem::State1()
{
    if (LoadFile()) {
        SetupMessage();
        m_State = 2;
    } else {
        m_IsSuccess = 0;
        m_State = 3;
    }
}

// 0x006018C0 (name is ours)
void MailSystem::State6()
{
    if (MakeText(m_pRecept->m_Labels[1], &MailSeq::RunText)) {
        m_pRecept->m_pBodyWord->Copy(m_pSeq->m_Text);
    } else {
        m_IsSuccess = 0;
    }
    m_State = 7;
}

// 0x0060194C (name is ours)
void MailSystem::State4()
{
    // (the label of the part: a debug log of the original, removed)
    m_File.Set(m_pRecept->m_BodyFile, "MailN/Msg");
    m_State = 5;
}

// 0x0060199C (name is ours)
void MailSystem::State5()
{
    if (LoadFile()) {
        SetupMessage();
        m_State = 6;
    } else {
        m_IsSuccess = 0;
        m_State = 7;
    }
}

// 0x006019F8 (name is ours)
void MailSystem::State31()
{
    ReleaseFile();
    m_State = 36;
}

// 0x00601A64 (name is ours)
void MailSystem::State19()
{
    ReleaseFile();
    m_State = 20;
}

// 0x00601AD0 (name is ours)
void MailSystem::State23()
{
    ReleaseFile();
    m_State = 24;
}

// 0x00601B3C (name is ours)
void MailSystem::State27()
{
    ReleaseFile();
    m_State = 28;
}

// 0x00601BA8 (name is ours)
void MailSystem::State10()
{
    if (MakeText(m_pRecept->m_Labels[4], &MailSeq::RunText)) {
        m_pRecept->m_pFromWord->Copy(m_pSeq->m_Text);
    } else {
        m_IsSuccess = 0;
    }
    m_State = 11;
}

// 0x00601C34 (name is ours)
void MailSystem::State8()
{
    // (the label of the part: a debug log of the original, removed)
    m_File.Set(m_pRecept->m_FromFile, "MailN/From");
    m_State = 9;
}

// 0x00601C88 (name is ours)
void MailSystem::State9()
{
    if (LoadFile()) {
        SetupMessage();
        m_State = 10;
    } else {
        m_IsSuccess = 0;
        m_State = 11;
    }
}

// 0x00601CE4 (name is ours)
void MailSystem::State3()
{
    ReleaseFile();
    m_State = 4;
}

// 0x00601D50 (name is ours)
void MailSystem::State7()
{
    ReleaseFile();
    m_State = 8;
}

// 0x00601DBC (name is ours)
void MailSystem::State11()
{
    ReleaseFile();
    m_State = 36;
}
// 0x00601E4C (name is ours)
void MailSystem::Reset()
{
    m_State = STATE_NONE;
    m_IsSuccess = 1;
    ReleaseFile();
    m_Label.clear();
    m_pRecept = NULL;
    Insert* insert = m_pPhrase->GetInsert();
    if (insert != NULL) {
        insert->Clear();
    }
    *m_pDate = *time::GetCurrentDate();
}

// 0x00601F0C (name is ours)
void MailSystem::SetupMessage()
{
    const u8* data = static_cast<const u8*>(m_File.m_Loader.GetData());
    const void* message = NULL;
    if (data != NULL) {
        s32 language = GetRegion();
        language = static_cast<u32>(language) < 10 ? TalkFile::s_RegionLanguages[language] : 0;
        message = data + reinterpret_cast<const u32*>(data)[language * 2];
    }
    m_Message.Initialize(message);
}

// 0x00601F5C (name is ours)
MailSystem::MailSystem()
    : m_pMeasure(new (std::nothrow) MailMeasure(this)), m_pInsert(new (std::nothrow) MailInsert(this)),
      m_pPhrase(new (std::nothrow) MailPhrase(m_pInsert)), m_pSeq(new (std::nothrow) MailSeq(this)),
      m_pDate(new (std::nothrow) nn::fnd::DateTimeParameters(time::GetFirstDate())), m_State(STATE_NONE),
      m_IsSuccess(1), m_File(), m_pRecept(NULL), m_Message(), m_Label()
{
}

// 0x00133EB8 (name is ours)
MailSystem::~MailSystem()
{
    ReleaseFile();
    delete m_pDate;
    delete m_pSeq;
    delete m_pPhrase;
    delete m_pInsert;
    delete m_pMeasure;
}

// 0x005FDBBC (name is ours)
void MessageFile::Set(const sead::SafeStringBase<char>& name, const char* directory)
{
    GetRegion(); // (the result is not used)
    sead::StringFormatter formatter(&m_Path);
    formatter.append("Script/%s/%s.umsbt").append(directory).appendValue(name).flush();
}

} // namespace script

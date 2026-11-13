#include "Other/dNoticeLoadAdapt.h"

#include "Other/dRegion.h"
#include "Sv/dSvTown.h"
#include "script/dMailMgr.h"
#include "script/dMailPhrase.h"

// 0x0029CE84 slot 0x94 (name after IMailRecept)
void NoticeLoadAdapt::GetTag5_0(script::IWord* word)
{
    word->Clear();
}

// 0x0029CE94 slot 0x98 (name after IMailRecept)
void NoticeLoadAdapt::GetTag5_1(script::IWord* word)
{
    if (static_cast<const script::IWord&>(m_Name).IsEmpty()) {
        word->Clear();
    } else {
        word->Copy(m_Name);
    }
}

// 0x0030272C (name after the RTTI)
NoticeLoadAdapt::NoticeLoadAdapt()
{
    // (only the base and the members are constructed)
}

// 0x003027BC (name is ours)
void NoticeLoadAdapt::Begin()
{
    script::MailMgr::s_pInstance->Begin(this);
}

// 0x003027D0 (name is ours)
void NoticeLoadAdapt::Load(SvBulletinBoardMessage* message, const char* file, s32 label,
                           const nn::fnd::DateTimeParameters* date, const script::IWord* name, const script::IWord* town)
{
    m_Body.Set(message->GetMessage());
    if (name != NULL) {
        static_cast<script::IWord&>(m_Name).Copy(*name);
    } else {
        static_cast<script::IWord&>(m_Name).Clear();
    }
    message->Func118750(date);
    SetupBbs(&m_Body, file, label);
    script::MailMgr::s_pInstance->Run();
    if (IsDone()) {
        message->Func118714(name);
        message->Func1186E4(town);
        const u32 region = GetRegionIndex();
        if (region == 0 || region == 3) {
            message->Func1187BC();
        }
    } else {
        message->Func118774();
    }
}

// 0x003028D0 (name is ours)
void NoticeLoadAdapt::End()
{
    script::MailMgr::s_pInstance->End();
}

// 0x003028E8 slot 0x00
// 0x0029CED8 (deleting dtor)
NoticeLoadAdapt::~NoticeLoadAdapt()
{
    // (only the members are destroyed)
}

// 0x00302924 (name is ours)
script::MailPhrase* NoticeLoadAdapt::GetPhrase()
{
    return script::MailMgr::s_pInstance->m_System.GetPhrase();
}

// 0x0071C100 slot 0x8C (name after IRecept)
s32 NoticeLoadAdapt::GetSex()
{
    return 4;
}

// 0x0071C108 slot 0x90 (name after IRecept)
s32 NoticeLoadAdapt::GetSpeakerSex()
{
    return 4;
}

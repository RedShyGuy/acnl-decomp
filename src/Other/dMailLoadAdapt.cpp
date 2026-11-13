#include "Other/dMailLoadAdapt.h"

#include <cstring>

#include "Sv/dSvMail.h"
#include "netgame/dGameFunctions.h"
#include "script/dMailMgr.h"
#include "script/dMailPhrase.h"
#include "script/dScriptExtern.h"
#include "script/dWordFix.h"
#include "script/dWordPtrSv.h"

namespace {
// (inline) the letter after the mail system ran: written, it gets the paper, the persons and the indent; else cleared
DECOMP_ALWAYS_INLINE void FinishNpcLetter(MailLoadAdapt* adapt, SvMail* mail, const void* npc, const SvPersonalId* player,
                                          const SvFgName* paper, u8 senderString)
{
    mail->SetPaper2(*paper);
    mail->stringIdOfSender = senderString;
    mail->receiver.SetPlayer(player);
    mail->SetSenderNpc(npc);
    mail->SetStatus(1);
    if (adapt->m_NameIndent >= 0) {
        mail->receiverNameIndent = adapt->m_NameIndent;
    } else {
        mail->receiverNameIndent = 0;
        mail->receiver.Func5CCF24();
    }
}

// (inline) the words of the letter, the villager and the player are taken over, the letter is cleared
DECOMP_ALWAYS_INLINE void TakeLetter(MailLoadAdapt* adapt, SvMail* mail)
{
    adapt->m_Header.Set(mail->GetHeader());
    adapt->m_Body.Set(mail->GetBody());
    adapt->m_Signature.Set(mail->GetSignature());
}
} // namespace

// 0x0022DE58 slot 0x94 (name after IMailRecept)
void MailLoadAdapt::GetTag5_0(script::IWord* word)
{
    script::WordFix<9> name;
    MakePlayerName(&name, m_Player.playerData.playerName, m_Player.playerData.gender & 1);
    word->Copy(name);
}

// 0x0022DF14 (name is ours)
bool MailLoadAdapt::LoadNpc(SvMail* mail, const char* file, s32 label, const void* npc, const SvPersonalId* player,
                            const SvFgName* paper)
{
    TakeLetter(this, mail);
    Func6148BC(&m_Npc, static_cast<const SvNpcRef*>(npc));
    netgame::AssignPersonalId(&m_Player, player);
    mail->Clear();
    SetupNormal(&m_Header, &m_Body, &m_Signature, &m_NameIndent, file, label, file, label, file, label);
    script::MailMgr::s_pInstance->Run();
    if (!IsDone()) {
        mail->Clear();
        return false;
    }
    FinishNpcLetter(this, mail, npc, player, paper, 0);
    return true;
}

// 0x0022E088 (name is ours)
bool MailLoadAdapt::LoadPlayer(SvMail* mail, const char* file, s32 label, const SvPersonalId* player,
                               const SvFgName* paper, u8 senderString)
{
    return LoadPlayer(mail, file, label, label, label, player, paper, senderString);
}

// 0x0022E0B4 slot 0x98 (name after IMailRecept)
void MailLoadAdapt::GetTag5_1(script::IWord* word)
{
    script::WordFix<9> name;
    Func56E310(&name, &m_Npc);
    word->Copy(name);
}

// 0x0022E168 (name is ours)
bool MailLoadAdapt::LoadNpc(SvMail* mail, const char* toFile, s32 toLabel, const char* bodyFile, s32 bodyLabel,
                            const char* fromFile, s32 fromLabel, const void* npc, const SvPersonalId* player,
                            const SvFgName* paper)
{
    TakeLetter(this, mail);
    Func6148BC(&m_Npc, static_cast<const SvNpcRef*>(npc));
    netgame::AssignPersonalId(&m_Player, player);
    mail->Clear();
    SetupNormal(&m_Header, &m_Body, &m_Signature, &m_NameIndent, toFile, toLabel, bodyFile, bodyLabel, fromFile,
                fromLabel);
    script::MailMgr::s_pInstance->Run();
    if (!IsDone()) {
        mail->Clear();
        return false;
    }
    FinishNpcLetter(this, mail, npc, player, paper, 0);
    return true;
}

// 0x0022E2EC (name is ours)
bool MailLoadAdapt::LoadPlayer(SvMail* mail, const char* file, s32 toLabel, s32 bodyLabel, s32 fromLabel,
                               const SvPersonalId* player, const SvFgName* paper, u8 senderString)
{
    TakeLetter(this, mail);
    Func30836C(&m_Npc);
    netgame::AssignPersonalId(&m_Player, player);
    mail->Clear();
    SetupNormal(&m_Header, &m_Body, &m_Signature, &m_NameIndent, file, toLabel, file, bodyLabel, file, fromLabel);
    script::MailMgr::s_pInstance->Run();
    if (!IsDone()) {
        mail->Clear();
        return false;
    }
    mail->SetPaper2(*paper);
    mail->stringIdOfSender = senderString;
    mail->receiver.SetPlayer(player);
    mail->sender.Func5CCF24();
    mail->SetStatus(1);
    if (m_NameIndent >= 0) {
        mail->receiverNameIndent = m_NameIndent;
    } else {
        mail->receiverNameIndent = 0;
        mail->receiver.Func5CCF24();
    }
    return true;
}

// 0x0022E470 (name is ours)
bool MailLoadAdapt::LoadNpcRandom(SvMail* mail, const char* toFile, s32 toLabel, const char* bodyFile, s32 labelA,
                                  s32 labelB, s32 labelC, const char* fromFile, s32 fromLabel, const void* npc,
                                  const SvPersonalId* player, const SvFgName* paper)
{
    TakeLetter(this, mail);
    Func6148BC(&m_Npc, static_cast<const SvNpcRef*>(npc));
    netgame::AssignPersonalId(&m_Player, player);
    mail->Clear();
    SetupRandom(&m_Header, &m_Body, &m_Signature, &m_NameIndent, toFile, toLabel, bodyFile, labelA, labelB, labelC,
                fromFile, fromLabel);
    script::MailMgr::s_pInstance->Run();
    if (!IsDone()) {
        mail->Clear();
        return false;
    }
    FinishNpcLetter(this, mail, npc, player, paper, 0);
    return true;
}

// 0x0022E604 (name is ours)
void MailLoadAdapt::End()
{
    script::MailMgr::s_pInstance->End();
}

// 0x0022E618 (name is ours)
void MailLoadAdapt::Begin()
{
    script::MailMgr::s_pInstance->Begin(this);
}

// 0x0022E62C (name is ours)
script::MailPhrase* MailLoadAdapt::GetPhrase()
{
    return script::MailMgr::s_pInstance->m_System.GetPhrase();
}

// 0x0022E648 (name after the RTTI)
MailLoadAdapt::MailLoadAdapt() : m_NameIndent(-1)
{
    std::memset(static_cast<void*>(&m_Npc), 0, sizeof(m_Npc));
}

// 0x0022E6F4 slot 0x00
// 0x0022E6C0 (deleting dtor)
MailLoadAdapt::~MailLoadAdapt()
{
    // (only the members are destroyed)
}

// (falls through into 0x00632534)
// 0x00632518 slot 0x88 (name after IRecept)
s32 MailLoadAdapt::GetValue88(u8 gender)
{
    return Func632534(Func2FE510(&m_Npc), gender);
}

// 0x00714330 slot 0x8C (name after IRecept)
s32 MailLoadAdapt::GetSex()
{
    s32 sex = 4;
    if (Func302050(&m_Npc)) {
        script::WordFix<9> name;
        Func56E310(&name, &m_Npc);
        const script::WordInfo* info = static_cast<const script::IWord&>(name).GetInfo();
        if (info != NULL) {
            sex = info->color;
        }
    }
    return sex;
}

// 0x00714404 slot 0x90 (name after IRecept)
s32 MailLoadAdapt::GetSpeakerSex()
{
    s32 sex = 4;
    if (m_Player.IsValid()) {
        script::WordFix<9> name;
        MakePlayerName(&name, m_Player.playerData.playerName, m_Player.playerData.gender & 1);
        const script::WordInfo* info = static_cast<const script::IWord&>(name).GetInfo();
        if (info != NULL) {
            sex = info->color;
        }
    }
    return sex;
}

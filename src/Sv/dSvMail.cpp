#include "Sv/dSvMail.h"

#include <cstring>

#include "Item/item_Api.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvGardenPlus.h"
#include "Sv/dSvPlayer.h"
#include "event/event_Extern.h"
#include "netgame/dGameFunctions.h"
#include "script/dWordPtrSv.h"
#include "Other/dMailLoadAdapt.h"
#include "sead/seadIDelegate1.h"
#include "script/dPhrase.h"
#include "script/dMailPhrase.h"
#include "script/dScriptExtern.h"
#include "script/dWordFix.h"

// The functions of a letter (game code; all names are ours except the classes; the file name is from the RTTI of
// SimpleFuncDelegate).

namespace {
// (inline) the header of the current player: the one of the future letters for a receiver of the kind 6
DECOMP_ALWAYS_INLINE void SetHeaderOfPlayer(SvMail* mail, const SvPlayer* player)
{
    if (mail->receiver.unk_0x30 == 6) {
        mail->receiverNameIndent = player->defaultFutureLtrReceiverNameIndent;
        std::memcpy(mail->header, player->futureLetterHeader, sizeof(mail->header));
    } else {
        mail->receiverNameIndent = player->defaultLtrReceiverNameIndent;
        std::memcpy(mail->header, player->letterHeader, sizeof(mail->header));
    }
}

// (inline) the paper: an item of the kind 42 sets the bit 0x20, its index (0 if it is not a byte)
DECOMP_ALWAYS_INLINE void SetPaper(SvMail* mail, const SvFgName& paper)
{
    if (paper.GetKind() == 42) {
        mail->letterFlag |= 0x20;
    }
    const s32 index = item::GetIndex(paper);
    mail->paperId = static_cast<u32>(index) <= 255 ? index : 0;
}
} // namespace

// 0x005CC70C (name is ours)
void SvMail::Func5CC70C(u32 kind)
{
    letterFlag = (letterFlag & ~7) | (kind & 7);
}

// 0x005CC724 (name is ours)
void SvMail::Func5CC724(const SvFgName& paper)
{
    Clear();
    SetPaper(this, paper);
    stringIdOfSender = 0;
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL) {
        return;
    }
    sender.unk_0x30 = 1;
    netgame::AssignPersonalId(&sender.id, &player->playerInfo);
    letterFlag = (letterFlag & ~7) | 3;
    if (player->defaultFutureLtrReceiverNameIndent >= 32) {
        Func1B29AC(player->letterHeader);
    }
    std::memcpy(signature, player->letterSignature, sizeof(signature));
    SetHeaderOfPlayer(this, player);
}

// 0x005CC804 (name is ours)
void SvMail::Func5CC804(s32 npcIndex)
{
    void* npc = event::Func300928(GetGardenPlus()->data.npcData.npcs, npcIndex);
    if (npc != NULL && Func302050(npc)) {
        receiver.unk_0x30 = 2;
        Func6148BC(&receiver.id, npc);
    }
    SvPlayer* player = SvGetCurrentPlayer();
    if (player != NULL) {
        SetHeaderOfPlayer(this, player);
    }
}

// 0x005CC894 (name is ours)
void SvMail::Func5CC894()
{
    SvPlayer* player = SvGetCurrentPlayer();
    if (player == NULL) {
        return;
    }
    receiver.unk_0x30 = 1;
    netgame::AssignPersonalId(&receiver.id, &player->playerInfo);
    receiver.unk_0x30 = 6;
    receiverNameIndent = player->defaultFutureLtrReceiverNameIndent;
    std::memcpy(header, player->futureLetterHeader, sizeof(header));
}

// 0x005CC8F4 (name is ours)
void SvMail::Func5CC8F4(const SvPersonalId* id)
{
    sender.Set(id, 1);
}

// 0x005CC904 (name is ours)
void SvMail::Func5CC904(const void* npc)
{
    sender.SetNpc(npc, 2);
}

// 0x005CC914 (name is ours)
void SvMail::Func5CC914(const SvFgName& paper)
{
    SetPaper(this, paper);
}

// 0x005CC954 (name is ours)
void SvMail::Func5CC954()
{
    if ((letterFlag & 7) == 1) {
        letterFlag = (letterFlag & ~7) | 2;
    }
}

// (another copy of Func5CC914)
// 0x005CC974 (name is ours)
void SvMail::Func5CC974(const SvFgName& paper)
{
    SetPaper(this, paper);
}

// 0x005CC9B4 (name is ours)
void SvMail::Func5CC9B4(u32 saveNo, bool keepHeader)
{
    SvPlayer* player = netgame::Func3038EC(saveNo & 0xFF);
    if (player != NULL && player->playerInfo.IsValid()) {
        receiver.Set(&player->playerInfo, 1);
    }
    if (keepHeader) {
        return;
    }
    SvPlayer* current = SvGetCurrentPlayer();
    if (current != NULL) {
        SetHeaderOfPlayer(this, current);
    }
}

// 0x005CCAF4 (name is ours)
void SvMail::Func5CCAF4()
{
    if ((letterFlag & 7) == 3) {
        letterFlag = (letterFlag & ~7) | 1;
    }
    if (!Func769190(&attachedItem)) {
        letterFlag = (letterFlag & ~0x18) | 8;
    }
}

// 0x005CCB40 (name is ours)
void SvMailPerson::Func5CCB40()
{
    unk_0x30 = 6;
}

// 0x005CCF24 (name is ours)
void SvMailPerson::Func5CCF24()
{
    switch (unk_0x30) {
    case 1:
    case 3:
        unk_0x30 = 3;
        break;
    case 2:
    case 4:
        unk_0x30 = 4;
        break;
    default:
        unk_0x30 = 5;
        break;
    }
}

// 0x005CCF64 (name is ours)
void SvMailPerson::SetNpc(const void* npc, u8 kind)
{
    unk_0x30 = kind;
    Func6148BC(&id, npc);
}

// 0x005CCF6C (name is ours)
void SvMailPerson::Set(const SvPersonalId* other, u8 kind)
{
    unk_0x30 = kind;
    netgame::AssignPersonalId(&id, other);
}

// 0x005CCF74 (name is ours)
void SvMail::Func5CCF74()
{
    letterFlag |= 0x40;
}

// 0x005CCF84 (name is ours)
script::WordPtrSv SvMail::GetBody()
{
    return script::WordPtrSv(body, 193);
}

// 0x005CCFA4 (name is ours)
void SvMail::Func5CCFA4()
{
    letterFlag &= ~0x40;
}

// 0x005CCFB4 (name is ours)
script::WordPtrSv SvMail::GetSignature()
{
    return script::WordPtrSv(signature, 33);
}

// 0x005CCFD4 (name is ours)
script::WordPtrSv SvMail::GetHeader()
{
    return script::WordPtrSv(header, 33);
}

// 0x005CCFF4
SvMail& SvMail::operator=(const SvMail& other)
{
    receiver = other.receiver;
    sender = other.sender;
    std::memcpy(header, other.header, sizeof(header));
    std::memcpy(body, other.body, sizeof(body));
    std::memcpy(signature, other.signature, sizeof(signature));
    receiverNameIndent = other.receiverNameIndent;
    paperId = other.paperId;
    letterFlag = other.letterFlag;
    stringIdOfSender = other.stringIdOfSender;
    letterType = other.letterType;
    attachedItem = other.attachedItem;
    unk_0x278 = other.unk_0x278;
    return *this;
}

namespace {
// a callback of the mail loads that calls a function (the class name is from the RTTI)
class SimpleFuncDelegate : public sead::IDelegate1<script::Phrase*>
{
public:
    typedef void (*Func)(script::Phrase*);

    explicit SimpleFuncDelegate(Func func) : m_Func(func) {}
    void invoke(script::Phrase* phrase) override;

    Func m_Func;
};

// 0x004DC2AC slot 0x00
inline void SimpleFuncDelegate::invoke(script::Phrase* phrase)
{
    if (m_Func != NULL) {
        m_Func(phrase);
    }
}
} // namespace

// 0x005CCA48 (name is ours)
void SvMail::Func5CCA48(script::Phrase* phrase, const void* npc)
{
    script::WordFix<9> name;
    Func56E310(&name, npc);
    phrase->SetWord2B8(name, 0);
}

// 0x005CCB4C (name is ours)
bool SvMail::LoadNpc(const char* toFile, s32 toLabel, const char* bodyFile, s32 bodyLabel, const char* fromFile,
                     s32 fromLabel, const void* npc, const SvPersonalId* player, const SvFgName* paper,
                     sead::IDelegate1<script::Phrase*>* callback)
{
    MailLoadAdapt adapt;
    adapt.Begin();
    if (callback != NULL) {
        callback->invoke(adapt.GetPhrase());
    }
    Func5CCA48(adapt.GetPhrase(), npc);
    adapt.LoadNpc(this, toFile, toLabel, bodyFile, bodyLabel, fromFile, fromLabel, npc, player, paper);
    adapt.End();
    return adapt.IsDone();
}

// 0x005CCC30 (name is ours)
bool SvMail::LoadPlayer(const char* file, s32 toLabel, s32 bodyLabel, s32 fromLabel, const SvPersonalId* player,
                        const SvFgName* paper, u8 senderString, sead::IDelegate1<script::Phrase*>* callback)
{
    MailLoadAdapt adapt;
    adapt.Begin();
    if (callback != NULL) {
        callback->invoke(adapt.GetPhrase());
    }
    adapt.LoadPlayer(this, file, toLabel, bodyLabel, fromLabel, player, paper, senderString);
    adapt.End();
    return adapt.IsDone();
}

// 0x005CCCE0 (name is ours)
bool SvMail::LoadNpcRandom(const char* toFile, s32 toLabel, const char* bodyFile, s32 labelA, s32 labelB, s32 labelC,
                           const char* fromFile, s32 fromLabel, const void* npc, const SvPersonalId* player,
                           const SvFgName* paper, sead::IDelegate1<script::Phrase*>* callback)
{
    MailLoadAdapt adapt;
    adapt.Begin();
    if (callback != NULL) {
        callback->invoke(adapt.GetPhrase());
    }
    Func5CCA48(adapt.GetPhrase(), npc);
    adapt.LoadNpcRandom(this, toFile, toLabel, bodyFile, labelA, labelB, labelC, fromFile, fromLabel, npc, player,
                        paper);
    adapt.End();
    return adapt.IsDone();
}

// 0x005CCDC4 (name is ours)
bool SvMail::LoadPlayer(const char* file, s32 label, const SvPersonalId* player, const SvFgName* paper,
                        u8 senderString, void (*callback)(script::Phrase*))
{
    SimpleFuncDelegate delegate(callback);
    MailLoadAdapt adapt;
    adapt.Begin();
    static_cast<sead::IDelegate1<script::Phrase*>&>(delegate).invoke(adapt.GetPhrase());
    adapt.LoadPlayer(this, file, label, player, paper, senderString);
    adapt.End();
    return adapt.IsDone();
}

// 0x005CCE74 (name is ours)
bool SvMail::LoadPlayer(const char* file, s32 toLabel, s32 bodyLabel, s32 fromLabel, const SvPersonalId* player,
                        const SvFgName* paper, u8 senderString, void (*callback)(script::Phrase*))
{
    SimpleFuncDelegate delegate(callback);
    MailLoadAdapt adapt;
    adapt.Begin();
    static_cast<sead::IDelegate1<script::Phrase*>&>(delegate).invoke(adapt.GetPhrase());
    adapt.LoadPlayer(this, file, toLabel, bodyLabel, fromLabel, player, paper, senderString);
    adapt.End();
    return adapt.IsDone();
}

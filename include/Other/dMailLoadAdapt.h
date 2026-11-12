#pragma once

#include "decomp.h"
#include "Sv/dSvPersonalId.h"
#include "script/dIMailRecept.h"
#include "script/dWordPtr.h"

struct SvFgName;
struct SvMail;
namespace script {
class MailPhrase;
} // namespace script

// The receiver of the mail system that writes a letter from messages (the class name is from the RTTI; all other names
// are ours): the header, the text and the signature of the letter are the words of the addressee, the text and the
// sender; the letter comes from a villager (the tags 5/1, the sex) and goes to a player (the tag 5/0, the speaker sex).
// RTTI 13MailLoadAdapt @ 0x008CBA08
// vtable 0x008EF2CC (vptr 0x008EF2D4), offset_to_top 0, 39 entries
class MailLoadAdapt : public ::script::IMailRecept
{
public:
    MailLoadAdapt(); // 0x0022E648
    virtual ~MailLoadAdapt(); // 0x0022E6F4 slot 0x00
    // 0x0022E6C0 slot 0x04 (deleting dtor)
    // (the bodies of the first three lie elsewhere in the program)
    s32 GetValue88(u8 gender) override; // 0x00632518 slot 0x88
    s32 GetSex() override; // 0x00714330 slot 0x8C
    s32 GetSpeakerSex() override; // 0x00714404 slot 0x90
    void GetTag5_0(script::IWord* word) override; // 0x0022DE58 slot 0x94
    void GetTag5_1(script::IWord* word) override; // 0x0022E0B4 slot 0x98

    // the mail system works with this receiver / ends / its phrase (the words of the tags)
    void Begin(); // 0x0022E618
    void End(); // 0x0022E604
    script::MailPhrase* GetPhrase(); // 0x0022E62C
    // the letter is written from the messages: one file and label for all three parts / a file and a label each / the
    // random letters (three texts) / from the player without a villager (the sender string); false: it failed (the
    // letter is cleared)
    bool LoadNpc(SvMail* mail, const char* file, s32 label, const void* npc, const SvPersonalId* player,
                 const SvFgName* paper); // 0x0022DF14
    bool LoadNpc(SvMail* mail, const char* toFile, s32 toLabel, const char* bodyFile, s32 bodyLabel,
                 const char* fromFile, s32 fromLabel, const void* npc, const SvPersonalId* player,
                 const SvFgName* paper); // 0x0022E168
    bool LoadNpcRandom(SvMail* mail, const char* toFile, s32 toLabel, const char* bodyFile, s32 labelA, s32 labelB,
                       s32 labelC, const char* fromFile, s32 fromLabel, const void* npc, const SvPersonalId* player,
                       const SvFgName* paper); // 0x0022E470
    bool LoadPlayer(SvMail* mail, const char* file, s32 toLabel, s32 bodyLabel, s32 fromLabel,
                    const SvPersonalId* player, const SvFgName* paper, u8 senderString); // 0x0022E2EC
    bool LoadPlayer(SvMail* mail, const char* file, s32 label, const SvPersonalId* player, const SvFgName* paper,
                    u8 senderString); // 0x0022E088

    script::WordPtr m_Header;       // 0x0B0
    script::WordPtr m_Body;         // 0x0BC
    script::WordPtr m_Signature;    // 0x0C8
    s32 m_NameIndent;               // 0x0D4 the indent of the name of the addressee (-1: none)
    u8 m_Npc[0x30];                 // 0x0D8 the villager (as the id of a person of a letter)
    SvPersonalId m_Player;          // 0x108 the player
};
ASSERT_SIZE(MailLoadAdapt, 0x138);

// (game code, not decompiled yet; names after the addresses) the villager of a letter person is cleared / its kind /
// the value of the kind and the gender
void Func30836C(void* npc); // 0x0030836C
u32 Func2FE510(const void* npc); // 0x002FE510
s32 Func632534(u32 kind, u8 gender); // 0x00632534

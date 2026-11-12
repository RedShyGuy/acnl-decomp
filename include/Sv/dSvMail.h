#pragma once

// Letters (mail) and mail related flags.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvPersonalId.h"
#include <cstring>

#pragma pack(push, 1)

// a person of a letter (the type and the names are ours): the id and its kind (1 a player, 2 and 6 see the functions of
// the letter; the constructor clears both)
namespace script {
class Phrase;
class WordPtrSv;
} // namespace script
namespace sead {
template <typename T0>
class IDelegate1;
} // namespace sead

// (game code, not decompiled yet; names after the addresses) the person from a villager / the villager (it may be
// written to) / a text of a player is checked / the attached item is checked
void Func6148BC(SvPersonalId* id, const void* npc); // 0x006148BC
bool Func302050(void* npc); // 0x00302050
void Func1B29AC(char16* header); // 0x001B29AC
bool Func769190(const SvFgName* item); // 0x00769190

struct SvMailPerson {
    SvMailPerson()
    {
        std::memset(static_cast<void*>(&id), 0, 0x30);
        unk_0x30 = 0;
    }

    // (names are ours) the person from an id / from a villager (0x006148BC), with the kind; the kind becomes 6 / 1 and 3
    // become 3, 2 and 4 become 4, the others 5
    void Set(const SvPersonalId* other, u8 kind); // 0x005CCF6C
    // (game code, not decompiled yet; falls through into the copy of the id) the kind 1 and the id
    void SetPlayer(const SvPersonalId* other); // 0x001B503C
    void SetNpc(const void* npc, u8 kind); // 0x005CCF64
    void Func5CCB40(); // 0x005CCB40
    void Func5CCF24(); // 0x005CCF24

    /* 0x0000 */ SvPersonalId id;
    /* 0x002E */ u16 pad_0x2E;
    /* 0x0030 */ u8 unk_0x30;
};
ASSERT_SIZE(SvMailPerson, 0x31);

// the 8 bytes at the end of a letter (type and name ours): an empty constructor, copied with its own assignment
struct SvMailUnk278 {
    SvMailUnk278() {}
    SvMailUnk278& operator=(const SvMailUnk278& other); // 0x00311C60

    /* 0x0000 */ u32 data[2];
};
ASSERT_SIZE(SvMailUnk278, 0x8);

struct SvMail {
    // (game code of dSvMail, not decompiled yet) the members are built and the letter is cleared / the members are
    // destroyed / the letter is cleared (name is ours)
    SvMail(); // 0x002FF2AC
    ~SvMail(); // 0x002F77EC
    void Clear(); // 0x002FF34C
    // (ARMCC's copy of the implicit assignment; the texts are copied with their terminators)
    SvMail& operator=(const SvMail& other); // 0x005CCFF4
    // (game code of the file of the letters, dSvMail.cpp; names are ours) the kind in the bits 0..2 of letterFlag /
    // a new letter of the current player on a paper / the receiver is the villager of an index / the receiver is the
    // current player (kind 6, future letter) / the sender is a player / a villager / the paper (two copies) / the kind 1
    // becomes 2 / the receiver is the player of a save number (then the header of the current player unless keepHeader)
    // / the kind 3 becomes 1, without an attached item the bits 3..4 become 1 / the bit 0x40 / the texts
    void Func5CC70C(u32 kind); // 0x005CC70C
    void Func5CC724(const SvFgName& paper); // 0x005CC724
    void Func5CC804(s32 npcIndex); // 0x005CC804
    void Func5CC894(); // 0x005CC894
    void Func5CC8F4(const SvPersonalId* id); // 0x005CC8F4
    void Func5CC904(const void* npc); // 0x005CC904
    void Func5CC914(const SvFgName& paper); // 0x005CC914
    void Func5CC954(); // 0x005CC954
    void Func5CC974(const SvFgName& paper); // 0x005CC974
    void Func5CC9B4(u32 saveNo, bool keepHeader); // 0x005CC9B4
    void Func5CCAF4(); // 0x005CCAF4
    void Func5CCF74(); // 0x005CCF74
    void Func5CCFA4(); // 0x005CCFA4
    script::WordPtrSv GetBody(); // 0x005CCF84
    script::WordPtrSv GetSignature(); // 0x005CCFB4
    script::WordPtrSv GetHeader(); // 0x005CCFD4
    // the letter is written from messages by MailLoadAdapt (see there; the callback gets the phrase first, the villager
    // is set as the word 2B8/0 of the phrase); true: it is written
    void Func5CCA48(script::Phrase* phrase, const void* npc); // 0x005CCA48
    bool LoadNpc(const char* toFile, s32 toLabel, const char* bodyFile, s32 bodyLabel, const char* fromFile,
                 s32 fromLabel, const void* npc, const SvPersonalId* player, const SvFgName* paper,
                 sead::IDelegate1<script::Phrase*>* callback); // 0x005CCB4C
    bool LoadPlayer(const char* file, s32 toLabel, s32 bodyLabel, s32 fromLabel, const SvPersonalId* player,
                    const SvFgName* paper, u8 senderString,
                    sead::IDelegate1<script::Phrase*>* callback); // 0x005CCC30
    bool LoadNpcRandom(const char* toFile, s32 toLabel, const char* bodyFile, s32 labelA, s32 labelB, s32 labelC,
                       const char* fromFile, s32 fromLabel, const void* npc, const SvPersonalId* player,
                       const SvFgName* paper, sead::IDelegate1<script::Phrase*>* callback); // 0x005CCCE0
    bool LoadPlayer(const char* file, s32 label, const SvPersonalId* player, const SvFgName* paper, u8 senderString,
                    void (*callback)(script::Phrase*)); // 0x005CCDC4
    bool LoadPlayer(const char* file, s32 toLabel, s32 bodyLabel, s32 fromLabel, const SvPersonalId* player,
                    const SvFgName* paper, u8 senderString, void (*callback)(script::Phrase*)); // 0x005CCE74

    /* 0x0000 */ SvMailPerson receiver;
    /* 0x0031 */ u8 pad_0x31[3];
    /* 0x0034 */ SvMailPerson sender;
    /* 0x0065 */ u8 pad_0x65[3];
    /* 0x0068 */ char16 header[0x21]; // Max amount is 32 UTF-16 characters (and the terminator)
    /* 0x00AA */ char16 body[0xC1]; // Max amount is 192 UTF-16 characters (and the terminator)
    /* 0x022C */ char16 signature[0x21]; // Max amount is 32 UTF-16 characters (and the terminator)
    /* 0x026E */ u8 receiverNameIndent;
    /* 0x026F */ u8 paperId;
    /* 0x0270 */ u8 letterFlag; // 9 closed
    /* 0x0271 */ u8 stringIdOfSender;
    /* 0x0272 */ u8 letterType; // 0x88 time capsule
    /* 0x0273 */ u8 unk_0x273;
    /* 0x0274 */ SvFgName attachedItem;
    /* 0x0278 */ SvMailUnk278 unk_0x278;
};
ASSERT_SIZE(SvMail, 0x280);

struct SvGulliverMail {
    enum Name {
        None = 0x0,
        SouthKorea = 0x1,
        EasterIslands = 0x2,
        Scotland = 0x3,
        Russia = 0x4,
        Singapore = 0x5,
        Denmark = 0x6,
        Belgium = 0x7,
        Spain = 0x8,
        France = 0x9,
        Rome = 0xA,
        Japan = 0xB,
        Ireland = 0xC,
        HowAreYou = 0xD, // he asks how you are and all
        Mexico = 0xE,
        Germany = 0xF,
        Netherlands = 0x10,
        DoYouMissMe = 0x11, // he says he is becoming a movie star
        Kenya = 0x12,
        Egypt = 0x13,
        China = 0x14,
        Greece = 0x15,
        England = 0x16,
        Australia = 0x17,
        Vietnam = 0x18,
        Sweden = 0x19,
        Thailand = 0x1A,
        India = 0x1B,
        Peru = 0x1C,
        Hawaii = 0x1D,
        Portugal = 0x1E,
        Empty = 0x1F, // Probably unused
        Empty2 = 0x20, // Probably unused
        Empty3 = 0x21, // Probably unused
        Empty4 = 0x22, // Probably unused
        Empty5 = 0x23, // Probably unused
        Empty6 = 0x24, // Probably unused
        Empty7 = 0x25, // Probably unused
        Empty8 = 0x26, // Probably unused
        Empty9 = 0x27, // Probably unused
        Empty10 = 0x28, // Probably unused
        Empty11 = 0x29, // Probably unused
        Empty12 = 0x2A, // Probably unused
        Empty13 = 0x2B, // Probably unused
        Empty14 = 0x2C, // Probably unused
        Empty15 = 0x2D, // Probably unused
        WrongGuess1 = 0x2E,
        WrongGuess2 = 0x2F,
        WrongGuess3 = 0x30,
    };
};

struct SvPrizeMailFlags {
    /* 0x0000.0 */ u8 first : 1;
    /* 0x0000.1 */ u8 second : 1;
    /* 0x0000.2 */ u8 third : 1;
    /* 0x0000.3 */ u8 pad_0x0_3 : 5;
};
ASSERT_SIZE(SvPrizeMailFlags, 0x1);

struct SvSnowmanMailFlags {
    /* 0x0000.0 */ u8 snowtykeSleigh : 1; // when only building snowtyke
    /* 0x0000.1 */ u8 snowtykeSnowBunny : 1; // when building 3 of his family members
    /* 0x0000.2 */ u8 snowtykeSmallIgloo : 1; // when building 2 of his family members
    /* 0x0000.3 */ u8 snowtykeSnowmanMatryoshka : 1; // when getting whole family together
    /* 0x0000.4 */ u8 snowboyGift : 1;
    /* 0x0000.5 */ u8 pad_0x0_5 : 3;
};
ASSERT_SIZE(SvSnowmanMailFlags, 0x1);

struct SvFutureMail {
    /* 0x0000 */ SvMail letterForFutureSelf;
    /* 0x0280 */ SvTime receiveDate; // ctor sets 0x7FFFFFFFFFFFFFFF (max positive U64)
};
ASSERT_SIZE(SvFutureMail, 0x288);

#pragma pack(pop)

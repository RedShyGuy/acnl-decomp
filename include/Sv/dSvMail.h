#pragma once

// Letters (mail) and mail related flags.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvPersonalId.h"

#pragma pack(push, 1)

struct SvMail {
    /* 0x0000 */ SvPersonalId receiver;
    /* 0x002E */ u16 pad_0x2E;
    /* 0x0030 */ u16 receiverId;
    /* 0x0032 */ u8 pad_0x32[0x32];
    /* 0x0064 */ u16 unk_0x64; // Some form of ID?
    /* 0x0066 */ u16 pad_0x66;
    /* 0x0068 */ char16 header[0x20]; // Max amount is 32 UTF-16 characters
    /* 0x00A8 */ u16 pad_0xA8;
    /* 0x00AA */ char16 body[0xC0]; // Max amount is 192 UTF-16 characters
    /* 0x022A */ u16 pad_0x22A;
    /* 0x022C */ char16 signature[0x20]; // Max amount is 32 UTF-16 characters
    /* 0x026C */ u16 pad_0x26C;
    /* 0x026E */ u8 receiverNameIndent;
    /* 0x026F */ u8 paperId;
    /* 0x0270 */ u8 letterFlag; // 9 closed
    /* 0x0271 */ u8 stringIdOfSender;
    /* 0x0272 */ u8 letterType; // 0x88 time capsule
    /* 0x0273 */ u8 unk_0x273;
    /* 0x0274 */ SvFgName attachedItem;
    /* 0x0278 */ u64 unk_0x278;
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

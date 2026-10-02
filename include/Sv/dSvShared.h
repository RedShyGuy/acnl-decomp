#pragma once

// Data shared by all players: census, mailboxes, secret storage.
//
// Offsets in /* */ are relative to the struct and verified by the ASSERT_SIZE checks.

#include "decomp.h"
#include "Sv/dSvCommon.h"
#include "Sv/dSvCensus.h"
#include "Sv/dSvFgName.h"
#include "Sv/dSvMail.h"

#pragma pack(push, 1)

struct SvStorageDataUnk15934 {
    /* 0x0000 */ u16 unk_0x0;
    /* 0x0002 */ u16 unk_0x2;
    /* 0x0004 */ u16 unk_0x4;
};
ASSERT_SIZE(SvStorageDataUnk15934, 0x6);

struct SvSharedFlags {
    /* 0x0000.0 */ u8 flag1 : 1;
    /* 0x0000.1 */ u8 flag2 : 1;
    /* 0x0000.2 */ u8 flag3 : 1;
    /* 0x0000.3 */ u8 flag4 : 1;
    /* 0x0000.4 */ u8 flag5 : 1;
    /* 0x0000.5 */ u8 flag6 : 1;
    /* 0x0000.6 */ u8 flag7 : 1;
    /* 0x0000.7 */ u8 flag8 : 1;
    /* 0x0001.0 */ u8 flag9 : 1;
    /* 0x0001.1 */ u8 flag10 : 1;
    /* 0x0001.2 */ u8 flag11 : 1;
    /* 0x0001.3 */ u8 flag12 : 1;
    /* 0x0001.4 */ u8 flag13 : 1;
    /* 0x0001.5 */ u8 flag14 : 1;
    /* 0x0001.6 */ u8 flag15 : 1;
    /* 0x0001.7 */ u8 flag16 : 1;
    /* 0x0002.0 */ u8 flag17 : 1;
    /* 0x0002.1 */ u8 spotpassFeaturesEnabled : 1;
    /* 0x0002.2 */ u8 spotpassFeaturesIntroduced : 1;
    /* 0x0002.3 */ u8 flag20 : 1;
    /* 0x0002.4 */ u8 flag21 : 1;
    /* 0x0002.5 */ u8 flag22 : 1;
    /* 0x0002.6 */ u8 flag23 : 1;
    /* 0x0002.7 */ u8 flag24 : 1;
    /* 0x0003.0 */ u8 flag25 : 1;
    /* 0x0003.1 */ u8 flag26 : 1;
    /* 0x0003.2 */ u8 amiiboCameraFeatureIntroduced : 1;
    /* 0x0003.3 */ u8 flag28 : 1;
    /* 0x0003.4 */ u8 flag29 : 1;
    /* 0x0003.5 */ u8 flag30 : 1;
    /* 0x0003.6 */ u8 flag31 : 1;
    /* 0x0003.7 */ u8 flag32 : 1;
};
ASSERT_SIZE(SvSharedFlags, 0x4);

struct SvSharedDataUnk0 {
    /* 0x0000 */ u32 checksum;
    /* 0x0004 */ u8 unk_0x4[0x1C];
    /* 0x0020 */ SvSharedFlags sharedFlags;
};
ASSERT_SIZE(SvSharedDataUnk0, 0x24);

struct SvSharedDataUnk24 {
    /* 0x0000 */ u32 checksum;
    /* 0x0004 */ u8 unk_0x4[0xBE4];
};
ASSERT_SIZE(SvSharedDataUnk24, 0xBE8);

struct SvSecretStorage {
    /* 0x0000 */ SvFgName storageItems[0x168];
};
ASSERT_SIZE(SvSecretStorage, 0x5A0);

struct SvMailbox {
    /* 0x0000 */ SvMail letters[0xA]; // MailBox
    /* 0x1900 */ SvFutureMail futureLetter;
};
ASSERT_SIZE(SvMailbox, 0x1B88);

struct SvStorageData {
    /* 0x0000 */ u32 checksum;
    /* 0x0004 */ SvMailbox playerMailBoxLetters[4];
    /* 0x6E24 */ SvSecretStorage playerSecretStorages[4];
    /* 0x84A4 */ SvMail letterPool[0x50];
    /* 0x14CA4 */ u8 unk_0x14CA4[0xC90]; // int sub_2B8890() Maybe MEOW Coupons? One method checks them (u64 MeowCoupons Count)
    /* 0x15934 */ SvStorageDataUnk15934 sharedData[4];
    /* 0x1594C */ u8 unk_0x1594C[0x840];
};
ASSERT_SIZE(SvStorageData, 0x1618C);

// Starts at 0x71900
struct SvSharedData {
    /* 0x0000 */ SvSharedDataUnk0 unk_0x0;
    /* 0x0024 */ SvSharedDataUnk24 unk_0x24;
    /* 0x0C0C */ u32 unk_0xC0C;
    /* 0x0C10 */ SvCensus playerStats;
    /* 0x2054 */ SvStorageData storageBox;
    /* 0x181E0 */ u8 unk_0x181E0[0x20];
};
ASSERT_SIZE(SvSharedData, 0x18200);

#pragma pack(pop)

#pragma once

// Types of CFL, Nintendo's face library (Miis). The names CFLiTexHeader, CFLiTexHandle, CFLiResShape,
// CFLiRecentDBFile, CFLProgramContext, CFLTexWrap and CFLTexFmt come from the reference symbols (nintendogs);
// all other type, member and enumerator names are ours. The fields of the Mii data follow the "Mii format" of
// 3dbrew (https://www.3dbrew.org/wiki/Mii).

#include "decomp.h"

// the results of the functions (names ours, after what returns them)
enum CFLResult
{
    CFL_RESULT_SUCCESS = 0,
    CFL_RESULT_NOT_AVAILABLE = 1,   // CFL is not initialized / the resource is not loaded
    CFL_RESULT_INVALID_ARGUMENT = 2,
    CFL_RESULT_BROKEN = 3,          // the database files are broken
    CFL_RESULT_4 = 4,
    CFL_RESULT_5 = 5,
    CFL_RESULT_INVALID_DATA = 6,    // a Mii that is empty or fails the verification
    CFL_RESULT_7 = 7,
    CFL_RESULT_NOT_FOUND = 8,
    CFL_RESULT_9 = 9,
    CFL_RESULT_BUSY = 10,           // an asynchronous operation runs
    CFL_RESULT_FILE_ERROR = 11
};

// the system ID of the console that made a Mii (the transferable ID, 3dbrew "System ID")
struct CFLiAuthorID
{
    u16 data[4];
};
ASSERT_SIZE(CFLiAuthorID, 0x8);

// "Mii ID" (big endian, bits 0-27 creation time, 28 always set, 29 temporary, 30 DSi Mii, 31 not special) and the
// MAC of the creator; byte 0 holds the flag bits
struct CFLiCreateID
{
    u16 data[5];
};
ASSERT_SIZE(CFLiCreateID, 0xA);

#define CFL_NAME_LENGTH 10

// the packed Mii (3dbrew "Mii format" 0x00-0x47)
struct CFLiMiiDataCore
{
    u32 version : 8;        // 0x00 always 3
    u32 copyable : 1;
    u32 ngWord : 1;         // profanity flag
    u32 regionMove : 2;     // region lock
    u32 fontRegion : 2;     // character set
    u32 _00_14 : 2;
    u32 pageIndex : 4;      // 0x02 position shown on the selection screen
    u32 slotIndex : 4;
    u32 _00_24 : 4;         // 0x03 bits 0-3
    u32 birthPlatform : 3;  // device the Mii was originally made on
    u32 _00_31 : 1;
    CFLiAuthorID authorID;  // 0x04 system ID
    CFLiCreateID createID;  // 0x0C Mii ID + creator MAC
    u16 _16;                // 0x16 padding
    u16 gender : 1;         // 0x18
    u16 birthMonth : 4;
    u16 birthDay : 5;
    u16 favoriteColor : 4;
    u16 favorite : 1;
    u16 _18_15 : 1;
    u16 name[CFL_NAME_LENGTH];  // 0x1A
    u8 height;              // 0x2E
    u8 build;               // 0x2F
    u16 localOnly : 1;      // 0x30 disable sharing
    u16 faceType : 4;
    u16 faceColor : 3;      // skin color
    u16 faceTex : 4;        // wrinkles
    u16 faceMake : 4;       // makeup
    u16 hairType : 8;       // 0x32
    u16 hairColor : 3;
    u16 hairFlip : 1;
    u16 _32_12 : 4;
    u16 eyeType : 6;        // 0x34
    u16 eyeColor : 3;
    u16 eyeScale : 4;
    u16 eyeAspect : 3;
    u16 eyeRotate : 5;      // 0x36
    u16 eyeX : 4;
    u16 eyeY : 5;
    u16 _36_14 : 2;
    u16 eyebrowType : 5;    // 0x38
    u16 eyebrowColor : 3;
    u16 eyebrowScale : 4;
    u16 eyebrowAspect : 3;
    u16 _38_15 : 1;
    u16 eyebrowRotate : 5;  // 0x3A
    u16 eyebrowX : 4;
    u16 eyebrowY : 5;
    u16 _3A_14 : 2;
    u16 noseType : 5;       // 0x3C
    u16 noseScale : 4;
    u16 noseY : 5;
    u16 _3C_14 : 2;
    u16 mouthType : 6;      // 0x3E
    u16 mouthColor : 3;
    u16 mouthScale : 4;
    u16 mouthAspect : 3;
    u16 mouthY : 5;         // 0x40
    u16 mustacheType : 3;
    u16 _40_8 : 8;
    u16 beardType : 3;      // 0x42
    u16 beardColor : 3;
    u16 mustacheScale : 4;
    u16 mustacheY : 5;
    u16 _42_15 : 1;
    u16 glassType : 4;      // 0x44
    u16 glassColor : 3;
    u16 glassScale : 4;
    u16 glassY : 5;
    u16 moleType : 1;       // 0x46
    u16 moleScale : 4;
    u16 moleX : 5;
    u16 moleY : 5;
    u16 _46_15 : 1;
};
ASSERT_SIZE(CFLiMiiDataCore, 0x48);

// the packed Mii with the name of its author (3dbrew "Mii format", 0x5C bytes)
struct CFLiMiiDataOfficial
{
    CFLiMiiDataCore core;                // 0x00
    u16 creatorName[CFL_NAME_LENGTH];    // 0x48
};
ASSERT_SIZE(CFLiMiiDataOfficial, 0x5C);

// "CFLStoreData" (3dbrew): a packed Mii with a CRC16 over the first 0x5E bytes (big endian)
struct CFLStoreData
{
    CFLiMiiDataOfficial official;  // 0x00
    u16 _5C;                       // 0x5C
    u16 crc;                       // 0x5E
};
ASSERT_SIZE(CFLStoreData, 0x60);

// the unpacked Mii: every field of CFLiMiiDataCore in its own word
struct CFLiCharInfo
{
    s32 miiVersion;         // 0x000
    s32 faceType;           // 0x004
    s32 faceColor;          // 0x008
    s32 faceTex;            // 0x00C
    s32 faceMake;           // 0x010
    s32 hairType;           // 0x014
    s32 hairColor;          // 0x018
    s32 hairFlip;           // 0x01C
    s32 eyeType;            // 0x020
    s32 eyeColor;           // 0x024
    s32 eyeScale;           // 0x028
    s32 eyeAspect;          // 0x02C
    s32 eyeRotate;          // 0x030
    s32 eyeX;               // 0x034
    s32 eyeY;               // 0x038
    s32 eyebrowType;        // 0x03C
    s32 eyebrowColor;       // 0x040
    s32 eyebrowScale;       // 0x044
    s32 eyebrowAspect;      // 0x048
    s32 eyebrowRotate;      // 0x04C
    s32 eyebrowX;           // 0x050
    s32 eyebrowY;           // 0x054
    s32 noseType;           // 0x058
    s32 noseScale;          // 0x05C
    s32 noseY;              // 0x060
    s32 mouthType;          // 0x064
    s32 mouthColor;         // 0x068
    s32 mouthScale;         // 0x06C
    s32 mouthAspect;        // 0x070
    s32 mouthY;             // 0x074
    s32 mustacheType;       // 0x078
    s32 beardType;          // 0x07C
    s32 beardColor;         // 0x080
    s32 mustacheScale;      // 0x084
    s32 mustacheY;          // 0x088
    s32 glassType;          // 0x08C
    s32 glassColor;         // 0x090
    s32 glassScale;         // 0x094
    s32 glassY;             // 0x098
    s32 moleType;           // 0x09C
    s32 moleScale;          // 0x0A0
    s32 moleX;              // 0x0A4
    s32 moleY;              // 0x0A8
    s32 height;             // 0x0AC
    s32 build;              // 0x0B0
    u16 name[CFL_NAME_LENGTH + 1];         // 0x0B4
    u16 creatorName[CFL_NAME_LENGTH + 1];  // 0x0CA
    s32 gender;             // 0x0E0
    s32 birthMonth;         // 0x0E4
    s32 birthDay;           // 0x0E8
    s32 favoriteColor;      // 0x0EC
    bool favorite;          // 0x0F0
    bool copyable;          // 0x0F1
    bool ngWord;            // 0x0F2
    bool localOnly;         // 0x0F3
    s32 regionMove;         // 0x0F4
    s32 fontRegion;         // 0x0F8
    s32 pageIndex;          // 0x0FC
    s32 slotIndex;          // 0x100
    s32 birthPlatform;      // 0x104
    CFLiCreateID createID;  // 0x108
    s32 _114;               // 0x114 bits 24-27 of the first word
    CFLiAuthorID authorID;  // 0x118
};
ASSERT_SIZE(CFLiCharInfo, 0x120);

// the packed Mii of the Wii (RFL) after CFLi_SwapRFLMiiDataEndian: the big endian halfwords as host halfwords, so
// the fields counted from bit 0 of each halfword
struct CFLiRFLMiiData
{
    u16 favorite : 1;       // 0x00
    u16 favoriteColor : 4;
    u16 birthDay : 5;
    u16 birthMonth : 4;
    u16 gender : 1;
    u16 _00_15 : 1;
    u16 name[CFL_NAME_LENGTH];  // 0x02
    u8 height;              // 0x16
    u8 build;               // 0x17
    u8 createID[8];         // 0x18
    u16 _20_0 : 2;          // 0x20
    u16 localOnly : 1;
    u16 _20_3 : 3;
    u16 faceFeature : 4;
    u16 faceColor : 3;
    u16 faceType : 3;
    u16 _22_0 : 5;          // 0x22
    u16 hairFlip : 1;
    u16 hairColor : 3;
    u16 hairType : 7;
    u16 _24_0 : 6;          // 0x24
    u16 eyebrowRotate : 5;
    u16 eyebrowType : 5;
    u16 eyebrowX : 4;       // 0x26
    u16 eyebrowY : 5;
    u16 eyebrowScale : 4;
    u16 eyebrowColor : 3;
    u16 eyeY : 5;           // 0x28
    u16 eyeRotate : 5;
    u16 eyeType : 6;
    u16 _2A_0 : 5;          // 0x2A
    u16 eyeX : 4;
    u16 eyeScale : 4;
    u16 eyeColor : 3;
    u16 _2C_0 : 3;          // 0x2C
    u16 noseY : 5;
    u16 noseScale : 4;
    u16 noseType : 4;
    u16 mouthY : 5;         // 0x2E
    u16 mouthScale : 4;
    u16 mouthColor : 2;
    u16 mouthType : 5;
    u16 glassY : 5;         // 0x30
    u16 glassScale : 4;
    u16 glassColor : 3;
    u16 glassType : 4;
    u16 mustacheY : 5;      // 0x32
    u16 mustacheScale : 4;
    u16 beardColor : 3;
    u16 beardType : 2;
    u16 mustacheType : 2;
    u16 _34_0 : 1;          // 0x34
    u16 moleX : 5;
    u16 moleY : 5;
    u16 moleScale : 4;
    u16 moleType : 1;
};
ASSERT_SIZE(CFLiRFLMiiData, 0x36);

// what the program may show of a Mii (CFL_GetAdditionalInfo; names are ours)
struct CFLAdditionalInfo
{
    u16 name[CFL_NAME_LENGTH + 1];         // 0x00
    u16 creatorName[CFL_NAME_LENGTH + 1];  // 0x16
    CFLiCreateID createID;                 // 0x2C
    u16 pageIndex : 4;                     // 0x36
    u16 slotIndex : 4;
    u16 _36_8 : 8;
    u32 gender : 1;                        // 0x38
    u32 birthMonth : 4;
    u32 birthDay : 5;
    u32 favoriteColor : 4;
    u32 favorite : 1;
    u32 height : 7;
    u32 build : 7;
    u32 ngWord : 1;
    u32 fontRegion : 2;
    u32 faceType : 4;                      // 0x3C
    u32 hairType : 8;
    u32 hairFlip : 1;
    u32 _3C_13 : 19;
    u32 facelineColor;                     // 0x40 RGBA8
};
ASSERT_SIZE(CFLAdditionalInfo, 0x44);

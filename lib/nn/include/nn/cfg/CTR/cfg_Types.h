#pragma once

// Types of nn::cfg. The type names are from the binary; members, enumerators and the names marked
// so are ours (after 3dbrew "Config Savegame" where noted).

#include "decomp.h"

namespace nn {
namespace cfg {
namespace CTR {
// the region of the console (the order of GetRegionCodeA3)
enum CfgRegionCode : u8
{
    CFG_REGION_JPN = 0,
    CFG_REGION_USA = 1,
    CFG_REGION_EUR = 2,
    CFG_REGION_AUS = 3,
    CFG_REGION_CHN = 4,
    CFG_REGION_KOR = 5,
    CFG_REGION_TWN = 6,
    CFG_REGION_MAX = 7,
};

// the language of the system settings (the order of GetLanguageCodeA2)
enum CfgLanguageCode : u8
{
    CFG_LANGUAGE_JA = 0,
    CFG_LANGUAGE_EN = 1,
    CFG_LANGUAGE_FR = 2,
    CFG_LANGUAGE_DE = 3,
    CFG_LANGUAGE_IT = 4,
    CFG_LANGUAGE_ES = 5,
    CFG_LANGUAGE_ZH = 6,
    CFG_LANGUAGE_KO = 7,
    CFG_LANGUAGE_NL = 8,
    CFG_LANGUAGE_PT = 9,
    CFG_LANGUAGE_RU = 10,
    CFG_LANGUAGE_TW = 11,
    CFG_LANGUAGE_MAX = 12,
};

// the country of the system settings: an index of GetCountryCodeA2 (187 entries)
enum CfgCountryCode : u8
{
    CFG_COUNTRY_MAX = 187,
};

// the user name of the system settings (first 24 bytes of config block 0xA0000)
struct UserName
{
    u16 name[11];       // 0x00 UTF-16, 10 characters and the terminator
    u8 unknown16[2];    // 0x16
};
ASSERT_SIZE(UserName, 0x18);

// the region of the system settings (config block 0xB0000 after 3dbrew: bits 24-31 country code,
// bits 16-23 region / province code)
struct SimpleAddressId
{
    u32 id; // 0x0
};
ASSERT_SIZE(SimpleAddressId, 0x4);

// config block 0xA0001 (name is ours)
struct Birthday
{
    u8 month; // 0x0
    u8 day;   // 0x1
};
ASSERT_SIZE(Birthday, 0x2);

// config block 0xC0000, the parental controls (name is ours; layout after 3dbrew)
struct ParentalControlInfo
{
    // restrictions (bits of flags)
    static const u32 FLAG_ENABLED = 1 << 0;
    static const u32 FLAG_RESTRICT_PHOTO_EXCHANGE = 1 << 3; // sharing images, audio, video, long text
    static const u32 FLAG_RESTRICT_P2P_INTERNET = 1 << 4;   // online interaction
    static const u32 FLAG_RESTRICT_P2P_CEC = 1 << 5;        // StreetPass
    static const u32 FLAG_RESTRICT_FRIEND_REGISTRATION = 1 << 6;

    u32 flags;               // 0x00
    u32 unknown04;           // 0x04
    u8 ratingSystem;         // 0x08
    u8 maxAllowedAge;        // 0x09
    u8 secretQuestionType;   // 0x0A
    u8 unknown0B;            // 0x0B
    char pinCode[8];         // 0x0C
    u16 secretAnswer[0x22];  // 0x14, UTF-16
    u8 unknown58[0x68];      // 0x58
};
ASSERT_OFFSET(ParentalControlInfo, pinCode, 0x0C);
ASSERT_SIZE(ParentalControlInfo, 0xC0);

// config block 0xC0001, COPPACS (US / Canadian restrictions; name is ours)
struct CoppacsInfo
{
    u8 isEnabled;       // 0x00
    u8 unknown01;       // 0x01 (GetCoppacsState: 1 if set with the parental controls enabled)
    u8 unknown02[0x12]; // 0x02
};
ASSERT_SIZE(CoppacsInfo, 0x14);
} // namespace CTR
} // namespace cfg
} // namespace nn

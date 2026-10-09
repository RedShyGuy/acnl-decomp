#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/CTR/CTR_SystemMenuData.h"
#include "nn/cfg/CTR/detail/detail_Api.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fs/CTR/CTR_Api.h"
#include "nn/os/os_CriticalSection.h"

namespace nn {
namespace cfg {
namespace CTR {
namespace {
// config blocks (3dbrew "Config Savegame")
const u32 BLOCK_LOCAL_FRIEND_CODE_SEED = 0x00090000;
const u32 BLOCK_USER_NAME = 0x000A0000;
const u32 BLOCK_BIRTHDAY = 0x000A0001;
const u32 BLOCK_LANGUAGE = 0x000A0002;
const u32 BLOCK_COUNTRY_INFO = 0x000B0000;
const u32 BLOCK_PARENTAL_CONTROL = 0x000C0000;
const u32 BLOCK_COPPACS = 0x000C0001;
const u32 BLOCK_EULA_VERSION = 0x000D0000;
const u32 BLOCK_DEBUG_MODE = 0x00130000;

// the user name block is longer than UserName
const size_t USER_NAME_BLOCK_SIZE = 0x1C;

// the shared configuration page: ENVINFO, bit 0 set on retail units (3dbrew "Configuration Memory")
const uptr CONFIG_MEMORY_ENVINFO = 0x1FF80014;

// the blocks as the getters read them (names are ours)
struct CountryInfo
{
    u8 unknown0[2];  // 0x0
    u8 province;     // 0x2
    u8 country;      // 0x3, CfgCountryCode
};

struct DebugModeInfo
{
    u8 unknown0;           // 0x0
    u8 flags;              // 0x1, bit 0: debug mode
    u8 fsLatencyEmulation; // 0x2, in 10 ms
    u8 unknown3;           // 0x3
};

struct EulaVersion
{
    u16 agreed; // 0x0
    u16 latest; // 0x2
};

// the defaults before a read (ARMCC loads them from .rodata at 0x008B3354)
const Birthday DEFAULT_BIRTHDAY = {1, 1};
const u8 DEFAULT_LANGUAGE = 0xFF;
const CountryInfo DEFAULT_COUNTRY_INFO = {{0xFF, 0xFF}, 0xFF, 0xFF};

// the languages of the regions (bits of CfgLanguageCode)
const u32 LANGUAGES_JPN = 1 << CFG_LANGUAGE_JA;
const u32 LANGUAGES_USA = (1 << CFG_LANGUAGE_EN) | (1 << CFG_LANGUAGE_FR) | (1 << CFG_LANGUAGE_ES) | (1 << CFG_LANGUAGE_PT);
const u32 LANGUAGES_EUR = (1 << CFG_LANGUAGE_EN) | (1 << CFG_LANGUAGE_FR) | (1 << CFG_LANGUAGE_DE) | (1 << CFG_LANGUAGE_IT) |
                          (1 << CFG_LANGUAGE_ES) | (1 << CFG_LANGUAGE_NL) | (1 << CFG_LANGUAGE_PT) | (1 << CFG_LANGUAGE_RU);
const u32 LANGUAGES_CHN = 1 << CFG_LANGUAGE_ZH;
const u32 LANGUAGES_KOR = 1 << CFG_LANGUAGE_KO;
const u32 LANGUAGES_TWN = 1 << CFG_LANGUAGE_TW;

// GetCoppacsState
const s32 COPPACS_STATE_OFF = 0;
const s32 COPPACS_STATE_AUTHORIZED = 1;
const s32 COPPACS_STATE_ON = 2;

const size_t PIN_CODE_LENGTH = 4;

inline void GetConfigOrThrow(void* buffer, size_t size, u32 blockId)
{
    nn::err::CTR::ThrowFatalErrAllIfFailure(detail::GetConfig(buffer, size, blockId));
}

inline u32 GetParentalControlFlags()
{
    ParentalControlInfo info;
    GetConfigOrThrow(&info, sizeof(info), BLOCK_PARENTAL_CONTROL);
    return info.flags;
}

inline bool IsRetailUnit()
{
    return *reinterpret_cast<volatile u8*>(CONFIG_MEMORY_ENVINFO) & 1;
}

// the debug mode block (a debug console only; the function opens a port for itself)
inline DebugModeInfo GetDebugModeInfo()
{
    detail::_IPCPortType port;
    nn::err::CTR::ThrowFatalErrAllIfFailure(detail::InitializeProperPort(&port));
    DebugModeInfo info;
    GetConfigOrThrow(&info, sizeof(info), BLOCK_DEBUG_MODE);
    detail::FinalizeProperPort(port);
    return info;
}
} // namespace

// the code tables (names are ours)
// 0x0097E840
const char* s_RegionCodeA3[CFG_REGION_MAX] = {"JPN", "USA", "EUR", "AUS", "CHN", "KOR", "TWN"};
// 0x00975EBC
const char* s_LanguageCodeA2[CFG_LANGUAGE_MAX] = {"ja", "en", "fr", "de", "it", "es", "zh", "ko", "nl", "pt", "ru", "zh"};
// 0x00975BD0
const char* s_CountryCodeA2[CFG_COUNTRY_MAX] = {
    0, "JP", 0, 0, 0, 0, 0, 0, "AI", "AG", "AR", "AW", "BS", "BB", "BZ", "BO", "BR", "VG", "CA",
    "KY", "CL", "CO", "CR", "DM", "DO", "EC", "SV", "GF", "GD", "GP", "GT", "GY", "HT", "HN", "JM",
    "MQ", "MX", "MS", "AN", "NI", "PA", "PY", "PE", "KN", "LC", "VC", "SR", "TT", "TC", "US", "UY",
    "VI", "VE", 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "AL", "AU", "AT", "BE", "BA", "BW", "BG", "HR",
    "CY", "CZ", "DK", "EE", "FI", "FR", "DE", "GR", "HU", "IS", "IE", "IT", "LV", "LS", "LI", "LT",
    "LU", "MK", "MT", "ME", "MZ", "NA", "NL", "NZ", "NO", "PL", "PT", "RO", "RU", "RS", "SK", "SI",
    "ZA", "ES", "SZ", "SE", "CH", "TR", "GB", "ZM", "ZW", "AZ", "MR", "ML", "NE", "TD", "SD", "ER",
    "DJ", "SO", "AD", "GI", "GG", "IM", "JE", "MC", "TW", 0, 0, 0, 0, 0, 0, 0, "KR", 0, 0, 0, 0, 0,
    0, 0, "HK", "MO", 0, 0, 0, 0, 0, 0, "ID", "SG", "TH", "PH", "MY", 0, 0, 0, "CN", 0, 0, 0, 0, 0,
    0, 0, "AE", "IN", "EG", "OM", "QA", "KW", "SA", "SY", "BH", "JO", 0, 0, 0, 0, 0, 0, "SM", "VA",
    "BM",
};

// the SMDH of the title for IsAgreedEula (names are ours)
// 0x00AE1AAC
nn::os::CriticalSection s_SystemMenuDataLock((nn::os::CriticalSection::InitializeTag()));
// 0x00948000
nn::CTR::SystemMenuData s_SystemMenuData;

// 0x0011D544 | nintendogs:bytes [tier A]
void Initialize()
{
    nn::err::CTR::ThrowFatalErrAllIfFailure(detail::Initialize());
}

// 0x00130A5C | tier X
void Finalize()
{
    detail::Finalize();
}

// 0x0011FEEC
nn::cfg::CTR::CfgRegionCode GetRegion()
{
    return detail::GetRegion();
}

// 0x0011E0A4 | fefates:bytes-fuzzy [tier B]
nn::cfg::CTR::CfgLanguageCode GetLanguage()
{
    u8 language = DEFAULT_LANGUAGE;
    GetConfigOrThrow(&language, sizeof(language), BLOCK_LANGUAGE);
    CfgRegionCode region = detail::GetRegion();
    bool isOfRegion;
    switch (region) {
    case CFG_REGION_JPN:
        isOfRegion = ((1 << language) & LANGUAGES_JPN) != 0;
        break;
    case CFG_REGION_USA:
        isOfRegion = ((1 << language) & LANGUAGES_USA) != 0;
        break;
    case CFG_REGION_EUR:
        isOfRegion = ((1 << language) & LANGUAGES_EUR) != 0;
        break;
    case CFG_REGION_CHN:
        isOfRegion = ((1 << language) & LANGUAGES_CHN) != 0;
        break;
    case CFG_REGION_KOR:
        isOfRegion = ((1 << language) & LANGUAGES_KOR) != 0;
        break;
    case CFG_REGION_TWN:
        isOfRegion = ((1 << language) & LANGUAGES_TWN) != 0;
        break;
    default:
        isOfRegion = true;
        break;
    }
    if (isOfRegion) {
        return static_cast<CfgLanguageCode>(language);
    }
    // the language of the region
    switch (region) {
    case CFG_REGION_USA:
    case CFG_REGION_EUR:
        return CFG_LANGUAGE_EN;
    case CFG_REGION_CHN:
        return CFG_LANGUAGE_ZH;
    case CFG_REGION_KOR:
        return CFG_LANGUAGE_KO;
    case CFG_REGION_TWN:
        return CFG_LANGUAGE_TW;
    default:
        return CFG_LANGUAGE_JA;
    }
}

// 0x0011E1CC | nintendogs:bytes [tier A]
bool IsDebugMode()
{
    if (IsRetailUnit()) {
        return false;
    }
    return GetDebugModeInfo().flags & 1;
}

// 0x0011E244 | fefates:bytes [tier B]
u8 GetFsLatencyEmulationParam()
{
    if (IsRetailUnit()) {
        return 0;
    }
    return GetDebugModeInfo().fsLatencyEmulation;
}

// 0x00350AE8 | nintendogs:bytes [tier A]
nn::cfg::CTR::CfgCountryCode GetCountry()
{
    CountryInfo info = DEFAULT_COUNTRY_INFO;
    GetConfigOrThrow(&info, sizeof(info), BLOCK_COUNTRY_INFO);
    return static_cast<CfgCountryCode>(info.country);
}

// 0x00350B24
void GetBirthday(nn::cfg::CTR::Birthday* birthday)
{
    Birthday value = DEFAULT_BIRTHDAY;
    GetConfigOrThrow(&value, sizeof(value), BLOCK_BIRTHDAY);
    birthday->month = value.month;
    birthday->day = value.day;
}

// 0x00350B70
void GetUserName(nn::cfg::CTR::UserName* userName)
{
    union {
        UserName name;
        u8 block[USER_NAME_BLOCK_SIZE];
    } buffer;
    GetConfigOrThrow(&buffer, sizeof(buffer), BLOCK_USER_NAME);
    *userName = buffer.name;
}

// 0x00350BAC | nintendogs:bytes [tier A]
bool IsAgreedEula()
{
    EulaVersion version;
    GetConfigOrThrow(&version, sizeof(version), BLOCK_EULA_VERSION);
    s_SystemMenuDataLock.Enter();
    if (nn::fs::CTR::GetSelfSystemMenuData(&s_SystemMenuData).IsFailure()) {
        s_SystemMenuDataLock.Exit();
        return false;
    }
    u8 minor = s_SystemMenuData.settings.eulaVersionMinor;
    u8 major = s_SystemMenuData.settings.eulaVersionMajor;
    s_SystemMenuDataLock.Exit();
    return version.agreed >= static_cast<u16>(minor | (major << 8));
}

// 0x00350C2C | fefates:bytes [tier B]
const char* GetRegionCodeA3(nn::cfg::CTR::CfgRegionCode region)
{
    return region < CFG_REGION_MAX ? s_RegionCodeA3[region] : 0;
}

// 0x00350C44 | fefates:bytes [tier B]
const char* GetCountryCodeA2(nn::cfg::CTR::CfgCountryCode country)
{
    return country < CFG_COUNTRY_MAX ? s_CountryCodeA2[country] : 0;
}

// 0x00350C5C | nintendogs:bytes [tier A]
bool IsRestrictP2pCec()
{
    return (GetParentalControlFlags() & ParentalControlInfo::FLAG_RESTRICT_P2P_CEC) != 0;
}

// 0x00350C94 | fefates:bytes [tier B]
const char* GetLanguageCodeA2(nn::cfg::CTR::CfgLanguageCode language)
{
    return language < CFG_LANGUAGE_MAX ? s_LanguageCodeA2[language] : 0;
}

// 0x00350CAC
// (falls into GetTransferableId(u32))
u64 GetTransferableId()
{
    return GetTransferableId(0);
}

// 0x00350CB4 | nintendogs:bytes [tier A]
u64 GetTransferableId(u32 unknown)
{
    u64 id;
    nn::err::CTR::ThrowFatalErrAllIfFailure(detail::GetTransferableId(unknown, &id));
    return id;
}

// 0x00350CDC
u64 GetLocalFriendCodeSeed()
{
    u64 seed = 0;
    GetConfigOrThrow(&seed, sizeof(seed), BLOCK_LOCAL_FRIEND_CODE_SEED);
    return seed;
}

// 0x00350D18 | fefates:bytes [tier B]
void GetSimpleAddressId(nn::cfg::CTR::SimpleAddressId* id)
{
    GetConfigOrThrow(id, sizeof(*id), BLOCK_COUNTRY_INFO);
}

// 0x00350D44
bool IsCoppacsSupported()
{
    bool isCanadaOrUsa;
    nn::err::CTR::ThrowFatalErrAllIfFailure(detail::GetRegionCanadaUSA(&isCanadaOrUsa));
    return isCanadaOrUsa;
}

// 0x00350D64
bool IsRestrictFriendRegistration()
{
    return (GetParentalControlFlags() & ParentalControlInfo::FLAG_RESTRICT_FRIEND_REGISTRATION) != 0;
}

// 0x00350D9C
s32 GetCoppacsState()
{
    CoppacsInfo coppacs;
    GetConfigOrThrow(&coppacs, sizeof(coppacs), BLOCK_COPPACS);
    ParentalControlInfo info;
    GetConfigOrThrow(&info, sizeof(info), BLOCK_PARENTAL_CONTROL);
    if (!coppacs.isEnabled) {
        return COPPACS_STATE_OFF;
    }
    bool isCanadaOrUsa;
    nn::err::CTR::ThrowFatalErrAllIfFailure(detail::GetRegionCanadaUSA(&isCanadaOrUsa));
    if (!isCanadaOrUsa) {
        return COPPACS_STATE_OFF;
    }
    if ((info.flags & ParentalControlInfo::FLAG_ENABLED) && coppacs.unknown01 != 0) {
        return COPPACS_STATE_AUTHORIZED;
    }
    return COPPACS_STATE_ON;
}

// 0x00350E38 | fefates:bytes [tier B]
bool IsRestrictP2pInternet()
{
    return (GetParentalControlFlags() & ParentalControlInfo::FLAG_RESTRICT_P2P_INTERNET) != 0;
}

// 0x00350E70
void GetParentalControlInfo(nn::cfg::CTR::ParentalControlInfo* info)
{
    GetConfigOrThrow(info, sizeof(*info), BLOCK_PARENTAL_CONTROL);
}

// 0x00350E98 | nintendogs:bytes [tier B]
bool IsRestrictPhotoExchange()
{
    return (GetParentalControlFlags() & ParentalControlInfo::FLAG_RESTRICT_PHOTO_EXCHANGE) != 0;
}

// 0x00350ED0
bool CheckParentalControlPinCode(const char* pinCode)
{
    ParentalControlInfo info;
    GetConfigOrThrow(&info, sizeof(info), BLOCK_PARENTAL_CONTROL);
    for (size_t i = 0; i < PIN_CODE_LENGTH; i++) {
        if (pinCode[i] != info.pinCode[i]) {
            return false;
        }
    }
    return true;
}

} // namespace CTR
} // namespace cfg
} // namespace nn

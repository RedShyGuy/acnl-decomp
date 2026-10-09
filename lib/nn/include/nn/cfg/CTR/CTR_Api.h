#pragma once

#include "decomp.h"
#include "nn/cfg/CTR/cfg_Types.h"

namespace nn {
namespace cfg {
namespace CTR {
// errors of the getters below are fatal (ThrowFatalErrAll)
void Initialize(); // 0x0011D544 | nintendogs:bytes [tier A]
// (a branch to detail::Finalize)
void Finalize(); // 0x00130A5C | tier X
// (a nop that falls into detail::GetRegion)
nn::cfg::CTR::CfgRegionCode GetRegion(); // 0x0011FEEC
// the language of the settings if the region has it, else the one of the region
nn::cfg::CTR::CfgLanguageCode GetLanguage(); // 0x0011E0A4 | fefates:bytes-fuzzy [tier B]
// config block 0x130000 of a debug console (opens a port itself)
bool IsDebugMode(); // 0x0011E1CC | nintendogs:bytes [tier A]
u8 GetFsLatencyEmulationParam(); // 0x0011E244 | fefates:bytes [tier B]
nn::cfg::CTR::CfgCountryCode GetCountry(); // 0x00350AE8 | nintendogs:bytes [tier A]
// config block 0xA0001 (name is ours)
void GetBirthday(nn::cfg::CTR::Birthday* birthday); // 0x00350B24
// config block 0xA0000 (symbols.json: nn::os::CriticalSection::ScopedLock::~ScopedLock)
void GetUserName(nn::cfg::CTR::UserName* userName); // 0x00350B70
// the agreed EULA version (config block 0xD0000) is at least the one of the title's SMDH
bool IsAgreedEula(); // 0x00350BAC | nintendogs:bytes [tier A]
// the codes of the tables (0 out of range)
const char* GetRegionCodeA3(nn::cfg::CTR::CfgRegionCode region); // 0x00350C2C | fefates:bytes [tier B]
const char* GetCountryCodeA2(nn::cfg::CTR::CfgCountryCode country); // 0x00350C44 | fefates:bytes [tier B]
const char* GetLanguageCodeA2(nn::cfg::CTR::CfgLanguageCode language); // 0x00350C94 | fefates:bytes [tier B]
// restrictions of the parental controls (config block 0xC0000)
bool IsRestrictP2pCec(); // 0x00350C5C | nintendogs:bytes [tier A]
bool IsRestrictP2pInternet(); // 0x00350E38 | fefates:bytes [tier B]
bool IsRestrictPhotoExchange(); // 0x00350E98 | nintendogs:bytes [tier B]
// (names are ours)
bool IsRestrictFriendRegistration(); // 0x00350D64
void GetParentalControlInfo(nn::cfg::CTR::ParentalControlInfo* info); // 0x00350E70
bool CheckParentalControlPinCode(const char* pinCode); // 0x00350ED0
// 0: COPPACS off, 1: on and authorized (parental controls), 2: on (name is ours)
s32 GetCoppacsState(); // 0x00350D9C
// US / Canadian console (3dbrew: GetRegionCanadaUSA, also IsCoppacsSupported)
bool IsCoppacsSupported(); // 0x00350D44
// an id of this console (the meaning of the argument is not known; ubl passes 0)
u64 GetTransferableId(u32 unknown); // 0x00350CB4 | nintendogs:bytes [tier A]
// GetTransferableId(0) (a fall through; name is ours)
u64 GetTransferableId(); // 0x00350CAC
// config block 0x90000 (name is ours; symbols.json: nn::cfg::CTR::GetUserName)
u64 GetLocalFriendCodeSeed(); // 0x00350CDC
void GetSimpleAddressId(nn::cfg::CTR::SimpleAddressId* id); // 0x00350D18 | fefates:bytes [tier B]
} // namespace CTR
} // namespace cfg
} // namespace nn

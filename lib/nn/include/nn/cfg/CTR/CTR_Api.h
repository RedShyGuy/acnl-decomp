#pragma once

#include "decomp.h"
#include "nn/cfg/CTR/cfg_Types.h"

namespace nn {
namespace cfg {
namespace CTR {
void Initialize(); // 0x0011D544 | nintendogs:bytes [tier A]
void Finalize(); // 0x00130A5C | tier X
// config block 0xA0000 (symbols.json: nn::os::CriticalSection::ScopedLock::~ScopedLock)
void GetUserName(nn::cfg::CTR::UserName* userName); // 0x00350B70
// config block 0x90000 (name is ours; symbols.json: nn::cfg::CTR::GetUserName)
u64 GetLocalFriendCodeSeed(); // 0x00350CDC
void GetLanguage(); // 0x0011E0A4 | fefates:bytes-fuzzy [tier B]
bool IsDebugMode(); // 0x0011E1CC | nintendogs:bytes [tier A]
u8 GetFsLatencyEmulationParam(); // 0x0011E244 | fefates:bytes [tier B]
void GetCountry(); // 0x00350AE8 | nintendogs:bytes [tier A]
void IsAgreedEula(); // 0x00350BAC | nintendogs:bytes [tier A]
void GetRegionCodeA3(nn::cfg::CTR::CfgRegionCode); // 0x00350C2C | fefates:bytes [tier B]
void GetCountryCodeA2(nn::cfg::CTR::CfgCountryCode); // 0x00350C44 | fefates:bytes [tier B]
void IsRestrictP2pCec(); // 0x00350C5C | nintendogs:bytes [tier A]
void GetLanguageCodeA2(nn::cfg::CTR::CfgLanguageCode); // 0x00350C94 | fefates:bytes [tier B]
void GetTransferableId(unsigned); // 0x00350CB4 | nintendogs:bytes [tier A]
void GetSimpleAddressId(nn::cfg::CTR::SimpleAddressId*); // 0x00350D18 | fefates:bytes [tier B]
void IsRestrictP2pInternet(); // 0x00350E38 | fefates:bytes [tier B]
void IsRestrictPhotoExchange(); // 0x00350E98 | nintendogs:bytes [tier B]
} // namespace CTR
} // namespace cfg
} // namespace nn

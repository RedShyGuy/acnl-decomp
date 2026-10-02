#pragma once

#include "decomp.h"

namespace nn {
namespace nfp {
namespace CTR {
void Initialize(); // 0x003DA730 | fefates:bytes [tier B]
void GetNfpState(); // 0x003DA758 | fefates:bytes [tier B]
void GetNfpRomInfo(nn::nfp::RomInfo*); // 0x003DA788 | fefates:bytes [tier B]
void StartDetection(); // 0x003DA804 | fefates:bytes [tier B]
void GetConnectResult(nn::Result*); // 0x003DA83C | fefates:bytes [tier B]
void StartAmiiboSettings(nn::nfp::CTR::Parameter*); // 0x003DA944 | fefates:bytes [tier B]
void GetTargetConnectionStatus(nn::nfp::TargetConnectionStatus*); // 0x003DAA3C | fefates:bytes [tier B]
void IsAmiiboSettingsAvailable(); // 0x003DAA9C | fefates:bytes [tier B]
void InitializeParameterForUpdate(nn::nfp::CTR::Parameter*); // 0x003DAABC | fefates:bytes [tier B]
void Finalize(); // 0x003DACEC | fefates:bytes [tier B]
} // namespace CTR
} // namespace nfp
} // namespace nn

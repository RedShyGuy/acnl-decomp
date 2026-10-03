#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/nfc/CTR/nfc_Types.h"
#include "nn/nfp/nfp_Types.h"

namespace nn {
namespace nfp {
namespace CTR {
// The amiibo library on top of the nfc:u service (nfp_Api.cpp). The results of the service are
// converted to the module of this library (NFP, 97).
nn::Result Initialize(); // 0x003DA730 | fefates:bytes [tier B]
nn::Result Finalize(); // 0x003DACEC | fefates:bytes [tier B]
nn::Result StartCommunication(); // 0x003DAC50 (name is ours, after NFC:StartCommunication)
nn::Result Disconnect(); // 0x003DA6FC | tier C (confirmed by the code)
nn::Result StartDetection(); // 0x003DA804 | fefates:bytes [tier B]
nn::Result StopDetection(); // 0x003DA7D0 | tier C (confirmed by the code)
nn::Result ResetTagScanState(); // 0x003DACB8 (name is ours, after NFC:ResetTagScanState)
nn::Result Command0A(); // 0x003DAC84 (name is ours, after NfcIpc::Command0A)
// the state of NFC, 0 when the library is not initialized
nn::nfc::CTR::NfcState GetNfpState(); // 0x003DA758 | fefates:bytes [tier B]
nn::Result GetTargetConnectionStatus(nn::nfp::TargetConnectionStatus* status); // 0x003DAA3C | fefates:bytes [tier B]
// *result: the converted result of the connection
nn::Result GetConnectResult(nn::Result* result); // 0x003DA83C | fefates:bytes [tier B]
nn::Result GetNfpRomInfo(nn::nfp::RomInfo* info); // 0x003DA788 | fefates:bytes [tier B]

// amiibo Settings (a system applet)
bool IsAmiiboSettingsAvailable(); // 0x003DAA9C | fefates:bytes [tier B]
void InitializeParameter(nn::nfp::CTR::Parameter* parameter); // 0x003DA8A0 (name is ours)
void InitializeParameterForUpdate(nn::nfp::CTR::Parameter* parameter); // 0x003DAABC | fefates:bytes [tier B]
// starts the applet and waits until it returns the parameter; false (and parameter->result set) on
// failure
bool StartAmiiboSettings(nn::nfp::CTR::Parameter* parameter); // 0x003DA944 | fefates:bytes [tier B]
} // namespace CTR
} // namespace nfp
} // namespace nn

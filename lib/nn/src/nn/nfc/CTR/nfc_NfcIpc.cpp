#include "nn/nfc/CTR/nfc_NfcIpc.h"

namespace nn {
namespace nfc {
namespace CTR {
// 0x003DA4EC | fefates:bytes [tier B]
void nn::nfc::CTR::NfcIpc::GetNfpRomInfo(nn::nfp::RomInfo*)
{
}

// 0x003DA528 | fefates:bytes [tier B]
void nn::nfc::CTR::NfcIpc::StopDetection()
{
}

// 0x003DA588 | fefates:bytes [tier B]
void nn::nfc::CTR::NfcIpc::GetConnectResult(nn::Result*)
{
}

// 0x003DA5BC | fefates:bytes [tier B]
void nn::nfc::CTR::NfcIpc::GetTargetConnectionStatus(int*)
{
}

// 0x003DA6C8 | fefates:bytes [tier B]
void nn::nfc::CTR::NfcIpc::GetStatus(nn::nfc::CTR::NfcState*)
{
}

} // namespace CTR
} // namespace nfc
} // namespace nn

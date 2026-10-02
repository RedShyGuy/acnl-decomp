#pragma once

#include "decomp.h"

namespace nn {
namespace nfc {
namespace CTR {
class NfcIpc
{
public:
    void GetNfpRomInfo(nn::nfp::RomInfo*); // 0x003DA4EC | fefates:bytes [tier B]
    void StopDetection(); // 0x003DA528 | fefates:bytes [tier B]
    void GetConnectResult(nn::Result*); // 0x003DA588 | fefates:bytes [tier B]
    void GetTargetConnectionStatus(int*); // 0x003DA5BC | fefates:bytes [tier B]
    void GetStatus(nn::nfc::CTR::NfcState*); // 0x003DA6C8 | fefates:bytes [tier B]
};
} // namespace CTR
} // namespace nfc
} // namespace nn

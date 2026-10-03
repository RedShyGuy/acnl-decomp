#pragma once

#include "decomp.h"

namespace nn {
namespace nfc {
namespace CTR {

// what the NFC service is initialized for (the enum name is from the symbols; nn::nfp uses 2)
enum Mode : u8 {
    MODE_NFP = 2,   // (name is ours)
};

// the tag state of NFC:GetTagState (the enum name is from the symbols; values not named)
enum NfcState : u8 {};

} // namespace CTR
} // namespace nfc
} // namespace nn

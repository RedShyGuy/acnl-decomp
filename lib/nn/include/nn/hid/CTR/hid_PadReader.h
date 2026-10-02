#pragma once

#include "decomp.h"

namespace nn {
namespace hid {
namespace CTR {
class PadReader
{
public:
    PadReader(); // TODO: default ctor added so derived stubs compile - may not exist
    void ReadLatest(nn::hid::CTR::PadStatus*); // 0x003546F0 | nintendogs:bytes [tier A]
    void NormalizeStickWithScale(float*, float*, short, short); // 0x00354820 | nintendogs:bytes [tier B]
    PadReader(nn::hid::CTR::Pad&); // 0x00354838 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace hid
} // namespace nn

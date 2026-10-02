#pragma once

#include "decomp.h"

namespace nn {
namespace snd {
namespace CTR {
class DspFxReverb
{
public:
    struct Param { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void Attach(nn::snd::CTR::AuxBusId); // 0x00460E64 | fefates:bytes [tier B]
    void Enable(bool); // 0x00460EBC | fefates:bytes [tier B]
    void Finalize(); // 0x00460F48 | fefates:bytes [tier B]
    void SetParam(const nn::snd::CTR::DspFxReverb::Param&); // 0x00461014 | fefates:bytes [tier B]
};
} // namespace CTR
} // namespace snd
} // namespace nn

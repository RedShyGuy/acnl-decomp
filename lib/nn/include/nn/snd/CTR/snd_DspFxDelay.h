#pragma once

#include "decomp.h"

namespace nn {
namespace snd {
namespace CTR {
class DspFxDelay
{
public:
    struct Param { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void Attach(nn::snd::CTR::AuxBusId); // 0x004608CC | fefates:bytes [tier B]
    void Enable(bool); // 0x0046095C | fefates:bytes [tier B]
    void Finalize(); // 0x004609E8 | fefates:bytes [tier B]
    void SetParam(const nn::snd::CTR::DspFxDelay::Param&); // 0x00460AB4 | fefates:bytes [tier B]
};
} // namespace CTR
} // namespace snd
} // namespace nn

#pragma once

#include "decomp.h"

namespace nn {
namespace snd {
namespace CTR {
// RTTI N2nn3snd3CTR7FxDelayE @ 0x008D02D8
// vtable 0x009020A0 (vptr 0x009020A8), offset_to_top 0, 2 entries
class FxDelay
{
public:
    struct Param { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual ~FxDelay(); // 0x00465874 slot 0x00 | fefates:bytes
    // 0x00465804 slot 0x04 | nintendogs:bytes (deleting dtor)
    void Initialize(); // 0x004653F4 | nintendogs:bytes [tier A]
    void UpdateBuffer(unsigned); // 0x0046549C | nintendogs:bytes [tier A]
    void AssignWorkBuffer(unsigned, unsigned); // 0x00465594 | nintendogs:bytes [tier A]
    void GetRequiredMemSize(); // 0x004655C8 | nintendogs:bytes [tier A]
    void Finalize(); // 0x004655E4 | nintendogs:bytes [tier A]
    void SetParam(const nn::snd::CTR::FxDelay::Param&); // 0x00465638 | nintendogs:bytes [tier A]
    FxDelay(); // 0x00465768 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace snd
} // namespace nn

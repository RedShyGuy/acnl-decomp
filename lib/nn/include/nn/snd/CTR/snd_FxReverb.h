#pragma once

#include "decomp.h"

namespace nn {
namespace snd {
namespace CTR {
// RTTI N2nn3snd3CTR8FxReverbE @ 0x008D02E0
// vtable 0x009020B0 (vptr 0x009020B8), offset_to_top 0, 2 entries
class FxReverb
{
public:
    struct Param { u32 _unknown; }; // TODO: real type unknown (placeholder)
    FxReverb(); // ctor candidate(s) 0x00466138 (unverified)
    virtual void vf_0x00(); // 0x004662E4 slot 0x00 | virtual slot, introduced by nn::snd::CTR::FxReverb
    virtual ~FxReverb(); // 0x0046628C slot 0x04 | nintendogs:bytes
    void Initialize(); // 0x00465964 | nintendogs:bytes [tier A]
    void UpdateBuffer(unsigned); // 0x00465A70 | nintendogs:callgraph [tier A]
    void InitializeParam(); // 0x00465CA0 | fefates:bytes [tier B]
    void AssignWorkBuffer(unsigned, unsigned); // 0x00465E9C | nintendogs:bytes [tier A]
    void GetRequiredMemSize(); // 0x00465ED0 | nintendogs:bytes [tier A]
    void Finalize(); // 0x00465F7C | nintendogs:bytes [tier A]
    void SetParam(const nn::snd::CTR::FxReverb::Param&); // 0x00465FC4 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace snd
} // namespace nn

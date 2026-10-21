#pragma once

#include "decomp.h"
#include "nn/nex/nex_InstanceControl.h"
#include "nn/nex/nex_ThreadVariable.h"

namespace nn {
namespace nex {
// An instance of the current context of the thread (0 unless contexts are used). The names of the
// static members are ours.
// RTTI N2nn3nex15PseudoSingletonE @ 0x008CE4CC
// vtable 0x008FCF04 (vptr 0x008FCF0C), offset_to_top 0, 2 entries
class PseudoSingleton : public ::nn::nex::InstanceControl
{
public:
    explicit PseudoSingleton(u32 kind); // 0x0037AF48
    // (only a nop in the original that falls into ~InstanceControl)
    virtual ~PseudoSingleton(); // 0x00378640 slot 0x00
    // 0x0037AF94 slot 0x04 (deleting dtor)

    // (inline)
    static u32 GetCurrentContext()
    {
        if (!s_IsContextUsed) {
            return 0;
        }
        return s_pCurrentContext->GetRef();
    }
    // the context of the current thread (false if contexts are not used)
    static bool SetContext(unsigned int context); // 0x0037AEA8 | fefates:callseq [tier B]

    static bool s_IsContextUsed;
    static ThreadVariable<u32>* s_pCurrentContext;
};
} // namespace nex
} // namespace nn

#pragma once

#include "decomp.h"

namespace nn {
namespace boss {
namespace detail {
// RTTI N2nn4boss6detail10IpcManagerE @ 0x008D039C
// vtable 0x009021DC (vptr 0x009021E4), offset_to_top 0, 2 entries
class IpcManager
{
public:
    IpcManager(); // ctor candidate(s) 0x007998DC (unverified)
    virtual ~IpcManager(); // 0x0046D074 slot 0x00 | nintendogs:bytes
    virtual void vf_0x04(); // 0x0046D010 slot 0x04 | virtual slot, introduced by nn::boss::detail::IpcManager
    void FinalizeUserIpc(); // 0x0046CF24 | nintendogs:bytes [tier A]
    void InitializeUserIpc(); // 0x0046CF6C | nintendogs:bytes [tier A]
};
} // namespace detail
} // namespace boss
} // namespace nn

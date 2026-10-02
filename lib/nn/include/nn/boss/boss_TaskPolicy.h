#pragma once

#include "decomp.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss10TaskPolicyE @ 0x008D0354
// vtable 0x00902150 (vptr 0x00902158), offset_to_top 0, 2 entries
class TaskPolicy
{
public:
    TaskPolicy(); // ctor candidate(s) 0x0046AA4C (unverified)
    virtual void vf_0x00(); // 0x0046AA74 slot 0x00 | virtual slot, introduced by nn::boss::TaskPolicy
    virtual void vf_0x04(); // 0x0046AA70 slot 0x04 | virtual slot, introduced by nn::boss::TaskPolicy
    void SetProperty(nn::boss::PropertyType, const void*, unsigned); // 0x0046A8B8 | nintendogs:bytes-fuzzy [tier A]
    void InitializeWithSecInterval(unsigned, unsigned); // 0x0046AA14 | nintendogs:bytes [tier A]
};
} // namespace boss
} // namespace nn

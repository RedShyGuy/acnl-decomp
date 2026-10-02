#pragma once

#include "decomp.h"
#include "nn/pia/session/session_SessionSearchCriteria.h"

namespace nn {
namespace pia {
namespace inet {
// RTTI N2nn3pia4inet24NexSessionSearchCriteriaE @ 0x008CF9F0
// vtable 0x0090059C (vptr 0x009005A4), offset_to_top 0, 3 entries
class NexSessionSearchCriteria : public ::nn::pia::session::SessionSearchCriteria
{
public:
    virtual void vf_0x00(); // 0x0040BCF8 slot 0x00 | virtual slot, introduced by nn::pia::inet::NexSessionSearchCriteria
    virtual void vf_0x04(); // 0x0040BCF0 slot 0x04 | virtual slot, introduced by nn::pia::inet::NexSessionSearchCriteria
    virtual void vf_0x08(); // 0x0072F84C slot 0x08 | virtual slot, introduced by nn::pia::inet::NexSessionSearchCriteria
    void SetGameMode(unsigned long); // 0x0040B298 | libgarden [tier A]
    void SetOpenedOnly(bool); // 0x0040B304 | libgarden [tier A]
    void SetMaxParticipants(unsigned short); // 0x0040B358 | libgarden [tier A]
    void SetMinParticipants(unsigned short); // 0x0040B378 | libgarden [tier A]
    NexSessionSearchCriteria(); // 0x0040BC10 | libgarden [tier A]
};
} // namespace inet
} // namespace pia
} // namespace nn

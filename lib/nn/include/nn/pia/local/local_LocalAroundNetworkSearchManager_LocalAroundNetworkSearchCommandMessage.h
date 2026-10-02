#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalMessage.h"

// RTTI N2nn3pia5local31LocalAroundNetworkSearchManager38LocalAroundNetworkSearchCommandMessageE @ 0x008CFD84
// vtable 0x00901388 (vptr 0x00901390), offset_to_top 0, 4 entries
class nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkSearchCommandMessage : public ::nn::pia::local::LocalMessage
{
public:
    LocalAroundNetworkSearchCommandMessage(); // ctor candidate(s) 0x00423F9C, 0x00424040 (unverified)
    virtual void vf_0x00(); // 0x0042412C slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void vf_0x04(); // 0x00424128 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void UpdateMessageHeader(); // 0x0042410C slot 0x08 | slot vf_0x08 of nn::pia::local::LocalMessage
    virtual void ParseMessageHeader(); // 0x004240E4 slot 0x0C | slot vf_0x0C of nn::pia::local::LocalMessage
};

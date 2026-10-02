#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalMessage.h"

// RTTI N2nn3pia5local31LocalAroundNetworkSearchManager34LocalAroundNetworkStatusAckMessageE @ 0x008CFD78
// vtable 0x00901370 (vptr 0x00901378), offset_to_top 0, 4 entries
class nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusAckMessage : public ::nn::pia::local::LocalMessage
{
public:
    LocalAroundNetworkStatusAckMessage(); // ctor candidate(s) 0x00423E8C (unverified)
    virtual void vf_0x00(); // 0x00423E88 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void vf_0x04(); // 0x00423E84 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void UpdateMessageHeader(); // 0x00423E68 slot 0x08 | slot vf_0x08 of nn::pia::local::LocalMessage
    virtual void ParseMessageHeader(); // 0x00423E40 slot 0x0C | slot vf_0x0C of nn::pia::local::LocalMessage
};

#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalMessage.h"

// RTTI N2nn3pia5local31LocalAroundNetworkSearchManager31LocalAroundNetworkStatusMessageE @ 0x008CFD6C
// vtable 0x00901358 (vptr 0x00901360), offset_to_top 0, 4 entries
class nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage : public ::nn::pia::local::LocalMessage
{
public:
    LocalAroundNetworkStatusMessage(); // ctor candidate(s) 0x00423B90 (unverified)
    virtual void vf_0x00(); // 0x00423CEC slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void vf_0x04(); // 0x00423CE8 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void UpdateMessageHeader(); // 0x00423CCC slot 0x08 | slot vf_0x08 of nn::pia::local::LocalMessage
    virtual void ParseMessageHeader(); // 0x00423CA4 slot 0x0C | slot vf_0x0C of nn::pia::local::LocalMessage
};

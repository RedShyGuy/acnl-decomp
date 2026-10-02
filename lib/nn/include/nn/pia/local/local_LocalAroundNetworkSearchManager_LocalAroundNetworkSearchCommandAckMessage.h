#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalMessage.h"

// RTTI N2nn3pia5local31LocalAroundNetworkSearchManager41LocalAroundNetworkSearchCommandAckMessageE @ 0x008CFD90
// vtable 0x009013A0 (vptr 0x009013A8), offset_to_top 0, 4 entries
class nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkSearchCommandAckMessage : public ::nn::pia::local::LocalMessage
{
public:
    LocalAroundNetworkSearchCommandAckMessage(); // ctor candidate(s) 0x004244CC (unverified)
    virtual void vf_0x00(); // 0x004243C8 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void vf_0x04(); // 0x004243C4 slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
    virtual void UpdateMessageHeader(); // 0x004243A8 slot 0x08 | slot vf_0x08 of nn::pia::local::LocalMessage
    virtual void ParseMessageHeader(); // 0x00424380 slot 0x0C | slot vf_0x0C of nn::pia::local::LocalMessage
};

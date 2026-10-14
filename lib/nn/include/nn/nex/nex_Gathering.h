#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_Gathering.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex9GatheringE @ 0x008CF750
// vtable 0x008FFD14 (vptr 0x008FFD1C), offset_to_top 0, 9 entries
class Gathering : public ::nn::nex::_DDL_Gathering
{
public:
    // the values of a new gathering (the names are ours)
    static const u32 DEFAULT_PARTICIPATION_POLICY = 1;
    static const u32 DEFAULT_FLAGS = 0x200;

    Gathering(); // 0x003D6A90 | fefates:bytes (symbols.json picks a conflicting nintendogs name)
    virtual ~Gathering(); // 0x003D6B0C slot 0x00 | fefates:callgraph
    // 0x003D6AEC slot 0x04 (deleting dtor)
    // (empty; the name is ours, after UserMessage)
    virtual void Trace() const; // 0x0072EA04 slot 0x20
    void Reset(); // 0x003D6A5C | fefates:bytes [tier B]
    // (name is ours)
    void SetDescription(const String& description); // 0x003D6A14
    void SetParticipationPolicy(unsigned int policy, unsigned int argument); // 0x003D6A50 | fefates:callgraph [tier C]
};
ASSERT_SIZE(Gathering, 0x30);
} // namespace nex
} // namespace nn

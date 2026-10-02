#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

// RTTI 12ParameterHIO @ 0x008CB7B4
// vtable 0x008EE72C (vptr 0x008EE734), offset_to_top 0, 1 entries
class ParameterHIO : public ::sead::hostio::Node
{
public:
    ParameterHIO(); // ctor candidate(s) 0x00796EEC (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
};

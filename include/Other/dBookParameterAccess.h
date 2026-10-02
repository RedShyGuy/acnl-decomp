#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"

// RTTI 19BookParameterAccess @ 0x008CC940
// vtable 0x008F4D7C (vptr 0x008F4D84), offset_to_top 0, 3 entries
class BookParameterAccess : public ::sead::hostio::Node
{
public:
    BookParameterAccess(); // ctor candidate(s) 0x002EEFDC (unverified)
    virtual void vf_0x00(); // 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
    virtual void vf_0x04(); // 0x002EF070 slot 0x04 | virtual slot, introduced by BookParameterAccess
    virtual void vf_0x08(); // 0x002EF030 slot 0x08 | virtual slot, introduced by BookParameterAccess
};

#pragma once

#include "decomp.h"

// RTTI 6SvProc @ 0x008CD314
// vtable 0x008F8CF0 (vptr 0x008F8CF8), offset_to_top 0, 2 entries: only the destructor
class SvProc
{
public:
    virtual ~SvProc();
};

// the instance (name is ours)
extern SvProc g_SvProc;

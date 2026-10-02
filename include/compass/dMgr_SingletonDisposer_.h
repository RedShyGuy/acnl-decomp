#pragma once

#include "decomp.h"
#include "compass/dMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N7compass3Mgr18SingletonDisposer_E @ 0x008D3F0C
// vtable 0x0090BCC4 (vptr 0x0090BCCC), offset_to_top 0, 2 entries
class compass::Mgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x00615EA8 (unverified)
    virtual ~SingletonDisposer_(); // 0x00615F50 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00615F0C slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};

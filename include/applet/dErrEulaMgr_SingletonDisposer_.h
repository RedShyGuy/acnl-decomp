#pragma once

#include "decomp.h"
#include "applet/dErrEulaMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N6applet10ErrEulaMgr18SingletonDisposer_E @ 0x008D3674
// vtable 0x00909338 (vptr 0x00909340), offset_to_top 0, 2 entries
class applet::ErrEulaMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x005CF520 (unverified)
    virtual ~SingletonDisposer_(); // 0x005CF004 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x005CEFC0 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};

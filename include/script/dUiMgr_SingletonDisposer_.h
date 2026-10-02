#pragma once

#include "decomp.h"
#include "script/dUiMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N6script5UiMgr18SingletonDisposer_E @ 0x008D3A78
// vtable 0x0090A584 (vptr 0x0090A58C), offset_to_top 0, 2 entries
class script::UiMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor address unknown
    virtual ~SingletonDisposer_(); // 0x005EB1D4 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x005EB134 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};

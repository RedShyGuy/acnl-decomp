#pragma once

#include "decomp.h"
#include "photo/dMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N5photo3Mgr18SingletonDisposer_E @ 0x008D2DAC
// vtable 0x0090926C (vptr 0x00909274), offset_to_top 0, 2 entries
class photo::Mgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor address unknown
    virtual ~SingletonDisposer_(); // 0x005B3554 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x005B3504 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};

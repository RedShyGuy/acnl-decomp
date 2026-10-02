#pragma once

#include "decomp.h"
#include "sead/seadIDisposer.h"
#include "sead/seadProcessMeter.h"

// RTTI N4sead12ProcessMeter18SingletonDisposer_E @ 0x008D15AC
// vtable 0x00905248 (vptr 0x00905250), offset_to_top 0, 2 entries
class sead::ProcessMeter::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor address unknown
    virtual ~SingletonDisposer_(); // 0x005410DC slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00541098 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};

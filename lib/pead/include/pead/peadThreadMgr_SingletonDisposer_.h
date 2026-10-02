#pragma once

#include "decomp.h"
#include "pead/peadIDisposer.h"
#include "pead/peadThreadMgr.h"

// RTTI N4pead9ThreadMgr18SingletonDisposer_E @ 0x008D1334
// vtable 0x00904DD4 (vptr 0x00904DDC), offset_to_top 0, 2 entries
class pead::ThreadMgr::SingletonDisposer_ : public ::pead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0053DC2C (unverified)
    virtual ~SingletonDisposer_(); // 0x0053DDEC slot 0x00 | slot vf_0x00 of pead::IDisposer
    // 0x0053DD94 slot 0x04 | slot vf_0x04 of pead::IDisposer (deleting dtor)
};

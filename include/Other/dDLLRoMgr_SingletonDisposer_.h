#pragma once

#include "decomp.h"
#include "Other/dDLLRoMgr.h"
#include "sead/seadIDisposer.h"

// RTTI N8DLLRoMgr18SingletonDisposer_E @ 0x008D3FFC
// vtable 0x0090C084 (vptr 0x0090C08C), offset_to_top 0, 2 entries
class DLLRoMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x0011F0FC (unverified)
    virtual ~SingletonDisposer_(); // 0x006A4FB4 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x006A4F58 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};

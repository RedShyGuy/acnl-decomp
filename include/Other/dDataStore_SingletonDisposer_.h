#pragma once

#include "decomp.h"
#include "Other/dDataStore.h"
#include "sead/seadIDisposer.h"

// RTTI N9DataStore18SingletonDisposer_E @ 0x008D40C0
// vtable 0x0090C228 (vptr 0x0090C230), offset_to_top 0, 2 entries
class DataStore::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor candidate(s) 0x006E3148 (unverified)
    virtual ~SingletonDisposer_(); // 0x006E3DF8 slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x006E3D38 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
};

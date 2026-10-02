#pragma once

#include "decomp.h"
#include "Other/dDataStore.h"
#include "net/dSearchDataResult.h"

// RTTI N9DataStore8DataInfoE @ 0x008D40CC
// vtable 0x0090C238 (vptr 0x0090C240), offset_to_top 0, 6 entries
class DataStore::DataInfo : public ::net::SearchDataResult
{
public:
    DataInfo(); // ctor candidate(s) 0x006E4DDC (unverified)
    virtual void vf_0x00(); // 0x0050D1AC slot 0x00 | virtual slot, introduced by net::SearchDataResult
    virtual void vf_0x04(); // 0x006E4DF4 slot 0x04 | virtual slot, introduced by net::SearchDataResult
    virtual void vf_0x14(); // 0x00770494 slot 0x14 | virtual slot, introduced by net::SearchDataResult
};

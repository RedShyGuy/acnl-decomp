#pragma once

#include "decomp.h"
#include "net/dScanBufBase.h"
#include "netgame/dMatchingList.h"

// RTTI N7netgame12MatchingList7ScanBufE @ 0x008D3F44
// vtable 0x0090BD24 (vptr 0x0090BD2C), offset_to_top 0, 4 entries
class netgame::MatchingList::ScanBuf : public ::net::ScanBufBase
{
public:
    ScanBuf(); // ctor candidate(s) 0x006207E0 (unverified)
    virtual void vf_0x00(); // 0x0062083C slot 0x00 | virtual slot, introduced by netgame::MatchingList::ScanBuf
    virtual void vf_0x04(); // 0x00620838 slot 0x04 | virtual slot, introduced by netgame::MatchingList::ScanBuf
    virtual void vf_0x08(); // 0x006207D8 slot 0x08 | virtual slot, introduced by netgame::MatchingList::ScanBuf
    virtual void vf_0x0C(); // 0x0075F290 slot 0x0C | virtual slot, introduced by netgame::MatchingList::ScanBuf
};

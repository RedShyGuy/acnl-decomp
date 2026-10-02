#pragma once

#include "decomp.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss6NsDataE @ 0x008D0394
// vtable 0x009021CC (vptr 0x009021D4), offset_to_top 0, 2 entries
class NsData
{
public:
    virtual void vf_0x00(); // 0x0046CF20 slot 0x00 | virtual slot, introduced by nn::boss::NsData
    virtual void vf_0x04(); // 0x0046CF1C slot 0x04 | virtual slot, introduced by nn::boss::NsData
    void Initialize(unsigned); // 0x0046C854 | nintendogs:bytes [tier A]
    void GetReadFlag(bool*); // 0x0046C874 | nintendogs:bytes [tier A]
    void SetReadFlag(bool); // 0x0046C920 | nintendogs:bytes [tier A]
    void GetHeaderInfo(nn::boss::HeaderInfoType, void*, unsigned); // 0x0046C9B8 | nintendogs:bytes [tier A]
    void GetLastUpdated(nn::fnd::DateTime*); // 0x0046CA84 | nintendogs:bytes [tier B]
    void ReadData(unsigned char*, unsigned); // 0x0046CC34 | nintendogs:bytes [tier A]
    NsData(); // 0x0046CED8 | nintendogs:bytes [tier A]
};
} // namespace boss
} // namespace nn

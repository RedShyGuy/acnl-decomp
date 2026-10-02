#pragma once

#include "decomp.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss12NsDataIdListE @ 0x008D0364
// vtable 0x00902170 (vptr 0x00902178), offset_to_top 0, 2 entries
class NsDataIdList
{
public:
    NsDataIdList(); // ctor candidate(s) 0x0046ADE8 (unverified)
    virtual void vf_0x00(); // 0x0046AE18 slot 0x00 | virtual slot, introduced by nn::boss::NsDataIdList
    virtual void vf_0x04(); // 0x0046AE14 slot 0x04 | virtual slot, introduced by nn::boss::NsDataIdList
    void GetNsDataId(unsigned short); // 0x0046ADBC | nintendogs:bytes [tier A]
    NsDataIdList(unsigned*, unsigned short); // 0x0046ADE8 | nintendogs:bytes [tier A]
};
} // namespace boss
} // namespace nn

#pragma once

#include "decomp.h"
#include "Other/dBase.h"

namespace photo {
// RTTI N5photo8BsInvokeE @ 0x008D2DB8
// vtable 0x0090927C (vptr 0x00909284), offset_to_top 0, 16 entries
class BsInvoke : public ::Base
{
public:
    BsInvoke(); // ctor address unknown
    virtual ~BsInvoke(); // 0x005B4894 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x005B4854 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x005B46BC slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x005B477C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x005B46EC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x005B46A8 slot 0x30 | slot vf_0x30 of oml::framework::Process
};
} // namespace photo

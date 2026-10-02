#pragma once

#include "decomp.h"
#include "lyt/dTextAreaEx.h"

namespace lyt {
// RTTI N3lyt12TextAreaMailE @ 0x008D0F1C
// vtable 0x009042D4 (vptr 0x009042DC), offset_to_top 0, 25 entries
class TextAreaMail : public ::lyt::TextAreaEx
{
public:
    TextAreaMail(); // ctor address unknown
    virtual ~TextAreaMail(); // 0x00504FEC slot 0x00 | slot vf_0x00 of lyt::Object
    // 0x00504F20 slot 0x04 | slot vf_0x04 of lyt::Object (deleting dtor)
    virtual void vf_0x08(); // 0x00503918 slot 0x08 | virtual slot, introduced by lyt::Object
    virtual void vf_0x10(); // 0x005034DC slot 0x10 | virtual slot, introduced by lyt::TextArea
    virtual void vf_0x14(); // 0x00502FE0 slot 0x14 | virtual slot, introduced by lyt::TextArea
    virtual void vf_0x1C(); // 0x007478A4 slot 0x1C | virtual slot, introduced by lyt::TextArea
    virtual void vf_0x38(); // 0x00502F38 slot 0x38 | virtual slot, introduced by lyt::TextAreaEx
    virtual void vf_0x44(); // 0x00501F04 slot 0x44 | virtual slot, introduced by lyt::TextAreaEx
    virtual void vf_0x48(); // 0x00502538 slot 0x48 | virtual slot, introduced by lyt::TextAreaEx
    virtual void vf_0x58(); // 0x005025A0 slot 0x58 | virtual slot, introduced by lyt::TextAreaEx
};
} // namespace lyt

#pragma once

#include "decomp.h"
#include "lyt/dTextAreaOneLine.h"

namespace lyt {
// RTTI N3lyt20TextAreaOneLineBirthE @ 0x008D0F4C
// vtable 0x0090442C (vptr 0x00904434), offset_to_top 0, 14 entries
class TextAreaOneLineBirth : public ::lyt::TextAreaOneLine
{
public:
    TextAreaOneLineBirth(); // ctor address unknown
    virtual ~TextAreaOneLineBirth(); // 0x00507C78 slot 0x00 | slot vf_0x00 of lyt::Object
    // 0x00507C68 slot 0x04 | slot vf_0x04 of lyt::Object (deleting dtor)
    virtual void vf_0x1C(); // 0x00747910 slot 0x1C | virtual slot, introduced by lyt::TextArea
};
} // namespace lyt

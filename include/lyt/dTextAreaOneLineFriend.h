#pragma once

#include "decomp.h"
#include "lyt/dTextAreaOneLine.h"

namespace lyt {
// RTTI N3lyt21TextAreaOneLineFriendE @ 0x008D0F64
// vtable 0x00904484 (vptr 0x0090448C), offset_to_top 0, 14 entries
class TextAreaOneLineFriend : public ::lyt::TextAreaOneLine
{
public:
    TextAreaOneLineFriend(); // ctor address unknown
    virtual ~TextAreaOneLineFriend(); // 0x00507E78 slot 0x00 | slot vf_0x00 of lyt::Object
    // 0x00507E68 slot 0x04 | slot vf_0x04 of lyt::Object (deleting dtor)
    virtual void vf_0x1C(); // 0x00747924 slot 0x1C | virtual slot, introduced by lyt::TextArea
};
} // namespace lyt

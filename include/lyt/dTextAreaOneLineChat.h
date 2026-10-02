#pragma once

#include "decomp.h"
#include "lyt/dTextAreaOneLine.h"

namespace lyt {
// RTTI N3lyt19TextAreaOneLineChatE @ 0x008D0F40
// vtable 0x009043EC (vptr 0x009043F4), offset_to_top 0, 14 entries
class TextAreaOneLineChat : public ::lyt::TextAreaOneLine
{
public:
    TextAreaOneLineChat(); // ctor address unknown
    virtual ~TextAreaOneLineChat(); // 0x00507C50 slot 0x00 | slot vf_0x00 of lyt::Object
    // 0x00507C58 slot 0x04 | slot vf_0x04 of lyt::Object (deleting dtor)
    virtual void vf_0x1C(); // 0x007478FC slot 0x1C | virtual slot, introduced by lyt::TextArea
};
} // namespace lyt

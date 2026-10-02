#pragma once

#include "decomp.h"
#include "lyt/dTextAreaOneLine.h"

namespace lyt {
// RTTI N3lyt22TextAreaOneLineCommentE @ 0x008D0F70
// vtable 0x009044C4 (vptr 0x009044CC), offset_to_top 0, 14 entries
class TextAreaOneLineComment : public ::lyt::TextAreaOneLine
{
public:
    TextAreaOneLineComment(); // ctor address unknown
    virtual ~TextAreaOneLineComment(); // 0x00507E8C slot 0x00 | slot vf_0x00 of lyt::Object
    // 0x00507E7C slot 0x04 | slot vf_0x04 of lyt::Object (deleting dtor)
    virtual void vf_0x1C(); // 0x0074793C slot 0x1C | virtual slot, introduced by lyt::TextArea
};
} // namespace lyt

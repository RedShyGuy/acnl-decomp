#pragma once

#include "decomp.h"
#include "lyt/dTextArea.h"

namespace lyt {
// RTTI N3lyt15TextAreaOneLineE @ 0x008D0F34
// vtable 0x009043AC (vptr 0x009043B4), offset_to_top 0, 14 entries
class TextAreaOneLine : public ::lyt::TextArea
{
public:
    TextAreaOneLine(); // ctor candidate(s) 0x00507C28 (unverified)
    virtual ~TextAreaOneLine(); // 0x00507C54 slot 0x00 | slot vf_0x00 of lyt::Object
    // 0x00507C40 slot 0x04 | slot vf_0x04 of lyt::Object (deleting dtor)
    virtual void vf_0x08(); // 0x00507030 slot 0x08 | virtual slot, introduced by lyt::Object
    virtual void vf_0x10(); // 0x00506D50 slot 0x10 | virtual slot, introduced by lyt::TextArea
    virtual void vf_0x14(); // 0x00506C4C slot 0x14 | virtual slot, introduced by lyt::TextArea
    virtual void vf_0x1C(); // 0x007478D0 slot 0x1C | virtual slot, introduced by lyt::TextArea
    virtual void vf_0x20(); // 0x007478F4 slot 0x20 | virtual slot, introduced by lyt::TextArea
    virtual void vf_0x24(); // 0x007478DC slot 0x24 | virtual slot, introduced by lyt::TextArea
    virtual void vf_0x28(); // 0x007478E4 slot 0x28 | virtual slot, introduced by lyt::TextArea
    virtual void vf_0x2C(); // 0x007478EC slot 0x2C | virtual slot, introduced by lyt::TextArea
};
} // namespace lyt

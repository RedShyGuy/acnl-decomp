#pragma once

#include "decomp.h"
#include "lyt/dTextAreaEx.h"

namespace lyt {
// RTTI N3lyt13TextAreaBoardE @ 0x008D0F28
// vtable 0x00904340 (vptr 0x00904348), offset_to_top 0, 25 entries
class TextAreaBoard : public ::lyt::TextAreaEx
{
public:
    TextAreaBoard(); // ctor address unknown
    virtual ~TextAreaBoard(); // 0x00506BCC slot 0x00 | slot vf_0x00 of lyt::Object
    // 0x00506B48 slot 0x04 | slot vf_0x04 of lyt::Object (deleting dtor)
    virtual void vf_0x08(); // 0x00505D50 slot 0x08 | virtual slot, introduced by lyt::Object
    virtual void vf_0x10(); // 0x005059B0 slot 0x10 | virtual slot, introduced by lyt::TextArea
    virtual void vf_0x14(); // 0x0050569C slot 0x14 | virtual slot, introduced by lyt::TextArea
    virtual void vf_0x1C(); // 0x007478B8 slot 0x1C | virtual slot, introduced by lyt::TextArea
    virtual void vf_0x44(); // 0x0050526C slot 0x44 | virtual slot, introduced by lyt::TextAreaEx
    virtual void vf_0x48(); // 0x0050554C slot 0x48 | virtual slot, introduced by lyt::TextAreaEx
    virtual void vf_0x58(); // 0x005055BC slot 0x58 | virtual slot, introduced by lyt::TextAreaEx
};
} // namespace lyt

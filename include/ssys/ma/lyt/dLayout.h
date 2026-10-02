#pragma once

#include "decomp.h"
#include "ssys/ma/lyt/dBase2D.h"

namespace ssys {
namespace ma {
namespace lyt {
// RTTI N4ssys2ma3lyt6LayoutE @ 0x008D249C
// vtable 0x00907310 (vptr 0x00907318), offset_to_top 0, 7 entries
class Layout : public ::ssys::ma::lyt::Base2D
{
public:
    Layout(); // ctor address unknown
    virtual ~Layout(); // 0x00569F68 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00569F1C slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)
    virtual void vf_0x08(); // 0x005699A4 slot 0x08 | virtual slot, introduced by ssys::ma::lyt::Layout
    virtual void Animate(); // 0x005696F8 slot 0x0C | libgarden
    virtual void Build(char const*, ssys::ma::lyt::ArcResAccReader const*, unsigned long); // 0x00569C6C slot 0x10 | libgarden
    virtual void vf_0x14(); // 0x00569D9C slot 0x14 | virtual slot, introduced by ssys::ma::lyt::Layout
    virtual void vf_0x18(); // 0x00569288 slot 0x18 | virtual slot, introduced by ssys::ma::lyt::Layout
    void UnbindAllAnimation(); // 0x00133A70 | libgarden [tier A]
    void FindGroup(char const*); // 0x004B5648 | libgarden [tier A]
    void BindAnimation(ssys::ma::lyt::Animation*, nw::lyt::Group const*, bool); // 0x00569118 | libgarden [tier A]
    void FindPane(char const*); // 0x00569274 | libgarden [tier A]
    void UnbindAnimation(ssys::ma::lyt::Animation*, nw::lyt::Group const*, bool); // 0x0056946C | libgarden [tier A]
    void FindPicture(char const*); // 0x0056950C | libgarden [tier A]
    void FindTextBox(char const*); // 0x00569568 | libgarden [tier A]
};
} // namespace lyt
} // namespace ma
} // namespace ssys

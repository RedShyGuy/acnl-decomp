#pragma once

#include "decomp.h"
#include "Button/dButtonActionNode.h"

namespace ssys {
namespace ma {
namespace lyt {
class ResAccInterface;
} // namespace lyt
} // namespace ma
} // namespace ssys

// A button with animations of its own (its destructor deletes their transforms).
// RTTI 20ButtonActionCstmNode @ 0x008CCB58
// vtable 0x008F576C (vptr 0x008F5774), offset_to_top 0, 16 entries
class ButtonActionCstmNode : public ::ButtonActionNode
{
public:
    ButtonActionCstmNode(); // 0x0031CE64 (name after the RTTI)
    virtual ~ButtonActionCstmNode(); // 0x0031CF18 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x0031CE7C slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)

    // the button with its layout and the animations of the names (names are ours)
    struct Description
    {
        const ssys::ma::lyt::ResAccInterface* resAcc;  // 0x00
        ssys::ma::lyt::Layout* layout;                 // 0x04
        const char* touch;                             // 0x08 the names of the animations (NULL: none)
        const char* select;                            // 0x0C
        const char* touchOk;                           // 0x10
        const char* selectOk;                          // 0x14
        ButtonActionNode::Info info;                   // 0x18
    };
    ASSERT_SIZE(Description, 0x34);

    // (name is ours)
    void Initialize(const Description& description); // 0x0031CDC8
};

#pragma once

#include "decomp.h"
#include "Other/dDragIconNode.h"
#include "Other/dLayout.h"

namespace ssys {
namespace ma {
namespace lyt {
class ArcResAccReader;
} // namespace lyt
} // namespace ma
} // namespace ssys

// An icon of a reaction (emoticon) of the reaction menu (class name from the RTTI, all other names are ours): react_icon.bclyt;
// its frame of the reaction animation is the place of the reaction in slot m_Index among the 40 reactions.
// RTTI 20ReactionDragIconNode @ 0x008CCBE8
// vtable 0x008F5968 (vptr 0x008F5970), offset_to_top 0, 29 entries
class ReactionDragIconNode : public ::DragIconNode
{
public:
    ReactionDragIconNode(); // 0x0032155C
    virtual ~ReactionDragIconNode(); // 0x003215B4 slot 0x00 | slot vf_0x00 of ssys::st::ListNode
    // 0x00321590 slot 0x04 | slot vf_0x04 of ssys::st::ListNode (deleting dtor)

    // the animation of the group plays from its start (rewind) or is set to its end
    void SetGroupAnimation(bool rewind); // 0x003212D8
    // the icon shows the reaction in slot m_Index of the current player (on the amiibo Camera stage after a check of the
    // players); hidden and inactive without one
    void UpdateIcon(); // 0x00321340
    // the layout, the button (index) and the reaction animation
    void Setup(s32 index, ssys::ma::lyt::Animation* reactionAnim, ssys::ma::lyt::ArcResAccReader* reader); // 0x0032146C

    // (names are ours)
    ::Layout m_IconLayout;                       // 0x130
    ssys::ma::lyt::Animation* m_pReactionAnim;   // 0x27C (frame: the place of the reaction)
    ssys::ma::lyt::Animation* m_pGroupAnim;      // 0x280 DragIconGroup::m_Extra (DragIconGroup::AddNode)
};
ASSERT_SIZE(ReactionDragIconNode, 0x284);

#pragma once

#include "decomp.h"
#include "ssys/ma/lyt/dAnimation.h"
#include "ssys/st/dList.h"
#include "ssys/st/dListNode.h"

class DragIconNode;
class ReactionDragIconNode;
namespace nw {
namespace lyt {
class Group;
class Pane;
} // namespace lyt
} // namespace nw
namespace ssys {
namespace ma {
namespace lyt {
class Layout;
class ResAccInterface;
} // namespace lyt
} // namespace ma
} // namespace ssys

// The icons of a DragIconControl that belong together (class name from the RTTI, all other names are ours): the list of
// the icons, the animations they share and a marker pane (with its animation) that shows on the touched icon.
// RTTI 13DragIconGroup @ 0x008CB9DC
// vtable 0x008EF270 (vptr 0x008EF278), offset_to_top 0, 2 entries
// vtable 0x008EF280 (vptr 0x008EF288), offset_to_top -12, 2 entries
class DragIconGroup : public ::ssys::st::List, public ::ssys::st::ListNode
{
public:
    // the arguments of Setup (the animation names may be NULL)
    struct SetupArg
    {
        const ssys::ma::lyt::ResAccInterface* markerResAcc;  // 0x00
        ssys::ma::lyt::Layout* layout;                       // 0x04 the layout of the marker
        nw::lyt::Pane* marker;                               // 0x08
        nw::lyt::Group* markerGroup;                         // 0x0C (NULL: the animation is bound to the layout)
        const char* markerAnim;                              // 0x10
        const ssys::ma::lyt::ResAccInterface* resAcc;        // 0x14
        const char* touch;                                   // 0x18 the animations of the icons
        const char* select;                                  // 0x1C
        const char* touchOk;                                 // 0x20
        const char* selectOk;                                // 0x24
        const char* drag;                                    // 0x28
    };

    DragIconGroup(); // 0x0022CAF8
    // 0x0022CB84 (deleting dtor)
    ~DragIconGroup() override; // 0x0022CB94 slot 0x00

    void Setup(const SetupArg& arg); // 0x0022C94C
    // the icon gets the animations of the group and rests on pane
    void AddNode(DragIconNode* node, nw::lyt::Pane* pane); // 0x0022CA48
    // the same; the reaction icon also gets the extra animation
    void AddNode(ReactionDragIconNode* node, nw::lyt::Pane* pane); // 0x0022C7C0
    // the marker on the icon (its animation from the first frame)
    void ShowMarker(DragIconNode* node); // 0x0022C878
    void HideMarker(); // 0x0022C924

    ssys::ma::lyt::Layout* m_pLayout;          // 0x018
    nw::lyt::Pane* m_pMarker;                  // 0x01C
    ssys::ma::lyt::Animation m_MarkerAnim;     // 0x020
    nw::lyt::Group* m_pMarkerGroup;            // 0x048
    ssys::ma::lyt::Animation m_Touch;          // 0x04C
    ssys::ma::lyt::Animation m_Select;         // 0x074
    ssys::ma::lyt::Animation m_TouchOk;        // 0x09C
    ssys::ma::lyt::Animation m_SelectOk;       // 0x0C4
    ssys::ma::lyt::Animation m_Drag;           // 0x0EC
    ssys::ma::lyt::Animation m_Extra;          // 0x114 (ReactionDragIconNode)
    bool m_IsEnabled;                          // 0x13C the icons can be chosen
    bool m_IsDragEnabled;                      // 0x13D the icons can be dragged
    bool m_IsTapEnabled;                       // 0x13E the icons can be chosen by a tap
    bool m_IsDropEnabled;                      // 0x13F the icons accept drops
};
ASSERT_OFFSET(DragIconGroup, m_pLayout, 0x18);
ASSERT_OFFSET(DragIconGroup, m_pMarkerGroup, 0x48);
ASSERT_OFFSET(DragIconGroup, m_Drag, 0xEC);
ASSERT_OFFSET(DragIconGroup, m_IsEnabled, 0x13C);
ASSERT_SIZE(DragIconGroup, 0x140);

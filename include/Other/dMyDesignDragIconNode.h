#pragma once

#include "decomp.h"
#include "Other/dDragIconNode.h"
#include "Other/dLayout.h"

struct SvMyDesign;
namespace nw {
namespace gfx {
namespace res {
struct ResImageTextureData;
} // namespace res
} // namespace gfx
namespace lyt {
class Picture;
} // namespace lyt
} // namespace nw
namespace ssys {
namespace ma {
namespace lyt {
class Animation;
class ArcResAccReader;
} // namespace lyt
} // namespace ma
} // namespace ssys

// An icon of a design (class name from the RTTI, all other names are ours): its own layout m_design_icon.bclyt shows
// the design (drawn into a texture) in the picture P_texture_00; P_pro_00 marks designs that can be worn.
// RTTI 20MyDesignDragIconNode @ 0x008CCBDC
// vtable 0x008F58EC (vptr 0x008F58F4), offset_to_top 0, 29 entries
class MyDesignDragIconNode : public ::DragIconNode
{
public:
    MyDesignDragIconNode(); // 0x0032106C
    // 0x003210B0 (deleting dtor)
    ~MyDesignDragIconNode() override; // 0x003210F8 slot 0x00

    // the sounds (isOn: the ok sound of choosing the icon, else the one of marking it)
    void SetSounds(bool isOn) override; // 0x00321038 slot 0x50

    // the layout and the panes; the button gets index
    void Setup(s32 index, ssys::ma::lyt::ArcResAccReader* reader); // 0x00320F08
    // the wearable mark
    void SetWearableMarkVisible(bool visible); // 0x00320C88
    // the picture shows the texture of the design
    void UpdatePicture(); // 0x00320CA8
    void UpdatePicture(const nw::gfx::res::ResImageTextureData* const* unused); // 0x00320CB0
    // the root pane of the layout
    void SetVisible(bool visible); // 0x00320DC4
    // the icon shows its design (or is hidden without one)
    void Refresh(); // 0x00320DE0
    // the animation m_pAnim is applied at its first (or last) frame; the icon is active only at its last
    void ApplyAnim(bool isFirst); // 0x00320E94

    ::Layout m_IconLayout;                                  // 0x130
    const nw::gfx::res::ResImageTextureData* m_pTexture;    // 0x27C the texture the design is drawn into
    nw::lyt::Pane* m_pWearableMark;                         // 0x280 P_pro_00
    nw::lyt::Picture* m_pPicture;                           // 0x284 P_texture_00
    ssys::ma::lyt::Animation* m_pAnim;                      // 0x288
    SvMyDesign* m_pDesign;                                  // 0x28C
};
ASSERT_OFFSET(MyDesignDragIconNode, m_pTexture, 0x27C);
ASSERT_OFFSET(MyDesignDragIconNode, m_pDesign, 0x28C);
ASSERT_SIZE(MyDesignDragIconNode, 0x290);

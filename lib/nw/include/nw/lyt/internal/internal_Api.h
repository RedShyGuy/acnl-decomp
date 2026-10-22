#pragma once

#include "decomp.h"
#include "nw/lyt/lyt_AnimResource.h"
#include "nw/lyt/lyt_Types.h"

namespace nw {
namespace lyt {
class AnimTransform;
class DrawInfo;
class Layout;
class Pane;
class ResourceAccessor;

namespace res {
struct AnimationBlock;
} // namespace res

namespace internal {
// the names of panes and groups (16 characters) / materials (20 characters) are equal
bool EqualsResName(const char* name1, const char* name2); // 0x004BEE20 | nintendogs:bytes [tier A]
bool EqualsMaterialName(const char* name1, const char* name2); // 0x004BEF48 | nintendogs:bytes [tier A]

// the links of the animation (NULL: all) are taken out of the list and reset
void UnbindAnimationLink(AnimationList* pAnimList, AnimTransform* pAnimTrans); // 0x004BEFD4 | nintendogs:bytes [tier A]
// the link of the animation / of an animation of the resource; NULL if there is none (names
// are ours)
AnimationLink* FindAnimationLink(AnimationList* pAnimList, AnimTransform* pAnimTrans); // 0x004BEEC8 (name is ours)
AnimationLink* FindAnimationLink(AnimationList* pAnimList, const AnimResource& animRes); // 0x004BEF08 (name is ours)

// A quad of the pane shader at basePt (relative to the model view matrix of the graphics
// resource): the vertex attributes and colors, the texture coordinates of the texture coordinate
// generators, the transform, then the draw call. The second one only draws another quad with
// the uniforms as they are (names are ours).
void DrawQuad(const DrawInfo& drawInfo, const nn::math::VEC2& basePt, const Size& size, u8 texCoordNum, const TexCoordQuad* texCoords, const ut::Color8* vtxColors); // 0x004BF394 (name is ours)
void DrawQuad(const DrawInfo& drawInfo, const nn::math::VEC2& basePt, const Size& size); // 0x004BEE3C (name is ours)

// The animation contents of an animation resource for a pane and its materials (by name): the
// animations the panes of a group share with it (Layout::BindAnimationAuto). Class and member
// names are ours.
class AnimPaneTree
{
public:
    // the number of materials of a pane (a window has a content and up to 8 frames)
    static const int MATERIAL_NUM_MAX = 9;

    AnimPaneTree(Pane* pTargetPane, const AnimResource& animRes); // 0x004BEDCC (name is ours)

    // (inline)
    bool IsEnabled() const { return m_LinkNum > 0; }
    // a new animation of the layout with the contents, bound to the pane and its materials
    AnimTransform* Bind(Layout* pLayout, Pane* pTargetPane, ResourceAccessor* pResAccessor) const; // 0x0073E95C (name is ours)

private:
    void Set(Pane* pTargetPane, const AnimResource& animRes); // 0x004BEC0C (name is ours)
    // the index of the content with the name and type; 0xFFFF if there is none (inline)
    static u16 FindAnimContent(const res::AnimationBlock* pAnimBlock, const char* animContName, u8 animContType);

    AnimResource m_AnimRes;                     // 0x00
    u16 m_AnimPaneIdx;                          // 0x10 (0xFFFF: none)
    u16 m_LinkNum;                              // 0x12 the contents found
    u16 m_AnimMatIdxs[MATERIAL_NUM_MAX];        // 0x14 (0xFFFF: none)
    u8 m_AnimMatCnt;                            // 0x26
};
ASSERT_SIZE(AnimPaneTree, 0x28);
} // namespace internal
} // namespace lyt
} // namespace nw

#pragma once

#include "decomp.h"
#include "fgobj/dObjectDeco.h"
#include "g3d/dSkeletalAnim.h"

class SvFgName;

namespace fgobj {
// The decoration of a decorated cedar (SvFgName::NAME_CEDAR_XMAS) that swings with the tree: the textured decoration with
// the skeletal animation "fg_treeB_4" of the animation of the tree; when the tree is cut it fades out (the class name is
// from the RTTI, the rest is ours).
// RTTI N5fgobj15ObjectDecoSwingE @ 0x008D29F4
// vtable 0x00908D24 (vptr 0x00908D2C), offset_to_top 0, 10 entries
class ObjectDecoSwing : public ::fgobj::ObjectDeco
{
public:
    // the decoration at the unit x, z with the animation of ObjectSwing (ObjectSwing::Animation)
    static ObjectDecoSwing* Create(const SvFgName& item, const s32* x, const s32* z, u8 animation); // 0x00595A24

    virtual ~ObjectDecoSwing(); // 0x00595C28 slot 0x00
    // 0x00595BD0 (deleting dtor)
    s32 Execute() override; // 0x00595B48 slot 0x20

    g3d::SkeletalAnim m_SkeletalAnim;   // 0x144
    bool m_IsCut;                       // 0x15C the animations of the cut (fades out)
};
ASSERT_SIZE(ObjectDecoSwing, 0x160);
} // namespace fgobj

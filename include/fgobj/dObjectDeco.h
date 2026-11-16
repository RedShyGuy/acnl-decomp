#pragma once

#include "decomp.h"
#include "fgobj/dObjectExe.h"
#include "g3d/dMaterialAnim.h"

namespace fgobj {
// A decoration of a tree: the model "fg_treeB_d" of FgObjAnim.bcres with its texture animation (the class name is from
// the RTTI, the rest is ours; the Proc builds it, see 0x0059BDAC).
// RTTI N5fgobj10ObjectDecoE @ 0x008D2928
// vtable 0x00908A94 (vptr 0x00908A9C), offset_to_top 0, 10 entries
class ObjectDeco : public ::fgobj::ObjectExe
{
public:
    virtual ~ObjectDeco(); // 0x00592074 slot 0x00
    // 0x00592030 (deleting dtor)
    bool vf_0x10() override; // 0x00591FFC slot 0x10
    s32 Execute() override; // 0x00592004 slot 0x20

    // the decoration at the unit x, z (isTextured: the model "fg_treeB_d_t"); the animation starts at a frame of the
    // unit and the frame counter of the stage
    void Setup(const SvFgName& item, const s32* x, const s32* z, bool isTextured); // 0x00591E18

    g3d::MaterialAnim m_MaterialAnim; // 0x124
};
ASSERT_SIZE(ObjectDeco, 0x144);
} // namespace fgobj

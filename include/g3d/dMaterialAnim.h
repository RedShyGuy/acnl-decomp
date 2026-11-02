#pragma once

#include "decomp.h"
#include "g3d/dBaseAnim.h"

namespace nw {
namespace gfx {
class AnimGroup;
class AnimObject;
} // namespace gfx
} // namespace nw

namespace g3d {
class Model;

// a material animation of a model (names are ours)
// RTTI N3g3d12MaterialAnimE @ 0x008D0CA0
// vtable 0x00903990 (vptr 0x00903998), offset_to_top 0, 8 entries
class MaterialAnim : public ::g3d::BaseAnim
{
public:
    MaterialAnim(); // 0x004ED208
    virtual ~MaterialAnim(); // 0x004ED268 slot 0x00
    // 0x004ED248 (deleting dtor)
    void Finalize() override; // 0x004ED1B4 slot 0x14
    // (the number of member animations is always 64)
    bool CreateAnimObject(nw::anim::res::ResAnim anim, s32 maxMemberAnims) override; // 0x004ED0C8 slot 0x18
    bool Bind() override; // 0x004ECF88 slot 0x1C

    // the evaluator of the animation in a heap of all that the heap of allocator can give, bound to the material
    // group of the model (heap name "MaterialAnim::build"); false if it could not be made
    bool Build(Model* model, nw::anim::res::ResAnim anim, ssys::ma::Allocator* allocator, bool allocateCache); // 0x004ECFD0

    // (inline) the group: of the model, else the one of the shared animation object
    nw::gfx::AnimGroup* GetTargetGroup() const;

    nw::gfx::AnimObject* m_pSharedAnimObject;  // 0x14 its group is used without a model
    Model* m_pModel;                           // 0x18
    bool m_AllocateCache;                      // 0x1C
};
ASSERT_SIZE(MaterialAnim, 0x20);
} // namespace g3d

#pragma once

#include "decomp.h"

namespace nw {
namespace gfx {
class AnimObject;
} // namespace gfx
namespace os {
class IAllocator;
} // namespace os
} // namespace nw

namespace g3d {
class Model;

// an animation object for the material group of a model (Model::SetMaterialAnim(const AnimPack*); names are ours)
// RTTI N3g3d8AnimPackE @ 0x008D0D84
// vtable 0x00903BF4 (vptr 0x00903BFC), offset_to_top 0, 2 entries
class AnimPack
{
public:
    AnimPack(); // 0x004F2C74
    virtual ~AnimPack(); // 0x004F2C98 slot 0x00
    // 0x004F2C88 (deleting dtor)

    // an AnimOverrider for up to maxAnimObjects animation objects (empty if their array could not be allocated);
    // false if it could not be made
    bool Create(s32 maxAnimObjects, nw::os::IAllocator* allocator); // 0x004F2AD8
    // the overrider, bound to the material group of the model; always true
    bool Build(Model* model, s32 maxAnimObjects, nw::os::IAllocator* allocator); // 0x004EF2DC
    // destroys the overrider
    void Finalize(); // 0x004F2C30

    nw::gfx::AnimObject* m_pAnimObject; // 0x4
};
ASSERT_SIZE(AnimPack, 0x8);
} // namespace g3d

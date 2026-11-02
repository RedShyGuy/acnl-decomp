#include "g3d/dResourceSet.h"
#include "nw/anim/res/anim_ResAnim.h"

namespace g3d {
// 0x004EB420 (name is ours)
void g3d::ResourceSet::SetHeaps(ssys::ma::VramHeap* heap0, ssys::ma::VramHeap* heap1, ssys::ma::VramHeap* heap2)
{
    m_pHeaps[0] = heap0;
    m_pHeaps[1] = heap1;
    m_pHeaps[2] = heap2;
}

// 0x004EB764 slot 0x00
// 0x004EB6F4 (deleting dtor)
g3d::ResourceSet::~ResourceSet()
{
    Cleanup();
}

// 0x004EB4FC (name is ours)
u32 g3d::ResourceSet::GetMaxSkeletalAnimMemberCount() const
{
    u32 maxCount = 0;
    const nw::gfx::res::ResGraphicsFileData* file = m_GraphicsFile.ptr();
    for (u32 i = 0; i < static_cast<u32>(file->skeletalAnims.count); i++) {
        const nw::anim::res::ResAnimData* anim = static_cast<const nw::anim::res::ResAnimData*>(file->skeletalAnims.GetData(i));
        if (static_cast<u32>(anim->memberAnimCount) > maxCount) {
            maxCount = anim->memberAnimCount;
        }
    }
    return maxCount;
}

} // namespace g3d

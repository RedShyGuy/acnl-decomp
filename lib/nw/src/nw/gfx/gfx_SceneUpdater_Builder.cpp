#include "nw/gfx/gfx_SceneUpdater_Builder.h"

#include <new>

#include "nw/gfx/gfx_SkeletonUpdater_Builder.h"
#include "nw/gfx/gfx_WorldMatrixUpdater_Builder.h"

// 0x00491864 | nintendogs:bytes [tier A]
nw::gfx::SceneUpdater* nw::gfx::SceneUpdater::Builder::Create(os::IAllocator* pAllocator)
{
    if (m_SkeletonUpdater.Get() == nullptr) {
        m_SkeletonUpdater = GfxPtr<SkeletonUpdater>(SkeletonUpdater::Builder().Create(pAllocator));
    }
    if (m_WorldMatrixUpdater.Get() == nullptr) {
        m_WorldMatrixUpdater = GfxPtr<WorldMatrixUpdater>(WorldMatrixUpdater::Builder().Create(pAllocator));
    }
    if (m_BillboardUpdater.Get() == nullptr) {
        m_BillboardUpdater = GfxPtr<BillboardUpdater>(BillboardUpdater::Create(pAllocator));
    }
    void* pMemory = pAllocator->Alloc(sizeof(SceneUpdater), 4);
    if (pMemory == nullptr) {
        return nullptr;
    }
    return new (pMemory) SceneUpdater(pAllocator, m_SkeletonUpdater, m_WorldMatrixUpdater, m_BillboardUpdater);
}

#include "Resource/dResourceLoadAsyncSkeletal.h"

#include "sead/seadHeap.h"
#include "sead/seadSafeStringBase.h"

// 0x0033FAD8 (name is ours)
ResourceLoadAsyncSkeletal::ResourceLoadAsyncSkeletal()
{
    // (only the bases are constructed)
}

// 0x0033FB7C slot 0x00
// 0x0033FB38 (deleting dtor)
ResourceLoadAsyncSkeletal::~ResourceLoadAsyncSkeletal()
{
    // (only the bases are destroyed)
}

// 0x0033FA1C slot 0x24 (name is ours)
bool ResourceLoadAsyncSkeletal::LoadFile(const char* path)
{
    if (!m_Loader.LoadAsync(path, m_Allocator.m_pHeap, g3d::ResourceLoader::CGFX_ALIGNMENT)) {
        return false;
    }
    const bool isLocked = (m_Allocator.m_pHeap->mFlag & sead::Heap::FLAG_ENABLE_LOCK) != 0;
    m_Allocator.m_pHeap->mFlag |= sead::Heap::FLAG_ENABLE_LOCK;
    m_Loader.InitializeCgfx(&m_Allocator, NULL, true);
    if (isLocked) {
        m_Allocator.m_pHeap->mFlag |= sead::Heap::FLAG_ENABLE_LOCK;
    } else {
        m_Allocator.m_pHeap->mFlag &= ~sead::Heap::FLAG_ENABLE_LOCK;
    }
    return true;
}

#include "ssys/ma/dFileData.h"
#include "ssys/ma/dAllocator.h"
#include "ssys/ma/dFlushCache.h"
#include "sead/seadFileDeviceMgr.h"
#include "sead/seadHeap.h"
#include "sead/seadHeapMgr.h"

namespace ssys {
namespace ma {
// 0x00126250 (name is ours)
bool ssys::ma::FileData::Load(const char* path, sead::Heap* heap, s32 alignment)
{
    if (m_pData != NULL) {
        return true;
    }
    if (m_pHeap != NULL) {
        return false;
    }
    sead::FileDevice::LoadArg arg;
    arg.path = path;
    arg.heap = heap;
    arg.bufferSizeAlignment = alignment;
    m_pData = sead::FileDeviceMgr::s_pInstance->tryLoad(arg);
    if (m_pData == NULL) {
        return false;
    }
    m_pHeap = heap;
    if (arg.readSize == 0) {
        Unload();
        return false;
    }
    m_Size = arg.readSize;
    FlushCache::Add(reinterpret_cast<uptr>(m_pData), m_Size);
    m_IsLoaded = true;
    return true;
}

// 0x005C0260 (name is ours)
bool ssys::ma::FileData::Load(const sead::SafeStringBase<char>& path, sead::Heap* heap, s32 alignment)
{
    return Load(path.getCString(), heap, alignment);
}

// 0x0056AF74 (name is ours)
void ssys::ma::FileData::Clear()
{
    m_pHeap = NULL;
    m_pData = NULL;
    m_Size = 0;
    m_IsLoaded = false;
}

// 0x0056B01C (name is ours)
bool ssys::ma::FileData::Load(const char* path, Allocator* allocator, s32 alignment)
{
    return Load(path, allocator->m_pHeap, alignment);
}

} // namespace ma
} // namespace ssys

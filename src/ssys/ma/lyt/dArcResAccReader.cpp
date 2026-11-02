#include "ssys/ma/lyt/dArcResAccReader.h"
#include "ssys/ma/lyt/lyt_Api.h"
#include "ssys/ma/dAllocator.h"

namespace ssys {
namespace ma {
namespace lyt {
// 0x00976C30
bool ArcResAccReader::s_UseFlushCache = false;
// 0x00976C34
u32 ArcResAccReader::s_DefaultAlignment = 0x80;
// 0x00976C38
u32 ArcResAccReader::s_DefaultChunkSize = 0x80000;

// 0x00133A20 | libgarden [tier A]
void ssys::ma::lyt::ArcResAccReader::Clear()
{
    m_pData = NULL;
    m_ArcResourceAccessor.Detach();
    m_LoadSplit.Reset();
    m_pArc = NULL;
    m_ArcResourceAccessor.m_FontList.Clear();
}

// 0x005687B0 (name after libgarden)
bool ssys::ma::lyt::ArcResAccReader::ReadArc(const char* path, sead::Heap* heap)
{
    return ReadArc(path, heap, s_DefaultAlignment, s_DefaultChunkSize);
}

// 0x005687CC (name after libgarden)
bool ssys::ma::lyt::ArcResAccReader::ReadArc(const char* path, sead::Heap* heap, u32 alignment, u32 chunkSize)
{
    bool done = m_LoadSplit.Read(sead::SafeStringBase<char>(path), heap, alignment, chunkSize);
    if (done && m_pArc == NULL) {
        SetArc(m_LoadSplit.m_pData);
    }
    return done;
}

// 0x00568848 (name is ours)
bool ssys::ma::lyt::ArcResAccReader::ShareArc(const ArcResAccReader* other)
{
    return SetData(other->m_LoadSplit.m_pData, ".");
}

// 0x00568884 (name is ours)
bool ssys::ma::lyt::ArcResAccReader::LoadArc(const char* path)
{
    return LoadArc(path, s_DefaultAlignment);
}

// 0x00568894 (name is ours)
bool ssys::ma::lyt::ArcResAccReader::LoadArc(const char* path, u32 alignment)
{
    m_LoadSplit.Load(sead::SafeStringBase<char>(path), GetDeviceAllocator()->m_pHeap, alignment);
    return SetArc(m_LoadSplit.m_pData);
}

// 0x0056890C | libgarden [tier A]
bool ssys::ma::lyt::ArcResAccReader::ReadArc(const char* path)
{
    return ReadArc(path, s_DefaultAlignment, s_DefaultChunkSize);
}

// 0x0056891C (name after libgarden)
bool ssys::ma::lyt::ArcResAccReader::ReadArc(const char* path, u32 alignment, u32 chunkSize)
{
    bool done = m_LoadSplit.Read(sead::SafeStringBase<char>(path), GetDeviceAllocator()->m_pHeap, alignment, chunkSize);
    if (done && m_pArc == NULL) {
        SetArc(m_LoadSplit.m_pData);
    }
    return done;
}

// 0x005689D8 slot 0x00 | libgarden
// 0x005689A4 (deleting dtor)
ssys::ma::lyt::ArcResAccReader::~ArcResAccReader()
{
    // (only the members are destroyed)
}

} // namespace lyt
} // namespace ma
} // namespace ssys

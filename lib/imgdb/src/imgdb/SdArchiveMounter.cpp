#include "imgdb/SdArchiveMounter.h"
#include "nn/fs/fs_Api.h"

namespace imgdb {

// 0x00983148
template <>
SdArchiveMounter* Singleton<imgdb::SdArchiveMounter>::s_pInstance = NULL;
// 0x0098314C
template <>
Allocator* Singleton<imgdb::SdArchiveMounter>::s_pAllocator = NULL;

// 0x0081613C
// 0x0081612C (deleting dtor)
template imgdb::Singleton<imgdb::SdArchiveMounter>::~Singleton();

// (when the card cannot be mounted it remembers whether a card is in the slot)
// 0x005AAE80 | nintendogs:bytes [tier B]
// 0x0077836C (thunk)
bool imgdb::SdArchiveMounter::Mount()
{
    if (m_IsMounted)
    {
        return false;
    }
    m_IsMounted = nn::fs::MountSdmc(s_MountNames[0]).IsSuccess();
    m_IsInserted = m_IsMounted ? false : nn::fs::IsSdmcInserted();
    return m_IsMounted;
}

// 0x005AAEC8 | nintendogs:bytes [tier B]
// 0x007783B4 (thunk)
void imgdb::SdArchiveMounter::Unmount()
{
    if (m_IsMounted)
    {
        nn::fs::Unmount(s_MountNames[0]);
        m_IsMounted = false;
    }
}

// 0x005AAEF8 | nintendogs:bytes [tier B]
imgdb::SdArchiveMounter::SdArchiveMounter() : m_IsMounted(false), m_IsInserted(false)
{
}

// 0x005AAF28
// 0x005AAF18 (deleting dtor)
// 0x007783F4 (thunk)
// 0x007783E4 (deleting thunk)
imgdb::SdArchiveMounter::~SdArchiveMounter()
{
    // (only the bases are destroyed)
}

// (falls into nn::fs::IsSdmcInserted)
// 0x00346284 | nintendogs:callgraph [tier C]
bool imgdb::SdArchiveMounter::IsInsert()
{
    return nn::fs::IsSdmcInserted();
}

// (falls into nn::fs::IsSdmcWritable)
// 0x003462BC (name is ours)
bool imgdb::SdArchiveMounter::IsWritable()
{
    return nn::fs::IsSdmcWritable();
}

// 0x00755B54
// 0x007785BC (thunk)
bool imgdb::SdArchiveMounter::IsMount() const
{
    return m_IsMounted;
}

// 0x00755AF4
// 0x0077855C (thunk)
s64 imgdb::SdArchiveMounter::GetFreeSize() const
{
    s64 freeSize = 0;
    if (IsMount())
    {
        s64 totalSize = 0;
        if (nn::fs::GetSdmcSize(&totalSize, &freeSize).IsFailure())
        {
            freeSize = 0;
        }
    }
    return freeSize;
}

} // namespace imgdb

#include "imgdb/TwlNandArchiveMounter.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fs/fs_Api.h"

namespace imgdb {

// 0x00983158
template <>
TwlNandArchiveMounter* Singleton<imgdb::TwlNandArchiveMounter>::s_pInstance = NULL;
// 0x0098315C
template <>
Allocator* Singleton<imgdb::TwlNandArchiveMounter>::s_pAllocator = NULL;

// 0x0081617C
// 0x0081616C (deleting dtor)
template imgdb::Singleton<imgdb::TwlNandArchiveMounter>::~Singleton();

// 0x005AB798 | nintendogs:bytes [tier B]
// 0x007784B8 (thunk)
bool imgdb::TwlNandArchiveMounter::Mount()
{
    if (m_IsMounted)
    {
        return false;
    }
    m_IsMounted = nn::fs::MountSpecialArchive(s_MountNames[2], TWL_PHOTO_ARCHIVE_ID).IsSuccess();
    return m_IsMounted;
}

// 0x005AB7D8
// 0x007784F8 (thunk)
void imgdb::TwlNandArchiveMounter::Unmount()
{
    if (m_IsMounted)
    {
        nn::err::CTR::ThrowFatalErrAllIfFailure(nn::fs::Unmount(s_MountNames[2]));
        m_IsMounted = false;
    }
}

// 0x005AB81C | nintendogs:callgraph [tier C]
imgdb::TwlNandArchiveMounter::TwlNandArchiveMounter() : m_IsMounted(false)
{
}

// 0x005AB848
// 0x005AB838 (deleting dtor)
// 0x0077854C (thunk)
// 0x0077853C (deleting thunk)
imgdb::TwlNandArchiveMounter::~TwlNandArchiveMounter()
{
    // (only the bases are destroyed)
}

// 0x00755E78 | nintendogs:bytes [tier B]
// 0x00778670 (thunk)
s64 imgdb::TwlNandArchiveMounter::GetFreeSize() const
{
    s64 freeSize = 0;
    if (IsMount())
    {
        s64 totalSize;
        if (nn::fs::GetTwlPhotoSize(&totalSize, &freeSize).IsFailure())
        {
            freeSize = 0;
        }
    }
    return freeSize;
}

// 0x00755ECC
// 0x007786C4 (thunk)
bool imgdb::TwlNandArchiveMounter::IsMount() const
{
    return m_IsMounted;
}

} // namespace imgdb

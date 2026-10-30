#include "imgdb/CtrNandArchiveMounter.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fs/fs_Api.h"

namespace imgdb {

// 0x00983150
template <>
CtrNandArchiveMounter* Singleton<imgdb::CtrNandArchiveMounter>::s_pInstance = NULL;
// 0x00983154
template <>
Allocator* Singleton<imgdb::CtrNandArchiveMounter>::s_pAllocator = NULL;

// 0x0081615C
// 0x0081614C (deleting dtor)
template imgdb::Singleton<imgdb::CtrNandArchiveMounter>::~Singleton();

// 0x005AB558
// 0x00778404 (thunk)
bool imgdb::CtrNandArchiveMounter::Mount()
{
    if (m_IsMounted)
    {
        return false;
    }
    nn::Result result = nn::fs::MountSharedExtSaveData(s_MountNames[1], SHARED_EXT_SAVE_DATA_ID);
    m_IsMounted = result.IsSuccess();
    if (!m_IsMounted)
    {
        nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    }
    return m_IsMounted;
}

// 0x005AB5A8
// 0x00778454 (thunk)
void imgdb::CtrNandArchiveMounter::Unmount()
{
    if (m_IsMounted)
    {
        nn::err::CTR::ThrowFatalErrAllIfFailure(nn::fs::Unmount(s_MountNames[1]));
        m_IsMounted = false;
    }
}

// 0x005AB5EC | nintendogs:callgraph [tier C]
imgdb::CtrNandArchiveMounter::CtrNandArchiveMounter() : m_IsMounted(false)
{
}

// 0x005AB618
// 0x005AB608 (deleting dtor)
// 0x007784A8 (thunk)
// 0x00778498 (deleting thunk)
imgdb::CtrNandArchiveMounter::~CtrNandArchiveMounter()
{
    // (only the bases are destroyed)
}

// (free blocks times the block size)
// 0x00755D70
// 0x007785C4 (thunk)
s64 imgdb::CtrNandArchiveMounter::GetFreeSize() const
{
    s64 freeSize = 0;
    if (IsMount())
    {
        s64 totalBlocks;
        s64 freeBlocks;
        int blockSize;
        if (GetSharedExtSaveDataBlockSize(totalBlocks, freeBlocks, blockSize))
        {
            freeSize = freeBlocks * blockSize;
        }
    }
    return freeSize;
}

// 0x00755E14 | nintendogs:bytes [tier B]
bool imgdb::CtrNandArchiveMounter::GetSharedExtSaveDataBlockSize(s64& totalBlocks, s64& freeBlocks,
                                                                int& blockSize) const
{
    if (!IsMount())
    {
        return false;
    }
    nn::Result result =
        nn::fs::GetSharedExtSaveDataBlockSize(&totalBlocks, &freeBlocks, &blockSize, SHARED_EXT_SAVE_DATA_ID);
    if (result.IsSuccess())
    {
        return true;
    }
    nn::err::CTR::ThrowFatalErrAllIfFailure(result);
    return false;
}

// 0x00755E70
// 0x00778668 (thunk)
bool imgdb::CtrNandArchiveMounter::IsMount() const
{
    return m_IsMounted;
}

} // namespace imgdb

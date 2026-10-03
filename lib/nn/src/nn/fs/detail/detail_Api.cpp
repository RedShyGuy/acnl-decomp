#include "nn/fs/detail/detail_Api.h"
#include "nn/dbg/dbg_Api.h"

namespace nn {
namespace fs {
namespace detail {
namespace {

// 0x00975F40 (name is ours)
FileSystemBase* s_pGlobalFileSystemBase;

} // namespace

// 0x0013057C | tier X
FileSystemBase* GetGlobalFileSystemBase()
{
    if (s_pGlobalFileSystemBase == 0) {
        nndbgPanic();
    }
    return s_pGlobalFileSystemBase;
}

// 0x00136244 | tier C
void RegisterGlobalFileSystemBase(FileSystemBase& fileSystem)
{
    s_pGlobalFileSystemBase = &fileSystem;
}

} // namespace detail
} // namespace fs
} // namespace nn

#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_ContentRomFsArchive.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// 0x003489DC slot 0x30
void nn::fs::CTR::MPCore::detail::ContentRomFsArchive::DeleteObject()
{
}

// 0x00348AB8 slot 0x34
// 0x00348A30 slot 0x38 (deleting dtor)
nn::fs::CTR::MPCore::detail::ContentRomFsArchive::~ContentRomFsArchive()
{
}

// 0x00348990 slot 0x3C
nn::Result nn::fs::CTR::MPCore::detail::ContentRomFsArchive::OpenDirect(nn::fs::CTR::MPCore::detail::IFile** file, nn::Handle handle)
{
}

// 0x00130280 | nintendogs:bytes-fuzzy [tier A]
void nn::fs::CTR::MPCore::detail::ContentRomFsArchive::AllocateBuffer()
{
}

// 0x00130368 | nintendogs:bytes [tier A]
nn::fs::CTR::MPCore::detail::ContentRomFsArchive::ContentRomFsArchive()
{
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn

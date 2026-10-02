#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_ContentRomFsArchive.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// 0x003489DC slot 0x30 | nintendogs:bytes
void nn::fs::CTR::MPCore::detail::ContentRomFsArchive::DeleteObject()
{
}

// 0x00348AB8 slot 0x34 | nintendogs:bytes-fuzzy
nn::fs::CTR::MPCore::detail::ContentRomFsArchive::~ContentRomFsArchive()
{
}

// 0x00348A30 slot 0x38 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::ContentRomFsArchive::vf_0x38()
{
}

// 0x00348990 slot 0x3C | nintendogs:bytes
void nn::fs::CTR::MPCore::detail::ContentRomFsArchive::OpenDirect(nn::fs::CTR::MPCore::detail::IFile**, nn::Handle)
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

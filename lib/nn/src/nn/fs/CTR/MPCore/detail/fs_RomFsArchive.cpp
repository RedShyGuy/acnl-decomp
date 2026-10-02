#include "nn/fs/CTR/MPCore/detail/fs_IArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// ctor address unknown
nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsArchive()
{
}

// 0x003472E8 slot 0x00 | nintendogs:callseq
void nn::fs::CTR::MPCore::detail::RomFsArchive::OpenFile(nn::fs::CTR::MPCore::detail::IFile**, const nn::fslow::LowPath<const char*, const wchar_t*>&, unsigned)
{
}

// 0x00346DC0 slot 0x04 | nintendogs:callseq
void nn::fs::CTR::MPCore::detail::RomFsArchive::OpenDirectory(nn::fs::CTR::MPCore::detail::IDirectory**, const nn::fslow::LowPath<const char*, const wchar_t*>&)
{
}

// 0x00346CF4 slot 0x08 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::vf_0x08()
{
}

// 0x00346D00 slot 0x0C | slot vf_0x0C of nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::RenameFile(const nn::fslow::LowPath<const char*,const wchar_t*>&, const nn::fslow::LowPath<const char*,const wchar_t*>&)
{
}

// 0x00346EA4 slot 0x10 | slot vf_0x10 of nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::DeleteDirectory(const nn::fslow::LowPath<const char*, const wchar_t*>&)
{
}

// 0x00347058 slot 0x14 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::vf_0x14()
{
}

// 0x00346CE8 slot 0x18 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::vf_0x18()
{
}

// 0x00346E98 slot 0x1C | slot vf_0x1C of nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::CreateDirectory(const nn::fslow::LowPath<const char*, const wchar_t*>&)
{
}

// 0x00346F18 slot 0x20 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::vf_0x20()
{
}

// 0x00346F34 slot 0x24 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::vf_0x24()
{
}

// 0x00346F24 slot 0x28 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::vf_0x28()
{
}

// 0x00348C24 slot 0x2C | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::vf_0x2C()
{
}

// 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
void nn::fs::CTR::MPCore::detail::RomFsArchive::DeleteObject()
{
}

// 0x003479BC slot 0x34 | slot vf_0x34 of nn::fs::CTR::MPCore::detail::RomFsArchive
nn::fs::CTR::MPCore::detail::RomFsArchive::~RomFsArchive()
{
}

// 0x00347934 slot 0x38 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::vf_0x38()
{
}

// 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
void nn::fs::CTR::MPCore::detail::RomFsArchive::OpenDirect(nn::fs::CTR::MPCore::detail::IFile**, nn::Handle)
{
}

// 0x00346EB0 slot 0x40 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::vf_0x40()
{
}

// 0x00346E88 slot 0x44 | slot vf_0x44 of nn::fs::CTR::MPCore::detail::RomFsArchive
void nn::fs::CTR::MPCore::detail::RomFsArchive::OpenLinkHandle(nn::Handle*)
{
}

// 0x0012FE88 | nintendogs:callseq [tier A]
void nn::fs::CTR::MPCore::detail::RomFsArchive::Initialize(nn::fs::CTR::MPCore::detail::IFile*, unsigned, unsigned, void*, unsigned, bool)
{
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn

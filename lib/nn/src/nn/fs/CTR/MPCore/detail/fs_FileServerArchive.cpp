#include "nn/os/os_HandleObject.h"
#include "nn/fs/CTR/MPCore/detail/fs_IArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_FileServerArchive.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// ctor candidate(s) 0x00129584, 0x003467C0, 0x00346A68, 0x00347E94, 0x00348B4C (unverified)
nn::fs::CTR::MPCore::detail::FileServerArchive::FileServerArchive()
{
}

// 0x003486D4 slot 0x00 | nintendogs:callseq
void nn::fs::CTR::MPCore::detail::FileServerArchive::OpenFile(nn::fs::CTR::MPCore::detail::IFile**, const nn::fslow::LowPath<const char*, const wchar_t*>&, unsigned)
{
}

// 0x0034811C slot 0x04 | nintendogs:callseq
void nn::fs::CTR::MPCore::detail::FileServerArchive::OpenDirectory(nn::fs::CTR::MPCore::detail::IDirectory**, const nn::fslow::LowPath<const char*, const wchar_t*>&)
{
}

// 0x00347FD8 slot 0x08 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
void nn::fs::CTR::MPCore::detail::FileServerArchive::vf_0x08()
{
}

// 0x00348034 slot 0x0C | slot vf_0x0C of nn::fs::CTR::MPCore::detail::FileServerArchive
void nn::fs::CTR::MPCore::detail::FileServerArchive::RenameFile(const nn::fslow::LowPath<const char*,const wchar_t*>&, const nn::fslow::LowPath<const char*,const wchar_t*>&)
{
}

// 0x00348224 slot 0x10 | slot vf_0x10 of nn::fs::CTR::MPCore::detail::FileServerArchive
void nn::fs::CTR::MPCore::detail::FileServerArchive::DeleteDirectory(const nn::fslow::LowPath<const char*, const wchar_t*>&)
{
}

// 0x00348354 slot 0x14 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
void nn::fs::CTR::MPCore::detail::FileServerArchive::vf_0x14()
{
}

// 0x00347F68 slot 0x18 | nintendogs:bytes
void nn::fs::CTR::MPCore::detail::FileServerArchive::CreateFile(const nn::fslow::LowPath<const char*, const wchar_t*>&, long long)
{
}

// 0x003481C8 slot 0x1C | nintendogs:bytes
void nn::fs::CTR::MPCore::detail::FileServerArchive::CreateDirectory(const nn::fslow::LowPath<const char*, const wchar_t*>&)
{
}

// 0x00348280 slot 0x20 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
void nn::fs::CTR::MPCore::detail::FileServerArchive::vf_0x20()
{
}

// 0x00348318 slot 0x24 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
void nn::fs::CTR::MPCore::detail::FileServerArchive::vf_0x24()
{
}

// 0x003482E4 slot 0x28 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
void nn::fs::CTR::MPCore::detail::FileServerArchive::vf_0x28()
{
}

// 0x003480E8 slot 0x2C | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
void nn::fs::CTR::MPCore::detail::FileServerArchive::vf_0x2C()
{
}

// 0x00348098 slot 0x30 | nintendogs:bytes
void nn::fs::CTR::MPCore::detail::FileServerArchive::DeleteObject()
{
}

// 0x00348938 slot 0x34 | nintendogs:bytes
nn::fs::CTR::MPCore::detail::FileServerArchive::~FileServerArchive()
{
}

// 0x003488DC slot 0x38 | virtual slot, introduced by nn::fs::CTR::MPCore::detail::FileServerArchive
void nn::fs::CTR::MPCore::detail::FileServerArchive::vf_0x38()
{
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn

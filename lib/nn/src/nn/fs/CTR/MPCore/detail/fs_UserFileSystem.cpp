#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// 0x0012925C | fefates:bytes [tier B]
void nn::fs::CTR::MPCore::detail::UserFileSystem::TryDeleteFile(const wchar_t*)
{
}

// 0x0013616C | nintendogs:bytes [tier A]
void nn::fs::CTR::MPCore::detail::UserFileSystem::Initialize(nn::Handle)
{
}

// 0x00136198 | nintendogs:bytes-fuzzy [tier A]
void nn::fs::CTR::MPCore::detail::UserFileSystem::TryCreateFile(const wchar_t*, long long)
{
}

// 0x0013AE4C | fefates:bytes [tier B]
void nn::fs::CTR::MPCore::detail::UserFileSystem::TryOpenFile(void**, const wchar_t*, unsigned int)
{
}

// 0x00140544 | nintendogs:bytes [tier A]
void nn::fs::CTR::MPCore::detail::UserFileSystem::TryReadFile(int*, void*, long long, void*, unsigned)
{
}

// 0x001405A4 | nintendogs:bytes [tier A]
void nn::fs::CTR::MPCore::detail::UserFileSystem::TryWriteFile(int*, void*, long long, const void*, unsigned, bool)
{
}

// 0x00140608 | nintendogs:callgraph [tier A]
void nn::fs::CTR::MPCore::detail::UserFileSystem::TryGetFileSize(long long*, const void*)
{
}

// 0x00347A40 | nintendogs:callgraph [tier A]
void nn::fs::CTR::MPCore::detail::UserFileSystem::CloseDirectory(void*)
{
}

// 0x00347A78 | nintendogs:callgraph [tier A]
void nn::fs::CTR::MPCore::detail::UserFileSystem::TryOpenDirectory(void**, const wchar_t*)
{
}

// 0x00347B04 | nintendogs:bytes [tier A]
void nn::fs::CTR::MPCore::detail::UserFileSystem::TryReadDirectory(int*, void*, nn::fs::DirectoryEntry*, int)
{
}

// 0x00347C2C | fefates:bytes [tier B]
void nn::fs::CTR::MPCore::detail::UserFileSystem::TrySetPriorityForFile(void*, int)
{
}

// 0x00373B68 | nintendogs:bytes [tier B]
void nn::fs::CTR::MPCore::detail::UserFileSystem::CloseFile(void*)
{
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn

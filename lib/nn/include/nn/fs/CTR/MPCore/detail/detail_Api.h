#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/fs/CTR/fs_ArchivePaths.h"
#include "nn/fs/fs_ExtSaveDataSpecifier.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
class IArchive;

// the archive table of fs_UserFileSystem.cpp: the archive of "name:" (0 if there is none)
template <typename CharT>
IArchive* FindArchive(const CharT* path); // 0x007D35B8 (char), 0x001359D4 (wchar_t) | tier A

// enters archive under the mount name of name ("name" or "name:..."); the binary takes 4
// arguments (symbols.json, from nintendogs: 3)
nn::Result RegisterArchive(const char* name, nn::fs::CTR::MPCore::detail::IArchive* archive, bool unknown, bool isNotOwned); // 0x001292C8 | nintendogs:callseq-callee [tier A]
nn::Result OpenSharedExtSaveData(nn::fs::CTR::MPCore::detail::IArchive** archive, const nn::fs::ExtSaveDataSpecifier& specifier); // 0x00129584 | nintendogs:bytes [tier B]
// waits for the emulated latency of a read or a write (a debug setting of the system)
void LatencyEmulation(bool isRead); // 0x00142FB0 | nintendogs:bytes [tier A]
// opens a content of a title (archive 0x2345678A) as a ContentRomFsArchive
nn::Result OpenDataContent(nn::fs::CTR::MPCore::detail::IArchive** archive, const nn::fs::CTR::DataContentArchivePath& path, u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache); // 0x00347CD8 | fefates:bytes [tier B]
nn::Result OpenExtSaveData(nn::fs::CTR::MPCore::detail::IArchive** archive, const nn::fs::ExtSaveDataSpecifier& specifier, bool isBoss); // 0x00347E94 | nintendogs:bytes [tier A]
nn::Result OpenSpecialArchiveRaw(nn::fs::CTR::MPCore::detail::IArchive** archive, u32 archiveId); // 0x00348B4C | nintendogs:callseq [tier A]
} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn

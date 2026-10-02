#include "nn/fs/CTR/MPCore/detail/detail_Api.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// 0x001292C8 | nintendogs:callseq-callee [tier A]
void RegisterArchive(const char*, nn::fs::CTR::MPCore::detail::IArchive*, bool)
{
}

// 0x00129584 | nintendogs:bytes [tier B]
void OpenSharedExtSaveData(nn::fs::CTR::MPCore::detail::IArchive**, const nn::fs::ExtSaveDataSpecifier&)
{
}

// 0x00142FB0 | nintendogs:bytes [tier A]
void LatencyEmulation(bool)
{
}

// 0x00347CD8 | fefates:bytes [tier B]
void OpenDataContent(nn::fs::CTR::MPCore::detail::IArchive**, const nn::fs::CTR::DataContentArchivePath&, unsigned int, unsigned int, void*, unsigned int, bool)
{
}

// 0x00347E94 | nintendogs:bytes [tier A]
void OpenExtSaveData(nn::fs::CTR::MPCore::detail::IArchive**, const nn::fs::ExtSaveDataSpecifier&, bool)
{
}

// 0x00348B4C | nintendogs:callseq [tier A]
void OpenSpecialArchiveRaw(nn::fs::CTR::MPCore::detail::IArchive**, unsigned)
{
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn

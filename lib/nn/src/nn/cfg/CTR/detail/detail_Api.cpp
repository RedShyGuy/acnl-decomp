#include "nn/cfg/CTR/detail/detail_Api.h"

namespace nn {
namespace cfg {
namespace CTR {
namespace detail {
// 0x0011FCF0 | nintendogs:callgraph [tier A]
void Initialize()
{
}

// 0x0011FDCC | nintendogs:callgraph [tier A]
void FinalizeProperPort(nn::cfg::CTR::detail::_IPCPortType)
{
}

// 0x0011FE84 | nintendogs:bytes [tier A]
void InitializeProperPort(nn::cfg::CTR::detail::_IPCPortType*)
{
}

// 0x0011FEF0 | nintendogs:bytes [tier A]
void GetRegion()
{
}

// 0x001242E0 | fefates:bytes [tier B]
void FinalizeSys()
{
}

// 0x0012435C | fefates:bytes [tier B]
void FinalizeInit()
{
}

// 0x001243D0 | nintendogs:callgraph [tier A]
void InitializeSys()
{
}

// 0x00124478 | nintendogs:callgraph [tier A]
void InitializeInit()
{
}

// 0x00124524 | nintendogs:callgraph [tier A]
nn::Result GetConfig(void* buffer, size_t size, u32 blockId)
{
}

// 0x00129BA4 | nintendogs:bytes [tier A]
void FinalizeBase(nn::Handle*)
{
}

// 0x00129BE8 | nintendogs:bytes [tier A]
void InitializeBase(nn::Handle*, const char*)
{
}

// 0x00129C44 | fefates:bytes [tier B]
void GetCriticalSectionForInitializeFinalize()
{
}

// 0x001367E4 | fefates:bytes [tier B]
void Finalize()
{
}

// 0x00350F48 | nintendogs:callgraph [tier A]
void GetTransferableId(unsigned, unsigned long long*)
{
}

} // namespace detail
} // namespace CTR
} // namespace cfg
} // namespace nn

#include "nn/boss/detail/boss_Privileged.h"

namespace nn {
namespace boss {
namespace detail {
// 0x0046D0D4 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::ReadNsData(unsigned, long long, unsigned char*, unsigned, unsigned*, unsigned*)
{
}

// 0x0046D168 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::GetErrorCode(unsigned*, nn::boss::TaskResultCode)
{
}

// 0x0046D214 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::RegisterTask(const unsigned char*, unsigned, unsigned char, unsigned char)
{
}

// 0x0046D268 | fefates:callgraph [tier A]
void nn::boss::detail::Privileged::SendProperty(nn::boss::PropertyType, const unsigned char*, unsigned int)
{
}

// 0x0046D2E8 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::GetTaskResult(const unsigned char*, unsigned, nn::boss::TaskResultCode*, unsigned*, unsigned char*)
{
}

// 0x0046D3A8 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::SetOptoutFlag(bool)
{
}

// 0x0046D3E0 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::GetStorageInfo(unsigned*)
{
}

// 0x0046D414 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::UnregisterTask(const unsigned char*, unsigned, unsigned char)
{
}

// 0x0046D460 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::GetNsDataIdList(unsigned, unsigned*, unsigned, unsigned short*, unsigned short, unsigned, unsigned short*)
{
}

// 0x0046D524 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::RegisterStorage(unsigned long long, unsigned, nn::fs::MediaType)
{
}

// 0x0046D564 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::UpdateTaskCount(const unsigned char*, unsigned, unsigned)
{
}

// 0x0046D5A4 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::GetNsDataNewFlag(unsigned, bool*)
{
}

// 0x0046D5E0 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::SetNsDataNewFlag(unsigned, bool)
{
}

// 0x0046D644 | fefates:callgraph [tier A]
void nn::boss::detail::Privileged::SendPropertyHandle(nn::boss::PropertyType, nn::Handle)
{
}

// 0x0046D688 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::StartTaskImmediate(const unsigned char*, unsigned)
{
}

// 0x0046D6C8 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::GetNsDataHeaderInfo(unsigned, nn::boss::HeaderInfoType, unsigned char*, unsigned)
{
}

// 0x0046D750 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::GetNsDataLastUpdated(unsigned, long long*)
{
}

// 0x0046D790 | fefates:callgraph [tier A]
void nn::boss::detail::Privileged::GetTaskServiceStatus(const unsigned char*, unsigned int, nn::boss::TaskServiceStatus*)
{
}

// 0x0046D7DC | nintendogs:bytes [tier A]
void nn::boss::detail::Privileged::ReadNsDataPrivileged(unsigned long long, unsigned, long long, unsigned char*, unsigned, unsigned*, unsigned*)
{
}

// 0x0046D850 | fefates:callgraph [tier A]
void nn::boss::detail::Privileged::RegisterImmediateTask(const unsigned char*, unsigned int, unsigned char, unsigned char)
{
}

// 0x0046D8A4 | fefates:bytes [tier A]
void nn::boss::detail::Privileged::DeleteNsDataPrivileged(unsigned long long, unsigned int)
{
}

// 0x0046D8DC | nintendogs:bytes [tier A]
void nn::boss::detail::Privileged::GetNsDataNewFlagPrivileged(unsigned long long, unsigned, bool*)
{
}

// 0x0046D920 | nintendogs:bytes [tier A]
void nn::boss::detail::Privileged::SetNsDataNewFlagPrivileged(unsigned long long, unsigned, bool)
{
}

// 0x0046D960 | nintendogs:bytes [tier A]
void nn::boss::detail::Privileged::GetNsDataHeaderInfoPrivileged(unsigned long long, unsigned, nn::boss::HeaderInfoType, unsigned char*, unsigned)
{
}

// 0x0046D9BC | nintendogs:bytes [tier A]
void nn::boss::detail::Privileged::GetNsDataLastUpdatedPrivileged(unsigned long long, unsigned, long long*)
{
}

// 0x0046DA04 | nintendogs:callgraph [tier A]
void nn::boss::detail::Privileged::StartTask(const unsigned char*, unsigned)
{
}

} // namespace detail
} // namespace boss
} // namespace nn

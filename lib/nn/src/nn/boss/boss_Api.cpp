#include "nn/boss/boss_Api.h"

namespace nn {
namespace boss {
// 0x0046A764 | nintendogs:bytes [tier A]
void Initialize()
{
}

// 0x0046AD38 | nintendogs:bytes [tier A]
void GetErrorCode(unsigned*, nn::boss::TaskResultCode)
{
}

// 0x0046AE1C | nintendogs:bytes [tier A]
void RegisterTask(nn::boss::Task*, nn::boss::TaskPolicy*, nn::boss::TaskAction*, nn::boss::TaskOption*, unsigned char)
{
}

// 0x0046B070 | nintendogs:bytes [tier B]
void SetOptoutFlag(bool)
{
}

// 0x0046B0C8 | nintendogs:bytes [tier A]
void GetStorageInfo(unsigned*)
{
}

// 0x0046B764 | nintendogs:bytes [tier A]
void UnregisterTask(nn::boss::Task*, unsigned char)
{
}

// 0x0046B808 | nintendogs:bytes [tier A]
void GetNsDataIdList(unsigned, nn::boss::NsDataIdList*)
{
}

// 0x0046B910 | nintendogs:bytes [tier B]
void RegisterStorage(unsigned, unsigned, nn::boss::StorageType)
{
}

// 0x0046BADC | fefates:bytes [tier B]
void UnregisterStorage()
{
}

// 0x0046BB24 | fefates:bytes [tier B]
void WaitFinishWaitEvent(const nn::fnd::TimeSpan&)
{
}

// 0x0046BCB4 | fefates:bytes [tier B]
void RegisterImmediateTask(nn::boss::Task*, nn::boss::TaskAction*, nn::boss::TaskPolicy*, nn::boss::TaskOption*, unsigned char)
{
}

// 0x0046F178 | nintendogs:bytes [tier A]
void Finalize()
{
}

} // namespace boss
} // namespace nn

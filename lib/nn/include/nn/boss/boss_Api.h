#pragma once

#include "decomp.h"

namespace nn {
namespace boss {
void Initialize(); // 0x0046A764 | nintendogs:bytes [tier A]
void GetErrorCode(unsigned*, nn::boss::TaskResultCode); // 0x0046AD38 | nintendogs:bytes [tier A]
void RegisterTask(nn::boss::Task*, nn::boss::TaskPolicy*, nn::boss::TaskAction*, nn::boss::TaskOption*, unsigned char); // 0x0046AE1C | nintendogs:bytes [tier A]
void SetOptoutFlag(bool); // 0x0046B070 | nintendogs:bytes [tier B]
void GetStorageInfo(unsigned*); // 0x0046B0C8 | nintendogs:bytes [tier A]
void UnregisterTask(nn::boss::Task*, unsigned char); // 0x0046B764 | nintendogs:bytes [tier A]
void GetNsDataIdList(unsigned, nn::boss::NsDataIdList*); // 0x0046B808 | nintendogs:bytes [tier A]
void RegisterStorage(unsigned, unsigned, nn::boss::StorageType); // 0x0046B910 | nintendogs:bytes [tier B]
void UnregisterStorage(); // 0x0046BADC | fefates:bytes [tier B]
void WaitFinishWaitEvent(const nn::fnd::TimeSpan&); // 0x0046BB24 | fefates:bytes [tier B]
void RegisterImmediateTask(nn::boss::Task*, nn::boss::TaskAction*, nn::boss::TaskPolicy*, nn::boss::TaskOption*, unsigned char); // 0x0046BCB4 | fefates:bytes [tier B]
void Finalize(); // 0x0046F178 | nintendogs:bytes [tier A]
} // namespace boss
} // namespace nn

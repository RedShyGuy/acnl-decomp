#pragma once

#include "decomp.h"

namespace nn {
namespace mic {
namespace CTR {
void Initialize(); // 0x0012A348 | nintendogs:callseq [tier A]
void GetSamplingBufferSize(unsigned*); // 0x0012A3F4 | nintendogs:bytes [tier B]
void SetBuffer(void*, unsigned); // 0x0012A41C | nintendogs:bytes [tier A]
void ResetBuffer(); // 0x00140A88 | nintendogs:bytes [tier A]
void Finalize(); // 0x00140BD0 | nintendogs:bytes [tier A]
void StartSampling(nn::mic::CTR::SamplingType, nn::mic::CTR::SamplingRate, int, unsigned, bool); // 0x0035486C | nintendogs:callseq [tier A]
void GetLastSamplingAddress(unsigned*); // 0x00354974 | nintendogs:bytes [tier B]
} // namespace CTR
} // namespace mic
} // namespace nn

#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/mic/CTR/mic_Types.h"

namespace nn {
namespace mic {
namespace CTR {
nn::Result Initialize(); // 0x0012A348 | nintendogs:callseq [tier A]
nn::Result Finalize(); // 0x00140BD0 | nintendogs:bytes [tier A]
// the usable size of the buffer of SetBuffer (its last word is the position of the newest sample)
nn::Result GetSamplingBufferSize(size_t* pSize); // 0x0012A3F4 | nintendogs:bytes [tier B]
// the buffer for the samples (page aligned), shared with the mic module
nn::Result SetBuffer(void* pBuffer, size_t size); // 0x0012A41C | nintendogs:bytes [tier A]
nn::Result ResetBuffer(); // 0x00140A88 | nintendogs:bytes [tier A]
nn::Result StartSampling(SamplingType type, SamplingRate rate, s32 offset, size_t size, bool loop); // 0x0035486C | nintendogs:callseq [tier A]
nn::Result AdjustSampling(SamplingRate rate); // 0x00354924 (name after the command)
// the address of the last sample
nn::Result GetLastSamplingAddress(uptr* pAddress); // 0x00354974 | nintendogs:bytes [tier B]
} // namespace CTR
} // namespace mic
} // namespace nn

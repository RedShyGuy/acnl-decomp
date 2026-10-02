#pragma once

#include "decomp.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_StepSequenceJob.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex21JobDataStoreGetRatingE @ 0x008CEAEC
// vtable 0x008FDDF0 (vptr 0x008FDDF8), offset_to_top 0, 15 entries
class JobDataStoreGetRating : public ::nn::nex::StepSequenceJob, public ::nn::nex::NonCopyable
{
public:
    virtual ~JobDataStoreGetRating(); // 0x00399D1C slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x00399C88 slot 0x04 | fefates:bytes (deleting dtor)
    virtual void vf_0x34(); // 0x00398C94 slot 0x34 | fefates:callseq
    virtual void vf_0x38(); // 0x00398BAC slot 0x38 | virtual slot, introduced by nn::nex::JobDataStoreGetRating
    void CompleteJob(const nn::nex::qResult&); // 0x00398DC8 | fefates:bytes [tier B]
    void StepConvertType(); // 0x00399194 | fefates:bytes [tier B]
    JobDataStoreGetRating(); // 0x00399C0C | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn

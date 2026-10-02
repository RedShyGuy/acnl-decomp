#pragma once

#include "decomp.h"
#include "nn/nex/nex_Job.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12JobDeriveKeyE @ 0x008CE0D4
// vtable 0x008FC4F8 (vptr 0x008FC500), offset_to_top 0, 12 entries
class JobDeriveKey : public ::nn::nex::Job
{
public:
    JobDeriveKey(); // ctor candidate(s) 0x00386044 (unverified)
    virtual ~JobDeriveKey(); // 0x0035D13C slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x0035D0C8 slot 0x04 | mk7dlp:callseq (deleting dtor)
    virtual void Execute(); // 0x0035CE64 slot 0x0C | fefates:callseq
};
} // namespace nex
} // namespace nn

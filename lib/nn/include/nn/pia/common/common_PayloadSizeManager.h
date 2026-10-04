#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common18PayloadSizeManagerE @ 0x008CFEA0
// vtable 0x009015B8 (vptr 0x009015C0), offset_to_top 0, 1 entries
//
// The MTU and the signature size; they set the sizes of PacketOld and ChunkOld. One instance
// (CreateInstance). The member names are ours.
class PayloadSizeManager : public ::nn::pia::common::RootObject
{
public:
    static const unsigned int MTU_SIZE_MIN = 548;
    static const unsigned int MTU_SIZE_MAX = 1462;

    // (inline in CreateInstance)
    PayloadSizeManager() : m_MtuSize(MTU_SIZE_MAX), m_SignatureSize(0) {}
    virtual void Trace(u64 flag) const; // 0x00731B00 slot 0x00 (name after StepSequenceJob::Trace)

    static PayloadSizeManager* GetInstance() { return s_pInstance; }
    static nn::Result CreateInstance(); // 0x00427FD4 | fefates:callgraph [tier C]
    static void DestroyInstance(); // 0x00428058 | fefates:callgraph [tier C]

    nn::Result SetSignatureSize(unsigned int signatureSize); // 0x00428080 | fefates:callgraph [tier C]
    nn::Result SetMtuSize(unsigned int mtuSize); // 0x0042808C | fefates:callgraph [tier C]

private:
    // both sizes (the two setters branch to it; name is ours)
    DECOMP_NOINLINE nn::Result SetSize(unsigned int mtuSize, unsigned int signatureSize); // 0x00428094

public:
    unsigned int m_MtuSize;       // 0x4
    unsigned int m_SignatureSize; // 0x8

    static PayloadSizeManager* s_pInstance; // 0x0097E3D0
};
ASSERT_SIZE(PayloadSizeManager, 0xC);
} // namespace common
} // namespace pia
} // namespace nn

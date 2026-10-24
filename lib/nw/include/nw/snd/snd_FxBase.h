#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"
#include "nw/snd/snd_Global.h"
#include "nw/ut/internal/ut_LinkListImpl.h"

namespace nw {
namespace snd {
// The base of the effects that run in the aux callback of a bus (the class and function names are
// from the reference symbols of FxDelay/FxReverb, the members are ours).
// RTTI N2nw3snd6FxBaseE @ 0x008D095C
class FxBase
{
public:
    // (inline)
    FxBase() {}
    virtual ~FxBase() {} // slot 0x00

    virtual bool Initialize() = 0; // slot 0x08
    virtual void Finalize() = 0; // slot 0x0C
    // processes the samples of the bus
    virtual void UpdateBuffer(int channelCount, nn::snd::CTR::AuxBusData* pData, int sampleLength, SampleFormat format, f32 sampleRate,
                              OutputMode outputMode) = 0; // slot 0x10
    virtual void OnChangeOutputMode(); // 0x004C3A34 slot 0x14

    ut::LinkListNode m_Link; // 0x4 (HardwareManager)
};
ASSERT_SIZE(FxBase, 0xC);
} // namespace snd
} // namespace nw

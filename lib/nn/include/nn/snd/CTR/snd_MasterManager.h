#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/snd/CTR/snd_Types.h"

namespace nn {
namespace snd {
namespace CTR {
class FxDelay;
class FxReverb;

// The master output and the aux buses as the application sets them (passed on to
// MasterManagerImpl); runs the effects of the aux buses. Member names are ours.
class MasterManager
{
public:
    MasterManager() : m_IsInitialized(false) {}
    ~MasterManager(); // 0x00461F1C (name is ours)

    void Initialize(); // 0x00461A48 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    void ClearEffect(AuxBusId bus); // 0x00461B28 | nintendogs:bytes [confirmed by fefates] [tier A]
    u32 GetDspCycles(); // 0x00461B88 | nintendogs:bytes [confirmed by fefates] [tier A]
    void ExecuteEffect(AuxBusId bus, uptr buffer); // 0x00461C08 | nintendogs:bytes [confirmed by fefates] [tier A]
    void GetAuxCallback(AuxBusId bus, AuxCallback* callback, uptr* arg); // 0x00461C7C | nintendogs:bytes [confirmed by fefates] [tier A]
    void AuxUserCallback(AuxBusId bus, uptr buffer); // 0x00461C94 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    void SetMasterVolume(float volume); // 0x00461CB0 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    void ClearAuxCallback(AuxBusId bus); // 0x00461CD0 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetAuxFrontBypass(AuxBusId bus, bool isBypass); // 0x00461CF0 (name is ours)
    OutputMode GetSoundOutputMode(); // 0x00461D04 (name is ours)
    void SetAuxReturnVolume(AuxBusId bus, float volume); // 0x00461D0C (name is ours)
    bool SetSoundOutputMode(OutputMode mode); // 0x00461D30 (name is ours)
    void RegisterAuxCallback(AuxBusId bus, AuxCallback callback, uptr arg); // 0x00461D40 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetOutputBufferCount(int count); // 0x00461D58 (name is ours)
    bool SetIsHeadsetConnected(bool isConnected); // 0x00461D64 | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
    void UpdateDroppedSoundFrameCount(); // 0x00461D74 | nintendogs:bytes [confirmed by fefates] [tier A]
    void Finalize(); // 0x00461DA0 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    bool SetEffect(AuxBusId bus, FxDelay* delay); // 0x00461DD4 | nintendogs:bytes [confirmed by fefates] [tier A]
    bool SetEffect(AuxBusId bus, FxReverb* reverb); // 0x00461E78 | nintendogs:bytes [confirmed by fefates] [tier A]

    struct Effect
    {
        FxDelay* m_Delay;
        FxReverb* m_Reverb;
    };

    bool m_IsInitialized;                       // 0x00
    u8 m_Padding1;                              // 0x01
    u8 m_ClippingMode;                          // 0x02
    u8 m_OutputMode;                            // 0x03
    f32 m_MasterVolume;                         // 0x04
    f32 m_Volume8;                              // 0x08
    f32 m_AuxReturnVolume[AUX_BUS_COUNT];       // 0x0C
    AuxCallback m_AuxCallback[AUX_BUS_COUNT];   // 0x14
    uptr m_AuxCallbackArg[AUX_BUS_COUNT];       // 0x1C
    bool m_AuxFrontBypass[AUX_BUS_COUNT];       // 0x24
    bool m_IsHeadsetConnected;                  // 0x26
    u8 m_Padding27;                             // 0x27
    f32 m_Volume28;                             // 0x28
    f32 m_Volume2C;                             // 0x2C
    s32 m_DroppedFrameCount;                    // 0x30
    Effect m_Effect[AUX_BUS_COUNT];             // 0x34
    nn::os::CriticalSection m_Lock;             // 0x44
};
ASSERT_SIZE(MasterManager, 0x50);

// (defined in the .cpp, with its address)
extern MasterManager g_MasterManager;
} // namespace CTR
} // namespace snd
} // namespace nn

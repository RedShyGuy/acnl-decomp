#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/snd/CTR/snd_Types.h"

namespace nn {
namespace snd {
namespace CTR {
// The settings of the master output and the aux buses on the DSP side (sent to Dspsnd, and sent
// again after a sleep). Member names are ours.
class MasterManagerImpl
{
public:
    MasterManagerImpl() : m_IsInitialized(false) {}
    ~MasterManagerImpl(); // 0x00463388 (name is ours)

    void Initialize(); // 0x00462E3C | nintendogs:bytes [confirmed by fefates] [tier A]
    void AuxUserCallback(AuxBusId bus, uptr buffer); // 0x00462E64 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    void InitializeParam(); // 0x00462EE4 | nintendogs:callgraph [confirmed by fefates] [tier A]
    void SetMasterVolume(float volume); // 0x004630CC | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    void ForceUpdateParams(); // 0x004630F4 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetAuxFrontBypass(AuxBusId bus, bool isBypass); // 0x004631E0 (name is ours)
    void SetAuxReturnVolume(AuxBusId bus, float volume); // 0x004631F4 (name is ours)
    bool SetSoundOutputMode(OutputMode mode); // 0x00463218 (name is ours)
    void RegisterAuxCallback(AuxBusId bus, AuxCallback callback, uptr arg); // 0x00463268 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetOutputBufferCount(int count); // 0x004632D4 | nintendogs:bytes [confirmed by fefates] [tier A]
    bool SetIsHeadsetConnected(bool isConnected); // 0x004632F8 (name is ours)
    void EnableFx(AuxBusId bus, bool isEnabled); // 0x00463304 | nintendogs:bytes [confirmed by fefates] [tier A]
    void Finalize(); // 0x00463368 | nintendogs:bytes [confirmed by fefates] [tier A]

    bool m_IsInitialized;                       // 0x00
    u8 m_ClippingMode;                          // 0x01
    u8 m_OutputMode;                            // 0x02
    u8 m_SyncMode;                              // 0x03
    f32 m_MasterVolume;                         // 0x04
    f32 m_Volume8;                              // 0x08, a factor of the master volume
    f32 m_AuxReturnVolume[AUX_BUS_COUNT];       // 0x0C
    AuxCallback m_AuxCallback[AUX_BUS_COUNT];   // 0x14
    uptr m_AuxCallbackArg[AUX_BUS_COUNT];       // 0x1C
    bool m_AuxFrontBypass[AUX_BUS_COUNT];       // 0x24
    u8 m_Padding26;                             // 0x26
    u8 m_SurroundSpeakerPosition;               // 0x27
    u16 m_SurroundDepth;                        // 0x28
    u16 m_RearRatio;                            // 0x2A
    u32 m_Unknown2C;                            // 0x2C
    bool m_IsFxEnabled[AUX_BUS_COUNT];          // 0x30
    u16 m_OutputBufferCount;                    // 0x32
    nn::os::CriticalSection m_Lock;             // 0x34
};
ASSERT_SIZE(MasterManagerImpl, 0x40);

// (defined in the .cpp, with its address)
extern MasterManagerImpl g_MasterManagerImpl;
} // namespace CTR
} // namespace snd
} // namespace nn

#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/snd/CTR/snd_Voice.h"
#include "nn/snd/CTR/snd_VoiceImpl.h"

namespace nn {
namespace snd {
namespace CTR {
// The voices: allocated by priority (a voice of lower priority is dropped when all are used)
// and kept in a list sorted by priority. Member names are ours.
class VoiceManager
{
public:
    static const s32 VOICE_COUNT = 24;

    VoiceManager();
    ~VoiceManager(); // 0x00461A44 (name is ours)

    void Initialize(); // 0x00130820 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    Voice* AllocVoice(int priority, VoiceDropCallback callback, uptr arg); // 0x00461500 (name is ours)
    void SetPriority(Voice* voice, int priority); // 0x00461690 | nintendogs:bytes [confirmed by fefates] [tier A]
    void UpdateDspParams(); // 0x0046176C (name is ours)
    void ForceUpdateDspParams(); // 0x004617A0 (name is ours)
    void AdjustVoicePlayState(int cycles, int usedCycles); // 0x004617D4 | nintendogs:bytes [confirmed by fefates] [tier A]
    void UpdateWaveBufferLists(); // 0x0046194C (name is ours)
    void Finalize(); // 0x00461980 | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
    void FreeVoice(Voice* voice); // 0x0046198C | nintendogs:bytes [confirmed by fefates] [tier A]
    void UpdateStatus(int channel, const void* status); // 0x004668CC (name is ours)

private:
    void Remove(Voice* voice);
    void Insert(Voice* voice, int priority);

    u32 m_UsedMask;                         // 0x0000
    Voice* m_Head;                          // 0x0004, the highest priority
    Voice* m_Tail;                          // 0x0008
    u16 m_UsedCount;                        // 0x000C
    u8 m_Mode;                              // 0x000E
    nn::os::CriticalSection m_Lock;         // 0x0010
    // the storage of the voices (constructed in the constructor)
    u32 m_VoiceBuffer[VOICE_COUNT][sizeof(Voice) / 4];          // 0x001C
    u32 m_VoiceImplBuffer[VOICE_COUNT][sizeof(VoiceImpl) / 4];  // 0x0A3C
    Voice* m_VoicePointers[VOICE_COUNT];    // 0x18DC
};
ASSERT_SIZE(VoiceManager, 0x193C);

// (defined in the .cpp, with its address)
extern VoiceManager g_VoiceManager;
} // namespace CTR
} // namespace snd
} // namespace nn

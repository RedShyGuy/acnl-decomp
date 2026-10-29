#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_DspFxDelay.h"
#include "nn/snd/CTR/snd_DspFxReverb.h"
#include "nw/snd/internal/snd_ThreadStack.h"
#include "nw/snd/snd_Global.h"

namespace nn {
namespace snd {
namespace CTR {
class FxDelay;
class FxReverb;
} // namespace CTR
} // namespace snd
} // namespace nn

namespace nw {
namespace snd {
class FxBase;

// Sets up the sound library: the command buffers, the sound and task threads, the voices and channels
// in one buffer, and the effects of the aux buses (the class and function names are from the reference
// symbols, the members and the names of the unnamed functions are ours).
class SoundSystem
{
public:
    // (the struct name is from the reference symbols, the members are ours)
    struct SoundSystemParam
    {
        // the defaults (inline in sead::AudioSystemCtr)
        SoundSystemParam()
            : soundThreadPriority(4), soundThreadCoreNo(0), soundThreadStackSize(16384),
              soundThreadCommandBufferSize(128 * 1024), taskThreadPriority(3), taskThreadStackSize(16384),
              taskThreadCommandBufferSize(8 * 1024), enableSoundThread(true), enableGetSoundThreadTick(false),
              enableUserThread(true), soundThreadProfileCount(-1)
        {
        }

        int soundThreadPriority;            // 0x00
        int soundThreadCoreNo;              // 0x04
        u32 soundThreadStackSize;           // 0x08
        u32 soundThreadCommandBufferSize;   // 0x0C
        int taskThreadPriority;             // 0x10
        u32 taskThreadStackSize;            // 0x14
        u32 taskThreadCommandBufferSize;    // 0x18
        bool enableSoundThread;             // 0x1C
        bool enableGetSoundThreadTick;      // 0x1D
        bool enableUserThread;              // 0x1E on core 1: a second stack
        u8 padding;                         // 0x1F
        int soundThreadProfileCount;        // 0x20 (SoundThreadProfile, 8 bytes each)
    };
    ASSERT_SIZE(SoundSystemParam, 0x24);

    static u32 GetRequiredMemSize(const SoundSystemParam& param); // 0x004C0034 | nintendogs:callseq [tier A]
    static void Initialize(const SoundSystemParam& param, uptr workMem, u32 workMemSize); // 0x004BFBEC | mk7dlp:callseq [tier A]
    static void Finalize(); // 0x004C0128 | nintendogs:callseq [tier A]
    static bool IsInitialized(); // 0x004C0024 | fefates:callgraph [tier C]
    // only the command buffers
    static u32 GetRequiredDriverCommandManagerMemSize(const SoundSystemParam& param); // 0x004C0118 (name is ours)
    static void InitializeDriverCommandManager(const SoundSystemParam& param, uptr workMem, u32 workMemSize); // 0x004C00C4 (name is ours)

    // the effects run in the sound thread; a DSP effect only if the bus has none of its kind
    static bool AppendEffect(AuxBus bus, FxBase* pEffect); // 0x004BFFC4 (name after the other AppendEffect)
    static bool AppendEffect(AuxBus bus, nn::snd::CTR::FxDelay* pEffect); // 0x004BFF34 | fefates:bytes [tier B]
    static bool AppendEffect(AuxBus bus, nn::snd::CTR::FxReverb* pEffect); // 0x004BFF7C | fefates:bytes [tier B]
    static bool AppendEffect(AuxBus bus, nn::snd::CTR::DspFxDelay* pEffect,
                             const nn::snd::CTR::DspFxDelay::Param& param); // 0x004BFE20 (name after the other AppendEffect)
    static bool AppendEffect(AuxBus bus, nn::snd::CTR::DspFxReverb* pEffect,
                             const nn::snd::CTR::DspFxReverb::Param& param); // 0x004BFE94 (name after the other AppendEffect)
    // waits until the effects of the bus fade out
    static void ClearEffect(AuxBus bus, int fadeTimes); // 0x001417FC | nintendogs:callgraph [tier A]

    // (the game reads it; names are ours) whether the sound thread runs (or the game calls the frame process)
    static bool s_IsEnableSoundThread;
    static bool s_IsInitialized;
    static bool s_IsInitializedDriverCommandManager;
    // the voices and channels of the library
    static int s_VoiceCount;
    // the stack of the task thread
    static internal::ThreadStack s_TaskThreadStack;
};
} // namespace snd
} // namespace nw

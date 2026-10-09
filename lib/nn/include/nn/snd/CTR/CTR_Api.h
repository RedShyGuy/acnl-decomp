#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/os/os_Tick.h"
#include "nn/snd/CTR/snd_Types.h"

namespace nn {
namespace snd {
namespace CTR {
class FxDelay;
class FxReverb;

nn::Result FlushDataCache(uptr address, size_t size); // 0x001409E8 | nintendogs:callgraph [confirmed by fefates] [tier A]
Voice* AllocVoice(int priority, VoiceDropCallback callback, uptr arg); // 0x00460794 | nintendogs:callgraph [confirmed by fefates] [tier A]
nn::Result Initialize(); // 0x00460C44 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void ClearEffect(AuxBusId bus); // 0x00460D00 | fefates:callgraph [tier C]
// the DSP cycles of the last frame (from the status of the DSP)
u32 GetDspCycles(); // 0x004614F0 | fefates:callgraph [tier C]
nn::os::Tick GetSoundThreadTick(); // 0x004621BC | nintendogs:callgraph [confirmed by fefates] [tier A]
void FinalizeSoundThread(); // 0x004621E4 | nintendogs:callgraph [confirmed by fefates] [tier A]
void EnableSoundThreadTickCounter(bool isEnabled); // 0x0046256C | fefates:callgraph [tier C]
void GetAuxCallback(AuxBusId bus, AuxCallback* callback, uptr* arg); // 0x004625E8 (name is ours)
void WaitForDspSync(nn::os::Tick* syncTick); // 0x00462600 | fefates:bytes [tier B]
void WaitForDspSync(); // 0x00462794 | fefates:bytes [tier B]
void DecodeAdpcmData(const u8* src, s16* dst, const AdpcmParam& param, AdpcmContext& context, int sampleCount); // 0x004628D8 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void LockSoundThread(); // 0x004629FC | nintendogs:callseq [tier C]
void SetMasterVolume(float volume); // 0x00462A10 (name is ours)
void ClearAuxCallback(AuxBusId bus); // 0x00462A1C | fefates:callgraph [tier C]
nn::Result StartSoundThread(const ThreadParameter* parameter, void (*callback)(uptr), uptr arg, const ThreadParameter* userParameter,
                            void (*userCallback)(uptr), uptr userArg, int coreNo); // 0x00462DFC | nintendogs:bytes [confirmed by fefates] [tier A]
void SetAuxFrontBypass(AuxBusId bus, bool isBypass); // 0x0046338C (name is ours)
void UnlockSoundThread(); // 0x004633A0 | nintendogs:callseq [tier C]
bool GetHeadphoneStatus(); // 0x004633B4 | nintendogs:callgraph [confirmed by fefates] [tier A]
OutputMode GetSoundOutputMode(); // 0x004633C4 (name is ours)
void SendParameterToDsp(); // 0x004633D0 | fefates:bytes [tier B]
void SetAuxReturnVolume(AuxBusId bus, float volume); // 0x00463470 | nintendogs:callseq-callee [confirmed by fefates, mk7dlp] [tier A]
bool SetSoundOutputMode(OutputMode mode); // 0x00463480 (name is ours)
void RegisterAuxCallback(AuxBusId bus, AuxCallback callback, uptr arg); // 0x00463490 | nintendogs:callseq-callee [CONFLICT with mk7dlp:callseq: nn::snd::CTR::UserSoundThreadFunc(unsigned)] [tier X]
void UserSoundThreadFunc(uptr arg); // 0x004634A8 | fefates:bytes [tier B]
void InitializeWaveBuffer(WaveBuffer* buffer); // 0x00463560 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void SetOutputBufferCount(int count); // 0x004635A0 (name is ours)
void OrderToWaitForFinalize(); // 0x004635B0 | nintendogs:bytes [confirmed by fefates] [tier A]
void Sleep(); // 0x004635EC | nintendogs:bytes [confirmed by fefates] [tier A]
void WakeUp(); // 0x0046538C | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
nn::Result Finalize(); // 0x004658E4 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
void FreeVoice(Voice* voice); // 0x0046633C | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
bool SetEffect(AuxBusId bus, FxDelay* delay); // 0x0046634C (name is ours)
bool SetEffect(AuxBusId bus, FxReverb* reverb); // 0x00466360 (name is ours)
} // namespace CTR
} // namespace snd
} // namespace nn

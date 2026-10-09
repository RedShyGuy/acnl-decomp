#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace dsp {
namespace CTR {
// a callback of RegisterSleepWakeUpCallback
typedef void (*SleepWakeUpCallback)();

nn::Result Initialize(); // 0x00351280 | nintendogs:callgraph [tier A]
void Finalize(); // 0x00351A50 (name is ours)

// Components: the program the DSP runs. The default one is in the binary; Sleep unloads it and
// WakeUp / Awake load it again.
nn::Result LoadDefaultComponent(); // 0x0035151C | nintendogs:callgraph [tier C]
nn::Result LoadComponent(const u8* component, size_t size, u16 programMask, u16 dataMask); // 0x00130A70 (name is ours)
nn::Result UnloadComponent(); // 0x003513C8 (name is ours)
bool IsComponentLoaded(); // 0x00130A60 | nintendogs:callgraph [tier A]
// readers of the component (against unloading)
void LockComponent(); // 0x00351388 | fefates:callgraph [tier C]
void UnlockComponent(); // 0x00351474 | fefates:callgraph [tier C]

// the wrappers of the DSP commands (the result 0xC8A0A7F8 without a session)
nn::Result SetSemaphore(u16 value); // 0x0035135C | fefates:callgraph [tier C]
nn::Result RecvDataIsReady(u16 registerNumber, bool* pIsReady); // 0x00351394 (name is ours)
nn::Result RecvData(u16 registerNumber, u16* pData); // 0x00351AE0 (name is ours)
nn::Result WriteProcessPipe(int channel, const void* buffer, unsigned size); // 0x00351480 | nintendogs:bytes [tier A]
nn::Result ReadPipeIfPossible(int channel, void* buffer, unsigned short size, unsigned short* pReadSize); // 0x003514C8 | nintendogs:bytes [tier A]
nn::Result SetSemaphoreMask(u16 mask); // 0x00351564 (name is ours)
nn::Result GetSemaphoreEventHandle(nn::Handle* pEvent); // 0x00351590 (name is ours)
// registers event for the interrupt, an invalid handle unregisters it
nn::Result RegisterInterruptEvents(nn::Handle event, int interrupt, int channel); // 0x003515BC | nintendogs:bytes [tier A]
nn::Result ConvertProcessAddressFromDspDram(unsigned int address, unsigned int* pAddress); // 0x003516FC | fefates:bytes [tier B]
nn::Result FlushDataCache(unsigned address, unsigned size); // 0x001409EC | nintendogs:bytes [tier A]

// up to 8 sets of callbacks: called before the component is unloaded for sleep, after it is
// loaded again, and by OrderToWaitForFinalize
bool RegisterSleepWakeUpCallback(SleepWakeUpCallback sleep, SleepWakeUpCallback wakeUp, SleepWakeUpCallback waitForFinalize); // 0x003516A4 | nintendogs:bytes [tier A]
bool ClearSleepWakeUpCallback(SleepWakeUpCallback sleep, SleepWakeUpCallback wakeUp, SleepWakeUpCallback waitForFinalize); // 0x00351654 | nintendogs:bytes [tier A]
void Sleep(); // 0x00130B60 | nintendogs:callgraph [tier A]
void WakeUp(); // 0x00130C04 | fefates:callgraph [tier C]
// wakes up after a sleep of the system (see the sleep callback in CTR_Api.cpp)
void Awake(); // 0x00129D38 | fefates:callgraph [tier C]
void OrderToWaitForFinalize(); // 0x001245B0 | nintendogs:bytes [tier A]
} // namespace CTR
} // namespace dsp
} // namespace nn

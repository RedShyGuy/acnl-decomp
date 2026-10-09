#pragma once

#include "decomp.h"
#include "nn/Handle.h"
#include "nn/Result.h"

namespace nn {
namespace dsp {
namespace CTR {
// The commands of the service dsp::DSP (3dbrew "DSP Services"); the object holds the session. The
// function names are from the symbols, the member name is ours.
class DSP
{
public:
    nn::Result RecvData(unsigned short registerNumber, unsigned short* pData); // 0x003519F8 | nintendogs:bytes [tier A]
    nn::Result RecvDataIsReady(unsigned short registerNumber, bool* pIsReady); // 0x00351820 | nintendogs:bytes [tier A]
    nn::Result SetSemaphore(unsigned short value); // 0x003517E8 | nintendogs:bytes [tier A]
    nn::Result ConvertProcessAddressFromDspDram(unsigned address, unsigned* pAddress); // 0x003519BC | nintendogs:bytes [tier A]
    nn::Result WriteProcessPipe(int channel, const unsigned char* buffer, unsigned size); // 0x0035189C | nintendogs:bytes [tier A]
    nn::Result ReadPipeIfPossible(int channel, int peer, unsigned char* buffer, unsigned short size, unsigned short* pReadSize); // 0x003518E4 | nintendogs:bytes [tier A]
    nn::Result LoadComponent(const unsigned char* component, unsigned size, unsigned short programMask, unsigned short dataMask, bool* pIsLoaded); // 0x00136864 | nintendogs:bytes [tier A]
    nn::Result UnloadComponent(); // 0x001368C4 | nintendogs:bytes [tier A]
    nn::Result FlushDataCache(nn::Handle process, unsigned address, unsigned size); // 0x001436D0 | nintendogs:bytes [tier A]
    nn::Result RegisterInterruptEvents(nn::Handle event, int interrupt, int channel); // 0x00351980 | nintendogs:bytes [tier A]
    nn::Result GetSemaphoreEventHandle(nn::Handle* pEvent); // 0x0035194C | nintendogs:bytes [tier A]
    nn::Result SetSemaphoreMask(unsigned short mask); // 0x00351864 | nintendogs:bytes [tier A]
    nn::Result ForceHeadphoneOut(bool isForced); // 0x00130B28 | fefates:bytes [tier B]

    nn::Handle m_Session; // 0x0
};
ASSERT_SIZE(DSP, 4);
} // namespace CTR
} // namespace dsp
} // namespace nn

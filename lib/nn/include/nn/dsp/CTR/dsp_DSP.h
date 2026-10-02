#pragma once

#include "decomp.h"

namespace nn {
namespace dsp {
namespace CTR {
class DSP
{
public:
    void ForceHeadphoneOut(bool); // 0x00130B28 | fefates:bytes [tier B]
    void LoadComponent(const unsigned char*, unsigned, unsigned short, unsigned short, bool*); // 0x00136864 | nintendogs:bytes [tier A]
    void UnloadComponent(); // 0x001368C4 | nintendogs:bytes [tier A]
    void FlushDataCache(nn::Handle, unsigned, unsigned); // 0x001436D0 | nintendogs:bytes [tier A]
    void SetSemaphore(unsigned short); // 0x003517E8 | nintendogs:bytes [tier A]
    void RecvDataIsReady(unsigned short, bool*); // 0x00351820 | nintendogs:bytes [tier A]
    void SetSemaphoreMask(unsigned short); // 0x00351864 | nintendogs:bytes [tier A]
    void WriteProcessPipe(int, const unsigned char*, unsigned); // 0x0035189C | nintendogs:bytes [tier A]
    void ReadPipeIfPossible(int, int, unsigned char*, unsigned short, unsigned short*); // 0x003518E4 | nintendogs:bytes [tier A]
    void GetSemaphoreEventHandle(nn::Handle*); // 0x0035194C | nintendogs:bytes [tier A]
    void RegisterInterruptEvents(nn::Handle, int, int); // 0x00351980 | nintendogs:bytes [tier A]
    void ConvertProcessAddressFromDspDram(unsigned, unsigned*); // 0x003519BC | nintendogs:bytes [tier A]
    void RecvData(unsigned short, unsigned short*); // 0x003519F8 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace dsp
} // namespace nn

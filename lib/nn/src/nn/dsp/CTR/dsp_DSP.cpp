#include "nn/dsp/CTR/dsp_DSP.h"

namespace nn {
namespace dsp {
namespace CTR {
// 0x00130B28 | fefates:bytes [tier B]
void nn::dsp::CTR::DSP::ForceHeadphoneOut(bool)
{
}

// 0x00136864 | nintendogs:bytes [tier A]
void nn::dsp::CTR::DSP::LoadComponent(const unsigned char*, unsigned, unsigned short, unsigned short, bool*)
{
}

// 0x001368C4 | nintendogs:bytes [tier A]
void nn::dsp::CTR::DSP::UnloadComponent()
{
}

// 0x001436D0 | nintendogs:bytes [tier A]
void nn::dsp::CTR::DSP::FlushDataCache(nn::Handle, unsigned, unsigned)
{
}

// 0x003517E8 | nintendogs:bytes [tier A]
void nn::dsp::CTR::DSP::SetSemaphore(unsigned short)
{
}

// 0x00351820 | nintendogs:bytes [tier A]
void nn::dsp::CTR::DSP::RecvDataIsReady(unsigned short, bool*)
{
}

// 0x00351864 | nintendogs:bytes [tier A]
void nn::dsp::CTR::DSP::SetSemaphoreMask(unsigned short)
{
}

// 0x0035189C | nintendogs:bytes [tier A]
void nn::dsp::CTR::DSP::WriteProcessPipe(int, const unsigned char*, unsigned)
{
}

// 0x003518E4 | nintendogs:bytes [tier A]
void nn::dsp::CTR::DSP::ReadPipeIfPossible(int, int, unsigned char*, unsigned short, unsigned short*)
{
}

// 0x0035194C | nintendogs:bytes [tier A]
void nn::dsp::CTR::DSP::GetSemaphoreEventHandle(nn::Handle*)
{
}

// 0x00351980 | nintendogs:bytes [tier A]
void nn::dsp::CTR::DSP::RegisterInterruptEvents(nn::Handle, int, int)
{
}

// 0x003519BC | nintendogs:bytes [tier A]
void nn::dsp::CTR::DSP::ConvertProcessAddressFromDspDram(unsigned, unsigned*)
{
}

// 0x003519F8 | nintendogs:bytes [tier A]
void nn::dsp::CTR::DSP::RecvData(unsigned short, unsigned short*)
{
}

} // namespace CTR
} // namespace dsp
} // namespace nn

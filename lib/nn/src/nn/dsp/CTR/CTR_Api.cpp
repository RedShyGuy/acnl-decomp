#include "nn/dsp/CTR/CTR_Api.h"

namespace nn {
namespace dsp {
namespace CTR {
// 0x001245B0 | nintendogs:bytes [tier A]
void OrderToWaitForFinalize()
{
}

// 0x00130A60 | nintendogs:callgraph [tier A]
void IsComponentLoaded()
{
}

// 0x00130B60 | nintendogs:callgraph [tier A]
void Sleep()
{
}

// 0x001409EC | nintendogs:bytes [tier A]
void FlushDataCache(unsigned, unsigned)
{
}

// 0x00351280 | nintendogs:callgraph [tier A]
void Initialize()
{
}

// 0x00351480 | nintendogs:bytes [tier A]
void WriteProcessPipe(int, const void*, unsigned)
{
}

// 0x003514C8 | nintendogs:bytes [tier A]
void ReadPipeIfPossible(int, void*, unsigned short, unsigned short*)
{
}

// 0x003515BC | nintendogs:bytes [tier A]
void RegisterInterruptEvents(nn::Handle, int, int)
{
}

// 0x00351654 | nintendogs:bytes [tier A]
void ClearSleepWakeUpCallback(void(*)(), void(*)(), void(*)())
{
}

// 0x003516A4 | nintendogs:bytes [tier A]
void RegisterSleepWakeUpCallback(void(*)(), void(*)(), void(*)())
{
}

// 0x003516FC | fefates:bytes [tier B]
void ConvertProcessAddressFromDspDram(unsigned int, unsigned int*)
{
}

} // namespace CTR
} // namespace dsp
} // namespace nn

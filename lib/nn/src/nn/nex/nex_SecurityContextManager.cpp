#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_SecurityContextManager.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::SecurityContextManager::SecurityContextManager()
{
}

// 0x0039E1E4 slot 0x00 | virtual slot, introduced by nn::nex::SecurityContextManager
void nn::nex::SecurityContextManager::vf_0x00()
{
}

// 0x0039E1B0 slot 0x04 | virtual slot, introduced by nn::nex::SecurityContextManager
void nn::nex::SecurityContextManager::vf_0x04()
{
}

// 0x0039DE34 | fefates:bytes [tier B]
void nn::nex::SecurityContextManager::StaticGetCurrentCID()
{
}

// 0x0039DE7C | fefates:bytes [tier B]
void nn::nex::SecurityContextManager::StaticGetCurrentPID()
{
}

// 0x0039DEC4 | mk7dlp:bytes [tier A]
void nn::nex::SecurityContextManager::Pop()
{
}

// 0x0039DF20 | mk7dlp:callseq [tier A]
void nn::nex::SecurityContextManager::Push(unsigned, unsigned)
{
}

// 0x0072D0B0 | fefates:bytes [tier B]
void nn::nex::SecurityContextManager::GetCurrentAddress() const
{
}

} // namespace nex
} // namespace nn

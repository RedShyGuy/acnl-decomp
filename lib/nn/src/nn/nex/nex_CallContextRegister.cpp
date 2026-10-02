#include "nn/nex/nex_SystemComponent.h"
#include "nn/nex/nex_CallContextRegister.h"

namespace nn {
namespace nex {
// 0x003926A0 slot 0x00 | fefates:callseq
nn::nex::CallContextRegister::~CallContextRegister()
{
}

// 0x0072CBA8 slot 0x08 | virtual slot, introduced by nn::nex::SystemComponent
void nn::nex::CallContextRegister::vf_0x08()
{
}

// 0x0072CBD8 slot 0x0C | virtual slot, introduced by nn::nex::SystemComponent
void nn::nex::CallContextRegister::vf_0x0C()
{
}

// 0x00392234 slot 0x10 | virtual slot, introduced by nn::nex::SystemComponent
void nn::nex::CallContextRegister::vf_0x10()
{
}

// 0x003921F8 slot 0x20 | slot vf_0x20 of nn::nex::SystemComponent
void nn::nex::CallContextRegister::BeginInitialization()
{
}

// 0x00391D20 slot 0x28 | fefates:bytes
void nn::nex::CallContextRegister::BeginTermination()
{
}

// 0x00391A54 | fefates:bytes [tier B]
void nn::nex::CallContextRegister::Unregister(unsigned int)
{
}

// 0x00391B78 | fefates:bytes [tier B]
void nn::nex::CallContextRegister::GetCallContext(unsigned int)
{
}

// 0x00391BD8 | fefates:bytes [tier B]
void nn::nex::CallContextRegister::GetAssociatedId(unsigned int, nn::nex::qVector<unsigned int>*)
{
}

// 0x00391DA8 | fefates:bytes-fuzzy [tier B]
void nn::nex::CallContextRegister::CancelCallContexts(void*, unsigned int)
{
}

// 0x00392208 | fefates:bytes [tier B]
void nn::nex::CallContextRegister::AssociateCallContext(unsigned int, unsigned int)
{
}

// 0x00392238 | fefates:bytes [tier B]
void nn::nex::CallContextRegister::Start()
{
}

// 0x00392350 | fefates:bytes [tier B]
void nn::nex::CallContextRegister::Register(nn::nex::CallContext*)
{
}

// 0x003924AC | fefates:bytes [tier B]
nn::nex::CallContextRegister::CallContextRegister()
{
}

} // namespace nex
} // namespace nn

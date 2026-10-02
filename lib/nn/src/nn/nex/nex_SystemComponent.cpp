#include "nn/nex/nex_RefCountedObject.h"
#include "nn/nex/nex_SystemComponent.h"

namespace nn {
namespace nex {
// TODO: default ctor added so derived stubs compile - may not exist
nn::nex::SystemComponent::SystemComponent()
{
}

// 0x0037E47C slot 0x00 | fefates:callgraph
nn::nex::SystemComponent::~SystemComponent()
{
}

// 0x0072B6EC slot 0x08 | virtual slot, introduced by nn::nex::SystemComponent
void nn::nex::SystemComponent::vf_0x08()
{
}

// 0x0072B714 slot 0x0C | virtual slot, introduced by nn::nex::SystemComponent
void nn::nex::SystemComponent::vf_0x0C()
{
}

// 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
void nn::nex::SystemComponent::vf_0x10()
{
}

// 0x0037DF18 slot 0x14 | slot vf_0x14 of nn::nex::SystemComponent
void nn::nex::SystemComponent::StateTransition(nn::nex::SystemComponent::_State)
{
}

// 0x0037DEEC slot 0x18 | slot vf_0x18 of nn::nex::SystemComponent
void nn::nex::SystemComponent::OnInitialize()
{
}

// 0x0037DEE0 slot 0x1C | slot vf_0x1C of nn::nex::SystemComponent
void nn::nex::SystemComponent::OnTerminate()
{
}

// 0x0037E018 slot 0x20 | slot vf_0x20 of nn::nex::SystemComponent
void nn::nex::SystemComponent::BeginInitialization()
{
}

// 0x0037E010 slot 0x24 | slot vf_0x24 of nn::nex::SystemComponent
void nn::nex::SystemComponent::EndInitialization()
{
}

// 0x0037E008 slot 0x28 | slot vf_0x28 of nn::nex::SystemComponent
void nn::nex::SystemComponent::BeginTermination()
{
}

// 0x0037DF10 slot 0x2C | slot vf_0x2C of nn::nex::SystemComponent
void nn::nex::SystemComponent::EndTermination()
{
}

// 0x0037DF1C slot 0x30 | fefates:bytes
void nn::nex::SystemComponent::ValidTransition(nn::nex::SystemComponent::_State)
{
}

// 0x0037DEF8 slot 0x34 | fefates:bytes
void nn::nex::SystemComponent::UseIsAllowed()
{
}

// 0x0037E3C0 slot 0x38 | slot vf_0x38 of nn::nex::SystemComponent
void nn::nex::SystemComponent::TestState()
{
}

// 0x0037E204 slot 0x3C | slot vf_0x3C of nn::nex::SystemComponent
void nn::nex::SystemComponent::DoWork()
{
}

// 0x0037DDCC | fefates:bytes [tier B]
void nn::nex::SystemComponent::Initialize()
{
}

// 0x0037E020 | fefates:bytes-fuzzy [tier B]
void nn::nex::SystemComponent::WaitForTerminatedState(unsigned int)
{
}

// 0x0037E210 | fefates:bytes [tier B]
void nn::nex::SystemComponent::Terminate()
{
}

// 0x0037E3C8 | fefates:bytes [tier B]
nn::nex::SystemComponent::SystemComponent(const nn::nex::String&)
{
}

} // namespace nex
} // namespace nn

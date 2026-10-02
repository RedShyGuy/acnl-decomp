#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex15SystemComponentE @ 0x008CE520
// vtable 0x008FD004 (vptr 0x008FD00C), offset_to_top 0, 16 entries
class SystemComponent : public ::nn::nex::RefCountedObject
{
public:
    class Use;
    struct _State { u32 _unknown; }; // TODO: real type unknown (placeholder)
    SystemComponent(); // TODO: default ctor added so derived stubs compile - may not exist
    virtual ~SystemComponent(); // 0x0037E47C slot 0x00 | fefates:callgraph
    // 0x0037E420 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void vf_0x08(); // 0x0072B6EC slot 0x08 | virtual slot, introduced by nn::nex::SystemComponent
    virtual void vf_0x0C(); // 0x0072B714 slot 0x0C | virtual slot, introduced by nn::nex::SystemComponent
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void StateTransition(nn::nex::SystemComponent::_State); // 0x0037DF18 slot 0x14 | slot vf_0x14 of nn::nex::SystemComponent
    virtual void OnInitialize(); // 0x0037DEEC slot 0x18 | slot vf_0x18 of nn::nex::SystemComponent
    virtual void OnTerminate(); // 0x0037DEE0 slot 0x1C | slot vf_0x1C of nn::nex::SystemComponent
    virtual void BeginInitialization(); // 0x0037E018 slot 0x20 | slot vf_0x20 of nn::nex::SystemComponent
    virtual void EndInitialization(); // 0x0037E010 slot 0x24 | slot vf_0x24 of nn::nex::SystemComponent
    virtual void BeginTermination(); // 0x0037E008 slot 0x28 | slot vf_0x28 of nn::nex::SystemComponent
    virtual void EndTermination(); // 0x0037DF10 slot 0x2C | slot vf_0x2C of nn::nex::SystemComponent
    virtual void ValidTransition(nn::nex::SystemComponent::_State); // 0x0037DF1C slot 0x30 | fefates:bytes
    virtual void UseIsAllowed(); // 0x0037DEF8 slot 0x34 | fefates:bytes
    virtual void TestState(); // 0x0037E3C0 slot 0x38 | slot vf_0x38 of nn::nex::SystemComponent
    virtual void DoWork(); // 0x0037E204 slot 0x3C | slot vf_0x3C of nn::nex::SystemComponent
    void Initialize(); // 0x0037DDCC | fefates:bytes [tier B]
    void WaitForTerminatedState(unsigned int); // 0x0037E020 | fefates:bytes-fuzzy [tier B]
    void Terminate(); // 0x0037E210 | fefates:bytes [tier B]
    SystemComponent(const nn::nex::String&); // 0x0037E3C8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn

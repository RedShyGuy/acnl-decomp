#pragma once

#include "decomp.h"
#include "nn/nex/nex_SystemComponent.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex20SystemComponentGroupE @ 0x008CEA28
// vtable 0x008FDC44 (vptr 0x008FDC4C), offset_to_top 0, 16 entries
class SystemComponentGroup : public ::nn::nex::SystemComponent
{
public:
    SystemComponentGroup(); // ctor candidate(s) 0x0039818C (unverified)
    virtual ~SystemComponentGroup(); // 0x00398290 slot 0x00 | fefates:bytes
    // 0x0039825C slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void vf_0x08(); // 0x0072CD4C slot 0x08 | virtual slot, introduced by nn::nex::SystemComponent
    virtual void vf_0x0C(); // 0x0072CD80 slot 0x0C | virtual slot, introduced by nn::nex::SystemComponent
    virtual void vf_0x10(); // 0x00398058 slot 0x10 | virtual slot, introduced by nn::nex::SystemComponent
    virtual void OnInitialize(); // 0x00397D44 slot 0x18 | slot vf_0x18 of nn::nex::SystemComponent
    virtual void OnTerminate(); // 0x00397D40 slot 0x1C | slot vf_0x1C of nn::nex::SystemComponent
    virtual void BeginInitialization(); // 0x00397EC8 slot 0x20 | slot vf_0x20 of nn::nex::SystemComponent
    virtual void EndInitialization(); // 0x00397DA8 slot 0x24 | slot vf_0x24 of nn::nex::SystemComponent
    virtual void BeginTermination(); // 0x00397D78 slot 0x28 | slot vf_0x28 of nn::nex::SystemComponent
    virtual void EndTermination(); // 0x00397D48 slot 0x2C | slot vf_0x2C of nn::nex::SystemComponent
    virtual void TestState(); // 0x0039816C slot 0x38 | fefates:bytes
    virtual void DoWork(); // 0x0039805C slot 0x3C | fefates:bytes
    void RegisterComponent(nn::nex::SystemComponent*); // 0x00397DD8 | fefates:bytes [tier B]
    void UnregisterComponent(nn::nex::SystemComponent*); // 0x00397EF8 | fefates:bytes [tier B]
    void UnregisterAllComponents(); // 0x00397FA0 | fefates:bytes [tier B]
    SystemComponentGroup(const nn::nex::String&); // 0x0039818C | fefates:bytes [tier B]
    void GetComponentsState() const; // 0x0072CC58 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn

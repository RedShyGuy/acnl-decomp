#pragma once

#include "decomp.h"
#include "nn/nex/nex_SystemComponent.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex19CallContextRegisterE @ 0x008CE790
// vtable 0x008FD5F8 (vptr 0x008FD600), offset_to_top 0, 16 entries
class CallContextRegister : public ::nn::nex::SystemComponent
{
public:
    virtual ~CallContextRegister(); // 0x003926A0 slot 0x00 | fefates:callseq
    // 0x00392670 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    virtual void vf_0x08(); // 0x0072CBA8 slot 0x08 | virtual slot, introduced by nn::nex::SystemComponent
    virtual void vf_0x0C(); // 0x0072CBD8 slot 0x0C | virtual slot, introduced by nn::nex::SystemComponent
    virtual void vf_0x10(); // 0x00392234 slot 0x10 | virtual slot, introduced by nn::nex::SystemComponent
    virtual void BeginInitialization(); // 0x003921F8 slot 0x20 | slot vf_0x20 of nn::nex::SystemComponent
    virtual void BeginTermination(); // 0x00391D20 slot 0x28 | fefates:bytes
    void Unregister(unsigned int); // 0x00391A54 | fefates:bytes [tier B]
    void GetCallContext(unsigned int); // 0x00391B78 | fefates:bytes [tier B]
    void GetAssociatedId(unsigned int, nn::nex::qVector<unsigned int>*); // 0x00391BD8 | fefates:bytes [tier B]
    void CancelCallContexts(void*, unsigned int); // 0x00391DA8 | fefates:bytes-fuzzy [tier B]
    void AssociateCallContext(unsigned int, unsigned int); // 0x00392208 | fefates:bytes [tier B]
    void Start(); // 0x00392238 | fefates:bytes [tier B]
    void Register(nn::nex::CallContext*); // 0x00392350 | fefates:bytes [tier B]
    CallContextRegister(); // 0x003924AC | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn

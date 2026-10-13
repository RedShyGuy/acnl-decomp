#pragma once

#include "decomp.h"
#include "nn/nex/nex_SystemComponent.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// A SystemComponent made of others: it initializes / terminates them in DoWork and its state
// follows theirs. The member name and the names of the functions without a reference name are
// ours.
// RTTI N2nn3nex20SystemComponentGroupE @ 0x008CEA28
// vtable 0x008FDC44 (vptr 0x008FDC4C), offset_to_top 0, 16 entries
class SystemComponentGroup : public ::nn::nex::SystemComponent
{
public:
    explicit SystemComponentGroup(const nn::nex::String& name); // 0x0039818C | fefates:bytes [tier B]
    virtual ~SystemComponentGroup(); // 0x00398290 slot 0x00 | fefates:bytes
    // 0x0039825C slot 0x04 (deleting dtor)
    // (inline in the original, with copies out of line)
    virtual const wchar_t* GetType() const; // 0x0072CD4C slot 0x08
    virtual bool IsAKindOf(const wchar_t* pType) const; // 0x0072CD80 slot 0x0C
    virtual void vf_0x10(); // 0x00398058 slot 0x10
    virtual void OnInitialize(); // 0x00397D44 slot 0x18 (slot name from the reference symbols)
    virtual void OnTerminate(); // 0x00397D40 slot 0x1C (slot name from the reference symbols)
    virtual bool BeginInitialization(); // 0x00397EC8 slot 0x20 (slot name from the reference symbols)
    virtual bool EndInitialization(); // 0x00397DA8 slot 0x24 (slot name from the reference symbols)
    virtual bool BeginTermination(); // 0x00397D78 slot 0x28 (slot name from the reference symbols)
    virtual bool EndTermination(); // 0x00397D48 slot 0x2C (slot name from the reference symbols)
    virtual _State TestState(); // 0x0039816C slot 0x38 | fefates:bytes
    virtual void DoWork(); // 0x0039805C slot 0x3C | fefates:bytes

    bool RegisterComponent(nn::nex::SystemComponent* pComponent); // 0x00397DD8 | fefates:bytes [tier B]
    // releases the component
    bool UnregisterComponent(nn::nex::SystemComponent* pComponent); // 0x00397EF8 | fefates:bytes [tier B]
    bool UnregisterAllComponents(); // 0x00397FA0 | fefates:bytes [tier B]
    // the state of the group from the states of the components
    _State GetComponentsState() const; // 0x0072CC58 | fefates:bytes [tier B]

    qList<SystemComponent*> m_Components; // 0x20
};
ASSERT_SIZE(SystemComponentGroup, 0x38);
} // namespace nex
} // namespace nn

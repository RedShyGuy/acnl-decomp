#pragma once

#include "decomp.h"
#include "nn/nex/nex_SystemComponentGroup.h"

namespace nn {
namespace nex {
// All SystemComponents of the Core ("System Components" with the group "Core"). The member names
// are ours.
// RTTI N2nn3nex16SystemComponentsE @ 0x008CE620
// vtable 0x008FD244 (vptr 0x008FD24C), offset_to_top 0, 16 entries
class SystemComponents : public ::nn::nex::SystemComponentGroup
{
public:
    // (inline in the constructor of Core)
    SystemComponents() : SystemComponentGroup(String(L"System Components")), m_Unknown0x38(0), m_Unknown0x3C(0), m_Unknown0x40(0)
    {
        m_pCoreGroup = new SystemComponentGroup(String(L"Core"));
        RegisterComponent(m_pCoreGroup);
    }
    // (only a nop in the original that falls into ~SystemComponentGroup)
    virtual ~SystemComponents(); // 0x0039828C slot 0x00
    // 0x00383A28 slot 0x04 (deleting dtor)

    u32 m_Unknown0x38;                  // 0x38
    u32 m_Unknown0x3C;                  // 0x3C
    u32 m_Unknown0x40;                  // 0x40
    SystemComponentGroup* m_pCoreGroup; // 0x44
};
ASSERT_SIZE(SystemComponents, 0x48);
} // namespace nex
} // namespace nn

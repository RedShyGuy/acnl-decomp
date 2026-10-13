#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_SystemError.h"

namespace nn {
namespace nex {
class InstanceControl;

// The instances (PseudoSingletons) of one context, one per kind. The names of the functions other
// than the class and of the members are ours.
// RTTI N2nn3nex20InstantiationContextE @ 0x008CE8A4
// vtable 0x008FD838 (vptr 0x008FD840), offset_to_top 0, 2 entries
class InstantiationContext : public ::nn::nex::RootObject
{
public:
    static const u32 INSTANCE_COUNT = 15;

    // (inline)
    InstantiationContext() : m_IsValid(false)
    {
        for (u32 i = 0; i < INSTANCE_COUNT; i++) {
            m_Instances[i] = nullptr;
        }
        m_IsValid = true;
    }
    virtual ~InstantiationContext(); // 0x00396898 slot 0x00
    // 0x00396838 slot 0x04 (deleting dtor)

    // false if the kind is invalid or taken
    bool Register(InstanceControl* pInstance, u32 kind); // 0x003967D8
    // (inline in the original; InstantiationContext.cpp)
    void Unregister(u32 kind)
    {
        if (kind >= INSTANCE_COUNT) {
            SystemError::SignalError(const_cast<wchar_t*>(L"D:/home/jenkins/workspace/Manual-Pack/5129d543/OnlineCore/src/Core/InstantiationContext.cpp"), 85,
                                     SystemError::ERROR_OUT_OF_RANGE, 0);
            return;
        }
        m_Instances[kind] = nullptr;
    }

    InstanceControl* m_Instances[INSTANCE_COUNT]; // 0x04
    bool m_IsValid;                               // 0x40
};
ASSERT_SIZE(InstantiationContext, 0x44);
} // namespace nex
} // namespace nn

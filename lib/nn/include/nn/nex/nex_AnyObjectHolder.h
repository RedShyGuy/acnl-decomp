#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// Instantiations found in the binary:
//   nn::nex::AnyObjectHolder<nn::nex::Data, nn::nex::String>  typeinfo 0x008CE43C  vtable 0x008FCE24
//   nn::nex::AnyObjectHolder<nn::nex::Gathering, nn::nex::String>  typeinfo 0x008CE454  vtable 0x008FCE34
//
// Holds an object of the DDL type (any<Gathering,string> in the protocol definitions) and owns
// it. The layout is from pia::inet::NexMatchmakeSession; ARMCC inlines its functions. The names
// are ours.
template <typename T0, typename T1>
class AnyObjectHolder
{
public:
    AnyObjectHolder() : m_pObject(nullptr) {}
    // (not decompiled yet)
    virtual ~AnyObjectHolder();

    T0* Get() const { return m_pObject; }
    // the object is deleted and replaced
    void Reset(T0* pObject)
    {
        if (m_pObject != nullptr) {
            delete m_pObject;
        }
        m_pObject = pObject;
    }
    // the caller owns the object now
    T0* Release()
    {
        T0* pObject = m_pObject;
        m_pObject = nullptr;
        return pObject;
    }

    T0* m_pObject; // 0x4
};
} // namespace nex
} // namespace nn

#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// Instantiations found in the binary:
//   nn::nex::qProtectedList<nn::nex::StationURL>  typeinfo 0x008CE430  vtable 0x008FCE14
//
// A list with its lock. The member names are ours.
template <typename T0>
class qProtectedList : public RootObject
{
public:
    // (inline)
    explicit qProtectedList(u32 level = 1) : m_Lock(level) {}
    // (out of line per instance)
    virtual ~qProtectedList() {}

    // (inline)
    void push_back(const T0& value)
    {
        ScopedCS lock(m_Lock);
        m_List.push_back(value);
    }

    CriticalSection m_Lock; // 0x04
    qList<T0> m_List;       // 0x2C
};
} // namespace nex
} // namespace nn

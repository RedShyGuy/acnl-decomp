#pragma once

#include "decomp.h"
#include "nn/pia/common/common_ObjList.h"

namespace nn {
namespace pia {
namespace common {
// An ObjList with its N nodes in the object itself. The template name is from the RTTI; the
// layout is from inet::NatTraversalTimeList, the member names are ours.
//
// Instantiations found in the binary:
//   nn::pia::common::FixedObjList<nn::pia::inet::NatDetecter::SendNatCheckMessage, 20u>  typeinfo 0x008CFE10
//   nn::pia::common::FixedObjList<nn::pia::inet::NatTraversalTime, 12u>  typeinfo 0x008CFE1C
template <typename T, u32 N>
class FixedObjList : public ObjList<T>
{
public:
    typedef typename ObjList<T>::Node Node;

    FixedObjList() { this->Initialize(reinterpret_cast<Node*>(m_NodeBuffer), N); }
    // (empty: NatDetecter::SendNatCheckMessageList has an empty destructor, NatTraversalTimeList
    // returns its nodes in its own)

    // 4-byte aligned like the original: the base sits right after the vptr of a derived class
    u32 m_NodeBuffer[(sizeof(Node) * N + 3) / 4]; // 0x2C
};
} // namespace common
} // namespace pia
} // namespace nn

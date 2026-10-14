#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// Instantiations found in the binary:
//   nn::nex::LogicalClockTmpl<unsigned short, short>  typeinfo 0x008CE5B4  vtable 0x008FD17C
//
// A wrapping counter (sequence ids of PRUDP); T the value, D its difference. The member names are
// ours.
template <typename T, typename D>
class LogicalClockTmpl : public ::nn::nex::RootObject
{
public:
    LogicalClockTmpl() : m_Value(0) {}
    virtual ~LogicalClockTmpl() {}

    T m_Value; // 0x04
};
} // namespace nex
} // namespace nn

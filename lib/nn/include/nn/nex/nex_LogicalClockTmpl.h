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
    explicit LogicalClockTmpl(T value) : m_Value(value) {}
    LogicalClockTmpl(const LogicalClockTmpl& other) : m_Value(other.m_Value) {}
    virtual ~LogicalClockTmpl() {}

    // (inline) before the other value (by the wrapping difference)
    bool operator<(const LogicalClockTmpl& other) const { return static_cast<D>(other.m_Value - m_Value) > 0; }

    T m_Value; // 0x04
};
} // namespace nex
} // namespace nn

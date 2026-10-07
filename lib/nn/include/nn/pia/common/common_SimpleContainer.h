#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// A fixed array of at most N objects that is used from the front (the template and its base are
// from the RTTI; the layout is from transport::StationManager, the member and function names are
// ours). Only the destructors and Trace are out of line.
//
// Instantiations found in the binary:
//   nn::pia::common::SimpleContainer<nn::pia::common::InetAddress, 4>  typeinfo 0x008CFE5C  vtable 0x00901544
//   nn::pia::common::SimpleContainer<nn::pia::transport::Station*, 12>  typeinfo 0x008CFE68  vtable 0x00901558
template <typename T, u32 N>
class SimpleContainer : public RootObject
{
public:
    SimpleContainer() : m_Num(0), m_Data() {}
    virtual ~SimpleContainer() {}
    virtual void Trace(u64 flag) const {}

    u32 GetNum() const { return m_Num; }
    bool IsFull() const { return m_Num == N; }
    T* Begin() { return m_Data; }
    T* End() { return m_Data + m_Num; }
    const T* Begin() const { return m_Data; }
    const T* End() const { return m_Data + m_Num; }
    T& Back() { return m_Data[m_Num - 1]; }

    void PushBack(const T& value)
    {
        if (m_Num != N) {
            m_Num++;
            m_Data[m_Num - 1] = value;
        }
    }
    // the element of value, End() if there is none
    T* Find(const T& value)
    {
        T* it;
        for (it = Begin(); it != End(); it++) {
            if (*it == value) {
                break;
            }
        }
        return it;
    }
    bool IsInclude(const T& value) { return Find(value) != End(); }
    // the element at it (the ones after it move forward)
    void Erase(T* it)
    {
        if (it < Begin() || it >= End()) {
            return;
        }
        for (; End() - it != 1; it++) {
            *it = *(it + 1);
        }
        m_Num--;
    }

    u32 m_Num;    // 0x4
    T m_Data[N];  // 0x8
};
} // namespace common
} // namespace pia
} // namespace nn

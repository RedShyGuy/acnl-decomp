#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
// A value that moves linearly to a target over a number of frames (the class name and the
// function names are from the reference symbols, the members are ours).
template <typename TValue, typename TCount>
class MoveValue
{
public:
    MoveValue() : m_Origin(0), m_Target(0), m_Frame(0), m_Counter(0) {}

    // the current value
    TValue GetValue() const
    {
        if (IsFinished()) {
            return m_Target;
        }
        return m_Origin + (m_Target - m_Origin) * m_Counter / m_Frame;
    }
    // from the current value to the target in frame frames
    void SetTarget(TValue target, TCount frame)
    {
        m_Origin = GetValue();
        m_Target = target;
        m_Frame = frame;
        m_Counter = 0;
    }
    // (inline) the next frame
    void Update()
    {
        if (m_Counter < m_Frame) {
            ++m_Counter;
        }
    }
    // (inline)
    bool IsFinished() const { return m_Counter >= m_Frame; }
    TValue GetTarget() const { return m_Target; }

    TValue m_Origin;  // 0x0
    TValue m_Target;  // 0x4
    TCount m_Frame;   // 0x8
    TCount m_Counter; // 0xC
};
} // namespace internal
} // namespace snd
} // namespace nw

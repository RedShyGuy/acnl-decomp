#pragma once

#include "decomp.h"
#include <algorithm>

namespace nn {
namespace pia {
namespace common {
// The last N values in a ring and their median (transport::RttCalculator keeps the round trip
// times in one). The class and GetMedian are from the fefates symbols; the rest is ours.
template <typename T, int N>
class LatestMedian
{
public:
    // a new value, the oldest one goes when the ring is full (inline in RttCalculator::Update)
    void Add(T value)
    {
        m_Values[(m_Head + m_Count) % N] = value;
        if (m_Count == N) {
            m_Head = (m_Head + 1) % N;
        } else {
            m_Count++;
        }
    }
    void Clear()
    {
        m_Count = 0;
        m_Head = 0;
    }
    u32 GetCount() const { return m_Count; }

    // the median of the latest num values (0 without values)
    T GetMedian(unsigned int num) const;

    u32 m_Unknown0x0; // 0x00 (not used by the functions here)
    u32 m_Count;      // 0x04
    u32 m_Head;       // 0x08
    T m_Values[N];    // 0x0C
};

// <int, 16> is instantiated in common_LatestMedian.cpp; session::SyncClockProtocol has <int, 10> inline
template <typename T, int N>
T LatestMedian<T, N>::GetMedian(unsigned int num) const
{
    if (m_Count == 0) {
        return 0;
    }
    u32 count = m_Count >= num ? num : m_Count;
    T values[N];
    for (u32 i = 0; i < count; i++) {
        values[i] = m_Values[(m_Count - i - 1 + m_Head) % N];
    }
    std::sort(values, values + count);
    if (count & 1) {
        return values[count / 2];
    }
    return (values[count / 2] + values[count / 2 - 1]) / 2;
}

extern template int LatestMedian<int, 16>::GetMedian(unsigned int num) const;
} // namespace common
} // namespace pia
} // namespace nn

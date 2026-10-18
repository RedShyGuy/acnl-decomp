#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Time.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// A queue of values ordered by the time when they are due (the network emulation). The names of
// the members and of the functions without a reference name are ours.
// Instantiations found in the binary:
//   nn::nex::TimedQueue<nn::nex::NetworkEmulator::NetworkPacket>  typeinfo 0x008CDFDC  vtable 0x008FC1EC
template <typename T>
class TimedQueue : public ::nn::nex::RootObject
{
    // (the members are defined in nex_NetworkEmulator.cpp, the only instance)
public:
    // a value and when it is due
    struct Entry
    {
        Entry(const T& value) : m_Value(value) {}

        T m_Value;    // 0x00
        Time m_Time;  // (behind the value, 8-aligned)
    };

    // (inline)
    TimedQueue() {}
    virtual ~TimedQueue();
    // the value is due at the time (behind the values due at the same time)
    virtual void Queue(const T& value, nn::nex::Time time);
    // true if the first value is due
    virtual bool IsHeadReady(nn::nex::Time time);
    // the first value
    virtual T GetHead(nn::nex::Time time);
    // the first value is removed
    virtual void PopHead();

    // (inline) the first value, which is removed
    T Dequeue(nn::nex::Time time)
    {
        T value = GetHead(time);
        PopHead();
        return value;
    }

    qList<Entry> m_Entries; // 0x04
};

} // namespace nex
} // namespace nn

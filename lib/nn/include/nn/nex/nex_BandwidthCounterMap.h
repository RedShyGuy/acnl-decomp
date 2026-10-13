#pragma once

#include "decomp.h"
#include "nn/nex/nex_BandwidthCounter.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
// The BandwidthCounters of the incoming and the outgoing packets per key (e.g. per protocol),
// made on demand and named "<name> <key>/Incoming" etc. The names of the members are ours.
// RTTI N2nn3nex19BandwidthCounterMapE @ 0x008CE784
// vtable 0x008FD5E8 (vptr 0x008FD5F0), offset_to_top 0, 2 entries
class BandwidthCounterMap : public ::nn::nex::RootObject
{
public:
    // the counters of a key (the class name is from the binary)
    class IOBandwidthCounter
    {
    public:
        // (inline)
        IOBandwidthCounter(const String& incomingName, const String& outgoingName) : m_Incoming(incomingName), m_Outgoing(outgoingName) {}

        u32 m_Unknown0x00;          // 0x00 (not set)
        BandwidthCounter m_Incoming; // 0x04
        BandwidthCounter m_Outgoing; // 0x28
    };

    BandwidthCounterMap(); // TODO: default ctor added so derived stubs compile - may not exist
    explicit BandwidthCounterMap(const nn::nex::String& name); // 0x00391564 | fefates:bytes [tier B]
    virtual ~BandwidthCounterMap(); // 0x0039164C slot 0x00 | fefates:bytes [tier B]
    // 0x0039161C slot 0x04 (deleting dtor)
    // the counters of the key (made if there are none)
    IOBandwidthCounter* operator[](unsigned int key); // 0x003917F8 | fefates:bytes [tier B]

    String m_Name;                              // 0x04
    qMap<u32, IOBandwidthCounter*> m_Counters;  // 0x0C
};
ASSERT_SIZE(BandwidthCounterMap, 0x28);
} // namespace nex
} // namespace nn

namespace std {
// (operator[] has the insert of this instance inline)
template <>
inline nn::nex::BandwidthCounterMap::IOBandwidthCounter*& map<u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*, less<u32>, nn::nex::MemAllocator<pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*> > >::operator[](const u32& key)
{
    return IndexInline(key);
}
} // namespace std

namespace __rw {
// (the original calls erase(first, last) of this instance out of line; defined in nex_BandwidthCounterMap.cpp)
template <>
DECOMP_NOINLINE __rb_tree<u32, std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, __select1st<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, u32>,
                          std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*> > >::iterator
__rb_tree<u32, std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, __select1st<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, u32>,
          std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*> > >::erase(iterator first, iterator last);
} // namespace __rw

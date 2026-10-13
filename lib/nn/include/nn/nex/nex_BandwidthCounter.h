#pragma once

#include "decomp.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qChain.h"

namespace nn {
namespace nex {
// Statistics of the sizes of the packets of a direction (count, total, smallest, largest). All
// counters are in a global list. The names of the members are ours.
// RTTI N2nn3nex16BandwidthCounterE @ 0x008CE554
// vtable 0x008FD084 (vptr 0x008FD08C), offset_to_top 0, 2 entries
class BandwidthCounter : public ::nn::nex::RootObject
{
public:
    explicit BandwidthCounter(const nn::nex::String& name); // 0x0037EEA4 | fefates:callgraph [tier C]
    virtual ~BandwidthCounter(); // 0x0037EF90 slot 0x00 | fefates:callgraph [tier C]
    // 0x0037EF60 slot 0x04 (deleting dtor)
    // counts a packet of the size
    nn::nex::BandwidthCounter& operator+=(unsigned int size); // 0x0037F05C | mk7dlp:bytes [tier A]

    BandwidthCounter* m_pChainNext; // 0x04
    BandwidthCounter* m_pChainPrev; // 0x08
    String m_Name;                  // 0x0C
    u32 m_MinSize;                  // 0x14
    u32 m_MaxSize;                  // 0x18
    u32 m_TotalSize;                // 0x1C
    u32 m_Count;                    // 0x20

    static qChain<BandwidthCounter*, DefaultChainPolicy<BandwidthCounter*> > s_Counters;
    static CriticalSection s_Lock;
};
ASSERT_SIZE(BandwidthCounter, 0x24);
} // namespace nex
} // namespace nn

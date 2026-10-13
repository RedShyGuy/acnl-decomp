#include "nn/nex/nex_BandwidthCounter.h"

namespace nn {
namespace nex {
// 0x00ADE820 (name is ours)
qChain<BandwidthCounter*, DefaultChainPolicy<BandwidthCounter*> > BandwidthCounter::s_Counters;
// 0x00ADE834 (name is ours)
CriticalSection BandwidthCounter::s_Lock(0x20000000);

// 0x0037EEA4 | fefates:callgraph [tier C]
nn::nex::BandwidthCounter::BandwidthCounter(const nn::nex::String& name)
    : m_pChainNext(nullptr), m_pChainPrev(nullptr), m_Name(name), m_MinSize(static_cast<u32>(-1)), m_MaxSize(0), m_TotalSize(0), m_Count(0)
{
    ScopedCS lock(s_Lock);
    s_Counters.push_back(this);
}

// 0x0037EF90 | fefates:callgraph [tier C]
// 0x0037EF60 (deleting dtor)
nn::nex::BandwidthCounter::~BandwidthCounter()
{
    ScopedCS lock(s_Lock);
    qChain<BandwidthCounter*, DefaultChainPolicy<BandwidthCounter*> >::iterator it = s_Counters.begin();
    while (it != s_Counters.end() && *it != this) {
        it = qChain<BandwidthCounter*, DefaultChainPolicy<BandwidthCounter*> >::iterator((*it)->m_pChainNext);
    }
    if (it != s_Counters.end()) {
        s_Counters.erase(it);
    }
}

// 0x0037F05C | mk7dlp:bytes [tier A]
nn::nex::BandwidthCounter& nn::nex::BandwidthCounter::operator+=(unsigned int size)
{
    m_Count++;
    m_TotalSize += size;
    if (m_MinSize >= size) {
        m_MinSize = size;
    }
    m_MaxSize = m_MaxSize >= size ? m_MaxSize : size;
    return *this;
}

// 0x007E43C8
// 0x007E433C (deleting dtor)
template class qChain<BandwidthCounter*, DefaultChainPolicy<BandwidthCounter*> >;

} // namespace nex
} // namespace nn

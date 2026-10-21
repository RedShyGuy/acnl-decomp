#include "nn/nex/nex_BandwidthCounterMap.h"
#include "nn/nex/nex_MemoryManager.h"

namespace nn {
namespace nex {
// 0x00391564 | fefates:bytes [tier B]
nn::nex::BandwidthCounterMap::BandwidthCounterMap(const nn::nex::String& name) : m_Name(name)
{
}

// 0x0039164C | fefates:bytes [tier B]
// 0x0039161C (deleting dtor)
nn::nex::BandwidthCounterMap::~BandwidthCounterMap()
{
    for (qMap<u32, IOBandwidthCounter*>::iterator it = m_Counters.begin(); it != m_Counters.end(); ++it) {
        IOBandwidthCounter* pCounter = it->second;
        if (pCounter != nullptr) {
            pCounter->~IOBandwidthCounter();
            MemoryManager::FreeInline(pCounter);
        }
    }
    m_Counters.clear();
}

// 0x003917F8 | fefates:bytes [tier B]
nn::nex::BandwidthCounterMap::IOBandwidthCounter* nn::nex::BandwidthCounterMap::operator[](unsigned int key)
{
    qMap<u32, IOBandwidthCounter*>::iterator it = m_Counters.find(key);
    if (it != m_Counters.end()) {
        return it->second;
    }
    String name(L"");
    name.Format(L"%s %d", m_Name.m_pString, key);
    IOBandwidthCounter* pCounter = new (MemoryManager::Allocate(sizeof(IOBandwidthCounter))) IOBandwidthCounter(name + L"/Incoming", name + L"/Outgoing");
    m_Counters[key] = pCounter;
    return pCounter;
}

} // namespace nex
} // namespace nn

namespace __rw {
// 0x008074E8 | fefates:callgraph [tier C]
template <>
DECOMP_NOINLINE __rb_tree<u32, std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, __select1st<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, u32>,
          std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*> > >::iterator
__rb_tree<u32, std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, __select1st<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, u32>,
          std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*> > >::erase(iterator first, iterator last)
{
    // (erase(it) inline)
    return EraseRangeAllInline(first, last);
}
} // namespace __rw

namespace __rw {
// the functions of the tree of the counters
// 0x00807C44 (name is ours)
template void __rb_tree<u32, std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, __select1st<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*> > >::_C_erase(Node* pNode);
// 0x00807C80 (name is ours)
template __rb_tree<u32, std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, __select1st<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*> > >::iterator __rb_tree<u32, std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, __select1st<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>, u32>, std::less<u32>, nn::nex::MemAllocator<std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*> > >::_C_insert(Node* pNode, Node* pParent, const std::pair<const u32, nn::nex::BandwidthCounterMap::IOBandwidthCounter*>& value);
} // namespace __rw

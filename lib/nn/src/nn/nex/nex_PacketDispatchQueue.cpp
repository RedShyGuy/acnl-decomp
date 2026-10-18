#include "nn/nex/nex_PacketDispatchQueue.h"
#include "nn/nex/nex_PacketIn.h"

namespace nn {
namespace nex {
namespace {
// a is a == b or after b (the sequence ids wrap around)
inline bool IsNotBefore(u16 a, u16 b)
{
    return a == b || static_cast<s16>(a - b) > 0;
}
} // namespace

// 0x00394534 | mk7dlp:bytes [tier A]
nn::nex::PacketDispatchQueue::PacketDispatchQueue() : m_NextSequenceId(1)
{
}

// 0x003941B8 | fefates:bytes [tier B]
nn::nex::PacketIn* nn::nex::PacketDispatchQueue::GetNextToDispatch()
{
    PacketIn* pPacket = nullptr;
    PacketMap::iterator it = m_Packets.find(m_NextSequenceId);
    if (it != m_Packets.end()) {
        m_NextSequenceId.m_Value++;
        pPacket = it->second;
    }
    return pPacket;
}

// 0x00394244 | fefates:bytes [tier B]
void nn::nex::PacketDispatchQueue::Purge()
{
    PacketMap::iterator it = m_Packets.begin();
    while (it != m_Packets.end()) {
        it->second->ReleaseRef();
        m_Packets.erase(it++);
    }
}

// 0x00394324 | fefates:bytes [tier B]
void nn::nex::PacketDispatchQueue::Queue(nn::nex::PacketIn* pPacket)
{
    if (!IsNotBefore(pPacket->m_SequenceId.m_Value, m_NextSequenceId.m_Value)) {
        return;
    }
    if (m_Packets.find(pPacket->m_SequenceId) != m_Packets.end()) {
        return;
    }
    pPacket->AcquireRef();
    m_Packets.IndexInline(SequenceId(pPacket->m_SequenceId.m_Value)) = pPacket;
}

} // namespace nex
} // namespace nn

namespace __rw {
// the functions of the tree of the packets
// 0x007F9E5C (name is ours)
template __rb_tree<nn::nex::LogicalClockTmpl<u16, s16>, std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketIn*>, __select1st<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketIn*>, nn::nex::LogicalClockTmpl<u16, s16> >, std::less<nn::nex::LogicalClockTmpl<u16, s16> >, nn::nex::MemAllocator<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketIn*> > >::iterator __rb_tree<nn::nex::LogicalClockTmpl<u16, s16>, std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketIn*>, __select1st<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketIn*>, nn::nex::LogicalClockTmpl<u16, s16> >, std::less<nn::nex::LogicalClockTmpl<u16, s16> >, nn::nex::MemAllocator<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketIn*> > >::erase(iterator it);
// 0x007FA630 (name is ours)
template __rb_tree<nn::nex::LogicalClockTmpl<u16, s16>, std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketIn*>, __select1st<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketIn*>, nn::nex::LogicalClockTmpl<u16, s16> >, std::less<nn::nex::LogicalClockTmpl<u16, s16> >, nn::nex::MemAllocator<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketIn*> > >::iterator __rb_tree<nn::nex::LogicalClockTmpl<u16, s16>, std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketIn*>, __select1st<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketIn*>, nn::nex::LogicalClockTmpl<u16, s16> >, std::less<nn::nex::LogicalClockTmpl<u16, s16> >, nn::nex::MemAllocator<std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketIn*> > >::_C_insert(Node* pNode, Node* pParent, const std::pair<const nn::nex::LogicalClockTmpl<u16, s16>, nn::nex::PacketIn*>& value);
} // namespace __rw

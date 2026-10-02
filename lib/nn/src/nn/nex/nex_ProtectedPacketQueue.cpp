#include "nn/nex/nex_PacketQueue.h"
#include "nn/nex/nex_ProtectedPacketQueue.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::ProtectedPacketQueue::ProtectedPacketQueue()
{
}

// 0x00397CDC slot 0x00 | virtual slot, introduced by nn::nex::PacketQueue
void nn::nex::ProtectedPacketQueue::vf_0x00()
{
}

// 0x00397C60 slot 0x04 | fefates:callseq
nn::nex::ProtectedPacketQueue::~ProtectedPacketQueue()
{
}

// 0x00397B88 slot 0x08 | slot vf_0x08 of nn::nex::PacketQueue
void nn::nex::ProtectedPacketQueue::Purge()
{
}

// 0x00397BC0 slot 0x0C | slot vf_0x0C of nn::nex::PacketQueue
void nn::nex::ProtectedPacketQueue::Queue(nn::nex::Packet*, bool)
{
}

// 0x00397B40 slot 0x10 | slot vf_0x10 of nn::nex::PacketQueue
void nn::nex::ProtectedPacketQueue::QueueFront(nn::nex::Packet*, bool)
{
}

// 0x00397C08 slot 0x14 | slot vf_0x14 of nn::nex::PacketQueue (was Dequeue)
void nn::nex::ProtectedPacketQueue::vf_0x14()
{
}

// 0x00397C58 slot 0x18 | slot vf_0x18 of nn::nex::PacketQueue
void nn::nex::ProtectedPacketQueue::GetLock()
{
}

} // namespace nex
} // namespace nn

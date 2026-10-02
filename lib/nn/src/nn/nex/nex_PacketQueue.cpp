#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_PacketQueue.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::PacketQueue::PacketQueue()
{
}

// 0x0035B610 slot 0x00 | virtual slot, introduced by nn::nex::PacketQueue
void nn::nex::PacketQueue::vf_0x00()
{
}

// 0x0035B5D0 slot 0x04 | fefates:bytes
nn::nex::PacketQueue::~PacketQueue()
{
}

// 0x0035B408 slot 0x08 | mk7dlp:bytes
void nn::nex::PacketQueue::Purge()
{
}

// 0x0035B454 slot 0x0C | slot vf_0x0C of nn::nex::PacketQueue
void nn::nex::PacketQueue::Queue(nn::nex::Packet*, bool)
{
}

// 0x0035B344 slot 0x10 | fefates:bytes
void nn::nex::PacketQueue::QueueFront(nn::nex::Packet*, bool)
{
}

// 0x0035B514 slot 0x14 | fefates:bytes (was Dequeue)
void nn::nex::PacketQueue::vf_0x14()
{
}

// 0x0035B5C8 slot 0x18 | slot vf_0x18 of nn::nex::PacketQueue
void nn::nex::PacketQueue::GetLock()
{
}

} // namespace nex
} // namespace nn

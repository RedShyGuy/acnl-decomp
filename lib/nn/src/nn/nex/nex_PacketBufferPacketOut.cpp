#include "nn/nex/nex_PacketBufferPacketOut.h"
#include "nn/nex/nex_PacketBufferManager.h"

#include <new>

namespace nn {
namespace nex {
// 0x0039A6E4 (name is ours)
nn::nex::PacketBufferPacketOut::PacketBufferPacketOut(nn::nex::PRUDPEndPoint* pEndPoint, nn::nex::PacketType type, unsigned short flags, nn::nex::Buffer* pPayload, unsigned int value)
{
    Timeout* pTimeout = nullptr;
    if ((flags & FLAG_NEED_ACK) != 0) {
        void* pMemory = PacketBufferManager::AllocateObject(sizeof(Timeout), false);
        if (pMemory != nullptr) {
            pTimeout = ::new (pMemory) Timeout();
        }
    }
    Initialize(pEndPoint, type, flags, pPayload, value, pTimeout);
}

// 0x003D7880 slot 0x00
// 0x0039A7C0 (deleting dtor)
nn::nex::PacketBufferPacketOut::~PacketBufferPacketOut()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

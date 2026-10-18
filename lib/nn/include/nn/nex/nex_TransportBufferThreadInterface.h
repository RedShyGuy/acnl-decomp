#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
class QueuingSocket;

// The threads that send and fill the transport buffer sets of the sockets of a transport. The
// names of the functions without a reference name are ours.
// RTTI N2nn3nex30TransportBufferThreadInterfaceE @ 0x008CF1E8
class TransportBufferThreadInterface : public ::nn::nex::RootObject
{
public:
    // (inline)
    TransportBufferThreadInterface() {}
    virtual ~TransportBufferThreadInterface() {} // slot 0x00
    // slot 0x04 (deleting dtor)
    virtual bool Start() = 0; // slot 0x08
    virtual void Stop() = 0; // slot 0x0C
    virtual bool InsertSocketPort(unsigned short port, nn::nex::QueuingSocket* pSocket) = 0; // slot 0x10
    virtual void EraseSocketPort(unsigned short port) = 0; // slot 0x14
    virtual void SetPriority(int priority) = 0; // slot 0x18
    virtual int GetPriority() = 0; // slot 0x1C
};
} // namespace nex
} // namespace nn

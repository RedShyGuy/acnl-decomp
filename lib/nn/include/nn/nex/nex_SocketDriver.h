#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12SocketDriverE @ 0x008CE1B8
class SocketDriver : public ::nn::nex::RootObject
{
public:
    class Socket;
    struct InetAddress { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct PollInfo { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct _SocketFlag { u32 _unknown; }; // TODO: real type unknown (placeholder)
    struct _TrafficType { u32 _unknown; }; // TODO: real type unknown (placeholder)
    SocketDriver(); // ctor address unknown
};
} // namespace nex
} // namespace nn

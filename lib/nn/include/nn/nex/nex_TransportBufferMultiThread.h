#pragma once

#include "decomp.h"
#include "nn/nex/nex_TransportBufferThreadInterface.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex26TransportBufferMultiThreadE @ 0x008CEF94
// vtable 0x008FEE38 (vptr 0x008FEE40), offset_to_top 0, 8 entries
class TransportBufferMultiThread : public ::nn::nex::TransportBufferThreadInterface
{
public:
    TransportBufferMultiThread(); // ctor address unknown
    virtual void vf_0x00(); // 0x003BB79C slot 0x00 | fefates:callseq
    virtual ~TransportBufferMultiThread(); // 0x003BB76C slot 0x04 | slot vf_0x04 of nn::nex::TransportBufferMultiThread
    virtual void Start(); // 0x003BB244 slot 0x08 | slot vf_0x08 of nn::nex::TransportBufferMultiThread
    virtual void vf_0x0C(); // 0x003BB148 slot 0x0C | fefates:callseq
    virtual void InsertSocketPort(unsigned short, nn::nex::QueuingSocket*); // 0x003BAA6C slot 0x10 | slot vf_0x10 of nn::nex::TransportBufferMultiThread
    virtual void EraseSocketPort(unsigned short); // 0x003BA81C slot 0x14 | slot vf_0x14 of nn::nex::TransportBufferMultiThread
    virtual void vf_0x18(); // 0x003BA788 slot 0x18 | virtual slot, introduced by nn::nex::TransportBufferMultiThread
    virtual void vf_0x1C(); // 0x0072DDDC slot 0x1C | virtual slot, introduced by nn::nex::TransportBufferMultiThread
};
} // namespace nex
} // namespace nn

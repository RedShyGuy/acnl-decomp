#pragma once

#include "decomp.h"
#include "nn/nex/nex_SocketDriver.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex20BerkeleySocketDriverE @ 0x008CE850
// vtable 0x008FD7C0 (vptr 0x008FD7C8), offset_to_top 0, 5 entries
class BerkeleySocketDriver : public ::nn::nex::SocketDriver
{
public:
    class BerkeleySocket;
    BerkeleySocketDriver(); // ctor address unknown
    virtual ~BerkeleySocketDriver(); // 0x00396214 slot 0x00 | slot vf_0x00 of nn::nex::BerkeleySocketDriver
    virtual void vf_0x04(); // 0x003961FC slot 0x04 | virtual slot, introduced by nn::nex::BerkeleySocketDriver
    virtual void vf_0x08(); // 0x0039615C slot 0x08 | fefates:callseq
    virtual void Delete(nn::nex::SocketDriver::Socket*); // 0x003961E4 slot 0x0C | fefates:bytes
    virtual void Poll(nn::nex::SocketDriver::PollInfo*, unsigned int, unsigned int); // 0x00395FDC slot 0x10 | fefates:bytes-fuzzy
};
} // namespace nex
} // namespace nn

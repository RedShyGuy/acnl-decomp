#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex28MessagingNotificationHandlerE @ 0x008CF0A8
// vtable 0x008FF084 (vptr 0x008FF08C), offset_to_top 0, 3 entries
class MessagingNotificationHandler : public ::nn::nex::RootObject
{
public:
    MessagingNotificationHandler(); // ctor candidate(s) 0x003BDDE0 (unverified)
    virtual void vf_0x00(); // 0x003BDDF8 slot 0x00 | virtual slot, introduced by nn::nex::MessagingNotificationHandler
    virtual void vf_0x04(); // 0x003BDDF0 slot 0x04 | virtual slot, introduced by nn::nex::MessagingNotificationHandler
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
};
} // namespace nex
} // namespace nn

#pragma once

#include "decomp.h"
#include "nn/nex/nex_NgsBridgeInterface.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex12NgsBridgeImpE @ 0x008CE108
// vtable 0x008FC56C (vptr 0x008FC574), offset_to_top 0, 9 entries
class NgsBridgeImp : public ::nn::nex::NgsBridgeInterface
{
public:
    NgsBridgeImp(); // ctor address unknown
    virtual void vf_0x00(); // 0x0035D5C8 slot 0x00 | virtual slot, introduced by nn::nex::NgsBridgeImp
    virtual void vf_0x04(); // 0x0035D5C4 slot 0x04 | virtual slot, introduced by nn::nex::NgsBridgeImp
    virtual void vf_0x08(); // 0x0035D5A0 slot 0x08 | virtual slot, introduced by nn::nex::NgsBridgeImp
    virtual void vf_0x0C(); // 0x0035D5AC slot 0x0C | virtual slot, introduced by nn::nex::NgsBridgeImp
    virtual void RegisterNotificationEventHandler(nn::nex::NotificationEventHandler*); // 0x00376C18 slot 0x10 | slot vf_0x10 of nn::nex::NgsBridgeImp
    virtual void UnregisterNotificationEventHandler(nn::nex::NotificationEventHandler*); // 0x00376C34 slot 0x14 | slot vf_0x14 of nn::nex::NgsBridgeImp
    virtual void vf_0x18(); // 0x0035D518 slot 0x18 | fefates:callseq
    virtual void vf_0x1C(); // 0x0035D55C slot 0x1C | fefates:callseq
    virtual void vf_0x20(); // 0x0072B460 slot 0x20 | virtual slot, introduced by nn::nex::NgsBridgeImp
};
} // namespace nex
} // namespace nn

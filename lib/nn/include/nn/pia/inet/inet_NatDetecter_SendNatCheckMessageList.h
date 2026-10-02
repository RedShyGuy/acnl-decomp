#pragma once

#include "decomp.h"
#include "nn/pia/common/common_FixedObjList.h"
#include "nn/pia/inet/inet_NatDetecter.h"

// RTTI N2nn3pia4inet11NatDetecter23SendNatCheckMessageListE @ 0x008CF800
// vtable 0x008FFE5C (vptr 0x008FFE64), offset_to_top 0, 2 entries
class nn::pia::inet::NatDetecter::SendNatCheckMessageList : public ::nn::pia::common::FixedObjList<nn::pia::inet::NatDetecter::SendNatCheckMessage, 20u>
{
public:
    SendNatCheckMessageList(); // ctor address unknown
    virtual void vf_0x00(); // 0x003E35E0 slot 0x00 | virtual slot, introduced by nn::pia::inet::NatDetecter::SendNatCheckMessageList
    virtual void vf_0x04(); // 0x003E35DC slot 0x04 | virtual slot, introduced by nn::pia::inet::NatDetecter::SendNatCheckMessageList
};

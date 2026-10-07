#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalMessage.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

// RTTI N2nn3pia5local31LocalAroundNetworkSearchManager38LocalAroundNetworkSearchCommandMessageE @ 0x008CFD84
// vtable 0x00901388 (vptr 0x00901390), offset_to_top 0, 4 entries
//
// The host starts (type 20) or stops (type 21) the search of the networks around; the value is the
// version of the command. The header has the value
// after the header of LocalMessage (the constructor is inline); the member name is ours.
class nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkSearchCommandMessage : public ::nn::pia::local::LocalMessage
{
public:
    static const u16 VALUE_HEADER_SIZE = 16;

    LocalAroundNetworkSearchCommandMessage(u8* pBuffer, u32 bufferSize, u8 type, u32 value) : LocalMessage(pBuffer, bufferSize)
    {
        m_Type = type;
        m_HeaderSize = VALUE_HEADER_SIZE;
        m_Value = value;
    }
    virtual ~LocalAroundNetworkSearchCommandMessage(); // 0x0042412C slot 0x00
    // 0x00424128 slot 0x04 (deleting dtor)
    virtual void UpdateMessageHeader(); // 0x0042410C slot 0x08
    virtual bool ParseMessageHeader(); // 0x004240E4 slot 0x0C

    u32 m_Value; // 0x14
};
ASSERT_SIZE(nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkSearchCommandMessage, 0x18);

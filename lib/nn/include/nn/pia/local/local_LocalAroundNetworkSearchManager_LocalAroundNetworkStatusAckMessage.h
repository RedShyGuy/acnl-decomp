#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalMessage.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

// RTTI N2nn3pia5local31LocalAroundNetworkSearchManager34LocalAroundNetworkStatusAckMessageE @ 0x008CFD78
// vtable 0x00901370 (vptr 0x00901378), offset_to_top 0, 4 entries
//
// The answer to a LocalAroundNetworkStatusMessage (type 38) with its version. The header has the value
// after the header of LocalMessage (the constructor is inline); the member name is ours.
class nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusAckMessage : public ::nn::pia::local::LocalMessage
{
public:
    static const u16 VALUE_HEADER_SIZE = 16;

    LocalAroundNetworkStatusAckMessage(u8* pBuffer, u32 bufferSize, u32 value) : LocalMessage(pBuffer, bufferSize)
    {
        m_Type = LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_38;
        m_HeaderSize = VALUE_HEADER_SIZE;
        m_Value = value;
    }
    virtual ~LocalAroundNetworkStatusAckMessage(); // 0x00423E88 slot 0x00
    // 0x00423E84 slot 0x04 (deleting dtor)
    virtual void UpdateMessageHeader(); // 0x00423E68 slot 0x08
    virtual bool ParseMessageHeader(); // 0x00423E40 slot 0x0C

    u32 m_Value; // 0x14
};
ASSERT_SIZE(nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusAckMessage, 0x18);

#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalMessage.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

// RTTI N2nn3pia5local31LocalAroundNetworkSearchManager31LocalAroundNetworkStatusMessageE @ 0x008CFD6C
// vtable 0x00901358 (vptr 0x00901360), offset_to_top 0, 4 entries
//
// A status of the search of the networks around (type 22); the value is the version of the status. The header has the value
// after the header of LocalMessage (the constructor is inline); the member name is ours.
class nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage : public ::nn::pia::local::LocalMessage
{
public:
    static const u16 VALUE_HEADER_SIZE = 16;

    LocalAroundNetworkStatusMessage(u8* pBuffer, u32 bufferSize, u32 value) : LocalMessage(pBuffer, bufferSize)
    {
        m_Type = LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_22;
        m_HeaderSize = VALUE_HEADER_SIZE;
        m_Value = value;
    }
    virtual ~LocalAroundNetworkStatusMessage(); // 0x00423CEC slot 0x00
    // 0x00423CE8 slot 0x04 (deleting dtor)
    virtual void UpdateMessageHeader(); // 0x00423CCC slot 0x08
    virtual bool ParseMessageHeader(); // 0x00423CA4 slot 0x0C

    u32 m_Value; // 0x14
};
ASSERT_SIZE(nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkStatusMessage, 0x18);

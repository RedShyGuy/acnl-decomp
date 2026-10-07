#pragma once

#include "decomp.h"
#include "nn/pia/local/local_LocalAroundNetworkSearchManager.h"
#include "nn/pia/local/local_LocalMessage.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

// RTTI N2nn3pia5local31LocalAroundNetworkSearchManager41LocalAroundNetworkSearchCommandAckMessageE @ 0x008CFD90
// vtable 0x009013A0 (vptr 0x009013A8), offset_to_top 0, 4 entries
//
// The answer to a LocalAroundNetworkSearchCommandMessage (type 36) with its version. The header has the value
// after the header of LocalMessage (the constructor is inline); the member name is ours.
class nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkSearchCommandAckMessage : public ::nn::pia::local::LocalMessage
{
public:
    static const u16 VALUE_HEADER_SIZE = 16;

    LocalAroundNetworkSearchCommandAckMessage(u8* pBuffer, u32 bufferSize, u32 value) : LocalMessage(pBuffer, bufferSize)
    {
        m_Type = LocalNetworkManager::MESSAGE_TYPE_AROUND_NETWORK_SEARCH_36;
        m_HeaderSize = VALUE_HEADER_SIZE;
        m_Value = value;
    }
    virtual ~LocalAroundNetworkSearchCommandAckMessage(); // 0x004243C8 slot 0x00
    // 0x004243C4 slot 0x04 (deleting dtor)
    virtual void UpdateMessageHeader(); // 0x004243A8 slot 0x08
    virtual bool ParseMessageHeader(); // 0x00424380 slot 0x0C

    u32 m_Value; // 0x14
};
ASSERT_SIZE(nn::pia::local::LocalAroundNetworkSearchManager::LocalAroundNetworkSearchCommandAckMessage, 0x18);

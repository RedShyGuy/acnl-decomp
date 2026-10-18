#pragma once

#include "decomp.h"
#include "nn/nex/nex_ServiceClient.h"
#include "nn/nex/nex_UtilityProtocolClient.h"

namespace nn {
namespace nex {
class ProtocolCallContext;

// The client of the utility protocol. The names of the members and of the functions without a
// reference name are ours.
// RTTI N2nn3nex13UtilityClientE @ 0x008CE2B4
// vtable 0x008FC9F0 (vptr 0x008FC9F8), offset_to_top 0, 10 entries
class UtilityClient : public ::nn::nex::ServiceClient
{
public:
    // the id of the utility protocol (and of the client)
    static const u16 PROTOCOL_ID = 110;

    UtilityClient(); // 0x0036F368
    virtual ~UtilityClient(); // 0x0036F41C slot 0x00
    // 0x0036F3B8 slot 0x04 (deleting dtor)
    virtual bool Bind(nn::nex::Credentials* pCredentials); // 0x0036F2FC slot 0x0C
    virtual bool Unbind(); // 0x0036F330 slot 0x10
    virtual void UpdateProtocolsDefaultCredentials(nn::nex::Credentials* pCredentials); // 0x00370DCC slot 0x20

    // the integer settings of the index (key -> value)
    bool GetIntegerSettings(nn::nex::ProtocolCallContext* pContext, unsigned int index, nn::nex::qMap<u16, u32>* pSettings); // 0x0036F2E4

    bool m_IsProtocolRegistered;      // 0x0D
    UtilityProtocolClient m_Protocol; // 0x10
};
ASSERT_OFFSET(UtilityClient, m_IsProtocolRegistered, 0x0D);
ASSERT_OFFSET(UtilityClient, m_Protocol, 0x10);
ASSERT_SIZE(UtilityClient, 0x54);
} // namespace nex
} // namespace nn

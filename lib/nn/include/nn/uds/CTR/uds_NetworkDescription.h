#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/uds/CTR/uds_Types.h"

namespace nn {
namespace uds {
namespace CTR {

// a network as the host creates it or a scan finds it (member names are ours)
class NetworkDescription
{
public:
    // from a received beacon
    void Initialize(const nn::uds::CTR::detail::NetworkDescriptionElement* element, u8 channel,
                    const u8* bssid); // 0x0046883C | fefates:bytes [tier B]
    // for a new network (CreateNetwork); applicationData may be NULL when applicationDataSize is 0
    nn::Result Initialize(u32 localCommunicationId, u8 subId, u8 nodeCountMax, u8 channel,
                          const void* applicationData, size_t applicationDataSize); // 0x00468878 | fefates:bytes [tier B]
    // returns the size copied, 0 when the buffer is too small
    size_t GetApplicationData(u8* buffer, size_t size) const; // 0x00737134 | fefates:bytes [tier B]

    u8 m_Bssid[6];                                          // 0x000
    u16 m_Channel;                                          // 0x006
    bool m_IsInitialized;                                   // 0x008
    u8 m_Padding[3];                                        // 0x009
    nn::uds::CTR::detail::NetworkDescriptionElement m_Element;  // 0x00C
};
ASSERT_SIZE(NetworkDescription, 0x108);

} // namespace CTR
} // namespace uds
} // namespace nn

#include "nn/uds/CTR/uds_NetworkDescription.h"
#include "nn/nstd/nstd_String.h"
#include "nn/uds/CTR/uds_Result.h"

#include <string.h>

namespace nn {
namespace uds {
namespace CTR {
namespace {

// the tag of the network element in a beacon: vendor specific, Nintendo's OUI 00:1F:32, type 0x15
const u8 NINTENDO_OUI[3] = { 0x00, 0x1F, 0x32 }; // 0x008B46F2
const u8 OUI_TYPE_NETWORK = 0x15;

const size_t APPLICATION_DATA_SIZE_MAX = 200;

} // namespace

// 0x0046883C | fefates:bytes [tier B]
void nn::uds::CTR::NetworkDescription::Initialize(const nn::uds::CTR::detail::NetworkDescriptionElement* element,
                                                 u8 channel, const u8* bssid)
{
    nnnstdMemCpy(&m_Element, element, sizeof(m_Element));
    nnnstdMemCpy(m_Bssid, bssid, sizeof(m_Bssid));
    m_Channel = channel;
    m_IsInitialized = true;
}

// 0x00468878 | fefates:bytes [tier B]
nn::Result nn::uds::CTR::NetworkDescription::Initialize(u32 localCommunicationId, u8 subId, u8 nodeCountMax,
                                                       u8 channel, const void* applicationData,
                                                       size_t applicationDataSize)
{
    if (applicationData != NULL) {
        if (applicationDataSize > APPLICATION_DATA_SIZE_MAX) {
            return nn::Result(RESULT_TOO_LARGE);
        }
    } else if (applicationDataSize != 0) {
        return nn::Result(RESULT_INVALID_POINTER);
    }

    nnnstdMemCpy(m_Element.oui, NINTENDO_OUI, sizeof(m_Element.oui));
    m_Element.subId = subId;
    m_Element.nodeCountMax = nodeCountMax;
    m_Element.ouiType = OUI_TYPE_NETWORK;
    m_Element.localCommunicationId = __builtin_bswap32(localCommunicationId);
    m_Element.attribute = 0x0080;       // big endian 0x8000
    m_Element.applicationDataSize = applicationDataSize;
    m_Element.unknown13 = 1;
    m_Element.networkId = 0;
    m_Element.nodeCount = 0;
    m_Element.unknown12 = 0;
    memset(m_Element.applicationData, 0, sizeof(m_Element.applicationData));
    nnnstdMemCpy(m_Element.applicationData, applicationData, applicationDataSize);
    m_Channel = channel;
    m_IsInitialized = true;
    return nn::Result();
}

// 0x00737134 | fefates:bytes [tier B]
size_t nn::uds::CTR::NetworkDescription::GetApplicationData(u8* buffer, size_t size) const
{
    if (m_Element.applicationDataSize > size) {
        return 0;
    }
    nnnstdMemCpy(buffer, m_Element.applicationData, m_Element.applicationDataSize);
    return m_Element.applicationDataSize;
}

} // namespace CTR
} // namespace uds
} // namespace nn

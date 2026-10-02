#include "nn/uds/CTR/uds_NetworkDescriptionReaderInternal.h"
#include "nn/crypto/crypto_Api.h"
#include "nn/uds/CTR/detail/detail_Api.h"

#include <stddef.h>
#include <string.h>

namespace nn {
namespace uds {
namespace CTR {
namespace {

const u8 NINTENDO_OUI[3] = { 0x00, 0x1F, 0x32 }; // 0x008C2404

// types of Nintendo's vendor tags in a beacon (3dbrew "UDS beacon")
const u8 ELEMENT_TYPE_NETWORK = 0x15;
const u8 ELEMENT_TYPE_NODES_1 = 0x18;
const u8 ELEMENT_TYPE_NODES_2 = 0x19;

// a tag starts with its id and length
const size_t TAG_HEADER_SIZE = 2;

} // namespace

inline const u8* nn::uds::CTR::NetworkDescriptionReaderInternal::GetElementData(u8 type)
{
    const u8* element = GetVendorSpecificElement(NINTENDO_OUI, type);
    return element != NULL ? element + TAG_HEADER_SIZE : NULL;
}

// 0x00468BF4 | fefates:bytes [tier B]
nn::Result nn::uds::CTR::NetworkDescriptionReaderInternal::GetNetworkDescription(nn::uds::CTR::NetworkDescription* network)
{
    detail::NetworkDescriptionElement* element =
        reinterpret_cast<detail::NetworkDescriptionElement*>(const_cast<u8*>(GetElementData(ELEMENT_TYPE_NETWORK)));
    if (element != NULL) {
        // the hash is calculated with the hash field cleared
        u8 hash[sizeof(element->hash)];
        memcpy(hash, element->hash, sizeof(hash));
        memset(element->hash, 0, sizeof(element->hash));
        u8 calculated[sizeof(element->hash)];
        nn::crypto::CalculateSha1(calculated, element,
                                  offsetof(detail::NetworkDescriptionElement, applicationData) + element->applicationDataSize);
        memcpy(element->hash, hash, sizeof(hash));
        if (memcmp(hash, calculated, sizeof(hash)) == 0) {
            nn::nwm::Mac bssid = GetBssid();
            network->Initialize(element, GetChannel(), bssid.address);
            return nn::Result();
        }
    }
    network->m_IsInitialized = false;
    memset(network->m_Bssid, 0, sizeof(network->m_Bssid));
    return nn::Result(RESULT_BEACON_WITHOUT_NETWORK);
}

// 0x00468D14 | fefates:bytes [tier B]
nn::Result nn::uds::CTR::NetworkDescriptionReaderInternal::GetNodeInformationList(nn::uds::CTR::detail::NodeInformationRaw* nodes)
{
    const u8* data = GetElementData(ELEMENT_TYPE_NETWORK);
    if (data == NULL) {
        return nn::Result(RESULT_BEACON_WITHOUT_NODES);
    }
    nn::nwm::Mac bssid = GetBssid();
    detail::s_ReaderNetworkDescription.Initialize(reinterpret_cast<const detail::NetworkDescriptionElement*>(data),
                                                  GetChannel(), bssid.address);

    data = GetElementData(ELEMENT_TYPE_NODES_1);
    if (data == NULL) {
        memset(nodes, 0, sizeof(detail::NodeInformationList));
        return nn::Result();
    }
    memcpy(&detail::s_ReaderNodeInformationElements[0], data, sizeof(detail::NodeInformationElement));
    data = GetElementData(ELEMENT_TYPE_NODES_2);
    if (data != NULL) {
        memcpy(&detail::s_ReaderNodeInformationElements[1], data, sizeof(detail::NodeInformationElement));
    } else {
        memset(&detail::s_ReaderNodeInformationElements[1], 0, sizeof(detail::NodeInformationElement));
    }

    nn::Result result = detail::GetUds().GetNodeInformationList2(
        reinterpret_cast<detail::NodeInformationList*>(nodes), detail::s_ReaderNetworkDescription,
        detail::s_ReaderNodeInformationElements[0], detail::s_ReaderNodeInformationElements[1]);
    detail::ThrowIfSessionClosed(result);
    // the names always end
    for (s32 i = 0; i < detail::NODE_MAX; i++) {
        nodes[i].userName.name[10] = 0;
    }
    return result;
}

} // namespace CTR
} // namespace uds
} // namespace nn

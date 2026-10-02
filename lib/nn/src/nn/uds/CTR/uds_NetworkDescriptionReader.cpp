#include "nn/uds/CTR/uds_NetworkDescriptionReader.h"
#include "nn/uds/CTR/detail/detail_Api.h"
#include "nn/uds/CTR/uds_NetworkDescriptionReaderInternal.h"

namespace nn {
namespace uds {
namespace CTR {

// 0x00468AF8 slot 0x00
nn::uds::CTR::NetworkDescriptionReader::~NetworkDescriptionReader()
{
}

// 0x004689FC | fefates:bytes [tier B]
nn::Result nn::uds::CTR::NetworkDescriptionReader::GetNetworkDescription(nn::uds::CTR::NetworkDescription* network)
{
    NetworkDescriptionReaderInternal reader(m_pBss);
    return reader.GetNetworkDescription(network);
}

// 0x00468A48 | fefates:bytes [tier B]
nn::Result nn::uds::CTR::NetworkDescriptionReader::GetNodeInformationList(nn::uds::CTR::NodeInformation* nodes)
{
    NetworkDescriptionReaderInternal reader(m_pBss);
    nn::Result result = reader.GetNodeInformationList(detail::s_NodeInformationList.nodes);
    if (result.IsSuccess()) {
        for (s32 i = 0; i < detail::NODE_MAX; i++) {
            const detail::NodeInformationRaw& raw = detail::s_NodeInformationList.nodes[i];
            nodes[i].nodeId = raw.nodeId;
            nodes[i].userName = raw.userName;
            detail::ScrambleLocalFriendCode(&nodes[i].scrambledLocalFriendCode, raw.localFriendCodeSeed, raw.nodeId);
        }
    }
    return result;
}

// 0x0046898C (name is ours)
nn::Result nn::uds::CTR::NetworkDescriptionReader::GetLinkLevel(u8* level)
{
    NetworkDescriptionReaderInternal reader(m_pBss);
    s16 strength = reader.GetSignalStrength();
    if (strength < 16) {
        *level = 0;
    } else if (strength < 19) {
        *level = 1;
    } else if (strength < 24) {
        *level = 2;
    } else {
        *level = 3;
    }
    return nn::Result();
}

} // namespace CTR
} // namespace uds
} // namespace nn

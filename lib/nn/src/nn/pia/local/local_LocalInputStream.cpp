#include "nn/pia/local/local_LocalInputStream.h"
#include "nn/pia/common/common_Packet.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_StationAddress.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"
#include "nn/pia/transport/transport_Transport.h"

namespace nn {
namespace pia {
namespace local {
namespace {
// the data of a packet at most (LocalNetworkManager::SendTo)
const u32 PACKET_SIZE_MAX = 1466;
} // namespace

// 0x00416A50 | fefates:bytes-fuzzy
// 0x00416A48 (thunk)
nn::Result nn::pia::local::LocalInputStream::Read(nn::pia::common::Packet* pPacket)
{
    LocalNetwork::s_pInstance->m_pNetworkManager->SetReceiveTime(transport::Transport::s_pInstance->m_DispatchTime);
    u8 transportId;
    nn::Result result = LocalNetwork::s_pInstance->m_pNetworkManager->ReceiveFrom(pPacket, PACKET_SIZE_MAX, &pPacket->m_Size, &transportId, false);
    if (result.IsFailure()) {
        if (result == common::RESULT_NO_DATA) {
            return result;
        }
        if (result == common::RESULT_LOCAL_NETWORK_LOST) {
            // the system ended the connection
            if (LocalNetwork::s_pInstance->m_pNetworkManager->IsDisconnectedByRequestFromSystem()) {
                return common::RESULT_SOCKET_UNAVAILABLE;
            }
            return common::RESULT_NO_DATA;
        }
        if (result == common::RESULT_INVALID_STATE_103) {
            return common::RESULT_NO_DATA;
        }
        return result;
    }
    common::StationAddress address;
    address.SetExtensionId(transportId);
    pPacket->m_SourceStationAddress = address;
    return nn::Result();
}

// 0x00416B68 | fefates:bytes [tier B]
nn::pia::local::LocalInputStream::LocalInputStream()
{
    // only the bases and the vptrs (in the original too)
}

// 0x00416A40
// 0x00416B88 (deleting dtor)
// 0x00774AC4 (thunk)
// 0x00774AB0 (deleting thunk)
nn::pia::local::LocalInputStream::~LocalInputStream()
{
    // empty (in the original too)
}

// 0x00730248
void nn::pia::local::LocalInputStream::vf_0x08()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn

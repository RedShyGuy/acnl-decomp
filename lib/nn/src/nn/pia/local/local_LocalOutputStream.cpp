#include "nn/pia/local/local_LocalOutputStream.h"
#include "nn/pia/common/common_Packet.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalNetwork.h"
#include "nn/pia/local/local_LocalNetworkManager.h"

namespace nn {
namespace pia {
namespace local {
namespace {
// the station index of a packet to all
const u8 STATION_INDEX_ALL = 0xFF;
} // namespace

// 0x00416BE4
// 0x00774ACC (thunk)
nn::Result nn::pia::local::LocalOutputStream::OnStationConnectionEvent()
{
    return nn::Result();
}

// 0x00416BF4 | fefates:bytes
// 0x00416BEC (thunk)
nn::Result nn::pia::local::LocalOutputStream::Write(const nn::pia::common::Packet& packet)
{
    u8 transportId;
    if (packet.m_DestinationStationIndex == STATION_INDEX_ALL) {
        transportId = LocalNetworkManager::TRANSPORT_ID_ALL;
    } else {
        transportId = static_cast<u8>(packet.m_DestinationStationAddress.m_ExtensionId);
    }
    u32 size = packet.m_Size;
    LocalNetwork::s_pInstance->SleepBeforeSend();
    nn::Result result = LocalNetwork::s_pInstance->m_pNetworkManager->SendTo(&packet, size, transportId);
    if (result.IsFailure()) {
        if (result == common::RESULT_INVALID_ARGUMENT || result == common::RESULT_INVALID_STATE) {
            return result;
        }
        // the station or the network is gone: no error of the stream
        if (result == common::RESULT_LOCAL_DESTINATION_NOT_FOUND || result == common::RESULT_INVALID_STATE_103 || result == common::RESULT_LOCAL_NETWORK_LOST) {
            return nn::Result();
        }
    }
    return result;
}

// 0x00416C98 | fefates:bytes [tier B]
nn::pia::local::LocalOutputStream::LocalOutputStream()
{
    // only the bases and the vptrs (in the original too)
}

// 0x00416CC8
// 0x00416CB8 (deleting dtor)
// 0x00774AE8 (thunk)
// 0x00774AD4 (deleting thunk)
nn::pia::local::LocalOutputStream::~LocalOutputStream()
{
    // empty (in the original too)
}

// 0x00730298
// 0x00774AF0 (thunk)
bool nn::pia::local::LocalOutputStream::IsBroadcast()
{
    return true;
}

// 0x007302A0
// 0x00774AF8 (thunk)
u32 nn::pia::local::LocalOutputStream::GetDestinationNumMax()
{
    return 1;
}

// 0x007302A8
void nn::pia::local::LocalOutputStream::vf_0x08()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn

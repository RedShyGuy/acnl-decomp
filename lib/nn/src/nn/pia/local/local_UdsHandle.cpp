#include "nn/pia/local/local_UdsHandle.h"
#include "nn/uds/CTR/CTR_Api.h"

namespace nn {
namespace pia {
namespace local {
namespace {
// the receive endpoint takes the data of all nodes
const u16 NODE_ID_ANY = 0xFFFF;
} // namespace

// 0x00425E80 | fefates:bytes [tier B]
nn::Result nn::pia::local::UdsHandle::CreateHandle(u16 dataChannel)
{
    nn::Result result = nn::uds::CTR::CreateEndpoint(&m_SendEndpoint);
    if (result.IsFailure()) {
        return result;
    }
    result = nn::uds::CTR::CreateEndpoint(&m_ReceiveEndpoint);
    if (result.IsFailure()) {
        nn::uds::CTR::DestroyEndpoint(&m_SendEndpoint);
        return result;
    }
    m_DataChannel = dataChannel;
    result = nn::uds::CTR::Attach(&m_ReceiveEndpoint, NODE_ID_ANY, dataChannel, RECEIVE_BUFFER_SIZE);
    if (result.IsFailure()) {
        nn::uds::CTR::DestroyEndpoint(&m_ReceiveEndpoint);
        nn::uds::CTR::DestroyEndpoint(&m_SendEndpoint);
        return result;
    }
    m_IsCreated = true;
    return nn::Result();
}

// 0x00425F18 | fefates:bytes [tier B]
nn::Result nn::pia::local::UdsHandle::DestroyHandle()
{
    m_IsCreated = false;
    nn::Result result = nn::uds::CTR::DestroyEndpoint(&m_ReceiveEndpoint);
    if (result.IsFailure()) {
        nn::uds::CTR::DestroyEndpoint(&m_SendEndpoint);
        return result;
    }
    result = nn::uds::CTR::DestroyEndpoint(&m_SendEndpoint);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x00425F64 (symbols.json: nn::nex::UdsHandle, wrong)
nn::pia::local::UdsHandle::UdsHandle() : m_DataChannel(0), m_IsCreated(false)
{
}

// 0x00425F84
// 0x00425F80 (deleting dtor)
nn::pia::local::UdsHandle::~UdsHandle()
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn

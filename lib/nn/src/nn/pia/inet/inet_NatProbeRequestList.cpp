#include "nn/pia/inet/inet_NatProbeRequestList.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003F8A70 | fefates:bytes [tier B]
nn::pia::inet::NatProbeRequestList::NatProbeRequestList() : m_pNodeBuffer(nullptr)
{
    m_pNodeBuffer = common::NewArray<u8>(REQUEST_NUM_MAX * sizeof(Node));
    if (m_pNodeBuffer != nullptr) {
        Initialize(reinterpret_cast<Node*>(m_pNodeBuffer), REQUEST_NUM_MAX);
    }
    Clear();
}

// 0x003F8C5C | fefates:callgraph
// 0x003F8BA4 (deleting dtor)
nn::pia::inet::NatProbeRequestList::~NatProbeRequestList()
{
    if (!common::IsValidPointer(m_pNodeBuffer)) {
        return;
    }
    Clear();
    if (m_pNodeBuffer != nullptr) {
        common::DeleteArray(m_pNodeBuffer);
    }
    m_pNodeBuffer = nullptr;
}

} // namespace inet
} // namespace pia
} // namespace nn

#include "nn/pia/inet/inet_NatProbeList.h"
#include "nn/pia/common/common_NewArray.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/common/common_TimeSpan.h"
#include "nn/pia/transport/transport_StationLocation.h"

namespace nn {
namespace pia {
namespace inet {
namespace {
const u64 TRACE_FLAG = 0x8000ULL;
} // namespace

// 0x0097FA14
nn::pia::inet::NatProbeSetting g_NatProbeSetting = {50, 90000};

// 0x003E4554 | fefates:bytes [tier B]
bool nn::pia::inet::NatProbeList::ProcessProbe(const nn::pia::transport::StationLocation& location, unsigned char kind)
{
    common::Time now;
    now.SetNow();
    NatProbe* pProbe = FindByConnectionId(location.m_StationKey);
    if (pProbe != nullptr) {
        bool isUpdated = pProbe->UpdateTargetAddress(location);
        removeDifferentConnectionIdAndSameAddressPortProbes(pProbe->m_Location);
        pProbe->m_UpdateTime = now;
        removeNotReceivedProbes(location.m_StationKey);
        return isUpdated;
    }
    location.Trace(TRACE_FLAG);
    common::TimeSpan timeout(common::TimeSpan::GetTicksPerMSec().GetTick() * g_NatProbeSetting.m_TimeoutMSec);
    NatProbe probe(location, now, timeout, kind, g_NatProbeSetting.m_SprayCountMax, 0);
    return AddProbe(probe);
}

// 0x003E4698 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::ProcessProbeReply(const nn::pia::transport::StationLocation& location, nn::pia::common::Time sendTime, unsigned char kind)
{
    NatProbe* pProbe = FindByAddressAndConnectionId(location);
    if (pProbe == nullptr) {
        location.Trace(TRACE_FLAG);
        return;
    }
    pProbe->UpdateTargetPort(location);
    removeDifferentConnectionIdAndSameAddressPortProbes(pProbe->m_Location);
    common::Time now;
    now.SetNow();
    if (sendTime.m_Tick != 0) {
        pProbe->UpdateRtt(now, sendTime);
    }
    pProbe->m_UpdateTime = now;
    pProbe->m_Kind = kind;
    removeNotReceivedProbes(location.m_StationKey);
}

// 0x003E4794 | fefates:bytes [tier B]
nn::pia::inet::NatProbe* nn::pia::inet::NatProbeList::FindByConnectionId(unsigned int connectionId)
{
    for (Node* node = Begin(); node != End(); node = Advance(node)) {
        if (node->m_Value.m_Location.m_StationKey == connectionId) {
            return &node->m_Value;
        }
    }
    return nullptr;
}

// 0x003E47C4 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::removeNotReceivedProbes(unsigned int connectionId)
{
    for (Node* node = Begin(); node != End();) {
        NatProbe* pProbe = &node->m_Value;
        if (pProbe->m_UpdateTime.m_Tick == 0 && pProbe->m_Location.m_StationKey == connectionId) {
            node = Advance(node);
            RemoveProbe(pProbe);
        } else {
            node = Advance(node);
        }
    }
}

// 0x003E4868 | fefates:bytes [tier B]
bool nn::pia::inet::NatProbeList::RemoveProbesByConnectionId(unsigned int connectionId)
{
    bool isRemoved = false;
    for (Node* node = Begin(); node != End();) {
        NatProbe* pProbe = &node->m_Value;
        node = Advance(node);
        if (pProbe->m_Location.m_StationKey == connectionId) {
            RemoveProbe(pProbe);
            isRemoved = true;
        }
    }
    return isRemoved;
}

// 0x003E4900 | fefates:bytes [tier B]
nn::pia::inet::NatProbe* nn::pia::inet::NatProbeList::FindByAddressAndConnectionId(const nn::pia::transport::StationLocation& location)
{
    for (Node* node = Begin(); node != End(); node = Advance(node)) {
        if (node->m_Value.m_Location.m_StationAddress.m_InetAddress.m_Address == location.m_StationAddress.m_InetAddress.m_Address &&
            node->m_Value.m_Location.m_StationKey == location.m_StationKey) {
            return &node->m_Value;
        }
    }
    return nullptr;
}

// 0x003E4958 | fefates:bytes [tier B]
nn::pia::inet::NatProbe* nn::pia::inet::NatProbeList::FindByAddressPortAndConnectionId(const nn::pia::transport::StationLocation& location)
{
    for (Node* node = Begin(); node != End(); node = Advance(node)) {
        if (node->m_Value.m_Location.m_StationAddress.m_InetAddress.GetKey() == location.m_StationAddress.m_InetAddress.GetKey() &&
            node->m_Value.m_Location.m_StationKey == location.m_StationKey) {
            return &node->m_Value;
        }
    }
    return nullptr;
}

// 0x003E49CC | fefates:bytes [tier B]
bool nn::pia::inet::NatProbeList::RemoveSameAddressPortAndConnectionIdProbes(const nn::pia::transport::StationLocation& location)
{
    bool isRemoved = false;
    for (Node* node = Begin(); node != End();) {
        NatProbe* pProbe = &node->m_Value;
        if (pProbe->m_Location.m_StationAddress.m_InetAddress.GetKey() == location.m_StationAddress.m_InetAddress.GetKey() &&
            pProbe->m_Location.m_StationKey == location.m_StationKey) {
            node = Advance(node);
            RemoveProbe(pProbe);
            isRemoved = true;
        } else {
            node = Advance(node);
        }
    }
    return isRemoved;
}

// 0x003E4AA0 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::removeDifferentConnectionIdAndSameAddressPortProbes(const nn::pia::transport::StationLocation& location)
{
    for (Node* node = Begin(); node != End();) {
        NatProbe* pProbe = &node->m_Value;
        if (pProbe->m_Location.m_StationKey != location.m_StationKey &&
            pProbe->m_Location.m_StationAddress.m_InetAddress.GetKey() == location.m_StationAddress.m_InetAddress.GetKey()) {
            node = Advance(node);
            RemoveProbe(pProbe);
        } else {
            node = Advance(node);
        }
    }
}

// 0x003E4B68 | fefates:bytes [tier B]
void nn::pia::inet::NatProbeList::removeDifferentPortAndSameAddressAndConnectionIdProbes(const nn::pia::transport::StationLocation& location)
{
    for (Node* node = Begin(); node != End();) {
        NatProbe* pProbe = &node->m_Value;
        if (pProbe->m_Location.m_StationAddress.m_InetAddress.m_Address == location.m_StationAddress.m_InetAddress.m_Address &&
            pProbe->m_Location.m_StationKey == location.m_StationKey) {
            node = Advance(node);
            RemoveProbe(pProbe);
        } else {
            node = Advance(node);
        }
    }
}

// 0x003E4C14 | fefates:bytes [tier B]
bool nn::pia::inet::NatProbeList::AddProbe(const nn::pia::inet::NatProbe& probe)
{
    probe.Trace(TRACE_FLAG);
    if (probe.m_UpdateTime.m_Tick != 0) {
        removeNotReceivedProbes(probe.m_Location.m_StationKey);
        removeDifferentConnectionIdAndSameAddressPortProbes(probe.m_Location);
    } else {
        removeDifferentPortAndSameAddressAndConnectionIdProbes(probe.m_Location);
        removeDifferentConnectionIdAndSameAddressPortProbes(probe.m_Location);
    }
    NatProbe* pProbe;
    if (probe.m_Unknown0x40) {
        pProbe = PushFrontNew();
    } else {
        pProbe = PushBackNew();
    }
    if (pProbe == nullptr) {
        return false;
    }
    *pProbe = probe;
    return true;
}

// 0x003E4E80 | fefates:bytes [tier B]
nn::pia::inet::NatProbeList::NatProbeList() : m_pNodeBuffer(nullptr)
{
    m_pNodeBuffer = common::NewArray<u8>(PROBE_NUM_MAX * sizeof(Node));
    if (m_pNodeBuffer != nullptr) {
        Initialize(reinterpret_cast<Node*>(m_pNodeBuffer), PROBE_NUM_MAX);
    }
    Clear();
}

// 0x003E5070 | fefates:callgraph
// 0x003E4FB8 (deleting dtor)
nn::pia::inet::NatProbeList::~NatProbeList()
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

// 0x0072EFEC slot 0x08
void nn::pia::inet::NatProbeList::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn

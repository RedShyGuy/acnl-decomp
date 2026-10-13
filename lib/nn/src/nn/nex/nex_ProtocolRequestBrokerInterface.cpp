#include "nn/nex/nex_ProtocolRequestBrokerInterface.h"
#include "nn/nex/nex_SecurityContextManager.h"

namespace nn {
namespace nex {
// 0x0096C95C (name is ours)
u32 (*ProtocolRequestBrokerInterface::s_pGetCurrentCID)();
// 0x0096C960 (name is ours)
u32 (*ProtocolRequestBrokerInterface::s_pGetCurrentPID)();
// 0x009712D4 (name is ours)
bool ProtocolRequestBrokerInterface::s_AreGettersSet;

// 0x003C30A4 | fefates:bytes [tier B]
nn::nex::ProtocolRequestBrokerInterface::ProtocolRequestBrokerInterface() : m_BandwidthCounters(String(L"Protocol"))
{
    if (!s_AreGettersSet) {
        s_pGetCurrentCID = SecurityContextManager::StaticGetCurrentCID;
        s_pGetCurrentPID = SecurityContextManager::StaticGetCurrentPID;
        s_AreGettersSet = true;
    }
}

// 0x003C3164 | fefates:callseq [tier C]
// 0x003C313C (deleting dtor)
nn::nex::ProtocolRequestBrokerInterface::~ProtocolRequestBrokerInterface()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

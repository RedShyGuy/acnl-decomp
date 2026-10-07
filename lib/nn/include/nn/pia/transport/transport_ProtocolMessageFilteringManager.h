#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// The protocol types whose messages are not filtered (ProtocolManager adds the protocols whose
// IsEnableProtocolFiltering is false). Layout from CreateInstance and AddNoFilteringProtocolType;
// the member names are ours.
class ProtocolMessageFilteringManager : public common::RootObject
{
public:
    static const u32 PROTOCOL_TYPE_NUM_MAX = 32;

    static nn::Result CreateInstance(); // 0x0045F1C0 | fefates:bytes [tier B]
    static void DestroyInstance(); // 0x0045F26C | fefates:callseq [tier C]

    // false if there is no room / the type is not in the list
    bool AddNoFilteringProtocolType(unsigned short protocolType); // 0x0045F294 | fefates:bytes [tier B]
    bool RemoveNoFilteringProtocolType(unsigned short protocolType); // 0x0045F2BC | fefates:bytes [tier B]
    bool IsFilteringEnabled(unsigned short protocolType) const; // 0x00736D10 | fefates:bytes [tier B]

    static ProtocolMessageFilteringManager* s_pInstance;

    u16 m_ProtocolTypes[PROTOCOL_TYPE_NUM_MAX]; // 0x00
    u32 m_ProtocolTypeNum;                      // 0x40
};
ASSERT_SIZE(ProtocolMessageFilteringManager, 0x44);
} // namespace transport
} // namespace pia
} // namespace nn

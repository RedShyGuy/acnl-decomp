#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_Channel.h"
#include "nw/snd/internal/snd_PoolImpl.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// The channels: a pool in the memory of the sound system and the list of the channels in use (the class
// and function names are from the reference symbols, the members are ours).
class ChannelManager
{
public:
    typedef ut::LinkList<Channel, 0x1B0> ChannelList;

    // (inline)
    ChannelManager() : m_IsInitialized(false), m_ChannelCount(0) {}

    static ChannelManager& GetInstance(); // 0x004CCB88 | nintendogs:callseq-callee [tier A]

    void Initialize(void* pMemory, u32 size); // 0x004CCB48 | nintendogs:bytes-fuzzy [tier A]
    void Finalize(); // 0x004CCD28 | nintendogs:bytes-fuzzy [tier A]
    // the memory for channelCount channels (and one for the alignment)
    u32 GetRequiredMemSize(int channelCount); // 0x004CCC1C | nintendogs:callseq-callee [tier A]
    Channel* Alloc(); // 0x004CCCF0 | nintendogs:bytes-fuzzy [tier A]
    void Free(Channel* pChannel); // 0x004CCCB4 | nintendogs:callseq-callee [tier A]
    void UpdateAllChannel(); // 0x004CCBE4 | fefates:bytes [tier B]

    PoolImpl m_Pool;            // 0x00
    ChannelList m_ChannelList;  // 0x04 in use
    bool m_IsInitialized;       // 0x10
    int m_ChannelCount;         // 0x14
    void* m_pMemory;            // 0x18
    u32 m_MemorySize;           // 0x1C
};
ASSERT_SIZE(ChannelManager, 0x20);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

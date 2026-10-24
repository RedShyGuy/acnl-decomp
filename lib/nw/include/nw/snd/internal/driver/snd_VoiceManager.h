#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_Voice.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// The voices of the sound library: a free list and the voices in use, sorted by priority (lowest
// first; the class and function names are from the reference symbols, the members are ours).
class VoiceManager
{
public:
    typedef ut::LinkList<Voice, 88> VoiceList;

    // (inline)
    VoiceManager() : m_IsInitialized(false) {}

    static VoiceManager& GetInstance(); // 0x004CE8F0 | nintendogs:callgraph [tier A]

    // the voices are built in the memory (96 bytes each)
    void Initialize(void* pMemory, u32 size); // 0x004CE878 | nintendogs:callseq [tier A]
    void Finalize(); // 0x004CEB7C | nintendogs:bytes [tier A]
    u32 GetRequiredMemSize(int voiceCount); // 0x004CEA40 | nintendogs:callseq-callee [tier A]

    // a voice of channelCount channels; drops the lowest voice if the priority allows (NULL: none)
    Voice* AllocVoice(int channelCount, int priority, Voice::Callback pCallback, void* pCallbackArg); // 0x004CE784 | nintendogs:callseq [tier A]
    void FreeVoice(Voice* pVoice); // 0x004CEC1C | nintendogs:bytes [tier A]
    // the voice into the list by its priority
    void AppendVoiceList(Voice* pVoice); // 0x004CE958 | nintendogs:bytes [tier A]
    void ChangeVoicePriority(Voice* pVoice); // 0x004CEA4C | nintendogs:bytes [tier A]
    // drops the lowest voice if its priority is at most priority; its channel count (0: none)
    int DropLowestPriorityVoice(int priority); // 0x004CEB0C | nintendogs:bytes [tier A]
    void UpdateAllVoices(); // 0x004CE9B4 | nintendogs:bytes [tier A]
    // the parameters of all active voices are sent again
    void UpdateAllVoicesSync(u32 syncFlag); // 0x004CEAC0 | nintendogs:bytes [tier A]
    // the channels of the voices in use
    u32 GetVoiceCount() const; // 0x007414D8 | nintendogs:bytes [tier A]

    bool m_IsInitialized;     // 0x00
    VoiceList m_VoiceList;    // 0x04 in use, by priority
    VoiceList m_FreeList;     // 0x10
    int m_VoiceCount;         // 0x1C
    int m_VoiceCountLimit;    // 0x20 at most this many in use
};
ASSERT_SIZE(VoiceManager, 0x24);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

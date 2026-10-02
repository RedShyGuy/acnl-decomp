#pragma once

#include "decomp.h"
#include "nw/snd/internal/driver/snd_Voice.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class VoiceManager
{
public:
    void AllocVoice(int, int, void(*)(nw::snd::internal::driver::Voice*, nw::snd::internal::driver::Voice::VoiceCallbackStatus, void*), void*); // 0x004CE784 | nintendogs:callseq [tier A]
    void Initialize(void*, unsigned); // 0x004CE878 | nintendogs:callseq [tier A]
    void GetInstance(); // 0x004CE8F0 | nintendogs:callgraph [tier A]
    void AppendVoiceList(nw::snd::internal::driver::Voice*); // 0x004CE958 | nintendogs:bytes [tier A]
    void UpdateAllVoices(); // 0x004CE9B4 | nintendogs:bytes [tier A]
    void GetRequiredMemSize(int); // 0x004CEA40 | nintendogs:callseq-callee [tier A]
    void ChangeVoicePriority(nw::snd::internal::driver::Voice*); // 0x004CEA4C | nintendogs:bytes [tier A]
    void UpdateAllVoicesSync(unsigned); // 0x004CEAC0 | nintendogs:bytes [tier A]
    void DropLowestPriorityVoice(int); // 0x004CEB0C | nintendogs:bytes [tier A]
    void Finalize(); // 0x004CEB7C | nintendogs:bytes [tier A]
    void FreeVoice(nw::snd::internal::driver::Voice*); // 0x004CEC1C | nintendogs:bytes [tier A]
    void GetVoiceCount() const; // 0x007414D8 | nintendogs:bytes [tier A]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

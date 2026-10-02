#include "nw/snd/internal/driver/snd_Voice.h"
#include "nw/snd/internal/driver/snd_VoiceManager.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CE784 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::VoiceManager::AllocVoice(int, int, void(*)(nw::snd::internal::driver::Voice*, nw::snd::internal::driver::Voice::VoiceCallbackStatus, void*), void*)
{
}

// 0x004CE878 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::VoiceManager::Initialize(void*, unsigned)
{
}

// 0x004CE8F0 | nintendogs:callgraph [tier A]
void nw::snd::internal::driver::VoiceManager::GetInstance()
{
}

// 0x004CE958 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::AppendVoiceList(nw::snd::internal::driver::Voice*)
{
}

// 0x004CE9B4 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::UpdateAllVoices()
{
}

// 0x004CEA40 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::VoiceManager::GetRequiredMemSize(int)
{
}

// 0x004CEA4C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::ChangeVoicePriority(nw::snd::internal::driver::Voice*)
{
}

// 0x004CEAC0 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::UpdateAllVoicesSync(unsigned)
{
}

// 0x004CEB0C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::DropLowestPriorityVoice(int)
{
}

// 0x004CEB7C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::Finalize()
{
}

// 0x004CEC1C | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::FreeVoice(nw::snd::internal::driver::Voice*)
{
}

// 0x007414D8 | nintendogs:bytes [tier A]
void nw::snd::internal::driver::VoiceManager::GetVoiceCount() const
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

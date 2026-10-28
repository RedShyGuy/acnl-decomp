#include "nw/snd/snd_SoundHeap.h"

#include "nw/snd/internal/snd_DriverCommandManager.h"

namespace nw {
namespace snd {
namespace {
// (inline) waits until the sound thread ran all commands
void WaitDriverCommands()
{
    internal::DriverCommandManager& manager = internal::DriverCommandManager::GetInstance();
    manager.WaitCommandReply(manager.FlushCommand(true));
}
} // namespace

// 0x0012BF60 | nintendogs:bytes [tier B]
int nw::snd::SoundHeap::SaveState()
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
    return m_FrameHeap.SaveState();
}

// 0x001327F4 | nintendogs:bytes [tier A]
bool nw::snd::SoundHeap::Create(void* pStartAddress, u32 size)
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
    return m_FrameHeap.Create(pStartAddress, size);
}

// 0x00132834 | nintendogs:bytes [tier A]
nw::snd::SoundHeap::SoundHeap()
{
    m_CriticalSection.Initialize();
}

// 0x0013C46C | nintendogs:bytes [tier B]
void nw::snd::SoundHeap::LoadState(int level)
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
    m_FrameHeap.LoadState(level);
    WaitDriverCommands();
}

// 0x0013F564 | nintendogs:bytes [tier A]
void nw::snd::SoundHeap::Destroy()
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
    if (!m_FrameHeap.IsValid()) {
        return;
    }
    Clear();
    m_FrameHeap.Destroy();
}

// 0x004D4EE8 | fefates:bytes [tier B]
void nw::snd::SoundHeap::DisposeCallbackFunc(void* pMemory, u32 size, void*)
{
    internal::DriverCommandManager& manager = internal::DriverCommandManager::GetInstance();
    internal::DriverCommandDispose* pCommand = manager.AllocCommand<internal::DriverCommandDispose>();
    pCommand->id = internal::DriverCommand::ID_DISPOSE;
    pCommand->pMemory = pMemory;
    pCommand->size = size;
    manager.PushCommand(pCommand);
}

// 0x004D4F2C slot 0x08 | nintendogs:bytes
void* nw::snd::SoundHeap::Alloc(u32 size)
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
    return Alloc(size, DisposeCallbackFunc, nullptr);
}

// 0x004D4F90 | nintendogs:bytes [tier B]
void* nw::snd::SoundHeap::Alloc(u32 size, internal::FrameHeap::FreeCallback callback, void* pCallbackArg)
{
    nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
    return m_FrameHeap.Alloc(size, callback, pCallbackArg);
}

// 0x004D4FD8 (name is ours)
void nw::snd::SoundHeap::Clear()
{
    if (!m_FrameHeap.IsValid()) {
        return;
    }
    {
        nn::os::CriticalSection::ScopedLock lock(m_CriticalSection);
        m_FrameHeap.Clear();
    }
    WaitDriverCommands();
}

// 0x004D50D4 slot 0x00 | nintendogs:bytes
// 0x004D502C (deleting dtor)
nw::snd::SoundHeap::~SoundHeap()
{
    Destroy();
    m_CriticalSection.Finalize();
}

} // namespace snd
} // namespace nw

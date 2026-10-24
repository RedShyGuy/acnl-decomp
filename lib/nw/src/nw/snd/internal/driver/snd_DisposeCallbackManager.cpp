#include "nw/snd/internal/driver/snd_DisposeCallbackManager.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004D3A60 | fefates:bytes [tier B]
nw::snd::internal::driver::DisposeCallbackManager& nw::snd::internal::driver::DisposeCallbackManager::GetInstance()
{
    // 0x00AF637C
    static DisposeCallbackManager s_DisposeCallbackManager;
    return s_DisposeCallbackManager;
}

// 0x004D3AB4 | fefates:bytes [tier B]
void nw::snd::internal::driver::DisposeCallbackManager::RegisterDisposeCallback(DisposeCallback* pCallback)
{
    m_CallbackList.PushBack(pCallback);
}

// 0x004D3AC4 | fefates:bytes [tier B]
void nw::snd::internal::driver::DisposeCallbackManager::UnregisterDisposeCallback(DisposeCallback* pCallback)
{
    m_CallbackList.Erase(pCallback);
}

// 0x004D3ACC | fefates:bytes [tier B]
void nw::snd::internal::driver::DisposeCallbackManager::Dispose(const void* pMemory, u32 size)
{
    const void* pEnd = static_cast<const u8*>(pMemory) + size;
    // (the original goes through the list of the instance, not of this)
    for (ut::internal::LinkListImpl::iterator it = GetInstance().m_CallbackList.begin(); it != GetInstance().m_CallbackList.end();) {
        DisposeCallback* pCallback = CallbackList::GetPointerFromNode(it.GetNode());
        ++it;
        pCallback->InvalidateData(pMemory, pEnd);
    }
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

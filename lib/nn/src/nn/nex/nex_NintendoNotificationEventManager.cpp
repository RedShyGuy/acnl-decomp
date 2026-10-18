#include "nn/nex/nex_NintendoNotificationEventManager.h"
#include "nn/nex/nex_NintendoNotificationEvent.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// 0x003CA5F4 slot 0x5C | fefates:bytes
void nn::nex::NintendoNotificationEventManager::ProcessNintendoNotificationEvent(const nn::nex::NintendoNotificationEvent& event)
{
    if (m_pParent != nullptr) {
        m_pParent->ProcessNintendoNotificationEvent(event);
        return;
    }
    for (std::set<NintendoNotificationEventHandler*>::iterator it = m_Handlers.begin(); it != m_Handlers.end(); ++it) {
        (*it)->ProcessNintendoNotificationEvent(event);
    }
}

// 0x003CA6A0 | fefates:bytes [tier B]
nn::nex::NintendoNotificationEventManager::NintendoNotificationEventManager(nn::nex::NintendoNotificationEventManager* pParent) : m_pParent(pParent)
{
    m_Flags |= FLAG_UNKNOWN_8;
    SetName(String(L"NintendoNotificationEventManager"));
}

// 0x003CA7E0 slot 0x00 | fefates:bytes
// 0x003CA7D0 (deleting dtor)
nn::nex::NintendoNotificationEventManager::~NintendoNotificationEventManager()
{
    m_Handlers.erase(m_Handlers.begin(), m_Handlers.end());
}

// 0x0072DF00 slot 0x4C
nn::nex::Protocol* nn::nex::NintendoNotificationEventManager::Clone()
{
    return new NintendoNotificationEventManager(this);
}

} // namespace nex
} // namespace nn

namespace __rw {
// 0x00801E08 (name is ours)
template <>
DECOMP_NOINLINE __rb_tree<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*,
                          __ident<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*>, std::less<nn::nex::NintendoNotificationEventHandler*>,
                          nn::nex::MemAllocator<nn::nex::NintendoNotificationEventHandler*> >::iterator
__rb_tree<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*,
          __ident<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*>, std::less<nn::nex::NintendoNotificationEventHandler*>,
          nn::nex::MemAllocator<nn::nex::NintendoNotificationEventHandler*> >::erase(iterator first, iterator last)
{
    return EraseRangeInline(first, last);
}

// 0x00801F5C (name is ours)
template void __rb_tree<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*,
                        __ident<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*>, std::less<nn::nex::NintendoNotificationEventHandler*>,
                        nn::nex::MemAllocator<nn::nex::NintendoNotificationEventHandler*> >::_C_erase(Node* pNode);

// 0x008017AC (name is ours)
template __rb_tree<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*,
                   __ident<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*>, std::less<nn::nex::NintendoNotificationEventHandler*>,
                   nn::nex::MemAllocator<nn::nex::NintendoNotificationEventHandler*> >::iterator
__rb_tree<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*,
          __ident<nn::nex::NintendoNotificationEventHandler*, nn::nex::NintendoNotificationEventHandler*>, std::less<nn::nex::NintendoNotificationEventHandler*>,
          nn::nex::MemAllocator<nn::nex::NintendoNotificationEventHandler*> >::erase(iterator it);
} // namespace __rw

#include "nw/gfx/gfx_Skeleton.h"

namespace nw {
namespace gfx {
// 0x009826F0
const TypeInfo nw::gfx::Skeleton::s_TypeInfo(&SceneObject::s_TypeInfo);

// sets s_TypeInfo
// 0x00796768 (static initializer)

// 0x004B0708 (name is ours)
void nw::gfx::Skeleton::CreateCallbacks(os::IAllocator* pAllocator, int maxCallbacks, bool isFixedSizeMemory)
{
    m_pPreCalculateCallbacks = CallbackList::Create(pAllocator, isFixedSizeMemory, maxCallbacks);
    m_pPostCalculateCallbacks = CallbackList::Create(pAllocator, isFixedSizeMemory, maxCallbacks);
}

// 0x004B0944 slot 0x00 | nintendogs:bytes
// 0x004B0938 (deleting dtor)
nw::gfx::Skeleton::~Skeleton()
{
    if (m_pPreCalculateCallbacks != nullptr) {
        m_pPreCalculateCallbacks->Destroy();
        m_pPreCalculateCallbacks = nullptr;
    }
    if (m_pPostCalculateCallbacks != nullptr) {
        m_pPostCalculateCallbacks->Destroy();
        m_pPostCalculateCallbacks = nullptr;
    }
}

} // namespace gfx
} // namespace nw

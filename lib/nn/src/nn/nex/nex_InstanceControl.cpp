#include "nn/nex/nex_InstanceControl.h"
#include "nn/nex/nex_InstanceTable.h"

namespace nn {
namespace nex {
// 0x00378514
nn::nex::InstanceControl::InstanceControl(u32 kind, u32 context)
{
    ScopedCS lock(InstanceTable::s_Lock);
    m_IsRegistered = false;
    if (InstanceTable::s_Instance.Register(this, kind, context)) {
        m_Context = context;
        m_Kind = kind;
        m_pOwner = nullptr;
        m_IsRegistered = true;
    }
}

// 0x00378644
// 0x00378610 (deleting dtor)
nn::nex::InstanceControl::~InstanceControl()
{
    ScopedCS lock(InstanceTable::s_Lock);
    if (m_IsRegistered) {
        InstanceTable::s_Instance.GetContext(m_Context)->Unregister(m_Kind);
    }
}

} // namespace nex
} // namespace nn

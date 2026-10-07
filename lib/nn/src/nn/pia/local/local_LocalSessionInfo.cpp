#include "nn/pia/local/local_LocalSessionInfo.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalNetwork.h"

namespace nn {
namespace pia {
namespace local {
// 0x00416B98
nn::Result nn::pia::local::LocalSessionInfo::UpdateLinkLevel(u32 index)
{
    return LocalNetwork::s_pInstance->GetLinkLevel(&m_LinkLevel, index);
}

// 0x00416BB0
void nn::pia::local::LocalSessionInfo::Clear()
{
    m_IsValid = false;
    m_LinkLevel = 0;
}

// 0x00416BC0
nn::pia::local::LocalSessionInfo::LocalSessionInfo() : m_IsValid(false)
{
}

// 0x00416BE0
// 0x00416BD8 (deleting dtor)
nn::pia::local::LocalSessionInfo::~LocalSessionInfo()
{
    // empty (in the original too)
}

// 0x0073024C
nn::Result nn::pia::local::LocalSessionInfo::GetLinkLevel(u8* pLevel) const
{
    if (!m_IsValid) {
        return common::RESULT_INVALID_STATE;
    }
    if (!common::IsValidPointer(pLevel)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    *pLevel = m_LinkLevel;
    return nn::Result();
}

// 0x00730288
bool nn::pia::local::LocalSessionInfo::IsValid() const
{
    return m_IsValid;
}

} // namespace local
} // namespace pia
} // namespace nn

#include "nn/pia/local/local_LocalSessionSearchCriteria.h"
#include "nn/pia/common/common_Result.h"
#include "nn/pia/local/local_LocalNetworkDescription.h"

namespace nn {
namespace pia {
namespace local {
// 0x004213F4
bool nn::pia::local::LocalSessionSearchCriteria::IsMatch(const nn::pia::local::LocalNetworkDescription* pDescription) const
{
    if (!common::IsValidPointer(pDescription)) {
        return false;
    }
    if (pDescription->GetLocalCommunicationId() != m_LocalCommunicationId || pDescription->GetSubId() != m_SubId) {
        return false;
    }
    if (m_MaxParticipantsMin != PARTICIPANTS_ANY && m_MaxParticipantsMax != PARTICIPANTS_ANY) {
        if (m_MaxParticipantsMin > pDescription->GetMaxParticipants() || pDescription->GetMaxParticipants() > m_MaxParticipantsMax) {
            return false;
        }
    }
    if (m_IsOpenedOnly && !pDescription->IsOpened()) {
        return false;
    }
    if (m_IsVacantOnly && pDescription->GetCurrentParticipants() == pDescription->GetMaxParticipants()) {
        return false;
    }
    return true;
}

// 0x00421518
// 0x00421514 (deleting dtor)
nn::pia::local::LocalSessionSearchCriteria::~LocalSessionSearchCriteria()
{
    // empty (in the original too)
}

// 0x007316B4
void nn::pia::local::LocalSessionSearchCriteria::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace local
} // namespace pia
} // namespace nn

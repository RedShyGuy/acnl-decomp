#include "nn/ngc/ngc_RegexDfaState.h"

namespace nn {
namespace ngc {
// 0x003DC2B8 | fefates:bytes [tier B]
bool nn::ngc::RegexDfaState::Initialize(nn::ngc::ProfanityFilterTemporaryPool* pool, u32 nfaStateCount, bool isClear)
{
    m_Pool = pool;
    m_NfaStateCount = nfaStateCount;
    m_NfaStates = static_cast<bool*>(pool->Allocate((nfaStateCount + ProfanityFilterTemporaryPool::UNIT_SIZE - 1) / ProfanityFilterTemporaryPool::UNIT_SIZE));
    if (m_NfaStates == NULL) {
        return false;
    }
    if (isClear) {
        for (u32 i = 0; i < nfaStateCount; i++) {
            m_NfaStates[i] = false;
        }
    }
    m_Links.SetPool(m_Pool);
    m_NfaLinks.SetPool(m_Pool);
    return true;
}

// 0x003DC350 | fefates:bytes [tier B]
nn::ngc::RegexDfaState::RegexDfaState() : m_Pool(NULL), m_NfaStates(NULL), m_NfaStateCount(0), m_IsAccept(false)
{
}

} // namespace ngc
} // namespace nn

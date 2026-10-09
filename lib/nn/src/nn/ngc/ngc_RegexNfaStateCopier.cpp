#include "nn/ngc/ngc_RegexNfaStateCopier.h"

namespace nn {
namespace ngc {
// 0x003DF9E8 | fefates:bytes [tier B]
bool nn::ngc::RegexNfaStateCopier::CopyStates(StateIterator* copy, StateIterator start, StateIterator end)
{
    StateIterator state = m_States->PushBackNew();
    if (state.IsNull()) {
        return false;
    }
    state->m_Links.SetPool(m_Pool);
    UnitList<StatePair>::Iterator pair = m_Pairs.PushBackNew();
    if (pair.IsNull()) {
        return false;
    }
    pair->m_Original = start;
    pair->m_Copy = state;
    if (start != end) {
        UnitList<RegexStateLink<RegexNfaState> >::Iterator linkEnd = start->m_Links.End();
        for (UnitList<RegexStateLink<RegexNfaState> >::Iterator link = start->m_Links.Begin(); link != linkEnd; ++link) {
            UnitList<StatePair>::Iterator target = GetStatePair(link->m_Target);
            if (target != m_Pairs.End()) {
                // copied already (a loop)
                UnitList<RegexStateLink<RegexNfaState> >::Iterator newLink = state->m_Links.PushBackNew();
                if (newLink.IsNull()) {
                    return false;
                }
                if (!newLink->DeepCopyFrom(*link, target->m_Copy, m_Pool)) {
                    return false;
                }
            } else {
                StateIterator targetCopy;
                if (!CopyStates(&targetCopy, link->m_Target, end)) {
                    return false;
                }
                UnitList<RegexStateLink<RegexNfaState> >::Iterator newLink = state->m_Links.PushBackNew();
                if (newLink.IsNull()) {
                    return false;
                }
                if (!newLink->DeepCopyFrom(*link, targetCopy, m_Pool)) {
                    return false;
                }
            }
        }
    }
    *copy = state;
    return true;
}

// 0x003DFC10 | fefates:bytes [tier B]
nn::ngc::UnitList<nn::ngc::RegexNfaStateCopier::StatePair>::Iterator nn::ngc::RegexNfaStateCopier::GetStatePair(StateIterator state)
{
    UnitList<StatePair>::Iterator it;
    for (it = m_Pairs.Begin(); it != m_Pairs.End(); ++it) {
        if (it->m_Original == state) {
            break;
        }
    }
    return it;
}

// 0x003DFC58 | fefates:bytes [tier B]
nn::ngc::RegexNfaStateCopier::RegexNfaStateCopier(nn::ngc::UnitList<nn::ngc::RegexNfaState>* states, nn::ngc::ProfanityFilterTemporaryPool* pool)
    : m_States(states), m_Pool(pool)
{
    m_Pairs.SetPool(pool);
}

} // namespace ngc
} // namespace nn

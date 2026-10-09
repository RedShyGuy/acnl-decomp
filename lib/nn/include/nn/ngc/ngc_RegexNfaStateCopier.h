#pragma once

#include "decomp.h"
#include "nn/ngc/ngc_RegexNfaState.h"
#include "nn/ngc/ngc_UnitList.h"

namespace nn {
namespace ngc {
// Copies the states of an NFA from a start state to an end state (for the repeats {n} etc.),
// remembering which copy belongs to which state. Member names are ours.
class RegexNfaStateCopier
{
public:
    typedef UnitList<RegexNfaState>::Iterator StateIterator;

    // a state and its copy (name is ours)
    struct StatePair
    {
        StateIterator m_Original;   // 0x0
        StateIterator m_Copy;       // 0x4
    };

    RegexNfaStateCopier(nn::ngc::UnitList<nn::ngc::RegexNfaState>* states, nn::ngc::ProfanityFilterTemporaryPool* pool); // 0x003DFC58 | fefates:bytes [tier B]
    bool CopyStates(StateIterator* copy, StateIterator start, StateIterator end); // 0x003DF9E8 | fefates:bytes [tier B]
    UnitList<StatePair>::Iterator GetStatePair(StateIterator state); // 0x003DFC10 | fefates:bytes [tier B]

    UnitList<StatePair> m_Pairs;            // 0x00
    UnitList<RegexNfaState>* m_States;      // 0x0C, where the copies go
    ProfanityFilterTemporaryPool* m_Pool;   // 0x10
};
ASSERT_SIZE(RegexNfaStateCopier, 0x14);
} // namespace ngc
} // namespace nn

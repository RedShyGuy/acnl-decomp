#include "nn/ngc/ngc_RegexDfaConverter.h"
#include <string.h>

namespace nn {
namespace ngc {
namespace {
typedef UnitList<RegexStateLink<RegexNfaState> >::Iterator NfaLinkIterator;
typedef UnitList<RegexStateLink<RegexDfaState> >::Iterator DfaLinkIterator;

inline u32 GetSetUnitCount(u32 nfaStateCount)
{
    return (nfaStateCount + ProfanityFilterTemporaryPool::UNIT_SIZE - 1) / ProfanityFilterTemporaryPool::UNIT_SIZE;
}
} // namespace

// 0x003DE1C4 | fefates:bytes [tier B]
nn::ngc::RegexDfaConverter::DfaIterator nn::ngc::RegexDfaConverter::RegisterDfaState(const bool* nfaStates)
{
    DfaIterator it;
    for (it = m_DfaStates.Begin(); it != m_DfaStates.End(); ++it) {
        if (memcmp(it->m_NfaStates, nfaStates, m_NfaStateCount) == 0) {
            break;
        }
    }
    if (it != m_DfaStates.End()) {
        return it;
    }
    DfaIterator state = m_DfaStates.PushBackNew();
    if (state.IsNull()) {
        return DfaIterator();
    }
    if (!state->Initialize(m_Pool, m_NfaStateCount, false)) {
        return DfaIterator();
    }
    memcpy(state->m_NfaStates, nfaStates, m_NfaStateCount);
    if (!CopyLinksFromNfaStates(state)) {
        return DfaIterator();
    }
    return state;
}

// 0x003DE2B8 | fefates:bytes [tier B]
void nn::ngc::RegexDfaConverter::GatherEpsilonLinks(bool* nfaStates)
{
    // adds the states that epsilon links lead to until there are no more
    bool isAdded;
    do {
        isAdded = false;
        for (u32 i = 0; i < m_NfaStateCount && !isAdded; i++) {
            if (!nfaStates[i]) {
                continue;
            }
            const UnitList<RegexStateLink<RegexNfaState> >& links = m_NfaStates[i]->m_Links;
            for (UnitList<RegexStateLink<RegexNfaState> >::ConstIterator link = links.Begin(); link != links.End(); ++link) {
                if (link->m_Type == RegexStateLink<RegexNfaState>::TYPE_EPSILON && !nfaStates[link->m_Target->m_Index]) {
                    nfaStates[link->m_Target->m_Index] = true;
                    isAdded = true;
                    break;
                }
            }
        }
    } while (isAdded);
}

// 0x003DE33C | fefates:bytes [tier B]
bool nn::ngc::RegexDfaConverter::CreateCharClassLink(DfaIterator state)
{
    NfaLinkIterator link = state->m_NfaLinks.Begin();
    while (link != state->m_NfaLinks.End()) {
        if (link->m_Type != RegexStateLink<RegexNfaState>::TYPE_CHAR_CLASS) {
            ++link;
            continue;
        }
        u32 unitCount = GetSetUnitCount(m_NfaStateCount);
        bool* nfaStates = static_cast<bool*>(m_Pool->Allocate(unitCount));
        if (nfaStates == NULL) {
            return false;
        }
        for (u32 i = 0; i < m_NfaStateCount; i++) {
            nfaStates[i] = false;
        }
        nfaStates[link->m_Target->m_Index] = true;

        // the characters of this class that the other classes share go to one new state
        CharacterRangeList shared;
        if (!shared.CopyFrom(link->m_CharClass)) {
            return false;
        }
        for (NfaLinkIterator other = state->m_NfaLinks.Begin(); other != state->m_NfaLinks.End();) {
            if (other == link || other->m_Type != RegexStateLink<RegexNfaState>::TYPE_CHAR_CLASS ||
                !other->m_CharClass.HasSharedCharRangeArea(&shared)) {
                ++other;
                continue;
            }
            if (!shared.Intersect(&other->m_CharClass)) {
                return false;
            }
            if (!other->m_CharClass.RemoveFromClass(&shared)) {
                return false;
            }
            nfaStates[other->m_Target->m_Index] = true;
            if (other->m_CharClass.IsEmpty()) {
                other = state->m_NfaLinks.Erase(other);
                continue;
            }
            ++other;
        }
        if (!link->m_CharClass.RemoveFromClass(&shared)) {
            return false;
        }
        GatherEpsilonLinks(nfaStates);
        DfaIterator target = RegisterDfaState(nfaStates);
        if (target.IsNull()) {
            return false;
        }
        m_Pool->Free(nfaStates, unitCount);
        DfaLinkIterator newLink = state->m_Links.PushBackNew();
        if (newLink.IsNull()) {
            return false;
        }
        newLink->LinkTo(target, m_Pool);
        newLink->m_CharClass.MoveFrom(shared);
        if (link->m_CharClass.IsEmpty()) {
            link = state->m_NfaLinks.Erase(link);
        }
    }
    return true;
}

// 0x003DE870 | fefates:bytes [tier B]
bool nn::ngc::RegexDfaConverter::CreateCharacterLink(DfaIterator state)
{
    NfaLinkIterator link = state->m_NfaLinks.Begin();
    while (link != state->m_NfaLinks.End()) {
        if (link->m_Type != RegexStateLink<RegexNfaState>::TYPE_CHARACTER) {
            ++link;
            continue;
        }
        u32 unitCount = GetSetUnitCount(m_NfaStateCount);
        wchar_t character = link->m_Character;
        bool* nfaStates = static_cast<bool*>(m_Pool->Allocate(unitCount));
        if (nfaStates == NULL) {
            return false;
        }
        for (u32 i = 0; i < m_NfaStateCount; i++) {
            nfaStates[i] = false;
        }
        nfaStates[link->m_Target->m_Index] = true;

        // every link that takes the character goes to the new state
        for (NfaLinkIterator other = state->m_NfaLinks.Begin(); other != state->m_NfaLinks.End();) {
            if (other != link) {
                if (other->m_Type == RegexStateLink<RegexNfaState>::TYPE_CHARACTER) {
                    if (other->m_Character == character) {
                        nfaStates[other->m_Target->m_Index] = true;
                        other = state->m_NfaLinks.Erase(other);
                        continue;
                    }
                } else if (other->m_Type == RegexStateLink<RegexNfaState>::TYPE_CHAR_CLASS) {
                    if (other->IsMatch(character)) {
                        if (!other->m_CharClass.RemoveFromClass(character)) {
                            return false;
                        }
                        nfaStates[other->m_Target->m_Index] = true;
                        if (other->m_CharClass.IsEmpty()) {
                            other = state->m_NfaLinks.Erase(other);
                            continue;
                        }
                    }
                }
            }
            ++other;
        }
        GatherEpsilonLinks(nfaStates);
        DfaIterator target = RegisterDfaState(nfaStates);
        if (target.IsNull()) {
            return false;
        }
        m_Pool->Free(nfaStates, unitCount);
        DfaLinkIterator newLink = state->m_Links.PushBackNew();
        if (newLink.IsNull()) {
            return false;
        }
        newLink->LinkTo(target, character);
        link = state->m_NfaLinks.Erase(link);
    }
    return true;
}

// 0x003DEB20 | fefates:bytes [tier B]
bool nn::ngc::RegexDfaConverter::CopyLinksFromNfaStates(DfaIterator state)
{
    for (u32 i = 0; i < m_NfaStateCount; i++) {
        if (!state->m_NfaStates[i]) {
            continue;
        }
        const UnitList<RegexStateLink<RegexNfaState> >& links = m_NfaStates[i]->m_Links;
        for (UnitList<RegexStateLink<RegexNfaState> >::ConstIterator link = links.Begin(); link != links.End(); ++link) {
            if (link->m_Type == RegexStateLink<RegexNfaState>::TYPE_EPSILON) {
                continue;
            }
            NfaLinkIterator copy = state->m_NfaLinks.PushBackNew();
            if (copy.IsNull()) {
                return false;
            }
            if (!copy->DeepCopyFrom(*link, m_Pool)) {
                return false;
            }
        }
    }
    return true;
}

// 0x003DEBF8 | fefates:bytes [tier B]
bool nn::ngc::RegexDfaConverter::CreateAtomicZeroWidthAssersionLink(DfaIterator state)
{
    // only the assertion links are left
    NfaLinkIterator end = state->m_NfaLinks.End();
    NfaLinkIterator link = state->m_NfaLinks.Begin();
    while (link != end) {
        u32 unitCount = GetSetUnitCount(m_NfaStateCount);
        bool* nfaStates = static_cast<bool*>(m_Pool->Allocate(unitCount));
        if (nfaStates == NULL) {
            return false;
        }
        memcpy(nfaStates, state->m_NfaStates, m_NfaStateCount);
        nfaStates[link->m_Target->m_Index] = true;
        for (NfaLinkIterator other = state->m_NfaLinks.Begin(); other != end;) {
            if (other != link && link->m_AssertionType == other->m_AssertionType) {
                nfaStates[other->m_Target->m_Index] = true;
                other = state->m_NfaLinks.Erase(other);
                continue;
            }
            ++other;
        }
        GatherEpsilonLinks(nfaStates);
        if (memcmp(nfaStates, state->m_NfaStates, m_NfaStateCount) != 0) {
            DfaIterator target = RegisterDfaState(nfaStates);
            if (target.IsNull()) {
                return false;
            }
            DfaLinkIterator newLink = state->m_Links.PushBackNew();
            if (newLink.IsNull()) {
                return false;
            }
            newLink->LinkTo(target, link->m_AssertionType);
        }
        m_Pool->Free(nfaStates, unitCount);
        link = state->m_NfaLinks.Erase(link);
    }
    return true;
}

// 0x003DEDDC | fefates:bytes [tier B]
bool nn::ngc::RegexDfaConverter::Convert(nn::ngc::ProfanityFilterTemporaryPool* pool, const nn::ngc::UnitList<nn::ngc::RegexNfaState>* nfaStates,
                                         nn::ngc::UnitList<nn::ngc::RegexNfaState>::ConstIterator nfaEnd)
{
    m_Pool = pool;
    u32 count = 0;
    for (UnitList<RegexNfaState>::ConstIterator it = nfaStates->Begin(); it != nfaStates->End(); ++it) {
        count++;
    }
    m_NfaStateCount = count;
    m_NfaStates = static_cast<UnitList<RegexNfaState>::ConstIterator*>(
        pool->Allocate((count * sizeof(UnitList<RegexNfaState>::ConstIterator) + ProfanityFilterTemporaryPool::UNIT_SIZE - 1) /
                       ProfanityFilterTemporaryPool::UNIT_SIZE));
    if (m_NfaStates == NULL) {
        return false;
    }
    u32 index = 0;
    for (UnitList<RegexNfaState>::ConstIterator it = nfaStates->Begin(); it != nfaStates->End(); ++it) {
        m_NfaStates[index++] = it;
    }

    // the start state: the first NFA state and where epsilon links lead
    m_DfaStates.SetPool(m_Pool);
    DfaIterator start = m_DfaStates.PushBackNew();
    if (start.IsNull()) {
        return false;
    }
    if (!start->Initialize(m_Pool, m_NfaStateCount, true)) {
        return false;
    }
    start->m_NfaStates[0] = true;
    GatherEpsilonLinks(start->m_NfaStates);
    if (!CopyLinksFromNfaStates(start)) {
        return false;
    }

    // the new states are added behind, so the loop reaches them, too
    for (DfaIterator it = m_DfaStates.Begin(); it != m_DfaStates.End(); ++it) {
        if (it->m_NfaLinks.IsEmpty()) {
            continue;
        }
        if (!CreateCharacterLink(it)) {
            return false;
        }
        if (it->m_NfaLinks.IsEmpty()) {
            continue;
        }
        if (!CreateCharClassLink(it)) {
            return false;
        }
        if (it->m_NfaLinks.IsEmpty()) {
            continue;
        }
        if (!CreateAtomicZeroWidthAssersionLink(it)) {
            return false;
        }
    }

    // (the original counts the DFA states here without using the count)
    for (DfaIterator it = m_DfaStates.Begin(); it != m_DfaStates.End(); ++it) {
        bool isAccept = false;
        for (u32 i = 0; i < m_NfaStateCount; i++) {
            if (it->m_NfaStates[i] && m_NfaStates[i] == nfaEnd) {
                isAccept = true;
                break;
            }
        }
        it->m_IsAccept = isAccept;
    }
    return true;
}

// 0x003DF038 | fefates:bytes [tier B]
nn::ngc::RegexDfaConverter::RegexDfaConverter() : m_Pool(NULL), m_NfaStateCount(0), m_NfaStates(NULL)
{
}

} // namespace ngc
} // namespace nn

#include "nn/ngc/ngc_RegexNfaParser.h"
#include "nn/ngc/ngc_Api.h"
#include "nn/ngc/ngc_RegexNfaStateCopier.h"

namespace nn {
namespace ngc {
namespace {
typedef RegexNfaParser P;
typedef UnitList<RegexStateLink<RegexNfaState> >::Iterator LinkIterator;

// by the token type
// 0x0097E8EC
RegexNfaParser::StatementFunction s_StatementFunctions[RegexToken::TYPE_COUNT] = {
    &P::ParseStatement_TokenCharacter,          // TYPE_CHARACTER
    &P::ParseStatement_TokenEndOfBranch,        // TYPE_ALTERNATION
    &P::ParseStatement_TokenAnyOf,              // TYPE_BEGIN_CHAR_CLASS
    &P::ParseStatement_TokenAnyBut,             // TYPE_BEGIN_DENIAL_CHAR_CLASS
    &P::ParseStatement_TokenEndCharClass,       // TYPE_END_CHAR_CLASS
    &P::ParseStatement_TokenCharClassOperator,  // TYPE_CHAR_CLASS_RANGE
    &P::ParseStatement_TokenCharClassOperator,  // TYPE_CHAR_CLASS_AND
    &P::ParseStatement_TokenBuiltInCharClass,   // TYPE_BUILT_IN_CHAR_CLASS
    &P::ParseStatement_TokenDenialCharClass,    // TYPE_DENIAL_CHAR_CLASS
    &P::ParseStatement_TokenBeginGroup,         // TYPE_BEGIN_GROUP
    &P::ParseStatement_TokenEndGroup,           // TYPE_END_GROUP
    &P::ParseStatement_TokenRepeat,             // TYPE_ONE_OR_ZERO
    &P::ParseStatement_TokenRepeat,             // TYPE_MORE_THAN_ZERO
    &P::ParseStatement_TokenRepeat,             // TYPE_MORE_THAN_ONE
    &P::ParseStatement_TokenRepeat,             // TYPE_REPEAT_EQUALS
    &P::ParseStatement_TokenRepeat,             // TYPE_MORE_THAN
    &P::ParseStatement_TokenRepeat,             // TYPE_RANGE
    &P::ParseStatement_TokenAtomicZeroWidth,    // TYPE_ATOMIC_ZERO_WIDTH
    &P::ParseStatement_TokenEndOfBranch,        // TYPE_END
};

// 0x0097E984
RegexNfaParser::RepeatFunction s_RepeatFunctions[RegexToken::TYPE_COUNT] = {
    &P::ParseRepeat_TokenNone,              // TYPE_CHARACTER
    &P::ParseRepeat_TokenNone,              // TYPE_ALTERNATION
    &P::ParseRepeat_TokenNone,              // TYPE_BEGIN_CHAR_CLASS
    &P::ParseRepeat_TokenNone,              // TYPE_BEGIN_DENIAL_CHAR_CLASS
    &P::ParseRepeat_TokenNone,              // TYPE_END_CHAR_CLASS
    &P::ParseRepeat_TokenNone,              // TYPE_CHAR_CLASS_RANGE
    &P::ParseRepeat_TokenNone,              // TYPE_CHAR_CLASS_AND
    &P::ParseRepeat_TokenNone,              // TYPE_BUILT_IN_CHAR_CLASS
    &P::ParseRepeat_TokenNone,              // TYPE_DENIAL_CHAR_CLASS
    &P::ParseRepeat_TokenNone,              // TYPE_BEGIN_GROUP
    &P::ParseRepeat_TokenNone,              // TYPE_END_GROUP
    &P::ParseRepeat_TokenOneOrZero,         // TYPE_ONE_OR_ZERO
    &P::ParseRepeat_TokenMoreThanZero,      // TYPE_MORE_THAN_ZERO
    &P::ParseRepeat_TokenMoreThanOne,       // TYPE_MORE_THAN_ONE
    &P::ParseRepeat_TokenRepeatEquals,      // TYPE_REPEAT_EQUALS
    &P::ParseRepeat_TokenMoreThan,          // TYPE_MORE_THAN
    &P::ParseRepeat_TokenRange,             // TYPE_RANGE
    &P::ParseRepeat_TokenNone,              // TYPE_ATOMIC_ZERO_WIDTH
    &P::ParseRepeat_TokenNone,              // TYPE_END
};
} // namespace

// 0x003DC76C | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseBranch(StateIterator* end, StateIterator start)
{
    StateIterator repeatEnd;
    StateIterator statementEnd;
    for (;;) {
        int result = (this->*s_StatementFunctions[m_Token->m_Type])(&statementEnd, start);
        if (result == RESULT_NOT_FOUND) {
            *end = start;
            return RESULT_SUCCESS;
        }
        if (result != RESULT_SUCCESS) {
            return result;
        }
        result = (this->*s_RepeatFunctions[m_Token->m_Type])(&repeatEnd, start, statementEnd);
        if (result != RESULT_NOT_FOUND && result != RESULT_SUCCESS) {
            return result;
        }
        start = repeatEnd;
    }
}

// 0x003DC864 | fefates:bytes [tier B]
nn::ngc::RegexNfaParser::StateIterator nn::ngc::RegexNfaParser::PushBackNewState()
{
    StateIterator state = m_States.PushBackNew();
    if (state.IsNull()) {
        return state;
    }
    state->m_Links.SetPool(m_Pool);
    return state;
}

// 0x003DC8CC | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseCharClassInner(nn::ngc::CharacterRangeList* list)
{
    // the characters, ranges and classes in [...] so far
    s32 count = 0;
    for (;;) {
        switch (m_Token->m_Type) {
        case RegexToken::TYPE_CHARACTER: {
            wchar_t begin = m_Token->m_Character;
            ++m_Token;
            int result;
            if (m_Token->m_Type == RegexToken::TYPE_CHAR_CLASS_RANGE) {
                ++m_Token;
                if (m_Token->m_Type != RegexToken::TYPE_CHARACTER) {
                    // "a-]": the hyphen is a character
                    result = ParseClass_RegisterChar(list, begin);
                    if (result == RESULT_SUCCESS) {
                        result = ParseClass_RegisterChar(list, L'-');
                    }
                } else {
                    wchar_t end = m_Token->m_Character;
                    if (end < begin) {
                        return RESULT_INVALID_CHAR_RANGE;
                    }
                    result = RESULT_SUCCESS;
                    for (wchar_t c = begin; c <= end; c++) {
                        if (ParseClass_RegisterChar(list, c) != RESULT_SUCCESS) {
                            result = RESULT_OUT_OF_MEMORY;
                            break;
                        }
                    }
                    if (result == RESULT_SUCCESS) {
                        ++m_Token;
                    }
                }
            } else {
                result = ParseClass_RegisterChar(list, begin);
            }
            if (result != RESULT_SUCCESS) {
                return result;
            }
            count++;
            break;
        }
        case RegexToken::TYPE_BEGIN_CHAR_CLASS: {
            ++m_Token;
            int result = ParseCharClassInner(list);
            if (result != RESULT_SUCCESS) {
                return result;
            }
            ++m_Token;
            break;
        }
        case RegexToken::TYPE_BEGIN_DENIAL_CHAR_CLASS: {
            CharacterRangeList denial;
            denial.SetPool(m_Pool);
            ++m_Token;
            int result = ParseCharClassInner(&denial);
            if (result != RESULT_SUCCESS) {
                return result;
            }
            ++m_Token;
            if (!denial.Inverse() || !list->MergeCharacterRanges(&denial)) {
                return RESULT_OUT_OF_MEMORY;
            }
            break;
        }
        case RegexToken::TYPE_END_CHAR_CLASS:
            if (count != 0) {
                return RESULT_SUCCESS;
            }
            return RESULT_EMPTY_CHAR_CLASS;
        case RegexToken::TYPE_CHAR_CLASS_RANGE:
            // a hyphen at the start is a character
            if (ParseClass_RegisterChar(list, L'-') != RESULT_SUCCESS) {
                return RESULT_OUT_OF_MEMORY;
            }
            count++;
            ++m_Token;
            break;
        case RegexToken::TYPE_CHAR_CLASS_AND: {
            ++m_Token;
            u8 type = m_Token->m_Type;
            if (count == 0) {
                if (type == RegexToken::TYPE_CHAR_CLASS_AND || type == RegexToken::TYPE_END_CHAR_CLASS) {
                    return RESULT_EMPTY_CHAR_CLASS;
                }
                break;
            }
            if (type == RegexToken::TYPE_CHAR_CLASS_AND || type == RegexToken::TYPE_END_CHAR_CLASS) {
                break;
            }
            // the rest of the class is intersected
            CharacterRangeList rest;
            rest.SetPool(m_Pool);
            int result = ParseCharClassInner(&rest);
            if (result != RESULT_SUCCESS) {
                return result;
            }
            if (!list->Intersect(&rest)) {
                return RESULT_OUT_OF_MEMORY;
            }
            break;
        }
        case RegexToken::TYPE_BUILT_IN_CHAR_CLASS:
            if (!CombineBuiltInCharClass(list, static_cast<BuiltInCharClassType>(m_Token->m_CharClassType))) {
                return RESULT_OUT_OF_MEMORY;
            }
            count++;
            ++m_Token;
            break;
        case RegexToken::TYPE_DENIAL_CHAR_CLASS: {
            CharacterRangeList denial;
            denial.SetPool(m_Pool);
            if (!CombineBuiltInCharClass(&denial, static_cast<BuiltInCharClassType>(m_Token->m_CharClassType)) || !denial.Inverse() ||
                !list->MergeCharacterRanges(&denial)) {
                return RESULT_OUT_OF_MEMORY;
            }
            count++;
            ++m_Token;
            break;
        }
        case RegexToken::TYPE_END:
            return RESULT_UNEXPECTED_END_OF_CHAR_CLASS;
        default:
            return RESULT_UNEXPECTED_TOKEN;
        }
    }
}

// 0x003DCF20 | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseRegularExpression(StateIterator* end, StateIterator start)
{
    StateIterator branchEnd;
    int result = ParseBranch(&branchEnd, start);
    if (result != RESULT_SUCCESS) {
        return result;
    }
    if (m_Token->m_Type != RegexToken::TYPE_ALTERNATION) {
        *end = branchEnd;
        return RESULT_SUCCESS;
    }
    // the branches meet in a new state
    StateIterator joint = PushBackNewState();
    if (joint.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    LinkIterator link = branchEnd->m_Links.PushBackNew();
    if (link.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    link->LinkTo(joint);
    while (m_Token->m_Type == RegexToken::TYPE_ALTERNATION) {
        ++m_Token;
        result = ParseBranch(&branchEnd, start);
        if (result != RESULT_SUCCESS) {
            return result;
        }
        link = branchEnd->m_Links.PushBackNew();
        if (link.IsNull()) {
            return RESULT_OUT_OF_MEMORY;
        }
        link->LinkTo(joint);
    }
    *end = joint;
    return RESULT_SUCCESS;
}

// 0x003DD188 | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseRepeat_TokenRange(StateIterator* end, StateIterator start, StateIterator statementEnd)
{
    // {n,m}: n copies in a row, from the n-th on each may go to the end
    StateIterator repeatEnd = PushBackNewState();
    if (repeatEnd.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    StateIterator last = statementEnd;
    for (u32 i = 1; i < m_Token->m_Max; i++) {
        RegexNfaStateCopier copier(&m_States, m_Pool);
        StateIterator copy;
        if (!copier.CopyStates(&copy, start, statementEnd)) {
            return RESULT_OUT_OF_MEMORY;
        }
        LinkIterator link = last->m_Links.PushBackNew();
        if (link.IsNull()) {
            return RESULT_OUT_OF_MEMORY;
        }
        link->LinkTo(copy);
        last = copier.GetStatePair(statementEnd)->m_Copy;
        if (i + 1 >= m_Token->m_Min) {
            link = last->m_Links.PushBackNew();
            if (link.IsNull()) {
                return RESULT_OUT_OF_MEMORY;
            }
            link->LinkTo(repeatEnd);
        }
    }
    if (m_Token->m_Min == 0) {
        LinkIterator link = start->m_Links.PushBackNew();
        if (link.IsNull()) {
            return RESULT_OUT_OF_MEMORY;
        }
        link->LinkTo(repeatEnd);
    }
    if (m_Token->m_Min <= 1 && m_Token->m_Max != 0) {
        LinkIterator link = statementEnd->m_Links.PushBackNew();
        if (link.IsNull()) {
            return RESULT_OUT_OF_MEMORY;
        }
        link->LinkTo(repeatEnd);
    }
    *end = repeatEnd;
    ++m_Token;
    return RESULT_SUCCESS;
}

// 0x003DD4D0 | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseClass_RegisterChar(nn::ngc::CharacterRangeList* list, wchar_t character)
{
    if (!list->AddCharacterRange(character, character)) {
        return RESULT_OUT_OF_MEMORY;
    }
    const CaseFoldingPair* pair = GetCaseFoldingPair(character);
    if (pair != NULL) {
        for (s32 i = 0; i < 3; i++) {
            if (pair->m_Pairs[i] == 0) {
                break;
            }
            if (!list->AddCharacterRange(pair->m_Pairs[i], pair->m_Pairs[i])) {
                return RESULT_OUT_OF_MEMORY;
            }
        }
    }
    return RESULT_SUCCESS;
}

// 0x003DD548 (name is ours)
int nn::ngc::RegexNfaParser::ParseRepeat_TokenNone(StateIterator* end, StateIterator start, StateIterator statementEnd)
{
    *end = statementEnd;
    return RESULT_NOT_FOUND;
}

// 0x003DD558 | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseRepeat_TokenMoreThan(StateIterator* end, StateIterator start, StateIterator statementEnd)
{
    // {n,}: n copies in a row, the last one loops
    if (m_Token->m_Min == 0) {
        LinkIterator link = statementEnd->m_Links.PushBackNew();
        if (link.IsNull()) {
            return RESULT_OUT_OF_MEMORY;
        }
        link->LinkTo(start);
        *end = start;
        ++m_Token;
        return RESULT_SUCCESS;
    }
    StateIterator loopStart = start;
    StateIterator last = statementEnd;
    for (s32 i = 1; i < m_Token->m_Min; i++) {
        RegexNfaStateCopier copier(&m_States, m_Pool);
        StateIterator copy;
        if (!copier.CopyStates(&copy, start, statementEnd)) {
            return RESULT_OUT_OF_MEMORY;
        }
        LinkIterator link = last->m_Links.PushBackNew();
        if (link.IsNull()) {
            return RESULT_OUT_OF_MEMORY;
        }
        link->LinkTo(copy);
        loopStart = copy;
        last = copier.GetStatePair(statementEnd)->m_Copy;
    }
    LinkIterator link = last->m_Links.PushBackNew();
    if (link.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    link->LinkTo(loopStart);
    *end = last;
    ++m_Token;
    return RESULT_SUCCESS;
}

// 0x003DD7D4 | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseStatement_TokenAnyOf(StateIterator* end, StateIterator start)
{
    StateIterator state = PushBackNewState();
    if (state.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    LinkIterator link = start->m_Links.PushBackNew();
    if (link.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    link->LinkTo(state, m_Pool);
    ++m_Token;
    int result = ParseCharClassInner(&link->m_CharClass);
    if (result != RESULT_SUCCESS) {
        return result;
    }
    *end = state;
    ++m_Token;
    return RESULT_SUCCESS;
}

// 0x003DD884 | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseRepeat_TokenOneOrZero(StateIterator* end, StateIterator start, StateIterator statementEnd)
{
    LinkIterator link = start->m_Links.PushBackNew();
    if (link.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    link->LinkTo(statementEnd);
    *end = statementEnd;
    ++m_Token;
    return RESULT_SUCCESS;
}

// 0x003DD8EC | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseStatement_TokenAnyBut(StateIterator* end, StateIterator start)
{
    StateIterator state = PushBackNewState();
    if (state.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    LinkIterator link = start->m_Links.PushBackNew();
    if (link.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    link->LinkTo(state, m_Pool);
    ++m_Token;
    int result = ParseCharClassInner(&link->m_CharClass);
    if (result != RESULT_SUCCESS) {
        return result;
    }
    if (!link->m_CharClass.Inverse()) {
        return RESULT_OUT_OF_MEMORY;
    }
    *end = state;
    ++m_Token;
    return RESULT_SUCCESS;
}

// 0x003DD9B0 (name is ours)
int nn::ngc::RegexNfaParser::ParseStatement_TokenEndOfBranch(StateIterator* end, StateIterator start)
{
    *end = start;
    return RESULT_NOT_FOUND;
}

// 0x003DD9C0 (name is ours)
int nn::ngc::RegexNfaParser::ParseStatement_TokenCharClassOperator(StateIterator* end, StateIterator start)
{
    return RESULT_UNEXPECTED_TOKEN;
}

// 0x003DD9C8 | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseRepeat_TokenMoreThanOne(StateIterator* end, StateIterator start, StateIterator statementEnd)
{
    LinkIterator link = statementEnd->m_Links.PushBackNew();
    if (link.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    link->LinkTo(start);
    *end = statementEnd;
    ++m_Token;
    return RESULT_SUCCESS;
}

// 0x003DDA34 (name is ours)
int nn::ngc::RegexNfaParser::ParseStatement_TokenEndCharClass(StateIterator* end, StateIterator start)
{
    return RESULT_UNEXPECTED_END_OF_CHAR_CLASS;
}

// 0x003DDA3C | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseStatement_TokenEndGroup(StateIterator* end, StateIterator start)
{
    if (m_GroupDepth == 0) {
        return RESULT_UNMATCHED_GROUP;
    }
    return RESULT_NOT_FOUND;
}

// 0x003DDA50 | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseRepeat_TokenMoreThanZero(StateIterator* end, StateIterator start, StateIterator statementEnd)
{
    LinkIterator link = statementEnd->m_Links.PushBackNew();
    if (link.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    link->LinkTo(start);
    *end = start;
    ++m_Token;
    return RESULT_SUCCESS;
}

// 0x003DDAB8 | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseRepeat_TokenRepeatEquals(StateIterator* end, StateIterator start, StateIterator statementEnd)
{
    if (m_Token->m_Min == 0) {
        // {0}: the statement is dropped
        while (!start->m_Links.IsEmpty()) {
            start->m_Links.Erase(start->m_Links.Begin());
        }
        LinkIterator link = start->m_Links.PushBackNew();
        if (link.IsNull()) {
            return RESULT_OUT_OF_MEMORY;
        }
        link->LinkTo(statementEnd);
        *end = statementEnd;
        ++m_Token;
        return RESULT_SUCCESS;
    }
    StateIterator last = statementEnd;
    for (s32 i = 1; i < m_Token->m_Min; i++) {
        RegexNfaStateCopier copier(&m_States, m_Pool);
        StateIterator copy;
        if (!copier.CopyStates(&copy, start, statementEnd)) {
            return RESULT_OUT_OF_MEMORY;
        }
        LinkIterator link = last->m_Links.PushBackNew();
        if (link.IsNull()) {
            return RESULT_OUT_OF_MEMORY;
        }
        link->LinkTo(copy);
        last = copier.GetStatePair(statementEnd)->m_Copy;
    }
    *end = last;
    ++m_Token;
    return RESULT_SUCCESS;
}

// 0x003DDD4C | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseStatement_TokenCharacter(StateIterator* end, StateIterator start)
{
    StateIterator state = PushBackNewState();
    if (state.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    // the character and the ones of the other cases
    const CaseFoldingPair* pair = GetCaseFoldingPair(m_Token->m_Character);
    LinkIterator link = start->m_Links.PushBackNew();
    if (link.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    link->LinkTo(state, m_Token->m_Character);
    if (pair != NULL) {
        for (s32 i = 0; i < 3; i++) {
            if (pair->m_Pairs[i] == 0) {
                break;
            }
            link = start->m_Links.PushBackNew();
            if (link.IsNull()) {
                return RESULT_OUT_OF_MEMORY;
            }
            link->LinkTo(state, pair->m_Pairs[i]);
        }
    }
    *end = state;
    ++m_Token;
    return RESULT_SUCCESS;
}

// 0x003DDE78 | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseStatement_TokenBeginGroup(StateIterator* end, StateIterator start)
{
    ++m_Token;
    m_GroupDepth++;
    int result = ParseRegularExpression(end, start);
    if (result != RESULT_SUCCESS) {
        return result;
    }
    if (m_Token->m_Type != RegexToken::TYPE_END_GROUP) {
        return RESULT_UNMATCHED_GROUP;
    }
    m_GroupDepth--;
    ++m_Token;
    return RESULT_SUCCESS;
}

// 0x003DDEE4 (name is ours)
int nn::ngc::RegexNfaParser::ParseStatement_TokenRepeat(StateIterator* end, StateIterator start)
{
    return RESULT_NOTHING_TO_REPEAT;
}

// 0x003DDEEC | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseStatement_TokenAtomicZeroWidth(StateIterator* end, StateIterator start)
{
    StateIterator state = PushBackNewState();
    if (state.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    LinkIterator link = start->m_Links.PushBackNew();
    if (link.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    link->LinkTo(state, static_cast<AtomicZeroWidthAssersionType>(m_Token->m_AssertionType));
    *end = state;
    ++m_Token;
    return RESULT_SUCCESS;
}

// 0x003DDF7C | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseStatement_TokenDenialCharClass(StateIterator* end, StateIterator start)
{
    StateIterator state = PushBackNewState();
    if (state.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    LinkIterator link = start->m_Links.PushBackNew();
    if (link.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    link->LinkTo(state, m_Pool);
    if (!CombineBuiltInCharClass(&link->m_CharClass, static_cast<BuiltInCharClassType>(m_Token->m_CharClassType))) {
        return RESULT_OUT_OF_MEMORY;
    }
    if (!link->m_CharClass.Inverse()) {
        return RESULT_OUT_OF_MEMORY;
    }
    *end = state;
    ++m_Token;
    return RESULT_SUCCESS;
}

// 0x003DE038 | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::ParseStatement_TokenBuiltInCharClass(StateIterator* end, StateIterator start)
{
    StateIterator state = PushBackNewState();
    if (state.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    LinkIterator link = start->m_Links.PushBackNew();
    if (link.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    link->LinkTo(state, m_Pool);
    if (!CombineBuiltInCharClass(&link->m_CharClass, static_cast<BuiltInCharClassType>(m_Token->m_CharClassType))) {
        return RESULT_OUT_OF_MEMORY;
    }
    *end = state;
    ++m_Token;
    return RESULT_SUCCESS;
}

// 0x003DE0E0 | fefates:bytes [tier B]
int nn::ngc::RegexNfaParser::Parse(nn::ngc::ProfanityFilterTemporaryPool* pool, const nn::ngc::UnitList<nn::ngc::RegexToken>* tokens)
{
    m_Token = tokens->Begin();
    m_States.SetPool(pool);
    m_Pool = pool;
    StateIterator start = PushBackNewState();
    if (start.IsNull()) {
        return RESULT_OUT_OF_MEMORY;
    }
    int result = ParseRegularExpression(&m_End, start);
    if (result != RESULT_SUCCESS) {
        return result;
    }
    u32 index = 0;
    for (StateIterator it = m_States.Begin(); it != m_States.End(); ++it) {
        it->m_Index = index++;
    }
    return RESULT_SUCCESS;
}

// 0x003DE19C | fefates:bytes [tier B]
nn::ngc::RegexNfaParser::RegexNfaParser() : m_Pool(NULL), m_GroupDepth(0)
{
}

} // namespace ngc
} // namespace nn

#pragma once

#include "decomp.h"
#include "nn/ngc/ngc_CharacterRangeList.h"
#include "nn/ngc/ngc_RegexNfaState.h"
#include "nn/ngc/ngc_RegexToken.h"
#include "nn/ngc/ngc_UnitList.h"

namespace nn {
namespace ngc {
// Builds the NFA of a token list (Thompson construction). A statement function makes the states
// of the token at m_Token after the state start and gives the last one; a repeat function
// repeats the states from start to end for the quantifier at m_Token. Both are chosen by the
// token type (s_StatementFunctions, s_RepeatFunctions) and return 0, 1 (no statement / no
// quantifier here) or an error (the RESULT_ values are ours). Member names are ours.
class RegexNfaParser
{
public:
    typedef UnitList<RegexNfaState>::Iterator StateIterator;
    typedef int (RegexNfaParser::*StatementFunction)(StateIterator* end, StateIterator start);
    typedef int (RegexNfaParser::*RepeatFunction)(StateIterator* end, StateIterator start, StateIterator statementEnd);

    static const int RESULT_SUCCESS = 0;
    static const int RESULT_NOT_FOUND = 1;
    static const int RESULT_OUT_OF_MEMORY = 2;
    static const int RESULT_UNMATCHED_GROUP = 3;
    static const int RESULT_NOTHING_TO_REPEAT = 4;
    static const int RESULT_UNEXPECTED_END_OF_CHAR_CLASS = 5;
    static const int RESULT_EMPTY_CHAR_CLASS = 6;
    static const int RESULT_INVALID_CHAR_RANGE = 7;
    static const int RESULT_UNEXPECTED_TOKEN = 8;

    RegexNfaParser(); // 0x003DE19C | fefates:bytes [tier B]
    int Parse(nn::ngc::ProfanityFilterTemporaryPool* pool, const nn::ngc::UnitList<nn::ngc::RegexToken>* tokens); // 0x003DE0E0 | fefates:bytes [tier B]

    int ParseBranch(StateIterator* end, StateIterator start); // 0x003DC76C | fefates:bytes [tier B]
    StateIterator PushBackNewState(); // 0x003DC864 | fefates:bytes [tier B]
    int ParseCharClassInner(nn::ngc::CharacterRangeList* list); // 0x003DC8CC | fefates:bytes [tier B]
    int ParseRegularExpression(StateIterator* end, StateIterator start); // 0x003DCF20 | fefates:bytes [tier B]
    int ParseRepeat_TokenRange(StateIterator* end, StateIterator start, StateIterator statementEnd); // 0x003DD188 | fefates:bytes [tier B]
    int ParseClass_RegisterChar(nn::ngc::CharacterRangeList* list, wchar_t character); // 0x003DD4D0 | fefates:bytes [tier B]
    int ParseRepeat_TokenNone(StateIterator* end, StateIterator start, StateIterator statementEnd); // 0x003DD548 (name is ours)
    int ParseRepeat_TokenMoreThan(StateIterator* end, StateIterator start, StateIterator statementEnd); // 0x003DD558 | fefates:bytes [tier B]
    int ParseStatement_TokenAnyOf(StateIterator* end, StateIterator start); // 0x003DD7D4 | fefates:bytes [tier B]
    int ParseRepeat_TokenOneOrZero(StateIterator* end, StateIterator start, StateIterator statementEnd); // 0x003DD884 | fefates:bytes [tier B]
    int ParseStatement_TokenAnyBut(StateIterator* end, StateIterator start); // 0x003DD8EC | fefates:bytes [tier B]
    int ParseStatement_TokenEndOfBranch(StateIterator* end, StateIterator start); // 0x003DD9B0 (name is ours)
    int ParseStatement_TokenCharClassOperator(StateIterator* end, StateIterator start); // 0x003DD9C0 (name is ours)
    int ParseRepeat_TokenMoreThanOne(StateIterator* end, StateIterator start, StateIterator statementEnd); // 0x003DD9C8 | fefates:bytes [tier B]
    int ParseStatement_TokenEndCharClass(StateIterator* end, StateIterator start); // 0x003DDA34 (name is ours)
    int ParseStatement_TokenEndGroup(StateIterator* end, StateIterator start); // 0x003DDA3C | fefates:bytes [tier B]
    int ParseRepeat_TokenMoreThanZero(StateIterator* end, StateIterator start, StateIterator statementEnd); // 0x003DDA50 | fefates:bytes [tier B]
    int ParseRepeat_TokenRepeatEquals(StateIterator* end, StateIterator start, StateIterator statementEnd); // 0x003DDAB8 | fefates:bytes [tier B]
    int ParseStatement_TokenCharacter(StateIterator* end, StateIterator start); // 0x003DDD4C | fefates:bytes [tier B]
    int ParseStatement_TokenBeginGroup(StateIterator* end, StateIterator start); // 0x003DDE78 | fefates:bytes [tier B]
    int ParseStatement_TokenRepeat(StateIterator* end, StateIterator start); // 0x003DDEE4 (name is ours)
    int ParseStatement_TokenAtomicZeroWidth(StateIterator* end, StateIterator start); // 0x003DDEEC | fefates:bytes [tier B]
    int ParseStatement_TokenDenialCharClass(StateIterator* end, StateIterator start); // 0x003DDF7C | fefates:bytes [tier B]
    int ParseStatement_TokenBuiltInCharClass(StateIterator* end, StateIterator start); // 0x003DE038 | fefates:bytes [tier B]

    UnitList<RegexToken>::ConstIterator m_Token;    // 0x00
    UnitList<RegexNfaState> m_States;               // 0x04, the first one is the start state
    StateIterator m_End;                            // 0x10, the end state
    ProfanityFilterTemporaryPool* m_Pool;           // 0x14
    s32 m_GroupDepth;                               // 0x18
};
ASSERT_SIZE(RegexNfaParser, 0x1C);
} // namespace ngc
} // namespace nn

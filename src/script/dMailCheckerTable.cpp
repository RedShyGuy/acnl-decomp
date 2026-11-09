#include "script/dMailChecker.h"

namespace script {
// the checks of the evaluation (in a file of their own, otherwise GCC calls them directly)
// 0x008954D8
const MailChecker::Evaluator::CheckFunc MailChecker::Evaluator::s_Checks[CHECK_NUM] = {
    &MailChecker::Evaluator::CheckRequired,
    &MailChecker::Evaluator::CheckMain,
    &MailChecker::Evaluator::CheckExtra,
    &MailChecker::Evaluator::CheckHeader,
    &MailChecker::Evaluator::CheckTownName,
};

// 0x00891BF0
const s32 MailChecker::Evaluator::s_ListIndices[3][3] = {
    {0, 0, 0},
    {1, 1, 1},
    {2, 2, 2},
};

// 0x00891C14
const char* const MailChecker::Evaluator::s_ListNames[3] = {
    "STR_Mailchk_Bday",
    "STR_Mailchk_Move",
    "STR_Mailchk_Winter",
};
} // namespace script

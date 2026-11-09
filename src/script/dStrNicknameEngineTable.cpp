#include "script/dStrNicknameEngine.h"

namespace script {
// the handlers of the tags of group 6 (in a file of its own, otherwise GCC calls them directly)
// 0x0089268C
const StrNicknameEngine::InsertFunc StrNicknameEngine::s_InsertFuncs[5] = {
    &StrNicknameEngine::InsertFirstChar, &StrNicknameEngine::InsertSecondChar, &StrNicknameEngine::InsertThirdChar,
    &StrNicknameEngine::InsertLastChar,  &StrNicknameEngine::InsertSecondLastChar,
};
} // namespace script

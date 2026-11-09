#include "script/dCmpEngine.h"
#include <cstring>

namespace script {
// 0x006006E4 (name is ours)
bool CmpEngine::Compare(const IWord* a, const IWord* b)
{
    if (a == b) {
        return true;
    }
    const char16* textA = a->GetWord();
    s32 lengthA = -1;
    if (textA != NULL && a->GetSize() != 0) {
        Run(textA);
        lengthA = m_pCur - textA;
    }
    const char16* textB = b->GetWord();
    s32 lengthB = -1;
    if (textB != NULL && b->GetSize() != 0) {
        Run(textB);
        lengthB = m_pCur - textB;
    }
    if (lengthA == lengthB && lengthA >= 0 && std::memcmp(textA, textB, lengthA * sizeof(char16)) == 0) {
        return true;
    }
    return false;
}

// 0x0060080C slot 0x00
// 0x006007FC (deleting dtor)
CmpEngine::~CmpEngine()
{
    // (only the base is destroyed)
}
} // namespace script

#include "script/dEpenthesisEngine.h"

namespace script {
namespace {
// the a vowels (with their accents), small and capital
// 0x00892914
const char16 s_EpenthesisCharsA[16] = {
    0x00E6, 0x00C6, 0x0061, 0x0041, 0x00E1, 0x00C1, 0x00E0, 0x00C0,
    0x00E2, 0x00C2, 0x00E4, 0x00C4, 0x00E3, 0x00C3, 0x00E5, 0x00C5,
};
// the e vowels
// 0x00892934
const char16 s_EpenthesisCharsE[10] = {
    0x0065, 0x0045, 0x00E9, 0x00C9, 0x00E8, 0x00C8, 0x00EA, 0x00CA,
    0x00EB, 0x00CB,
};
} // namespace

// 0x005E71CC slot 0x14
void EpenthesisEngine::OnChar(const CharInfo* info)
{
    if (info->pText != NULL) {
        char16 c = *info->pText;
        if (m_IsE) {
            for (u32 i = 0; i < sizeof(s_EpenthesisCharsE) / sizeof(s_EpenthesisCharsE[0]); i++) {
                if (s_EpenthesisCharsE[i] == c) {
                    m_IsMatch = 1;
                    break;
                }
            }
        } else {
            for (u32 i = 0; i < sizeof(s_EpenthesisCharsA) / sizeof(s_EpenthesisCharsA[0]); i++) {
                if (s_EpenthesisCharsA[i] == c) {
                    m_IsMatch = 1;
                    break;
                }
            }
        }
    }
    m_IsDone = 1;
}

// 0x005E7278 slot 0x10
bool EpenthesisEngine::IsContinue()
{
    return m_IsDone == 0;
}

// 0x005E729C slot 0x00
// 0x005E728C (deleting dtor)
EpenthesisEngine::~EpenthesisEngine()
{
    // (only the base is destroyed)
}
} // namespace script

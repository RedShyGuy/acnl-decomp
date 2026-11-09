#include "script/dCmpPartFuzzyEngine.h"
#include "script/dWordCPtr.h"

namespace script {
// 0x005E9948 slot 0x10
bool CmpPartFuzzyEngine::IsContinue()
{
    WordCPtr rest(m_pCur, m_Size - (m_pCur - m_pText));
    const char16* text = rest.GetWord();
    u32 size = rest.GetSize();
    if (text != NULL && size != 0 && *text != 0) {
        EngineA engine;
        engine.m_B.m_IsFound = false;
        if (&rest == m_pWord) {
            engine.m_B.m_IsFound = true;
        } else {
            const char16* textA = rest.GetWord();
            u32 sizeA = rest.GetSize();
            const char16* textB = m_pWord->GetWord();
            u32 sizeB = m_pWord->GetSize();
            if (textA != NULL && sizeA != 0 && textB != NULL && sizeB != 0) {
                engine.SetText(&textA, &s_pNoStop);
                engine.m_B.SetText(&textB, &s_pNoStop);
                engine.m_B.m_IsFound = true;
                engine.m_B.m_IsEnd = false;
                engine.m_B.m_IsEqual = true;
                engine.SetText2(&s_pNoStop);
                if (engine.m_B.m_IsFound) {
                    // the whole word must have been compared
                    const char16* wordRest = engine.m_B.m_pCur;
                    if (wordRest == NULL || *wordRest != 0) {
                        engine.m_B.m_IsFound = false;
                    }
                }
            }
        }
        if (engine.m_B.m_IsFound) {
            m_IsFound = 1;
        }
    }
    return m_IsFound == 0;
}

// 0x005E9B30 slot 0x00
// 0x005E9B20 (deleting dtor)
CmpPartFuzzyEngine::~CmpPartFuzzyEngine()
{
    // (only the base is destroyed)
}
} // namespace script

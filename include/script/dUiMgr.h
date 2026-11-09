#pragma once

#include "decomp.h"
#include "script/dIUiRecept.h"
#include "script/dInsert.h"
#include "script/dStrMgr.h"
#include "script/dUiInsert.h"
#include "script/dUiPhrase.h"
#include "script/dUiSeq.h"
#include "script/dUiSystem.h"
#include "time/time_Api.h"

namespace script {
class IWord;
class UiPhrase;

// The texts of the user interface with inserted words: a singleton whose SingletonDisposer_ lies in its first 16 bytes
// (the game locks it with Begin/End around ReadWord; names are ours).
class UiMgr
{
public:
    class SingletonDisposer_;

    // (inline in the setup)
    UiMgr() : m_pRecept(NULL) {}

    // the recept of the next texts (NULL: none)
    DECOMP_EMIT_COPY void Begin(IUiRecept* recept) // 0x005EB3C8
    {
        m_pRecept = recept;
        m_System.m_pRecept = recept;
        if (recept != NULL) {
            recept->m_pOwner = &m_System;
        }
        m_System.m_Unknown0x10 = 1;
        *m_System.m_pDate = *time::GetCurrentDate();
        Insert* insert = m_System.m_pPhrase->GetInsert();
        if (insert != NULL) {
            insert->Clear();
        }
        m_System.m_Text.Clear();
    }
    DECOMP_EMIT_COPY void End() // 0x005EB3AC
    {
        IUiRecept* recept = m_System.m_pRecept;
        if (recept != NULL) {
            recept->m_pOwner = NULL;
            m_System.m_pRecept = NULL;
        }
        m_pRecept = NULL;
    }
    // the string of the list and index of the string lists with the inserted words into word; true: found
    DECOMP_EMIT_COPY bool ReadWord(IWord* word, const char* label, u32 index, u32 isColored) // 0x005EB434
    {
        bool result = false;
        if (StrMgr::s_pInstance->GetString(&m_System.m_Text, label, index) &&
            m_System.m_pSeq->Run(word, &m_System.m_Text, isColored)) {
            result = true;
        }
        return result;
    }
    // the string of the list and label with the inserted words into word; true: found
    bool ReadWordByLabel(IWord* word, const char* name, const char* label, u32 isColored); // 0x005EB26C
    // the string of the list and number (label "%03d") with the inserted words into word; true: found
    bool ReadWordNumbered(IWord* word, const char* name, u32 number, u32 isColored); // 0x005EB30C
    DECOMP_EMIT_COPY UiPhrase* GetPhrase() { return m_System.m_pPhrase; } // 0x005EB488

    static UiMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4];  // 0x000 the SingletonDisposer_
    UiSystem m_System;                 // 0x010
    IUiRecept* m_pRecept;              // 0xB34
};
ASSERT_SIZE(UiMgr, 0xB38);

// (inline; defined here, where the parts are complete)
inline UiSystem::~UiSystem()
{
    delete m_pDate;
    delete m_pSeq;
    delete m_pPhrase;
    delete m_pInsert;
}
} // namespace script

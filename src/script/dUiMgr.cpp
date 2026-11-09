#include "script/dUiMgr.h"
#include "script/dIUiRecept.h"
#include "script/dInsert.h"
#include "script/dStrMgr.h"
#include "script/dUiPhrase.h"
#include "script/dUiSeq.h"
#include "script/dWordRes.h"
#include "time/time_Api.h"

namespace script {
// 0x0095EEEC
UiMgr* UiMgr::s_pInstance = NULL;
// 0x0095EEF0
UiMgr::SingletonDisposer_* UiMgr::s_pSingletonDisposer = NULL;

// 0x005EB26C (name is ours)
bool UiMgr::ReadWordByLabel(IWord* word, const char* name, const char* label, u32 isColored)
{
    bool result = false;
    WordRes res;
    bool isFound = StrMgr::s_pInstance->FindByLabel(&res, name, label);
    if (isFound) {
        m_System.m_Text.Copy(res);
    } else {
        m_System.m_Text.Clear();
    }
    if (isFound && m_System.m_pSeq->Run(word, &m_System.m_Text, isColored)) {
        result = true;
    }
    return result;
}

// 0x005EB30C (name is ours)
bool UiMgr::ReadWordNumbered(IWord* word, const char* name, u32 number, u32 isColored)
{
    bool result = false;
    WordRes res;
    bool isFound = StrMgr::s_pInstance->FindNumbered(&res, name, number);
    if (isFound) {
        m_System.m_Text.Copy(res);
    } else {
        m_System.m_Text.Clear();
    }
    if (isFound && m_System.m_pSeq->Run(word, &m_System.m_Text, isColored)) {
        result = true;
    }
    return result;
}
} // namespace script

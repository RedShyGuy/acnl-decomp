#include "script/dDialogCompo_TextRecept.h"
#include "script/dMsgEngine.h"
#include "script/dUiSeq.h"
#include "script/dUiSystem.h"

namespace script {
// 0x005D91A8 slot 0x98 (name is ours)
bool DialogCompo::TextRecept::ProcessTag98(const Tag* tag)
{
    UiSeq* seq = m_pOwner->m_pSeq;
    seq->m_IsSuccess = PutTag(&seq->m_pWrite, &seq->m_pLast, tag) & seq->m_IsSuccess;
    return seq->m_IsSuccess;
}

// 0x005D91D8 slot 0x94 (name is ours)
bool DialogCompo::TextRecept::ProcessTag94(const Tag* tag)
{
    UiSeq* seq = m_pOwner->m_pSeq;
    seq->m_IsSuccess = PutTag(&seq->m_pWrite, &seq->m_pLast, tag) & seq->m_IsSuccess;
    return seq->m_IsSuccess;
}

// 0x005D920C slot 0x00 (name after the RTTI)
// 0x005D9208 (deleting dtor)
DialogCompo::TextRecept::~TextRecept()
{
    // empty (in the original too)
}
} // namespace script

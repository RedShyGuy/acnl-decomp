#include "script/dUiSystem.h"
#include "script/dUiInsert.h"
#include "script/dUiPhrase.h"
#include "script/dUiSeq.h"
#include "time/time_Api.h"

#include <new>

namespace script {
// 0x005FD430 (name is ours)
UiSystem::UiSystem()
    : m_pInsert(new (std::nothrow) UiInsert(this)), m_pPhrase(new (std::nothrow) UiPhrase(m_pInsert)),
      m_pSeq(new (std::nothrow) UiSeq(this)),
      m_pDate(new (std::nothrow) nn::fnd::DateTimeParameters(time::GetFirstDate())), m_Unknown0x10(1), m_pRecept(NULL),
      m_Text(m_TextBuffer, TEXT_SIZE), m_TextBuffer()
{
    m_Text.Clear();
}

} // namespace script

#include "script/dIMailRecept.h"

namespace script {
// 0x005DDF60 (name after the RTTI)
IMailRecept::IMailRecept()
    : m_pToWord(NULL), m_pBodyWord(NULL), m_pFromWord(NULL), m_pToResult(NULL), m_ToFile(), m_BodyFile(), m_FromFile(),
      m_Mode(MODE_NONE), m_Status(STATUS_NONE)
{
    m_Labels[0] = -1;
    m_Labels[1] = -1;
    m_Labels[2] = -1;
    m_Labels[3] = -1;
    m_Labels[4] = -1;
}

// 0x005DDFDC slot 0x00 (name after the RTTI)
// 0x005DDFCC (deleting dtor)
IMailRecept::~IMailRecept()
{
    // (only the members are destroyed)
}
} // namespace script

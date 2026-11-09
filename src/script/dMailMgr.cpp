#include "script/dMailMgr.h"
#include "script/dIMailRecept.h"

#include <new>

namespace script {
// 0x0095EEE4
MailMgr* MailMgr::s_pInstance = NULL;
// 0x0095EEE8
MailMgr::SingletonDisposer_* MailMgr::s_pSingletonDisposer = NULL;

// 0x005F57B8 (name is ours)
void MailMgr::End()
{
    m_pRecept = NULL;
    m_CriticalSection.unlock();
}

// 0x005F57C8 (name is ours)
void MailMgr::Run()
{
    IMailRecept* recept = m_pRecept;
    recept->m_Status = IMailRecept::STATUS_RUNNING;
    switch (recept->m_Mode) {
    case IMailRecept::MODE_NORMAL:
        m_System.m_State = MailSystem::STATE_NORMAL;
        break;
    case IMailRecept::MODE_RANDOM:
        m_System.m_State = MailSystem::STATE_RANDOM;
        break;
    case IMailRecept::MODE_BBS:
        m_System.m_State = MailSystem::STATE_BBS;
        break;
    }
    m_System.m_pRecept = recept;
    while (m_System.m_State < MailSystem::STATE_NONE) {
        (m_System.*MailSystem::s_StateFuncs[m_System.m_State])();
    }
    m_System.m_pRecept = NULL;
    recept->m_Status = m_System.m_IsSuccess ? IMailRecept::STATUS_DONE : IMailRecept::STATUS_FAILED;
}

// 0x00601E28 (name is ours)
void MailMgr::Begin(IMailRecept* recept)
{
    m_CriticalSection.lock();
    m_pRecept = recept;
    m_System.Reset();
}
} // namespace script

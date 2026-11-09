#pragma once

#include "decomp.h"
#include "script/dMailChecker.h"
#include "script/dMailSystem.h"
#include "sead/seadCriticalSection.h"

#include <new>

namespace script {

// The letters: a singleton whose SingletonDisposer_ lies in its first 16 bytes; the game writes a letter with
// Begin/Run/End (locked by a critical section). Names are ours.
class MailMgr
{
public:
    class SingletonDisposer_;

    // (inline in the setup)
    MailMgr() : m_IsLoaded(0), m_pRecept(NULL), m_pChecker(new (std::nothrow) MailChecker()) {}
    // (inline; the checker is freed without a destructor)
    ~MailMgr() { ::operator delete(m_pChecker); }

    // the recept of the next letter (the critical section is locked until End)
    void Begin(IMailRecept* recept); // 0x00601E28
    void End(); // 0x005F57B8
    // the letter of the recept is written (IMailRecept::m_Status: the result)
    void Run(); // 0x005F57C8
    DECOMP_EMIT_COPY MailChecker* GetChecker() { return m_pChecker; } // 0x005F56E8

    static MailMgr* s_pInstance;
    static SingletonDisposer_* s_pSingletonDisposer;

    u32 m_SingletonDisposerBuffer[4];  // 0x000 the SingletonDisposer_
    u8 m_IsLoaded;                     // 0x010
    MailSystem m_System;               // 0x014
    IMailRecept* m_pRecept;            // 0x198
    sead::CriticalSection m_CriticalSection; // 0x19C
    MailChecker* m_pChecker;           // 0x1B8 (STR_Mailchk; 0x608 bytes)
};
ASSERT_SIZE(MailMgr, 0x1BC);
} // namespace script

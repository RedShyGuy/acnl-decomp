#include "Sv/dSvStepInner.h"

#include "Other/dHeapInit.h"
#include "Other/dProcThread.h"
#include "Other/dSaveProcess.h"
#include "Sv/dSvApi.h"
#include "Sv/dSvProc.h"
#include "Sv/dSvSaveMemory.h"
#include "applet/dErrEulaMgr.h"
#include "sead/seadExpHeap.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadNew.h"
#include "time/time_Api.h"

// 0x00304AA0 (name is ours)
SvStepInner::SvStepInner(u64 flags)
    : m_pProcess(NULL), m_Flags(flags), m_BeforeLoad(NULL), m_AfterSave(NULL), m_OnSuccess(NULL), m_OnFailure(NULL),
      m_AfterLoad(NULL), m_BeforeCommit(NULL), m_OnEnd(NULL), m_Result(0), m_IsDone(false), m_Detail(-1)
{
}

// 0x00304AE0 (name is ours)
SvStepInner::~SvStepInner()
{
    if (m_pProcess != NULL && !SvTryReleaseSaveThread()) {
        SaveProcess* process = m_pProcess;
        if (process->m_pRequest != NULL) {
            process->m_pRequest->m_pProcess = NULL;
        }
        g_SvDummyRequest.m_pProcess = process;
        process->m_pRequest = &g_SvDummyRequest;
    }
}

// 0x005CDD40 (name is ours)
void SvStepInner::Reset()
{
    if (m_pProcess != NULL && !SvTryReleaseSaveThread()) {
        SaveProcess* process = m_pProcess;
        if (process->m_pRequest != NULL) {
            process->m_pRequest->m_pProcess = NULL;
        }
        g_SvDummyRequest.m_pProcess = process;
        process->m_pRequest = &g_SvDummyRequest;
        m_pProcess = NULL;
    }
    m_Flags = FLAG_KEEP_ERROR;
    m_BeforeLoad = NULL;
    m_AfterSave = NULL;
    m_OnSuccess = NULL;
    m_OnFailure = NULL;
    m_AfterLoad = NULL;
    m_BeforeCommit = NULL;
    m_OnEnd = NULL;
    m_Result = 0;
    m_IsDone = false;
    m_Detail = -1;
}

// 0x003047E8 (name is ours)
bool SvStepInner::Start()
{
    if (!SvTryReleaseSaveThread() || applet::ErrEulaMgr::s_pInstance->m_FB0) {
        return false;
    }
    if (applet::ErrEulaMgr::IsInputBlocked()) {
        return false;
    }
    g_SvWasHeapLockEnabled = sead::HeapMgr::s_pInstance->getCurrentHeap()->isEnableLock();
    sead::HeapMgr::s_pInstance->getCurrentHeap()->mFlag |= sead::Heap::FLAG_ENABLE_LOCK;
    ProcThread* thread = new (HeapInit::s_pSaveHeap, 4)
        ProcThread(sead::SafeStringBase<char>("SvProc Thread"), HeapInit::s_pSaveHeap, sead::Thread::DEFAULT_PRIORITY + 1,
                   this, sead::HeapMgr::s_pInstance->getCurrentHeap());
    g_pSvProcThread = thread;
    if (m_Flags & FLAG_CLEAR_SAVE_MEMORY) {
        g_SvIsSaveMemoryValid = 0;
        SvClearSaveData(SvGetSaveMemoryChecked());
        SvResetSaveData(SvGetSaveMemoryChecked());
    }
    if (m_Flags & FLAG_FREEZE_TIME_MASK) {
        time::Freeze();
    }
    if (m_Flags & FLAG_SAVING_MASK) {
        g_SvIsSaving = 1;
    }
    if (m_Flags & FLAG_LOCK_MASK) {
        g_SvIsSaveMemoryLocked = 1;
    }
    g_pSvProcThread->start();
    return true;
}

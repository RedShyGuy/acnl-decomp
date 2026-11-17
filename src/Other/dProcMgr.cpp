#include "Other/dProcMgr.h"

#include "oml/framework/dProcess.h"
#include "oml/framework/dProcessManager.h"

// 0x0094F486
ProcName ProcMgr::s_CreatingName = static_cast<ProcName>(0xFFFF);

// 0x0052A32C (name after libgarden)
oml::framework::Process* ProcMgr::Create(ProcName name, oml::framework::Process* parent, u32 arg, ProcType type)
{
    if (name >= PROC_NAME_NUM) {
        return NULL;
    }
    s_CreatingName = name;
    oml::framework::Process* process = oml::framework::ProcessManager::Get()->CreateProcess(name, parent, arg, type);
    s_CreatingName = static_cast<ProcName>(0xFFFF);
    if (process != NULL && process->IsStopped()) {
        return NULL;
    }
    return process;
}

// 0x0052A3B4 (name after libgarden)
oml::framework::Process* ProcMgr::Create(ProcName name, u32 arg, ProcType type)
{
    if (name >= PROC_NAME_NUM) {
        return NULL;
    }
    s_CreatingName = name;
    oml::framework::Process* process = oml::framework::ProcessManager::Get()->CreateProcess(name, arg, type);
    s_CreatingName = static_cast<ProcName>(0xFFFF);
    if (process != NULL && process->IsStopped()) {
        return NULL;
    }
    return process;
}

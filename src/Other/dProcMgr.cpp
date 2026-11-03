#include "Other/dProcMgr.h"

// 0x0052A32C (name after libgarden)
oml::framework::Process* ProcMgr::Create(ProcName, oml::framework::Process*, u32, ProcType)
{
}

// 0x0052A3B4 | libgarden [tier A]
void ProcMgr::Create(ProcName, unsigned long, ProcType)
{
}


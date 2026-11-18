#include "Bs/dBsRoot.h"
#include "Bs/dBsSceneExtern.h"
#include "Other/dProcMgr.h"

// 0x009513BC
BsRoot* BsRoot::s_pInstance = NULL;

// 0x005BFFE4 slot 0x00
// 0x005BFFD4 (deleting dtor)
BsRoot::~BsRoot()
{
    // (only the base is destroyed)
}

// 0x005BFF1C slot 0x0C
oml::framework::Result BsRoot::Initialize()
{
    s_pInstance = this;
    // 0x009513C4 (guard 0x009513C0)
    static const ProcName s_Children[] = {PROC_BS_DEMO_PROC, PROC_BS_MENU_MGR};
    for (u32 i = 0; i < 2; i++) {
        ProcMgr::Create(s_Children[i], s_pInstance, 0, PROC_TYPE_BASE);
    }
    return oml::framework::RESULT_SUCCESS;
}

// 0x005BFFBC slot 0x18
oml::framework::Result BsRoot::Finalize()
{
    s_pInstance = NULL;
    return oml::framework::RESULT_SUCCESS;
}

// 0x005BFFA4 slot 0x24
oml::framework::Result BsRoot::Calc()
{
    Func4E2CC4(&g_Unknown9CA054);
    return oml::framework::RESULT_SUCCESS;
}

// 0x005BFF14 slot 0x30
oml::framework::Result BsRoot::Draw()
{
    return oml::framework::RESULT_SUCCESS;
}

// the factory of the process
// 0x007F2F40
template oml::framework::Process* oml::framework::Process::Create<BsRoot>();

#include "demo/dProc.h"
#include "demo/demo_Api.h"

namespace demo {
// 0x0052DB8C slot 0x30 (name after oml::framework::Process)
oml::framework::Result demo::Proc::Draw()
{
    return oml::framework::RESULT_SUCCESS;
}

// 0x0052DB94 slot 0x0C (name after oml::framework::Process)
oml::framework::Result demo::Proc::Initialize()
{
    demo::Initialize(true);
    return oml::framework::RESULT_SUCCESS;
}

// 0x0052DBA8 slot 0x24 (name after oml::framework::Process)
oml::framework::Result demo::Proc::Calc()
{
    UpdateRunning();
    UpdateEnding();
    StartNextOrder();
    UpdateStarted();
    UpdateList();
    return oml::framework::RESULT_SUCCESS;
}

// 0x0052DBC8 slot 0x18 (name after oml::framework::Process)
oml::framework::Result demo::Proc::Finalize()
{
    return oml::framework::RESULT_SUCCESS;
}

// 0x0052DBD0 (name after the RTTI)
demo::Proc::Proc()
{
    // (only the base is constructed)
}

// 0x0052DBF8 slot 0x00 (name after the RTTI)
// 0x0052DBE8 (deleting dtor)
demo::Proc::~Proc()
{
    // (only the base is destroyed)
}
} // namespace demo

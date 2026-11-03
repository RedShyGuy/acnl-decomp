#include "oml/framework/dProcess.h"
#include <cstring>
#include <new>
#include "oml/framework/dProcessManager.h"
#include "sead/seadNew.h"

namespace oml {
namespace framework {
// 0x00313B24 | libgarden [tier A]
void oml::framework::Process::operator delete(void* p)
{
    ::operator delete[](p);
}

// 0x00520B5C | libgarden [tier A]
void oml::framework::Process::StopSelf()
{
    ProcessManager::Get()->Stop(this);
}

// 0x00521614 slot 0x10
void oml::framework::Process::HandleInitializationResult(Result /*result*/)
{
    // empty (in the original too)
}

// 0x0052161C slot 0x1C
void oml::framework::Process::HandleFinalizationResult(Result /*result*/)
{
    // empty (in the original too)
}

// 0x00521620 slot 0x20
bool oml::framework::Process::CanCalc() const
{
    return true;
}

// 0x00521710 slot 0x38 | libgarden
void oml::framework::Process::OnNotify()
{
    // empty (in the original too)
}

// 0x00521718 slot 0x28
void oml::framework::Process::HandleCalcResult(Result /*result*/)
{
    // empty (in the original too)
}

// 0x00521754 | libgarden [tier A]
void oml::framework::Process::Stop()
{
    ProcessManager::Get()->Stop(this);
}

// 0x0052176C (name is ours)
void oml::framework::Process::Set(ProcessIndex index, u32 arg, u8 type)
{
    m_Index = index;
    m_Arg = arg;
    m_Type = type;
}

// 0x005220B0 slot 0x30
Result oml::framework::Process::Draw()
{
    return RESULT_SUCCESS;
}

// 0x005220BC slot 0x0C
Result oml::framework::Process::Initialize()
{
    return RESULT_SUCCESS;
}

// 0x005220C8 slot 0x24
Result oml::framework::Process::Calc()
{
    return RESULT_SUCCESS;
}

// 0x005220D0 slot 0x2C
bool oml::framework::Process::CanDraw() const
{
    return true;
}

// 0x005220DC slot 0x18
Result oml::framework::Process::Finalize()
{
    return RESULT_SUCCESS;
}

// 0x005220E8 slot 0x34 | libgarden
void oml::framework::Process::ProcessDrawResult(Result /*result*/)
{
    // empty (in the original too)
}

// 0x005220EC (name is ours)
void oml::framework::Process::SetArg(u32 arg)
{
    m_Arg = arg;
}

// 0x00522128 slot 0x08
bool oml::framework::Process::CanInitialize() const
{
    return true;
}

// 0x00522130 slot 0x14
bool oml::framework::Process::CanFinalize() const
{
    return true;
}

// 0x00522138 | libgarden [tier A]
oml::framework::Process::Process()
    : m_Handle(-1), m_Arg(0), m_Index(0), m_State(STATE_CREATED), m_IsStopped(false), m_IsActivationPending(false),
      m_IsInitializationDeferred(false), m_Type(0)
{
    m_Handle = ProcessManager::Get()->AllocHandle();
}

// 0x0052218C slot 0x00 | libgarden
// 0x00522184 (deleting dtor)
oml::framework::Process::~Process()
{
    // empty (in the original too)
}

// 0x005251F0 | libgarden [tier A]
void oml::framework::Process::Destroy(Process* process)
{
    delete process;
}

// 0x0052A488 | libgarden [tier A]
void* oml::framework::Process::operator new(size_t size)
{
    void* p = ::operator new[](size, std::nothrow);
    if (p != NULL) {
        std::memset(p, 0, size);
    }
    return p;
}

} // namespace framework
} // namespace oml

#include "oml/framework/dProcess.h"

#include <stdlib.h>
#include <string.h>

namespace oml {
namespace framework {
// 0x0052218C slot 0x00 | libgarden
oml::framework::Process::~Process()
{
}

// 0x00522128 slot 0x08 | slot vf_0x08 of oml::framework::Process
void oml::framework::Process::CanInitialize() const
{
}

// 0x005220BC slot 0x0C | slot vf_0x0C of oml::framework::Process
void oml::framework::Process::Initialize()
{
}

// 0x00521614 slot 0x10 | slot vf_0x10 of oml::framework::Process
void oml::framework::Process::HandleInitializationResult(oml::framework::Result)
{
}

// 0x00522130 slot 0x14 | slot vf_0x14 of oml::framework::Process
void oml::framework::Process::CanFinalize() const
{
}

// 0x005220DC slot 0x18 | slot vf_0x18 of oml::framework::Process
void oml::framework::Process::Finalize()
{
}

// 0x0052161C slot 0x1C | slot vf_0x1C of oml::framework::Process
void oml::framework::Process::HandleFinalizationResult(oml::framework::Result)
{
}

// 0x00521620 slot 0x20 | slot vf_0x20 of oml::framework::Process
void oml::framework::Process::CanCalc() const
{
}

// 0x005220C8 slot 0x24 | slot vf_0x24 of oml::framework::Process
void oml::framework::Process::Calc()
{
}

// 0x00521718 slot 0x28 | slot vf_0x28 of oml::framework::Process
void oml::framework::Process::HandleCalcResult(oml::framework::Result)
{
}

// 0x005220D0 slot 0x2C | slot vf_0x2C of oml::framework::Process
void oml::framework::Process::CanDraw() const
{
}

// 0x005220B0 slot 0x30 | slot vf_0x30 of oml::framework::Process
void oml::framework::Process::Draw()
{
}

// 0x005220E8 slot 0x34 | libgarden
void oml::framework::Process::ProcessDrawResult(oml::framework::Result)
{
}

// 0x00521710 slot 0x38 | libgarden
void oml::framework::Process::OnNotify()
{
}

// 0x00313B24 | libgarden [tier A]
void oml::framework::Process::operator delete(void*)
{
}

// 0x00520B5C | libgarden [tier A]
void oml::framework::Process::StopSelf()
{
}

// 0x00521754 | libgarden [tier A]
void oml::framework::Process::Stop()
{
}

// 0x00522138 | libgarden [tier A]
oml::framework::Process::Process()
{
}

// 0x005251F0 | libgarden [tier A]
void oml::framework::Process::Destroy(oml::framework::Process*)
{
}

// 0x0052A488 | libgarden [tier A]
void* oml::framework::Process::operator new(unsigned int size)
{
    void* p = malloc(size);
    if (p) {
        memset(p, 0, size);
    }
    return p;
}

} // namespace framework
} // namespace oml

#include "oml/framework/dProcess.h"

namespace oml {
namespace framework {
namespace {
typedef Result (Process::*RunFunc)();
typedef bool (Process::*CanFunc)() const;
typedef void (Process::*HandleFunc)(Result result);
} // namespace

// a step of a process (dProcessManager.cpp; in the original in its .rodata)
struct ProcessPhase
{
    RunFunc run;        // 0x00
    CanFunc can;        // 0x08
    HandleFunc handle;  // 0x10
};

// the steps Initialize, Finalize, Calc and Draw
// 0x008A4AF8
extern const ProcessPhase s_ProcessPhases[4] = {
    {&Process::Initialize, &Process::CanInitialize, &Process::HandleInitializationResult},
    {&Process::Finalize, &Process::CanFinalize, &Process::HandleFinalizationResult},
    {&Process::Calc, &Process::CanCalc, &Process::HandleCalcResult},
    {&Process::Draw, &Process::CanDraw, &Process::ProcessDrawResult},
};
} // namespace framework
} // namespace oml

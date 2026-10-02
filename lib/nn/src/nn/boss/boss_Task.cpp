#include "nn/boss/boss_Task.h"

namespace nn {
namespace boss {
// ctor candidate(s) 0x0046C824 (unverified)
nn::boss::Task::Task()
{
}

// 0x0046C850 slot 0x00 | virtual slot, introduced by nn::boss::Task
void nn::boss::Task::vf_0x00()
{
}

// 0x0046C848 slot 0x04 | virtual slot, introduced by nn::boss::Task
void nn::boss::Task::vf_0x04()
{
}

// 0x0046C1AC | nintendogs:bytes [tier B]
void nn::boss::Task::Initialize(const char*)
{
}

// 0x0046C20C | fefates:bytes-fuzzy [tier B]
void nn::boss::Task::WaitFinish(const nn::fnd::TimeSpan&)
{
}

// 0x0046C2FC | nintendogs:bytes [tier A]
void nn::boss::Task::UpdateCount(unsigned)
{
}

// 0x0046C394 | fefates:bytes [tier B]
void nn::boss::Task::GetStateDetail(nn::boss::TaskStatus*, bool, unsigned char*, unsigned char)
{
}

// 0x0046C484 | nintendogs:bytes [tier A]
void nn::boss::Task::StartImmediate()
{
}

// 0x0046C524 | fefates:bytes [tier B]
void nn::boss::Task::GetServiceStatus()
{
}

// 0x0046C5C0 | nintendogs:bytes [tier A]
void nn::boss::Task::Start()
{
}

// 0x0046C64C | fefates:bytes-fuzzy [tier B]
void nn::boss::Task::GetState(bool, unsigned int*, unsigned char*)
{
}

// 0x0046C738 | nintendogs:bytes [tier A]
void nn::boss::Task::GetResult(unsigned*, unsigned char*)
{
}

} // namespace boss
} // namespace nn

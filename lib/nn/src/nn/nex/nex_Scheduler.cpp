#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_Scheduler.h"

namespace nn {
namespace nex {
// ctor address unknown
nn::nex::Scheduler::Scheduler()
{
}

// 0x003D987C slot 0x00 | slot vf_0x00 of nn::nex::Scheduler
nn::nex::Scheduler::~Scheduler()
{
}

// 0x003D80B0 | fefates:bytes-fuzzy [tier B]
void nn::nex::Scheduler::PreventBlockCall(bool, unsigned int, bool, bool (*)(nn::nex::RootObject*,unsigned int), nn::nex::RootObject*, bool*)
{
}

// 0x003D87C0 | fefates:bytes-fuzzy [tier B]
void nn::nex::Scheduler::PreventRegularBlockCall(bool, unsigned int, bool (*)(nn::nex::RootObject*,unsigned int), nn::nex::RootObject*, nn::nex::qResult*)
{
}

} // namespace nex
} // namespace nn

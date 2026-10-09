#include "nn/cec/CTR/cec_CecControlSys.h"

namespace nn {
namespace cec {
namespace CTR {
// (the static initializer __sti___18_cec_ControlSys_cpp 0x00797D88 also makes a heap 0x00AE94DC
// and a shared memory block 0x00AE9534 for the system functions; this program does not use them)
// the flag (name is ours)
// 0x0097E82C
bool s_IsInitializedSys = false;

// 0x00143684 | nintendogs:callgraph [tier A]
bool nn::cec::CTR::CecControlSys::IsInitializedSys()
{
    return s_IsInitializedSys;
}

} // namespace CTR
} // namespace cec
} // namespace nn

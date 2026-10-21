#include "nn/cec/CTR/cec_CecControlSys.h"
#include "nn/fnd/fnd_ExpHeapTemplate.h"
#include "nn/os/os_LockPolicy_NoLock_LockObject.h"
#include "nn/os/os_SharedMemoryBlock.h"

namespace nn {
namespace cec {
namespace CTR {
// the heap and the shared memory block of the system functions (this program does not use them;
// names are ours)
// 0x00AE94DC
nn::fnd::ExpHeapTemplate<nn::os::LockPolicy::NoLock> s_SysHeap;
// 0x00AE9534
nn::os::SharedMemoryBlock s_SysSharedMemory;

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

// ARMCC's static initializer of the file (cec_ControlSys.cpp): the constructors of the globals above
// 0x00797D88 (static initializer)

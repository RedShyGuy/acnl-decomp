#include "nn/hid/CTR/hid_ExtraPad.h"
#include "nn/ir/CTR/CTR_Api.h"
#include "nn/os/os_IpcSession.h"
#include "nn/os/os_SharedMemoryBlock.h"

namespace nn {
namespace hid {
namespace CTR {
// the shared memory block and the session of the extra pad (the original file is hid_CStick.cpp;
// this program does not use them; the session type is inferred from its size and destructor, the
// names are ours)
// 0x00AF7FE8
nn::os::SharedMemoryBlock s_ExtraPadSharedMemory;
// 0x00982EC4
nn::os::ipc::Session s_ExtraPadSession;

// 0x00354680 | nintendogs:bytes [tier A]
bool nn::hid::CTR::ExtraPad::IsSampling()
{
    return nn::ir::CTR::CepdGetStatus() == nn::ir::CTR::CEPD_STATUS_SAMPLING;
}

} // namespace CTR
} // namespace hid
} // namespace nn

// ARMCC's static initializer of the file (hid_CStick.cpp): the constructors of the globals above
// 0x00790B7C (static initializer)

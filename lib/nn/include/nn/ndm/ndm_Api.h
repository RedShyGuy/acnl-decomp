#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/ndm/CTR/ndm_Types.h"

namespace nn {
namespace ndm {
// for an application: all daemons under its control, BOSS and NIM suspended
void SetupDaemonsDefault(); // 0x0011E2B8 | nintendogs:bytes [tier A]
// counted: the session is made by the first call and closed by the last Finalize
nn::Result Initialize(); // 0x0011FF34 | nintendogs:bytes [tier A]
nn::Result Finalize(); // 0x00354BD4 | nintendogs:bytes [tier A]
nn::Result SuspendDaemons(bit32 mask); // 0x00124720 | nintendogs:callgraph [tier A]
nn::Result Suspend(nn::ndm::CTR::DaemonName name); // 0x00354BB4 | nintendogs:callgraph [tier A]
nn::Result Resume(nn::ndm::CTR::DaemonName name); // 0x0014370C | nintendogs:callgraph [tier A]
nn::Result QueryExclusiveMode(nn::ndm::CTR::ExclusiveMode& mode); // 0x00354ACC | nintendogs:bytes [tier B]
} // namespace ndm
} // namespace nn

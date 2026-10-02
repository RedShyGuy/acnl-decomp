#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace ndm {
void SetupDaemonsDefault(); // 0x0011E2B8 | nintendogs:bytes [tier A]
nn::Result Initialize(); // 0x0011FF34 | nintendogs:bytes [tier A]
void SuspendDaemons(unsigned); // 0x00124720 | nintendogs:callgraph [tier A]
void Resume(nn::ndm::CTR::DaemonName); // 0x0014370C | nintendogs:callgraph [tier A]
void QueryExclusiveMode(nn::ndm::CTR::ExclusiveMode&); // 0x00354ACC | nintendogs:bytes [tier B]
void Suspend(nn::ndm::CTR::DaemonName); // 0x00354BB4 | nintendogs:callgraph [tier A]
void Finalize(); // 0x00354BD4 | nintendogs:bytes [tier A]
} // namespace ndm
} // namespace nn

#include "nn/ndm/ndm_Api.h"

namespace nn {
namespace ndm {
// 0x0011E2B8 | nintendogs:bytes [tier A]
void SetupDaemonsDefault()
{
}

// 0x0011FF34 | nintendogs:bytes [tier A]
nn::Result Initialize()
{
}

// 0x00124720 | nintendogs:callgraph [tier A]
void SuspendDaemons(unsigned)
{
}

// 0x0014370C | nintendogs:callgraph [tier A]
void Resume(nn::ndm::CTR::DaemonName)
{
}

// 0x00354ACC | nintendogs:bytes [tier B]
void QueryExclusiveMode(nn::ndm::CTR::ExclusiveMode&)
{
}

// 0x00354BB4 | nintendogs:callgraph [tier A]
void Suspend(nn::ndm::CTR::DaemonName)
{
}

// 0x00354BD4 | nintendogs:bytes [tier A]
void Finalize()
{
}

} // namespace ndm
} // namespace nn

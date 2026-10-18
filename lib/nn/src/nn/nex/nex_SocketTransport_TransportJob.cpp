#include "nn/nex/nex_SocketTransport_TransportJob.h"

namespace nn {
namespace nex {
// 0x0037B8B0 slot 0x00
// 0x0037B884 (deleting dtor)
nn::nex::SocketTransport::TransportJob::~TransportJob()
{
    // nothing to do
}

// 0x0037BEF4 slot 0x0C
void nn::nex::SocketTransport::TransportJob::Execute()
{
    m_pTransport->DoWork();
}

} // namespace nex
} // namespace nn

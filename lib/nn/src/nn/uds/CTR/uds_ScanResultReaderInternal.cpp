#include "nn/uds/CTR/uds_ScanResultReaderInternal.h"

namespace nn {
namespace uds {
namespace CTR {

// 0x00468B00 slot 0x00
// 0x00468AFC (deleting dtor)
nn::uds::CTR::ScanResultReaderInternal::~ScanResultReaderInternal()
{
    // nothing to do: the members and bases are destroyed by the compiler
}

} // namespace CTR
} // namespace uds
} // namespace nn

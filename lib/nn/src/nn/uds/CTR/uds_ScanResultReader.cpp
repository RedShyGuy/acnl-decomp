#include "nn/uds/CTR/uds_ScanResultReader.h"
#include "nn/uds/CTR/uds_ScanResultReaderInternal.h"

namespace nn {
namespace uds {
namespace CTR {

// 0x004685D8 slot 0x00
nn::uds::CTR::ScanResultReader::~ScanResultReader()
{
}

// 0x00468578 | fefates:bytes [tier B]
nn::uds::CTR::NetworkDescriptionReader nn::uds::CTR::ScanResultReader::GetNextDescription()
{
    ScanResultReaderInternal reader(m_pBuffer, m_pCurrent);
    const nn::nwm::BssDescription* bss = reinterpret_cast<const nn::nwm::BssDescription*>(reader.GetCurrentPointer());
    if (!reader.ProcessBssPointer()) {
        bss = NULL;
    }
    NetworkDescriptionReader description(bss);
    m_pCurrent = reader.GetCurrentPointer();
    return description;
}

// 0x007370FC | fefates:bytes [tier B]
u32 nn::uds::CTR::ScanResultReader::GetCount() const
{
    ScanResultReaderInternal reader(m_pBuffer, NULL);
    return reader.GetCount();
}

} // namespace CTR
} // namespace uds
} // namespace nn

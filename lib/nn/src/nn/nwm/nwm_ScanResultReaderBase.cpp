#include "nn/nwm/nwm_ScanResultReaderBase.h"

namespace nn {
namespace nwm {
namespace {
// the header of a scan result buffer: its size at +4, the number of descriptions at +8 (3dbrew
// "NWMUDS:StartScan"; names are ours)
struct ScanResultHeader
{
    u32 unknown00;
    u32 size;
    u32 count;
};
} // namespace

// 0x003E227C | mk7dlp:bytes [tier A]
u8* nn::nwm::ScanResultReaderBase::ProcessBssPointer()
{
    const ScanResultHeader* header = static_cast<const ScanResultHeader*>(m_pBuffer);
    if (header == NULL) {
        return NULL;
    }
    if (reinterpret_cast<const u8*>(header) + header->size <= m_pCurrent) {
        return NULL;
    }
    if (m_pCurrent == NULL) {
        return NULL;
    }
    m_pCurrent += *reinterpret_cast<const u32*>(m_pCurrent);
    return m_pCurrent;
}

// 0x003E22C0 | fefates:bytes [tier B]
nn::nwm::ScanResultReaderBase::ScanResultReaderBase(const void* buffer, u8* current) : m_pBuffer(buffer), m_pCurrent(current)
{
    if (buffer != NULL && current == NULL) {
        m_pCurrent = static_cast<u8*>(const_cast<void*>(buffer)) + sizeof(ScanResultHeader);
    }
}

// 0x003E22F0 slot 0x00
// 0x003E22EC (deleting dtor)
nn::nwm::ScanResultReaderBase::~ScanResultReaderBase()
{
}

// 0x0072EC5C | tier C
u32 nn::nwm::ScanResultReaderBase::GetCount() const
{
    const ScanResultHeader* header = static_cast<const ScanResultHeader*>(m_pBuffer);
    if (header == NULL) {
        return 0;
    }
    return header->count;
}

} // namespace nwm
} // namespace nn

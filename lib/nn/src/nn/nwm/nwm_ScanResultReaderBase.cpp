#include "nn/nwm/nwm_ScanResultReaderBase.h"

namespace nn {
namespace nwm {
// ctor candidate(s) 0x003E22C0 (unverified)
nn::nwm::ScanResultReaderBase::ScanResultReaderBase()
{
}

// 0x003E227C | mk7dlp:bytes [tier A]
bool nn::nwm::ScanResultReaderBase::ProcessBssPointer()
{
}

// 0x003E22C0 | fefates:bytes [tier B]
nn::nwm::ScanResultReaderBase::ScanResultReaderBase(const void* buffer, u8* current)
{
}

} // namespace nwm
} // namespace nn

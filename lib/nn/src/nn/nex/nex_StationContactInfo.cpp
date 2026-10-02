#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_StationContactInfo.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x00390314 (unverified)
nn::nex::StationContactInfo::StationContactInfo()
{
}

// 0x003903E0 slot 0x00 | fefates:bytes
nn::nex::StationContactInfo::~StationContactInfo()
{
}

// 0x00390314 | fefates:bytes [tier B]
nn::nex::StationContactInfo::StationContactInfo(const nn::nex::qList<nn::nex::StationURL>&)
{
}

// 0x0072C2B8 | fefates:bytes [tier B]
void nn::nex::StationContactInfo::SortAndFilterTarget(nn::nex::StationContactInfo&) const
{
}

} // namespace nex
} // namespace nn

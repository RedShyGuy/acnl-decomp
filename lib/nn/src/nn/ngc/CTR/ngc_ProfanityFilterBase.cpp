#include "nn/ngc/CTR/ngc_ProfanityFilterBase.h"

namespace nn {
namespace ngc {
namespace CTR {
// ctor address unknown
nn::ngc::CTR::ProfanityFilterBase::ProfanityFilterBase()
{
}

// 0x003E1640 | fefates:bytes [tier B]
void nn::ngc::CTR::ProfanityFilterBase::IsIncludesAtSign(const wchar_t*, int)
{
}

// 0x003E192C | fefates:bytes [tier B]
void nn::ngc::CTR::ProfanityFilterBase::ConvertUserInputForWord(wchar_t*, int, const wchar_t*)
{
}

// 0x003E1BA8 | fefates:bytes-fuzzy [tier B]
void nn::ngc::CTR::ProfanityFilterBase::GetPatternListsFromRegion(nn::ngc::CTR::ProfanityFilterPatternList*, int*, bool)
{
}

} // namespace CTR
} // namespace ngc
} // namespace nn

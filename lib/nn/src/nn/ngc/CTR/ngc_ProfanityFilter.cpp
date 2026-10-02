#include "nn/ngc/CTR/ngc_ProfanityFilterBase.h"
#include "nn/ngc/CTR/ngc_ProfanityFilter.h"

namespace nn {
namespace ngc {
namespace CTR {
// 0x003E15CC slot 0x00 | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
void nn::ngc::CTR::ProfanityFilter::vf_0x00()
{
}

// 0x003E1554 slot 0x04 | fefates:bytes
nn::ngc::CTR::ProfanityFilter::~ProfanityFilter()
{
}

// 0x003E0240 slot 0x08 | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
void nn::ngc::CTR::ProfanityFilter::vf_0x08()
{
}

// 0x003E048C slot 0x0C | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
void nn::ngc::CTR::ProfanityFilter::vf_0x0C()
{
}

// 0x003E0360 slot 0x10 | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
void nn::ngc::CTR::ProfanityFilter::vf_0x10()
{
}

// 0x003E0640 slot 0x14 | fefates:bytes
void nn::ngc::CTR::ProfanityFilter::CheckProfanityWords(unsigned int*, bool, const wchar_t**, unsigned int)
{
}

// 0x003E0C08 slot 0x18 | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
void nn::ngc::CTR::ProfanityFilter::vf_0x18()
{
}

// 0x003E0AC8 slot 0x1C | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
void nn::ngc::CTR::ProfanityFilter::vf_0x1C()
{
}

// 0x003E0DD4 slot 0x20 | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
void nn::ngc::CTR::ProfanityFilter::vf_0x20()
{
}

// 0x003DFFFC | fefates:bytes [tier B]
void nn::ngc::CTR::ProfanityFilter::CheckWords(unsigned int*, const wchar_t*, unsigned int, const wchar_t**, unsigned int)
{
}

// 0x003E01E4 | fefates:bytes [tier B]
void nn::ngc::CTR::ProfanityFilter::Initialize(unsigned int)
{
}

// 0x003E0834 | fefates:bytes [tier B]
void nn::ngc::CTR::ProfanityFilter::MountSharedContents()
{
}

// 0x003E08D0 | fefates:bytes [tier B]
void nn::ngc::CTR::ProfanityFilter::CheckProfanityWords_Impl(unsigned int*, nn::fs::FileInputStream*, const wchar_t**, unsigned int)
{
}

// 0x003E1504 | fefates:bytes [tier B]
nn::ngc::CTR::ProfanityFilter::ProfanityFilter()
{
}

// 0x0072EC20 | fefates:bytes [tier B]
void nn::ngc::CTR::ProfanityFilter::CheckArguments_Word(const unsigned int*, const wchar_t**, unsigned int) const
{
}

} // namespace CTR
} // namespace ngc
} // namespace nn

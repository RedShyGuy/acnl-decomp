#pragma once

#include "decomp.h"
#include "nn/ngc/CTR/ngc_ProfanityFilterBase.h"

namespace nn {
namespace ngc {
namespace CTR {
// RTTI N2nn3ngc3CTR15ProfanityFilterE @ 0x008CF7B0
// vtable 0x008FFDB8 (vptr 0x008FFDC0), offset_to_top 0, 9 entries
class ProfanityFilter : public ::nn::ngc::CTR::ProfanityFilterBase
{
public:
    virtual void vf_0x00(); // 0x003E15CC slot 0x00 | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
    virtual ~ProfanityFilter(); // 0x003E1554 slot 0x04 | fefates:bytes
    virtual void vf_0x08(); // 0x003E0240 slot 0x08 | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
    virtual void vf_0x0C(); // 0x003E048C slot 0x0C | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
    virtual void vf_0x10(); // 0x003E0360 slot 0x10 | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
    virtual void CheckProfanityWords(unsigned int*, bool, const wchar_t**, unsigned int); // 0x003E0640 slot 0x14 | fefates:bytes
    virtual void vf_0x18(); // 0x003E0C08 slot 0x18 | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
    virtual void vf_0x1C(); // 0x003E0AC8 slot 0x1C | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
    virtual void vf_0x20(); // 0x003E0DD4 slot 0x20 | virtual slot, introduced by nn::ngc::CTR::ProfanityFilter
    void CheckWords(unsigned int*, const wchar_t*, unsigned int, const wchar_t**, unsigned int); // 0x003DFFFC | fefates:bytes [tier B]
    void Initialize(unsigned int); // 0x003E01E4 | fefates:bytes [tier B]
    void MountSharedContents(); // 0x003E0834 | fefates:bytes [tier B]
    void CheckProfanityWords_Impl(unsigned int*, nn::fs::FileInputStream*, const wchar_t**, unsigned int); // 0x003E08D0 | fefates:bytes [tier B]
    ProfanityFilter(); // 0x003E1504 | fefates:bytes [tier B]
    void CheckArguments_Word(const unsigned int*, const wchar_t**, unsigned int) const; // 0x0072EC20 | fefates:bytes [tier B]
};
} // namespace CTR
} // namespace ngc
} // namespace nn

#include "sead/ptcl/seadPtclRenderer.h"

namespace sead {
namespace ptcl {
// 0x0055630C | mk7dlp:bytes [tier B]
void sead::ptcl::PtclRenderer::beginRender(unsigned*, const sead::Matrix44<float>&, const sead::Matrix34<float>&, const sead::Matrix34<float>&)
{
}

// 0x00556884 | mk7dlp:bytes [tier B]
void sead::ptcl::PtclRenderer::beginStripeCalc(unsigned*)
{
}

// 0x00556B40 | mk7dlp:bytes [tier B]
void sead::ptcl::PtclRenderer::endCalc(unsigned*)
{
}

// 0x00556B7C | mk7dlp:bytes [tier B]
void sead::ptcl::PtclRenderer::beginCalc(unsigned*)
{
}

// 0x00556C18 | mk7dlp:bytes [tier B]
void sead::ptcl::PtclRenderer::endRender(unsigned*)
{
}

} // namespace ptcl
} // namespace sead

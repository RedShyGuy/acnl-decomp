#include "sead/hostio/seadNode.h"
#include "sead/ptcl/seadPtclSystem.h"

namespace sead {
namespace ptcl {
// ctor address unknown
sead::ptcl::PtclSystem::PtclSystem()
{
}

// 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
void sead::ptcl::PtclSystem::vf_0x00()
{
}

// 0x0054F690 | mk7dlp:bytes-fuzzy [tier B]
void sead::ptcl::PtclSystem::beginRender(unsigned*, const sead::Matrix44<float>&, const sead::Matrix34<float>&, const sead::Matrix34<float>&)
{
}

// 0x0054F6EC | mk7dlp:bytes-fuzzy [tier B]
void sead::ptcl::PtclSystem::killEmitter(sead::ptcl::EmitterInstance*)
{
}

// 0x0054F8A8 | mk7dlp:bytes-fuzzy [tier B]
void sead::ptcl::PtclSystem::removeStripe(sead::ptcl::PtclStripe*)
{
}

// 0x00550EAC | mk7dlp:bytes [tier B]
void sead::ptcl::PtclSystem::initIBO(void*, int)
{
}

// 0x006D272C | mk7dlp:bytes [tier B]
void sead::ptcl::PtclSystem::calcEditor()
{
}

} // namespace ptcl
} // namespace sead

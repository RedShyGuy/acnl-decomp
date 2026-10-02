#include "sead/hostio/seadNode.h"
#include "sead/ptcl/seadPtclResource.h"

namespace sead {
namespace ptcl {
// ctor address unknown
sead::ptcl::PtclResource::PtclResource()
{
}

// 0x0074EE78 slot 0x00 | virtual slot, introduced by (anonymous namespace)::EditHostIO
void sead::ptcl::PtclResource::vf_0x00()
{
}

// 0x00557D7C slot 0x04 | mk7dlp:callseq
void sead::ptcl::PtclResource::vf_0x04()
{
}

// 0x00557D6C slot 0x08 | slot vf_0x08 of sead::ptcl::PtclResource
sead::ptcl::PtclResource::~PtclResource()
{
}

// 0x00557578 | mk7dlp:bytes [tier B]
void sead::ptcl::PtclResource::bindTarget(int, sead::ptcl::ResourceBind*, sead::ptcl::EmitterTblData*, int, const sead::SafeStringBase<char>&, unsigned)
{
}

// 0x00557988 | mk7dlp:bytes [tier B]
void sead::ptcl::PtclResource::unbind(sead::ptcl::ResourceBind*, bool, bool)
{
}

} // namespace ptcl
} // namespace sead

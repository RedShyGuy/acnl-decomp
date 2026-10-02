#include "nn/nex/nex_BackEndServices.h"
#include "nn/nex/nex_RendezVous.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x00355168 (unverified)
nn::nex::RendezVous::RendezVous()
{
}

// 0x00377284 slot 0x00 | fefates:callgraph
nn::nex::RendezVous::~RendezVous()
{
}

// 0x00355108 | fefates:bytes [tier B]
void nn::nex::RendezVous::Login(nn::nex::CallContext*, const wchar_t*, const char*, const wchar_t*, unsigned short, const nn::nex::AnyObjectHolder<nn::nex::Data,nn::nex::String>&, nn::nex::Credentials**, nn::nex::String*)
{
}

// 0x00376C50 | fefates:bytes [tier B]
void nn::nex::RendezVous::Logout(nn::nex::CallContext*, nn::nex::Credentials*)
{
}

} // namespace nex
} // namespace nn

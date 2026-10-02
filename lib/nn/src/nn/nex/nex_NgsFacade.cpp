#include "nn/nex/nex_RendezVous.h"
#include "nn/nex/nex_NgsFacade.h"

namespace nn {
namespace nex {
// 0x003D73D0 slot 0x00 | fefates:bytes
nn::nex::NgsFacade::~NgsFacade()
{
}

// 0x003D6CC4 | fefates:bytes [tier B]
void nn::nex::NgsFacade::LoginAndRequestAuthenticationTokenImpl(nn::nex::ProtocolCallContext*, unsigned int, const wchar_t*, const char*, char*, const void*, int, unsigned int)
{
}

// 0x003D7164 | fefates:bytes [tier B]
void nn::nex::NgsFacade::Login(nn::nex::ProtocolCallContext*, unsigned int, const wchar_t*, int, unsigned int)
{
}

// 0x003D719C | fefates:bytes [tier B]
void nn::nex::NgsFacade::Logout(nn::nex::ProtocolCallContext*)
{
}

// 0x003D725C | fefates:bytes-fuzzy [tier B]
nn::nex::NgsFacade::NgsFacade()
{
}

// 0x0072EA08 | fefates:bytes [tier B]
void nn::nex::NgsFacade::GetLastLoginErrorCode() const
{
}

} // namespace nex
} // namespace nn

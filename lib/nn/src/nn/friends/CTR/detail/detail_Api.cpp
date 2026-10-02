#include "nn/friends/CTR/detail/detail_Api.h"

namespace nn {
namespace friends {
namespace CTR {
namespace detail {
// 0x00125524 | fefates:bytes [tier B]
void Initialize()
{
}

// 0x001256A0 | fefates:bytes [tier B]
void GetFriendKeyList(nnfriendsFriendKey*, unsigned int*, unsigned int, unsigned int)
{
}

// 0x001374C8 | fefates:bytes [tier B]
void Finalize()
{
}

// 0x003D0BE8 | fefates:bytes [tier B]
void GetFriendScreenName(wchar_t(*)[11], const nnfriendsFriendKey*, unsigned int, bool, unsigned char*)
{
}

// 0x0048A904 | fefates:bytes [tier B]
void HasLoggedIn()
{
}

// 0x0048A968 | fefates:bytes [tier B]
void GetMyPassword(char*)
{
}

// 0x0048AA1C | fefates:bytes [tier B]
void GetMyPrincipalId()
{
}

// 0x0048AA88 | fefates:bytes [tier B]
void ResultToErrorCode(const nn::Result&)
{
}

// 0x0048AF90 | fefates:bytes [tier B]
void GetFriendAttributeFlags(unsigned int*, const nnfriendsFriendKey*, unsigned int)
{
}

// 0x0048B028 | fefates:bytes [tier B]
void RequestServiceLocatorWithoutLogin(nn::os::Event*, unsigned int, const char*, const char*, unsigned char, unsigned char)
{
}

// 0x0048B0F4 | fefates:bytes [tier B]
void RequestGameAuthenticationWithoutLogin(nn::os::Event*, unsigned int, const wchar_t*, unsigned char, unsigned char)
{
}

// 0x0048B1B0 | fefates:bytes [tier B]
void Login(nn::os::Event*)
{
}

} // namespace detail
} // namespace CTR
} // namespace friends
} // namespace nn

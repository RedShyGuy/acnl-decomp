#pragma once

#include "decomp.h"

namespace nn {
namespace friends {
namespace CTR {
namespace detail {
class Friends
{
public:
    void GetMyPresence(nnfriendsMyPresence*); // 0x0012BDE8 | fefates:bytes [tier B]
    void HasLoggedIn(bool*); // 0x0048B2A8 | fefates:bytes [tier B]
    void GetMyFriendKey(nnfriendsFriendKey*); // 0x0048B340 | fefates:bytes [tier B]
    void ResultToErrorCode(unsigned int*, nn::Result); // 0x0048B3DC | fefates:bytes [tier B]
    void GetLastResponseResult(); // 0x0048B54C | fefates:bytes [tier B]
    void GetServiceLocatorData(nnfriesndsServiceLocatorData*); // 0x0048B57C | fefates:bytes [tier B]
    void RequestServiceLocator(nn::Handle, unsigned int, const char*, const char*, unsigned char, unsigned char); // 0x0048B5CC | fefates:bytes [tier B]
    void GetGameAuthenticationData(nnfriesndsGameAuthenticationData*); // 0x0048B650 | fefates:bytes [tier B]
    void RequestGameAuthentication(nn::Handle, unsigned int, const wchar_t*, unsigned char, unsigned char); // 0x0048B6A0 | fefates:bytes [tier B]
    void Login(nn::Handle); // 0x0048B7A4 | fefates:bytes [tier B]
    void Logout(); // 0x0048B7E4 | fefates:bytes [tier B]
};
} // namespace detail
} // namespace CTR
} // namespace friends
} // namespace nn

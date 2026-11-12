#pragma once

#include "decomp.h"

// (game code that is not decompiled yet: only what net uses; the names except the class are ours)
class FriendList
{
public:
    class SingletonDisposer_;

    // the principal id of the console (0 while the friend list is not ready)
    u32 GetMyPrincipalId() const; // 0x0070B660 (name is ours)
    // (names are ours; game code that is not decompiled yet)
    void Update(); // 0x001AAE44
    void GetFriendCode(const u32* principalId, u32* localFriendCode); // 0x001AAA58
    bool AddProfile(const void* profile); // 0x001AABE8
    bool IsProfileDone(); // 0x001AAD1C
    bool GetProfile(void* profile); // 0x001AADBC
    bool IsFriend(const void* id); // 0x001AAF04
    s32 GetResult() const { return m_Result; }

    u8 unk_0x0[0x660];
    s32 m_Result; // 0x660

    static FriendList* s_pInstance;
};

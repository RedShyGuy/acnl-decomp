#include "Sv/dSvBestFriendList.h"

// 0x0071E09C | libgarden [tier A]
bool SvBestFriendList::HasFriend() const
{
    for (u32 i = 0; i < 32; i++) {
        if (entries[i].personalId.IsValid()) {
            return true;
        }
    }
    return false;
}

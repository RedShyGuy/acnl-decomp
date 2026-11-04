#include "Bs/dBsPartsSaveMgr.h"
#include "Sv/dSvBestFriendList.h"

// 0x00983034
template <> bool BsPartsSaveMgr<SvBestFriendList>::s_HasData = false;
// 0x00983038
template <> SvBestFriendList* BsPartsSaveMgr<SvBestFriendList>::s_pData = NULL;
// 0x0098304C
template <> bool BsPartsSaveMgr<SvBestFriendList>::s_IsDataBlocked = false;

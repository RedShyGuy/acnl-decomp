#include "Bs/dBsPartsSaveMgr.h"

// the constants of BsPartsSaveMgr<T> (in a file of their own: GCC would fold them, the original reads them)

// (the alignment of the data of mvplayer_pls.dat; dBsPartsSaveMgr.h)
// 0x008C9AC8
extern const s32 g_SvMovePlayerPlsAlignment = 8;
// 0x008C9ACC
template <> const s32 BsPartsSaveMgr<SvBestFriendList>::ALIGNMENT = 8;
// 0x008C9AD0
template <> const u32 BsPartsSaveMgr<SvBestFriendList>::LOAD_FLAGS = 0x40000A;
// 0x008C9AD4
template <> const u32 BsPartsSaveMgr<SvBestFriendList>::SAVE_FLAGS = 0x8005000;
// 0x008C9AD8
template <> const s32 BsPartsSaveMgr<svholder::Mail>::ALIGNMENT = 4;
// 0x008C9ADC
template <> const u32 BsPartsSaveMgr<svholder::Mail>::LOAD_FLAGS = 0x400008;
// 0x008C9AE0
template <> const u32 BsPartsSaveMgr<svholder::Mail>::SAVE_FLAGS = 0x8004800;
// 0x008C9AE4
template <> const s32 BsPartsSaveMgr<svholder::MyDesign>::ALIGNMENT = 4;
// 0x008C9AE8
template <> const u32 BsPartsSaveMgr<svholder::MyDesign>::LOAD_FLAGS = 0x400006;
// 0x008C9AEC
template <> const u32 BsPartsSaveMgr<svholder::MyDesign>::SAVE_FLAGS = 0x8004000;

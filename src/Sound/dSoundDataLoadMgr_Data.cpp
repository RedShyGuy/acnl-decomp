#include "Sound/dSoundDataLoadMgr.h"

// (in a file of its own, GCC would fold the values into the loops)
// 0x0083A1F8
const u32 SoundDataLoadMgr::s_HeapSizes[HEAP_OWN_NUM] = {0x6000, 0x26C00, 0x20000, 0x20000, 0};

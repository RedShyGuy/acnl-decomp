#pragma once

#include "decomp.h"
#include "sead/seadFixedSafeString.h"

// A string buffer the game formats file names into before it uses them (the static initializers of every file that
// uses it build it under one guard, 0x009830A0; name is ours).
extern sead::FixedSafeString<256> g_FormatBuffer;

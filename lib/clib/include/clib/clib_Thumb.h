#pragma once

// Included by the Thumb sources of the C library (*Thumb.cpp, built with -mthumb -mfloat-abi=softfp): their
// functions use no VFP registers for arguments, so the objects can be linked with the hard float ones.
asm(".eabi_attribute Tag_ABI_VFP_args, 3");

#include "types.h"
#include <wchar.h>

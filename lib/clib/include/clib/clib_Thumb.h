#pragma once

// Included by the Thumb sources of the C library (*Thumb.cpp, built with -mthumb -mfloat-abi=softfp): their
// functions use no VFP registers for arguments, so the objects can be linked with the hard float ones.
asm(".eabi_attribute Tag_ABI_VFP_args, 3");

#include "types.h"

// the conversion state of the multibyte functions (only the first word is used by the "C" locale)
struct mbstate_t
{
    u32 m_State;
    u32 _unknown4;
};

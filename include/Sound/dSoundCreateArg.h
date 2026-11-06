#pragma once

#include "decomp.h"

namespace sead {
class Heap;
}

// The argument of the factory functions of the sound objects of the game (SoundTvHandle, AudioObjFgBase,
// AudioObjFurnitureBase); only the heap is read (the class and its names are ours).
struct SoundCreateArg
{
    u32 _0;           // 0x00
    sead::Heap* heap; // 0x04
};

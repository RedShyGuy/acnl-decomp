#pragma once

#include "decomp.h"
#include "netgame/netgame_Api.h"

class SvFgName;
namespace sead {
class Heap;
} // namespace sead

class TourResult
{
public:
    class SingletonDisposer_;
    void ChangeScore(netgame::PlayerNo, long, SvFgName const&); // 0x001B82A4 | libgarden [tier A]

    // the instance (60 bytes) in heap, with its SingletonDisposer_ (name after the sead singletons)
    static TourResult* createInstance(sead::Heap* heap); // 0x001B7C44

    // an ore that a player picked up on a tour (name after the address until it is decompiled)
    void Func70D7AC(const SvFgName* item); // 0x0070D7AC

    // (name after libgarden)
    static TourResult* s_pInstance;
};

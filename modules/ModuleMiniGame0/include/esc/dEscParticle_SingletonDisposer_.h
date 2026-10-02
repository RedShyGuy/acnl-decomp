#pragma once

#include "decomp.h"
#include "esc/dEscParticle.h"
#include "sead/seadIDisposer.h"

// vtable +0xDE99C in ModuleMiniGame0.cro, offset_to_top 0, 2 entries
class esc::EscParticle::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor address unknown
    virtual ~SingletonDisposer_(); // ModuleMiniGame0.cro +0x00EF2C slot 0x00
};

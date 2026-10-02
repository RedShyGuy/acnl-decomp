#pragma once

#include "decomp.h"
#include "esc/dEscapeBgm.h"
#include "sead/seadIDisposer.h"

// vtable +0xDEC88 in ModuleMiniGame0.cro, offset_to_top 0, 2 entries
class esc::EscapeBgm::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor address unknown
    virtual ~SingletonDisposer_(); // ModuleMiniGame0.cro +0x013A08 slot 0x00
};

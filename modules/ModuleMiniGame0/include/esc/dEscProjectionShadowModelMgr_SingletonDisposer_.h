#pragma once

#include "decomp.h"
#include "esc/dEscProjectionShadowModelMgr.h"
#include "sead/seadIDisposer.h"

// vtable +0xDEC40 in ModuleMiniGame0.cro, offset_to_top 0, 2 entries
class esc::EscProjectionShadowModelMgr::SingletonDisposer_ : public ::sead::IDisposer
{
public:
    SingletonDisposer_(); // ctor address unknown
    virtual ~SingletonDisposer_(); // ModuleMiniGame0.cro +0x01223C slot 0x00
};

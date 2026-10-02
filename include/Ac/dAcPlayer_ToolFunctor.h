#pragma once

#include "decomp.h"
#include "Ac/dAcPlayer.h"
#include "Other/dObjcBody.h"
#include "Other/dObjcBody_Functor.h"

// RTTI N8AcPlayer11ToolFunctorE @ 0x008D3FB8
// vtable 0x0090BF44 (vptr 0x0090BF4C), offset_to_top 0, 3 entries
class AcPlayer::ToolFunctor : public ::ObjcBody::Functor
{
public:
    ToolFunctor(); // ctor address unknown
    virtual void vf_0x00(); // 0x0064D9A4 slot 0x00 | virtual slot, introduced by AcPlayer::ToolFunctor
    virtual void vf_0x04(); // 0x0064D9A0 slot 0x04 | virtual slot, introduced by AcPlayer::ToolFunctor
    virtual void vf_0x08(); // 0x0064D900 slot 0x08 | virtual slot, introduced by AcPlayer::ToolFunctor
};

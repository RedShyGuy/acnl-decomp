#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 11BsPlayerMgr @ 0x008CB284
// vtable 0x008EC94C (vptr 0x008EC954), offset_to_top 0, 22 entries
class BsPlayerMgr : public ::UtlBase<Base>
{
public:
    BsPlayerMgr(); // ctor address unknown
    virtual ~BsPlayerMgr(); // 0x001C50E0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001C5038 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual oml::framework::Result Calc(); // 0x001C4BF8 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual oml::framework::Result Draw(); // 0x001C4B40 slot 0x30 | slot vf_0x30 of oml::framework::Process
    bool StartInitialize() override; // 0x001C4748 slot 0x40
    oml::framework::Result UpdateInitialize() override; // 0x001C4740 slot 0x44
    bool EndInitialize() override; // 0x001C472C slot 0x48
    bool StartFinalize() override; // 0x001C481C slot 0x4C
    oml::framework::Result UpdateFinalize() override; // 0x001C4800 slot 0x50
    bool EndFinalize() override; // 0x001C47D0 slot 0x54

    // places the other players of a multiplayer game (name is ours)
    void PlaceOtherPlayers(); // 0x001C4834
};

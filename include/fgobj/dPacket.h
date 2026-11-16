#pragma once

#include "decomp.h"
#include "fgobj/fgobj_Api.h"
#include "netgame/netgame_Api.h"
#include "Sv/dSvFgName.h"

namespace fgobj {
struct PlaceRequest;

// what happens to a tree when it is shaken (libgarden)
enum TreeShakeType : u8
{
    TREE_SHAKE_TYPE_NONE,
    TREE_SHAKE_TYPE_FRUIT,
    TREE_SHAKE_TYPE_ITEM,
    TREE_SHAKE_TYPE_BEES,
    TREE_SHAKE_TYPE_FRUIT_DIE,
};

// the parameters of a place request (libgarden: fgobj::PlaceParam)
struct PlaceParam {
    u8 layer : 1;
    u8 fallDirection : 1;
    u8 treeShakeType : 3;
    u8 isCutdown : 1;
    u8 cutCounter : 2;
};
ASSERT_SIZE(PlaceParam, 0x1);

// the network packet of the field objects (type 0x3A; layout and names after libgarden fgobj/place.hpp)
struct Packet {
    enum Type
    {
        HANDLE_REQUEST,
        REQUEST_SUCCEEDED,
        REQUEST_FAILED,
        PLACE,
        REQUEST_UNLOCK_UNIT,
        UNLOCK_UNIT_ALL,
        CONFIRM_PENDING_REQUEST,
        PLACE_AND_SEND,
        REQUEST_BEGIN_LOCK_UNIT,
        REQUEST_DONE_UNLOCK_UNIT,
        TYPE_NONE, // (the constructor)
    };

    DECOMP_NOINLINE Packet(); // 0x005A1134 | libgarden [tier A]
    // the packet of a request of this console (libgarden)
    DECOMP_NOINLINE void Assign(const PlaceRequest& request, Type type = HANDLE_REQUEST); // 0x005A02B0
    DECOMP_EMIT_COPY ~Packet() {} // 0x005A119C (only the members are destroyed)
    DECOMP_NOINLINE void Process(); // 0x0059FFC0 | libgarden [tier A]
    // a packet that places item at x, y of stage (PLACE_TYPE_FALL_ITEM from this console)
    void SetFallItem(const SvFgName* item, u8 x, u8 y, stage::Name stage); // 0x005A0204
    // processed here (single player or as player 0: PLACE / PLACE_AND_SEND), else sent (PLACE_AND_SEND)
    void Place(); // 0x0059FF40
    void Send(netgame::PlayerNo target) const
    {
        netgame::EnqueuePacket(0x3A, target, this, sizeof(*this));
    }
    // processed by the host: here (single player or host) or sent to it (name is ours)
    void SendToHost()
    {
        if (!netgame::IsMultiplePlayer() || netgame::IsHost()) {
            Process();
        } else {
            Send(netgame::PLAYER_NO_HOST);
        }
    }
    // (names are ours) the host handles a request: the unit is locked and the change placed (with the objects that fall out
    // of a shaken tree), the answer REQUEST_SUCCEEDED or REQUEST_FAILED goes back to the player
    DECOMP_NOINLINE void HandleRequest(); // 0x005A0364
    // whether the field object at the unit fits the request (replace and the conditions of the type)
    DECOMP_NOINLINE bool CanPlace() const; // 0x005A0EF8
    // the item, bells, egg or beehive of a shaken tree falls onto a free unit around it (libgarden: ProcessShakeItemTree)
    DECOMP_NOINLINE void ProcessShakeItemTree(); // 0x005A050C
    // the fruits (three, two of a palm) of a shaken tree fall onto free units around it
    DECOMP_NOINLINE void ProcessShakeFruitTree(); // 0x005A0AA0

    /* 0x00 */ SvFgName replace;
    /* 0x04 */ SvFgName fg1;
    /* 0x08 */ SvFgName fg2;
    /* 0x0C */ u8 player : 2;
    /* 0x0C */ u8 placeType : 6;
    /* 0x0D */ u8 requestIndex : 2;
    /* 0x0D */ u8 action : 4;
    /* 0x0D */ u8 unknown : 2;
    /* 0x0E */ PlaceParam param;
    /* 0x0F */ u8 dropOffset[3];
    /* 0x12 */ stage::Name stage;
    /* 0x13 */ Position8 pos;
    /* 0x15 */ u8 reserved;
};
ASSERT_SIZE(Packet, 0x16);
} // namespace fgobj

#pragma once

#include "decomp.h"
#include "nn/nex/nex_PseudoGlobalVariable.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_StreamSettings.h"

namespace nn {
namespace nex {
class Packet;

// A stream of a type (the high 4 bits of the port of a PRUDP packet); every type has its settings.
// The names of the members and of the functions without a reference name are ours.
// RTTI N2nn3nex6StreamE @ 0x008CF5D0
// vtable 0x008FFA5C (vptr 0x008FFA64), offset_to_top 0, 5 entries
class Stream : public ::nn::nex::RootObject
{
public:
    // (the values are not named yet)
    enum Type : u32
    {
    };
    // the number of stream types
    static const u32 TYPE_COUNT = 16;

    // (inline)
    explicit Stream(Type type) : m_Type(type) {}
    // (for the derived classes that are not decompiled yet)
    Stream() : m_Type(0) {}
    virtual ~Stream(); // 0x003D113C slot 0x00
    // 0x003D1124 slot 0x04 (deleting dtor)
    virtual void ReceiveIncomingPacket(unsigned short port, unsigned char session, nn::nex::Packet* pPacket) = 0; // slot 0x08
    virtual void DoWork() = 0; // slot 0x0C
    virtual bool IsDuplicateReorderingPacket(nn::nex::Packet* pPacket); // 0x003D111C slot 0x10

    // the settings of the type (in the current context)
    static nn::nex::StreamSettings* GetSettings(nn::nex::Stream::Type type); // 0x003D1060 | fefates:callgraph [tier C]
    nn::nex::StreamSettings* GetThisSettings(); // 0x003D10BC | fefates:callgraph [tier C]

    u8 m_Type; // 0x04

    static PseudoGlobalVariable<StreamSettings> s_Settings[TYPE_COUNT];
};
} // namespace nex
} // namespace nn

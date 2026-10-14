#pragma once

#include "decomp.h"
#include "nn/nex/nex_DDLTypes.h"
#include "nn/nex/nex_Message.h"
#include "nn/nex/nex_RootObject.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
class Data;

// The DDL part of Data, the polymorphic base of some DDL structures (the others derive from
// RootObject directly). The member name and the names of the slots 0x10/0x14 are ours.
// RTTI N2nn3nex9_DDL_DataE @ 0x008CF7A4
class _DDL_Data : public ::nn::nex::RootObject
{
public:
    // (inline)
    _DDL_Data() : m_StructureVersion(0) {}
    virtual ~_DDL_Data() {}
    virtual nn::nex::Data* Clone() const; // 0x0072EB7C slot 0x08 | fefates:bytes
    virtual nn::nex::String GetDataType() const; // 0x0072EB4C slot 0x0C | mk7dlp:bytes
    // the name is the one of the structure / of the structure or a base
    virtual bool IsA(const nn::nex::String& name) const; // 0x0072EB60 slot 0x10
    virtual bool IsAKindOf(const nn::nex::String& name) const; // 0x0072EBA8 slot 0x14
    virtual void StreamIn(nn::nex::Message* pMessage) const; // 0x003D9AF4 slot 0x18
    virtual void StreamOut(nn::nex::Message* pMessage); // 0x003D9B68 slot 0x1C

    static void Add(nn::nex::Message* pMessage, const nn::nex::_DDL_Data& data); // 0x003D9B04 | fefates:bytes [tier B]
    static void Extract(nn::nex::Message* pMessage, nn::nex::_DDL_Data* pData); // 0x003D9B78 | fefates:bytes [tier B]

    u8 m_StructureVersion; // 0x04
};
} // namespace nex
} // namespace nn

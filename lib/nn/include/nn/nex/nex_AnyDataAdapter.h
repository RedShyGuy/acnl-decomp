#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectAdapter.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_Data.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// The adapter of any<Data,string>: a prototype per type name; a read object is a clone of the
// prototype of its type (or of "DynamicData", which keeps the type name). The clients register
// the use (Register / Unregister). The names of the functions without a reference name and of the
// statics are ours.
// RTTI N2nn3nex14AnyDataAdapterE @ 0x008CE2CC
// vtable 0x008FCA3C (vptr 0x008FCA44), offset_to_top 0, 4 entries
class AnyDataAdapter : public ::nn::nex::AnyObjectAdapter<nn::nex::Data, nn::nex::String>
{
public:
    // (inline)
    AnyDataAdapter() {}
    virtual ~AnyDataAdapter(); // 0x00370800 slot 0x00 | fefates:bytes
    // 0x003707C8 slot 0x04 (deleting dtor)
    virtual void StreamIn(nn::nex::Message* pMessage, const nn::nex::Data& data) const; // 0x0072ABF0 slot 0x08 | fefates:bytes
    virtual void StreamOut(nn::nex::Message* pMessage, nn::nex::Data** ppData) const; // 0x0072AC9C slot 0x0C | fefates:bytes

    // a use of the adapter (the first one creates the prototype list)
    static void Register(); // 0x003706E0 | fefates:callgraph [tier C]
    // the last use deletes the prototype list (not the prototypes)
    static void Unregister(); // 0x00370260 | fefates:callgraph [tier C]
    // false if there is a prototype of the type or the clone of the prototype has another type
    bool RegisterPrototype(nn::nex::Data* pPrototype); // 0x00370380
    // the prototype of the type is deleted; false if there is none
    bool UnregisterPrototype(const nn::nex::String& type); // 0x00370518
    void UnregisterAllPrototypes(); // 0x003705E8 | fefates:bytes [tier B]
    nn::nex::Data* FindPrototype(const nn::nex::String& type) const; // 0x0072AB70 | fefates:bytes [tier B]

    static u32 s_UseCount;
    static qList<Data*>* s_pPrototypes;
    static AnyDataAdapter s_Instance;
    static CriticalSection s_Lock;
};

// A Data of a type that the client does not know: the type name and the header flag of the message
// are kept. (There is no such class in this binary; the two members are from
// AnyDataAdapter::StreamOut, the names are ours.)
class DynamicData : public ::nn::nex::Data
{
public:
    u8 m_Unknown0x08[0x20];         // 0x08
    String m_DataType;              // 0x28
    bool m_IsWithoutStructureHeader; // 0x30
};
ASSERT_OFFSET(DynamicData, m_DataType, 0x28);
ASSERT_OFFSET(DynamicData, m_IsWithoutStructureHeader, 0x30);
} // namespace nex
} // namespace nn

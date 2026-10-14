#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectAdapter.h"
#include "nn/nex/nex_CriticalSection.h"
#include "nn/nex/nex_Gathering.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_qList.h"

namespace nn {
namespace nex {
// The adapter of any<Gathering,string>, like AnyDataAdapter but with the lock around every access
// and the type "PersistentGathering" called "Community" in the messages; an unknown type is read
// into a clone of "DynamicGathering". The names of the functions without a reference name, of the
// member and of the statics are ours.
// RTTI N2nn3nex19AnyGatheringAdapterE @ 0x008CE778
// vtable 0x008FD5D0 (vptr 0x008FD5D8), offset_to_top 0, 4 entries
class AnyGatheringAdapter : public ::nn::nex::AnyObjectAdapter<nn::nex::Gathering, nn::nex::String>
{
public:
    // (inline)
    AnyGatheringAdapter() : m_UseCount(0) {}
    virtual ~AnyGatheringAdapter(); // 0x00391560 slot 0x00
    // 0x0039155C slot 0x04 (deleting dtor)
    virtual void StreamIn(nn::nex::Message* pMessage, const nn::nex::Gathering& gathering) const; // 0x0072C890 slot 0x08 | fefates:bytes
    virtual void StreamOut(nn::nex::Message* pMessage, nn::nex::Gathering** ppGathering) const; // 0x0072C998 slot 0x0C | fefates:bytes

    // a use of the adapter (the first one creates the prototype list); the number of uses
    static u32 RegisterUse(); // 0x00390FC0 | fefates:callseq [tier C]
    // the last use deletes the prototypes and their list; the number of uses
    static u32 UnregisterUse(); // 0x003910AC | fefates:callgraph [tier C]
    // false if there is a prototype of the type or the clone of the prototype has another type
    bool RegisterPrototype(nn::nex::Gathering* pPrototype); // 0x003911E0 | fefates:callseq [tier C]
    // the prototype of the type is deleted; false if there is none
    bool UnregisterPrototype(const nn::nex::String& type); // 0x003913C4
    void UnregisterAllPrototypes(); // 0x003914CC
    nn::nex::Gathering* FindPrototype(const nn::nex::String& type) const; // 0x0072C7D4 | fefates:callgraph [tier C]

    u32 m_UseCount; // 0x04

    static qList<Gathering*>* s_pPrototypes;
    static AnyGatheringAdapter s_Instance;
    static CriticalSection s_Lock;
};
ASSERT_SIZE(AnyGatheringAdapter, 0x8);
} // namespace nex
} // namespace nn

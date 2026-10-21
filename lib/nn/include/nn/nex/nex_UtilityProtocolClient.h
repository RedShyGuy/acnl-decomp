#pragma once

#include "decomp.h"
#include "nn/nex/nex_ClientProtocol.h"
#include "nn/nex/nex_qMap.h"

namespace nn {
namespace nex {
class ProtocolCallContext;

// The client of the utility protocol (protocol id 110). Only method 7 is called in this build. The
// names of the members and of the methods are ours.
// RTTI N2nn3nex21UtilityProtocolClientE @ 0x008CEB68
// vtable 0x008FDF38 (vptr 0x008FDF40), offset_to_top 0, 23 entries
class UtilityProtocolClient : public ::nn::nex::ClientProtocol
{
public:
    // the methods of the protocol
    enum Method
    {
        METHOD_GET_INTEGER_SETTINGS = 7,
        METHOD_COUNT = 8
    };
    // the extraction of the results of a method
    typedef void (*ProtoReturnFunction)(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext);

    // id: the protocol id (0: the responder, not registered)
    // (out of line: the constructor of the responder is called)
    explicit DECOMP_NOINLINE UtilityProtocolClient(unsigned short id); // 0x0039B574
    virtual ~UtilityProtocolClient(); // 0x00370EEC slot 0x00
    // 0x0039B65C slot 0x04 (deleting dtor)
    virtual void ExtractCallSpecificResults(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x0039AFA4 slot 0x50
    virtual nn::nex::ClientProtocol* CreateResponder() const; // 0x0072CE48 slot 0x54

    // the integer settings of the index (key -> value)
    bool CallGetIntegerSettings(nn::nex::ProtocolCallContext* pContext, const unsigned int& index, nn::nex::qMap<u16, u32>* pSettings); // 0x0039AE48
    static void ProtoReturn_GetIntegerSettings(nn::nex::Message* pMessage, nn::nex::ProtocolCallContext* pContext); // 0x0039B028

    // the extraction of the results of the methods (by the method id - 1; set by the calls)
    static ProtoReturnFunction s_ProtoReturnFunctions[METHOD_COUNT];
};
ASSERT_SIZE(UtilityProtocolClient, 0x44);

// (out of line in the original; nex_UtilityProtocolClient.cpp)
template <>
DECOMP_NOINLINE qMap<u16, u32>::qMap();
template <>
DECOMP_NOINLINE qMap<u16, u32>::~qMap();
} // namespace nex
} // namespace nn

namespace __rw {
template <>
DECOMP_NOINLINE __rb_tree<u16, std::pair<const u16, u32>, __select1st<std::pair<const u16, u32>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, u32> > >::iterator
__rb_tree<u16, std::pair<const u16, u32>, __select1st<std::pair<const u16, u32>, u16>, std::less<u16>, nn::nex::MemAllocator<std::pair<const u16, u32> > >::erase(iterator first,
                                                                                                                                                               iterator last);
} // namespace __rw

namespace std {
// (inline: the insert of operator[] is inline, its _C_insert out of line)
template <>
inline u32& map<u16, u32, less<u16>, nn::nex::MemAllocator<pair<const u16, u32> > >::operator[](const u16& key)
{
    return IndexInline(key);
}
} // namespace std

#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_ListBase.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/pia_Types.h"
#include "nn/pia/transport/transport_Protocol.h"
#include "nn/pia/transport/transport_ProtocolId.h"
#include <new>

namespace nn {
namespace pia {
namespace transport {
class PacketHandler;
class ProtocolEvent;

// RTTI N2nn3pia9transport15ProtocolManagerE @ 0x008D01A0
// vtable 0x00901DE0 (vptr 0x00901DE8), offset_to_top 0, 1 entries
//
// The protocols of the transport (Transport holds it at 0x10), sorted by their id. Layout from
// the constructor; the member names, Initialize and CreateProtocol / GetProtocol are ours.
class ProtocolManager : public ::nn::pia::common::RootObject
{
public:
    ProtocolManager(); // 0x00451090 | fefates:bytes [tier B]
    ~ProtocolManager(); // 0x004510CC | fefates:bytes [tier B]
    virtual void Trace(u64 flag) const; // 0x007352EC slot 0x00

    nn::Result Initialize(); // 0x00450848 (name is ours)
    void Finalize(); // 0x00451034 | fefates:bytes [tier B]
    nn::Result Startup(nn::pia::transport::PacketHandler* pPacketHandler); // 0x00450F30 | fefates:bytes [tier B]
    void Cleanup(); // 0x00450ED8 | fefates:bytes [tier B]
    nn::Result StartupProtocols(nn::pia::StationIndex stationIndex); // 0x00450A14 | fefates:bytes [tier B]
    void CleanupProtocols(); // 0x004509BC | fefates:bytes [tier B]
    nn::Result Dispatch(); // 0x00450FBC | fefates:bytes [tier B]
    nn::Result UpdateProtocolEvent(const nn::pia::transport::ProtocolEvent& event); // 0x00450E4C | fefates:bytes [tier B]

    // zeroed memory for a protocol on the current heap
    void* AllocProtocol(unsigned int size); // 0x00450858 | fefates:bytes [tier B]
    // adds the protocol under the id (its port is set); INVALID if there is one with that id
    ProtocolId CreateProtocolImpl(nn::pia::transport::Protocol* pProtocol, nn::pia::transport::ProtocolId protocolId); // 0x00450AEC | fefates:bytes [tier B]
    void DestroyProtocol(unsigned int protocolId); // 0x00450904 | fefates:bytes [tier B]
    // the protocol with the id if it has the type
    Protocol* SearchProtocol(nn::pia::transport::ProtocolId protocolId, unsigned short protocolType); // 0x004508A4 | fefates:bytes [tier B]

    // (inline everywhere)
    template <typename T>
    DECOMP_ALWAYS_INLINE ProtocolId CreateProtocol(u16 protocolType, u16 port)
    {
        if (m_IsStarted) {
            return ProtocolId(0, 0);
        }
        void* p = AllocProtocol(sizeof(T));
        T* pProtocol = p != nullptr ? ::new (p) T() : nullptr;
        ProtocolId protocolId;
        protocolId.SetType(protocolType);
        protocolId.SetPort(port);
        return CreateProtocolImpl(pProtocol, protocolId);
    }
    template <typename T>
    T* GetProtocol(ProtocolId protocolId, u16 protocolType)
    {
        return static_cast<T*>(SearchProtocol(protocolId, protocolType));
    }

    common::OffsetList<Protocol> m_ProtocolList; // 0x04
    bool m_IsStarted;                            // 0x18
};
ASSERT_OFFSET(ProtocolManager, m_IsStarted, 0x18);
ASSERT_SIZE(ProtocolManager, 0x1C);
} // namespace transport
} // namespace pia
} // namespace nn

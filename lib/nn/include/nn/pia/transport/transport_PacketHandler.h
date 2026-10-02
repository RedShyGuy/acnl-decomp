#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport13PacketHandlerE @ 0x008D0164
// vtable 0x00901D74 (vptr 0x00901D7C), offset_to_top 0, 16 entries
class PacketHandler : public ::nn::pia::common::RootObject
{
public:
    class Iterator;
    virtual void checkDerivedRuntimeTypeInfo(const pead::RuntimeTypeInfo::Interface*) const; // 0x00734CE0 slot 0x00 | slot vf_0x00 of nn::pia::transport::PacketHandler
    virtual void vf_0x04(); // 0x00734BEC slot 0x04 | virtual slot, introduced by nn::pia::transport::PacketHandler
    virtual ~PacketHandler(); // 0x0044ED8C slot 0x08 | fefates:bytes
    // 0x0044ED64 slot 0x0C | fefates:bytes (deleting dtor)
    virtual void AssignByStationIndex(const nn::pia::transport::ProtocolId&, nn::pia::StationIndex, unsigned int, bool); // 0x0044EAEC slot 0x10 | slot vf_0x10 of nn::pia::transport::PacketHandler
    virtual void AssignByStationBitmap(const nn::pia::transport::ProtocolId&, unsigned int, unsigned int, bool); // 0x0044EB0C slot 0x14 | slot vf_0x14 of nn::pia::transport::PacketHandler
    virtual void AssignByStationAddress(const nn::pia::transport::ProtocolId&, const nn::pia::common::StationAddress&, unsigned int, bool); // 0x0044EB14 slot 0x18 | fefates:bytes
    virtual void vf_0x1C(); // 0x0044EABC slot 0x1C | virtual slot, introduced by nn::pia::transport::PacketHandler
    virtual void AssignPacket(nn::pia::StationIndex, unsigned int, const nn::pia::common::StationAddress&, bool); // 0x0044E16C slot 0x20 | fefates:bytes
    virtual void RelayMessage(const nn::pia::transport::ProtocolMessageReader&); // 0x0044E2AC slot 0x24 | slot vf_0x24 of nn::pia::transport::PacketHandler
    virtual void BeginIteration(); // 0x0044E4E4 slot 0x28 | fefates:bytes
    virtual void IsEndIteration() const; // 0x00734BD0 slot 0x2C | fefates:bytes
    virtual void NextIteration(); // 0x0044E2B0 slot 0x30 | fefates:bytes-fuzzy
    virtual void CheckPacket(const nn::pia::common::Packet&); // 0x0044DE94 slot 0x34 | slot vf_0x34 of nn::pia::transport::PacketHandler
    virtual void CheckMessage(const nn::pia::transport::ProtocolMessageReader&); // 0x0044E1D8 slot 0x38 | slot vf_0x38 of nn::pia::transport::PacketHandler
    virtual void CheckReceive(const nn::pia::transport::ProtocolMessageReader&); // 0x0044E1E0 slot 0x3C | slot vf_0x3C of nn::pia::transport::PacketHandler
    void CleanupCore(); // 0x0044DED0 | fefates:bytes [tier B]
    void GetIterator(const nn::pia::transport::ProtocolId&); // 0x0044DF50 | fefates:bytes [tier B]
    void GetIterator(unsigned short); // 0x0044DF88 | fefates:bytes [tier B]
    void FinalizeCore(); // 0x0044E21C | fefates:bytes [tier B]
    void InitializeCore(unsigned int, bool, unsigned int); // 0x0044E518 | fefates:bytes [tier B]
    void EndDispatchCore(); // 0x0044E6A0 | fefates:bytes [tier B]
    void BeginDispatchCore(); // 0x0044E814 | fefates:bytes [tier B]
    void AssignPacketPayload(nn::pia::common::Packet*, unsigned int); // 0x0044EAC4 | fefates:bytes [tier B]
    void ReserveMessageWriter(); // 0x0044EAF4 | fefates:bytes [tier B]
    void ClearPacketAnalysisData(); // 0x0044ECAC | fefates:bytes [tier B]
    void Commit(); // 0x0044ECE8 | fefates:bytes [tier B]
    PacketHandler(); // 0x0044ED18 | fefates:bytes [tier B]
    void GetPacketAnalysisData(nn::pia::transport::PacketAnalysisData*, nn::pia::transport::PacketAnalysisData*) const; // 0x00734C44 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn

#pragma once

#include "decomp.h"
#include "nn/pia/transport/transport_PacketHandler.h"
#include "nn/pia/transport/transport_PacketStream.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport20StationPacketHandlerE @ 0x008D0224
// vtable 0x00901F00 (vptr 0x00901F08), offset_to_top 0, 16 entries
class StationPacketHandler : public ::nn::pia::transport::PacketHandler
{
public:
    virtual void checkDerivedRuntimeTypeInfo(const pead::RuntimeTypeInfo::Interface*) const; // 0x007360FC slot 0x00 | slot vf_0x00 of nn::pia::transport::PacketHandler
    virtual void vf_0x04(); // 0x007360B0 slot 0x04 | virtual slot, introduced by nn::pia::transport::PacketHandler
    virtual ~StationPacketHandler(); // 0x0045A16C slot 0x08 | slot vf_0x08 of nn::pia::transport::PacketHandler
    // 0x0045A12C slot 0x0C | fefates:bytes (deleting dtor)
    virtual void AssignByStationIndex(const nn::pia::transport::ProtocolId&, nn::pia::StationIndex, unsigned int, bool); // 0x00459974 slot 0x10 | fefates:bytes
    virtual void AssignByStationBitmap(const nn::pia::transport::ProtocolId&, unsigned int, unsigned int, bool); // 0x00459A10 slot 0x14 | fefates:bytes
    virtual void vf_0x1C(); // 0x004595B4 slot 0x1C | virtual slot, introduced by nn::pia::transport::PacketHandler
    virtual void AssignPacket(nn::pia::StationIndex, unsigned int, const nn::pia::common::StationAddress&, bool); // 0x00458D94 slot 0x20 | fefates:bytes
    virtual void RelayMessage(const nn::pia::transport::ProtocolMessageReader&); // 0x004590CC slot 0x24 | fefates:bytes
    virtual void CheckPacket(const nn::pia::common::Packet&); // 0x00458CCC slot 0x34 | fefates:callseq
    virtual void CheckMessage(const nn::pia::transport::ProtocolMessageReader&); // 0x00458FE0 slot 0x38 | fefates:bytes
    virtual void CheckReceive(const nn::pia::transport::ProtocolMessageReader&); // 0x00459050 slot 0x3C | fefates:bytes
    void EndDispatch(const nn::pia::common::Time&); // 0x0044E60C | fefates:bytes [tier B]
    void Initialize(unsigned int, bool, unsigned int); // 0x00458A60 | fefates:bytes [tier B]
    void AssignMulti(const nn::pia::transport::ProtocolId&, unsigned int, unsigned int, bool, nn::pia::StationIndex, unsigned int, bool); // 0x00458A9C | fefates:bytes [tier B]
    void AssignSingle(const nn::pia::transport::ProtocolId&, nn::pia::StationIndex, unsigned int, bool); // 0x00458E3C | fefates:bytes [tier B]
    void BeginDispatch(const nn::pia::common::Time&); // 0x00459194 | fefates:bytes [tier B]
    void UpdateMyStation(); // 0x004592CC | fefates:bytes [tier B]
    void AssignAllMessage(unsigned int, unsigned int, unsigned int, bool); // 0x00459328 | fefates:bytes [tier B]
    void AssignHostMessage(unsigned int, unsigned int, bool); // 0x0045946C | fefates:bytes [tier B]
    void AssignDirectMessage(unsigned int, unsigned int, unsigned int, bool); // 0x0045976C | fefates:bytes [tier B]
    void AssignRelayNodeMessage(unsigned int, unsigned int, unsigned int, bool); // 0x00459A7C | fefates:bytes [tier B]
    void AssignRelayStationMessage(nn::pia::StationIndex, unsigned int, unsigned int, unsigned int, bool); // 0x00459C48 | fefates:bytes [tier B]
    void Startup(nn::pia::transport::PacketStream::Writer*, nn::pia::transport::PacketStream::Reader*, const nn::pia::transport::RelayRouteManager*, const nn::pia::common::StationAddress*, const nn::pia::common::CryptoSetting*); // 0x00459DD4 | fefates:bytes [tier B]
    void AssignAll(const nn::pia::transport::ProtocolId&, unsigned int, bool); // 0x00459EA0 | fefates:bytes [tier B]
    StationPacketHandler(); // 0x0045A0FC | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn

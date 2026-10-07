#include "nn/pia/transport/transport_StationConnectionInfo.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00451234 (name is ours)
void nn::pia::transport::StationConnectionInfo::SetStationConnectionInfo(const nn::pia::transport::StationConnectionInfo& rhs)
{
    m_PublicLocation.SetStationLocation(rhs.m_PublicLocation);
    m_PrivateLocation.SetStationLocation(rhs.m_PrivateLocation);
}

// 0x0045B518 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationConnectionInfo::Deserialize(const unsigned char* pBuffer)
{
    if (!common::IsValidPointer(pBuffer)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    nn::Result result = m_PublicLocation.Deserialize(pBuffer);
    const u8* p = pBuffer + m_PublicLocation.GetSerializedSize();
    if (result.IsFailure()) {
        return result;
    }
    result = m_PrivateLocation.Deserialize(p);
    m_PrivateLocation.GetSerializedSize();
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

// 0x0045B590 (name is ours)
nn::Result nn::pia::transport::StationConnectionInfo::DeserializeLegacy(const unsigned char* pBuffer)
{
    if (!common::IsValidPointer(pBuffer)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    const u8* p = pBuffer;
    common::InetAddress address;
    nn::Result result = address.Deserialize(p);
    p += common::InetAddress::SERIALIZED_SIZE;
    if (result.IsFailure()) {
        return result;
    }
    result = m_PublicLocation.m_StationAddress.SetInetAddress(address);
    if (result.IsFailure()) {
        return result;
    }
    result = address.Deserialize(p);
    p += common::InetAddress::SERIALIZED_SIZE;
    if (result.IsFailure()) {
        return result;
    }
    result = m_PrivateLocation.m_StationAddress.SetInetAddress(address);
    if (result.IsFailure()) {
        return result;
    }
    m_PublicLocation.m_StationAddress.SetExtensionId(common::deserializeU16(p));
    p += sizeof(u16);
    m_PrivateLocation.m_StationAddress.SetExtensionId(common::deserializeU16(p));
    p += sizeof(u16);
    // the locations, packed (see StationLocation)
    m_PublicLocation.m_PrincipalId = common::deserializeU32(p);
    p += sizeof(u32);
    m_PublicLocation.m_Type = common::deserializeU32(p);
    p += sizeof(u32);
    u32 value = common::deserializeU32(p);
    m_PublicLocation.m_ConnectionId = value & 0x3FFFFF;
    m_PublicLocation.m_UrlType = (value >> 22) & 3;
    m_PublicLocation.m_StreamId = value >> 24;
    value = common::deserializeU32(p + sizeof(u32));
    m_PublicLocation.m_StationKey = value & 0x3FFFFF;
    m_PublicLocation.m_NatMapping = (value >> 24) & 3;
    m_PublicLocation.m_NatFiltering = (value >> 26) & 3;
    m_PublicLocation.m_StreamType = value >> 28;
    p += 2 * sizeof(u32);
    m_PrivateLocation.m_PrincipalId = common::deserializeU32(p);
    p += sizeof(u32);
    m_PrivateLocation.m_Type = common::deserializeU32(p);
    p += sizeof(u32);
    value = common::deserializeU32(p);
    m_PrivateLocation.m_ConnectionId = value & 0x3FFFFF;
    m_PrivateLocation.m_UrlType = (value >> 22) & 3;
    m_PrivateLocation.m_StreamId = value >> 24;
    value = common::deserializeU32(p + sizeof(u32));
    m_PrivateLocation.m_StationKey = value & 0x3FFFFF;
    m_PrivateLocation.m_NatMapping = (value >> 24) & 3;
    m_PrivateLocation.m_NatFiltering = (value >> 26) & 3;
    m_PrivateLocation.m_StreamType = value >> 28;
    return nn::Result();
}

// 0x0045B780 | fefates:bytes [tier B]
nn::pia::transport::StationConnectionInfo::StationConnectionInfo(const nn::pia::transport::StationConnectionInfo& rhs)
{
    SetStationConnectionInfo(rhs);
}

// 0x0045B7C4 | fefates:bytes [tier B]
nn::pia::transport::StationConnectionInfo::StationConnectionInfo()
{
    // only the locations
}

// 0x0045B810 | fefates:callgraph
// 0x0045B7E8 (deleting dtor)
nn::pia::transport::StationConnectionInfo::~StationConnectionInfo()
{
    // empty (in the original too: only the destructors of the locations)
}

// 0x0045B834 | fefates:bytes [tier B]
nn::pia::transport::StationConnectionInfo& nn::pia::transport::StationConnectionInfo::operator=(const nn::pia::transport::StationConnectionInfo& rhs)
{
    if (this != &rhs) {
        SetStationConnectionInfo(rhs);
    }
    return *this;
}

// 0x00736228 | fefates:bytes
u32 nn::pia::transport::StationConnectionInfo::GetSerializedSize() const
{
    return m_PublicLocation.GetSerializedSize() + m_PrivateLocation.GetSerializedSize();
}

// 0x0073624C (name is ours)
nn::Result nn::pia::transport::StationConnectionInfo::SerializeLegacy(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const
{
    if (!common::IsValidPointer(pBuffer) || !common::IsValidPointer(pSize)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (bufferSize < GetLegacySerializedSize()) {
        return common::RESULT_BUFFER_IS_FULL;
    }
    u8* p = pBuffer;
    u32 size;
    nn::Result result = m_PublicLocation.m_StationAddress.m_InetAddress.Serialize(p, &size, common::InetAddress::SERIALIZED_SIZE);
    p += common::InetAddress::SERIALIZED_SIZE;
    if (result.IsFailure()) {
        return result;
    }
    result = m_PrivateLocation.m_StationAddress.m_InetAddress.Serialize(p, &size, common::InetAddress::SERIALIZED_SIZE);
    p += common::InetAddress::SERIALIZED_SIZE;
    if (result.IsFailure()) {
        return result;
    }
    common::serializeU16(p, m_PublicLocation.m_StationAddress.m_ExtensionId);
    p += sizeof(u16);
    common::serializeU16(p, m_PrivateLocation.m_StationAddress.m_ExtensionId);
    p += sizeof(u16);
    // the locations, packed (see StationLocation)
    common::serializeU32(p, m_PublicLocation.m_PrincipalId);
    p += sizeof(u32);
    common::serializeU32(p, m_PublicLocation.m_Type);
    p += sizeof(u32);
    common::serializeU32(p, (m_PublicLocation.m_ConnectionId & 0x3FFFFF) | ((m_PublicLocation.m_UrlType & 3) << 22) |
                                (m_PublicLocation.m_StreamId << 24));
    common::serializeU32(p + sizeof(u32), (m_PublicLocation.m_StationKey & 0x3FFFFF) | ((m_PublicLocation.m_NatMapping & 3) << 24) |
                                              ((m_PublicLocation.m_NatFiltering & 3) << 26) | (m_PublicLocation.m_StreamType << 28));
    p += 2 * sizeof(u32);
    common::serializeU32(p, m_PrivateLocation.m_PrincipalId);
    p += sizeof(u32);
    common::serializeU32(p, m_PrivateLocation.m_Type);
    p += sizeof(u32);
    common::serializeU32(p, (m_PrivateLocation.m_ConnectionId & 0x3FFFFF) | ((m_PrivateLocation.m_UrlType & 3) << 22) |
                                (m_PrivateLocation.m_StreamId << 24));
    common::serializeU32(p + sizeof(u32), (m_PrivateLocation.m_StationKey & 0x3FFFFF) | ((m_PrivateLocation.m_NatMapping & 3) << 24) |
                                              ((m_PrivateLocation.m_NatFiltering & 3) << 26) | (m_PrivateLocation.m_StreamType << 28));
    *pSize = LEGACY_SERIALIZED_SIZE;
    return nn::Result();
}

// 0x0073641C (name is ours)
u32 nn::pia::transport::StationConnectionInfo::GetLegacySerializedSize() const
{
    return 100;
}

// 0x00736424
void nn::pia::transport::StationConnectionInfo::Trace(u64) const
{
    // empty (in the original too)
}

// 0x00736428 | fefates:bytes
nn::Result nn::pia::transport::StationConnectionInfo::Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const
{
    if (!common::IsValidPointer(pBuffer) || !common::IsValidPointer(pSize)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (bufferSize < GetSerializedSize()) {
        return common::RESULT_BUFFER_IS_FULL;
    }
    u32 size;
    nn::Result result = m_PublicLocation.Serialize(pBuffer, &size, m_PublicLocation.GetSerializedSize());
    u8* p = pBuffer + m_PublicLocation.GetSerializedSize();
    if (result.IsFailure()) {
        return result;
    }
    result = m_PrivateLocation.Serialize(p, &size, m_PrivateLocation.GetSerializedSize());
    p += m_PrivateLocation.GetSerializedSize();
    if (result.IsFailure()) {
        return result;
    }
    *pSize = p - pBuffer;
    return nn::Result();
}

// 0x00736508 | fefates:bytes [tier B]
bool nn::pia::transport::StationConnectionInfo::operator==(const nn::pia::transport::StationConnectionInfo& rhs) const
{
    return m_PublicLocation == rhs.m_PublicLocation && m_PrivateLocation == rhs.m_PrivateLocation;
}

} // namespace transport
} // namespace pia
} // namespace nn

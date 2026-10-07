#include "nn/pia/transport/transport_StationLocation.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"

namespace nn {
namespace pia {
namespace transport {
// 0x00451138 | fefates:bytes [tier B]
nn::Result nn::pia::transport::StationLocation::Deserialize(const unsigned char* pBuffer)
{
    if (!common::IsValidPointer(pBuffer)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    nn::Result result = m_StationAddress.Deserialize(pBuffer);
    const u8* p = pBuffer + m_StationAddress.GetSerializedSize();
    if (result.IsFailure()) {
        return result;
    }
    m_PrincipalId = common::deserializeU32(p);
    p += sizeof(u32);
    m_ConnectionId = common::deserializeU32(p);
    p += sizeof(u32);
    m_StationKey = common::deserializeU32(p);
    p += sizeof(u32);
    m_UrlType = *p++;
    m_StreamId = *p++;
    m_StreamType = *p++;
    m_NatMapping = *p++;
    m_NatFiltering = *p++;
    m_Type = *p++;
    m_ProbeRequestInitiation = *p;
    return nn::Result();
}

// 0x0045122C (name is ours)
void nn::pia::transport::StationLocation::SetStationAddress(const common::StationAddress& address)
{
    m_StationAddress = address;
}

// 0x0045125C | fefates:bytes [tier B]
void nn::pia::transport::StationLocation::SetStationLocation(const nn::pia::transport::StationLocation& rhs)
{
    m_StationAddress = rhs.m_StationAddress;
    m_UrlType = rhs.m_UrlType;
    m_PrincipalId = rhs.m_PrincipalId;
    m_ConnectionId = rhs.m_ConnectionId;
    m_StationKey = rhs.m_StationKey;
    m_StreamId = rhs.m_StreamId;
    m_StreamType = rhs.m_StreamType;
    m_Type = rhs.m_Type;
    m_NatMapping = rhs.m_NatMapping;
    m_NatFiltering = rhs.m_NatFiltering;
    m_ProbeRequestInitiation = rhs.m_ProbeRequestInitiation;
}

// 0x004512C8 | fefates:bytes [tier B]
nn::pia::transport::StationLocation::StationLocation(const nn::pia::transport::StationLocation& rhs)
{
    SetStationLocation(rhs);
}

// 0x00451344 | fefates:bytes [tier B]
nn::pia::transport::StationLocation::StationLocation()
    : m_PrincipalId(0), m_ConnectionId(0), m_StationKey(0), m_UrlType(0), m_StreamId(0), m_StreamType(0),
      m_NatMapping(0), m_NatFiltering(0), m_Type(0), m_ProbeRequestInitiation(0)
{
}

// 0x004513B8 | fefates:bytes
// 0x0045138C (deleting dtor)
nn::pia::transport::StationLocation::~StationLocation()
{
    // empty (in the original too: only the destructor of the address)
}

// 0x004513E0 | fefates:bytes [tier B]
nn::pia::transport::StationLocation& nn::pia::transport::StationLocation::operator=(const nn::pia::transport::StationLocation& rhs)
{
    if (this != &rhs) {
        SetStationLocation(rhs);
    }
    return *this;
}

// 0x007352F0 | fefates:bytes
u32 nn::pia::transport::StationLocation::GetSerializedSize() const
{
    return m_StationAddress.GetSerializedSize() + 3 * sizeof(u32) + 7;
}

// 0x00735304
void nn::pia::transport::StationLocation::Trace(unsigned long long) const
{
    // empty (in the original too)
}

// 0x00735308 | fefates:bytes
nn::Result nn::pia::transport::StationLocation::Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const
{
    if (!common::IsValidPointer(pBuffer) || !common::IsValidPointer(pSize)) {
        return common::RESULT_INVALID_ARGUMENT;
    }
    if (bufferSize < GetSerializedSize()) {
        return common::RESULT_BUFFER_IS_FULL;
    }
    u32 size;
    nn::Result result = m_StationAddress.Serialize(pBuffer, &size, m_StationAddress.GetSerializedSize());
    u8* p = pBuffer + m_StationAddress.GetSerializedSize();
    if (result.IsFailure()) {
        return result;
    }
    common::serializeU32(p, m_PrincipalId);
    p += sizeof(u32);
    common::serializeU32(p, m_ConnectionId);
    p += sizeof(u32);
    common::serializeU32(p, m_StationKey);
    p += sizeof(u32);
    *p++ = m_UrlType;
    *p++ = m_StreamId;
    *p++ = m_StreamType;
    *p++ = m_NatMapping;
    *p++ = m_NatFiltering;
    *p++ = m_Type;
    *p++ = m_ProbeRequestInitiation;
    *pSize = p - pBuffer;
    return nn::Result();
}

// 0x00735450 | fefates:bytes [tier B]
bool nn::pia::transport::StationLocation::operator==(const nn::pia::transport::StationLocation& rhs) const
{
    return m_StationAddress == rhs.m_StationAddress && m_PrincipalId == rhs.m_PrincipalId && m_ConnectionId == rhs.m_ConnectionId &&
           m_StationKey == rhs.m_StationKey && m_Type == rhs.m_Type && m_UrlType == rhs.m_UrlType &&
           m_StreamId == rhs.m_StreamId && m_StreamType == rhs.m_StreamType && m_NatMapping == rhs.m_NatMapping &&
           m_NatFiltering == rhs.m_NatFiltering && m_ProbeRequestInitiation == rhs.m_ProbeRequestInitiation;
}

} // namespace transport
} // namespace pia
} // namespace nn

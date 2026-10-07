#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/common/common_InetAddress.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common14StationAddressE @ 0x008CFE38
// vtable 0x0090152C (vptr 0x00901534), offset_to_top 0, 1 entries
//
// The address of a station: its internet address and an extension id. Layout from the
// constructors; the member names are ours.
class StationAddress : public ::nn::pia::common::RootObject
{
public:
    // the serialized size (internet address and extension id)
    static const u32 SERIALIZED_SIZE = 8;

    StationAddress(); // 0x00426EF0 | fefates:bytes [tier B]
    StationAddress(const nn::pia::common::StationAddress& rhs); // 0x00426EC4 | fefates:bytes [tier B]
    // (inline; ARMCC also emitted copies at 0x00426F14 and 0x00427B48)
    ~StationAddress() {} // 0x00426F14 (also out of line in the original, unused)
    StationAddress& operator=(const nn::pia::common::StationAddress& rhs); // 0x00426F40 | fefates:bytes [tier B]

    virtual void Trace(u64 flag) const; // 0x00731978 slot 0x00 (name after StepSequenceJob::Trace)

    nn::Result Deserialize(const unsigned char* pBuffer); // 0x00426DB0 | fefates:bytes [tier B]
    nn::Result Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const; // 0x007319A4 | fefates:bytes [tier B]
    nn::Result SetExtensionId(unsigned short extensionId); // 0x00426E00 | fefates:callgraph [tier C]
    nn::Result SetInetAddress(const nn::pia::common::InetAddress& address); // 0x00426E0C | fefates:bytes [tier B]
    void Clear(); // 0x00426E20 | fefates:bytes [tier B]
    bool IsValid() const; // 0x0073197C | fefates:bytes [tier B]

    // -1, 0 or 1 by the internet address and then the extension id
    static int Compare(const nn::pia::common::StationAddress& lhs, const nn::pia::common::StationAddress& rhs); // 0x00426E3C | fefates:bytes [tier B]
    bool operator==(const nn::pia::common::StationAddress& rhs) const; // 0x00731A1C | fefates:bytes [tier B]
    bool operator<(const nn::pia::common::StationAddress& rhs) const; // 0x00731A70 | fefates:bytes [tier B]

    // (inline; ARMCC kept the this computation of the call)
    u32 GetSerializedSize() const { return SERIALIZED_SIZE; }
    const InetAddress& GetInetAddress() const { return m_InetAddress; }
    u16 GetExtensionId() const { return m_ExtensionId; }

    InetAddress m_InetAddress; // 0x04
    u16 m_ExtensionId;         // 0x0C
};
ASSERT_SIZE(StationAddress, 0x10);
} // namespace common
} // namespace pia
} // namespace nn

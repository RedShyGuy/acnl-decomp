#include "nn/pia/common/common_SessionBeginMonitoringContent.h"
#include "nn/cfg/CTR/CTR_Api.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"
#include <string.h>

namespace nn {
namespace pia {
namespace common {
namespace {
// m_Version (4.11.1, the version of pia?) and the other constants Initialize sets
const u32 MONITORING_VERSION = 0x040B01FF;
const u8 VALUE_0x1C = 3;
const u8 VALUE_0x1D = 2;
// the end of the block ("PiaM")
const u32 TERMINATOR = 0x5069614D;
const u8 INVALID_U8 = 0xFF;
const u16 INVALID_U16 = 0xFFFF;
const u32 INVALID_U32 = 0xFFFFFFFF;
} // namespace

// 0x00AE6C40
SessionBeginMonitoringContent g_SessionBeginMonitoringContent;

// 0x0042843C (name is ours)
void nn::pia::common::SessionBeginMonitoringContent::Initialize()
{
    memset(this, 0xFF, sizeof(*this));
    m_Header.m_Version = MonitoringHeader::VERSION;
    m_Header.m_Type = 0;
    m_Header.m_Size = GetSerializedSize();
    m_Version = MONITORING_VERSION;
    m_Unknown0x1C = VALUE_0x1C;
    m_Unknown0x1D = VALUE_0x1D;
    nn::cfg::CTR::Initialize();
    nn::cfg::CTR::SimpleAddressId id;
    nn::cfg::CTR::GetSimpleAddressId(&id);
    m_CountryCode = id.id >> 24;
    m_RegionCode = id.id >> 16;
    nn::cfg::CTR::Finalize();
    m_Terminator = TERMINATOR;
}

// 0x004284B8 | fefates:callgraph [tier C]
unsigned int nn::pia::common::SessionBeginMonitoringContent::GetSerializedSize()
{
    return 0x486;
}

// 0x004284C4 | fefates:callseq-callee [tier C]
void nn::pia::common::SessionBeginMonitoringContent::Cleanup()
{
    m_MinRtt = INVALID_U16;
    m_MinRttPrincipalIdHash = INVALID_U32;
    m_MaxRtt = INVALID_U16;
    m_MaxRttPrincipalIdHash = INVALID_U32;
    m_Unknown0xA4 = INVALID_U8;
    m_Unknown0xA5 = INVALID_U8;
    m_JoinStationNum = INVALID_U8;
    m_JoinResult = INVALID_U32;
    m_JoinElapsedMSec = INVALID_U32;
    m_RelayedStationNum = INVALID_U8;
    for (int i = 0; i < 23; i++) {
        m_RelayedStationPrincipalIdHashes[i] = INVALID_U32;
    }
    m_RelayStationNum = INVALID_U8;
    for (int i = 0; i < 12; i++) {
        m_RelayStationPrincipalIdHashes[i] = INVALID_U32;
    }
    m_Unknown0x144 = INVALID_U16;
    m_Unknown0x180 = INVALID_U32;
    m_Unknown0x184 = INVALID_U32;
    m_Unknown0x1A0 = INVALID_U16;
    m_Unknown0x1A4 = INVALID_U32;
    m_Unknown0x1A8 = INVALID_U8;
    m_Unknown0x1A9 = INVALID_U8;
    m_Unknown0x1AA = INVALID_U8;
    m_Unknown0x1AB = INVALID_U8;
    m_Unknown0x1E4 = INVALID_U8;
    m_Unknown0x1E6 = INVALID_U8;
    m_Unknown0x1E8 = INVALID_U32;
    m_Unknown0x1EC = INVALID_U8;
    m_Unknown0x1ED = INVALID_U8;
    m_Unknown0x1EE = INVALID_U8;
    m_Unknown0x1EF = INVALID_U8;
    m_Unknown0x228 = INVALID_U8;
    m_Unknown0x22A = INVALID_U8;
    for (int i = 0; i < 6; i++) {
        m_Unknown0x188[i] = INVALID_U32;
        m_Unknown0x1AC[i] = INVALID_U8;
        m_Unknown0x1B4[i] = INVALID_U8;
        m_Unknown0x1CC[i] = INVALID_U8;
        m_Unknown0x1F0[i] = INVALID_U8;
        m_Unknown0x1F8[i] = INVALID_U8;
        m_Unknown0x210[i] = INVALID_U8;
    }
    m_Unknown0x22C = INVALID_U8;
    m_Unknown0x22D = INVALID_U8;
    m_Unknown0x230 = INVALID_U32;
    m_Unknown0x234 = INVALID_U32;
    m_Unknown0x238 = INVALID_U32;
    m_Unknown0x23C = INVALID_U8;
    m_Unknown0x23D = INVALID_U8;
    m_Unknown0x240 = INVALID_U32;
    m_Unknown0x244 = INVALID_U32;
    m_Unknown0x248 = INVALID_U32;
    m_Unknown0x24C = INVALID_U8;
    m_Unknown0x24D = INVALID_U8;
    m_HostPrincipalId = INVALID_U32;
    m_Unknown0x258 = INVALID_U32;
    for (int i = 0; i < 23; i++) {
        m_Unknown0x274[i] = INVALID_U32;
        m_Unknown0x2D0[i] = INVALID_U32;
        m_Unknown0x32C[i] = INVALID_U32;
        m_Unknown0x388[i] = INVALID_U32;
        m_Unknown0x3E4[i] = INVALID_U8;
        m_Unknown0x440[i] = INVALID_U8;
    }
    m_Unknown0x49E = INVALID_U16;
    m_Unknown0x4A0 = INVALID_U16;
    m_Unknown0x4A2 = INVALID_U16;
    m_Unknown0x49C = INVALID_U8;
    m_Unknown0x49D = INVALID_U8;
    m_BandwidthCheckPacketSize = INVALID_U16;
    m_BandwidthCheckPacketLoss = INVALID_U16;
    m_BandwidthCheckBandwidth = INVALID_U32;
    m_BandwidthCheckResult = INVALID_U32;
}

// 0x00731E44 | fefates:callgraph [tier C]
nn::Result nn::pia::common::SessionBeginMonitoringContent::Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const
{
    if (!IsValidPointer(pBuffer) || !IsValidPointer(pSize)) {
        return RESULT_INVALID_ARGUMENT;
    }
    if (bufferSize < GetSerializedSize()) {
        return RESULT_INVALID_ARGUMENT;
    }
    unsigned int size;
    SerializeHeader(pBuffer, &size, bufferSize);
    unsigned char* p = pBuffer + size;
    serializeU32(p, m_Version);
    p += 4;
    serializeU32(p, m_Unknown0x14);
    p += 4;
    serializeU32(p, m_Unknown0x18);
    p += 4;
    serializeU8(p, m_Unknown0x1C);
    p += 1;
    serializeU8(p, m_Unknown0x1D);
    p += 1;
    serializeU8(p, m_CountryCode);
    p += 1;
    serializeU8(p, m_RegionCode);
    p += 1;
    serializeU32(p, m_CommonHeapSize);
    p += 4;
    serializeU32(p, m_UnusedProtocolBitmap);
    p += 4;
    serializeU32(p, m_LocalCommunicationId);
    p += 4;
    serializeU8(p, m_NodeCountMax);
    p += 1;
    serializeU32(p, m_ReceiveBufferSize);
    p += 4;
    serializeU32(p, m_ScanBufferSize);
    p += 4;
    serializeU8(p, m_SendOption);
    p += 1;
    serializeU8(p, m_ReceiveOption);
    p += 1;
    serializeU32(p, m_Unknown0x3C);
    p += 4;
    serializeU32(p, m_Unknown0x40);
    p += 4;
    serializeU8(p, m_Unknown0x44);
    p += 1;
    serializeU8(p, m_Unknown0x45);
    p += 1;
    serializeU8(p, m_Unknown0x46);
    p += 1;
    serializeU8(p, m_Unknown0x47);
    p += 1;
    serializeU16(p, m_Unknown0x48);
    p += 2;
    serializeU16(p, m_Unknown0x4A);
    p += 2;
    serializeU16(p, m_Unknown0x4C);
    p += 2;
    serializeU32(p, m_Unknown0x50);
    p += 4;
    serializeU32(p, m_Unknown0x54);
    p += 4;
    serializeU32(p, m_Unknown0x58);
    p += 4;
    serializeU32(p, m_Unknown0x5C);
    p += 4;
    serializeU8(p, m_Unknown0x60);
    p += 1;
    serializeU32(p, m_Unknown0x64);
    p += 4;
    serializeU32(p, m_Unknown0x68);
    p += 4;
    serializeU32(p, m_Unknown0x6C);
    p += 4;
    serializeU8(p, m_Unknown0x70);
    p += 1;
    serializeU8(p, m_Unknown0x71);
    p += 1;
    serializeU16(p, m_StationNumMax);
    p += 2;
    serializeU32(p, m_SendPacketNum);
    p += 4;
    serializeU32(p, m_ReceivePacketNum);
    p += 4;
    serializeU32(p, m_ReliableSendNum);
    p += 4;
    serializeU32(p, m_ReliableReceiveNum);
    p += 4;
    serializeU32(p, m_Unknown0x84);
    p += 4;
    serializeU16(p, m_MinRtt);
    p += 2;
    serializeU32(p, m_MinRttPrincipalIdHash);
    p += 4;
    serializeU16(p, m_MaxRtt);
    p += 2;
    serializeU32(p, m_MaxRttPrincipalIdHash);
    p += 4;
    serializeU16(p, m_RelayRttLimit);
    p += 2;
    serializeU16(p, m_RelayCountMax);
    p += 2;
    serializeU16(p, m_Unknown0x9C);
    p += 2;
    serializeU32(p, m_Unknown0xA0);
    p += 4;
    serializeU8(p, m_Unknown0xA4);
    p += 1;
    serializeU8(p, m_Unknown0xA5);
    p += 1;
    serializeU8(p, m_JoinStationNum);
    p += 1;
    serializeU32(p, m_JoinResult);
    p += 4;
    serializeU32(p, m_JoinElapsedMSec);
    p += 4;
    serializeU8(p, m_RelayedStationNum);
    p += 1;
    for (u32 i = 0; i < 23; i++) {
        serializeU32(p, m_RelayedStationPrincipalIdHashes[i]);
        p += 4;
    }
    serializeU8(p, m_RelayStationNum);
    p += 1;
    for (u32 i = 0; i < 12; i++) {
        serializeU32(p, m_RelayStationPrincipalIdHashes[i]);
        p += 4;
    }
    serializeU16(p, m_Unknown0x144);
    p += 2;
    serializeU8(p, m_Unknown0x146);
    p += 1;
    serializeU8(p, m_Unknown0x147);
    p += 1;
    serializeU32(p, m_Unknown0x148);
    p += 4;
    serializeU16(p, m_Unknown0x14C);
    p += 2;
    serializeU16(p, m_Unknown0x14E);
    p += 2;
    serializeU16(p, m_Unknown0x150);
    p += 2;
    serializeU16(p, m_Unknown0x152);
    p += 2;
    serializeU16(p, m_Unknown0x154);
    p += 2;
    serializeU16(p, m_Unknown0x156);
    p += 2;
    serializeU16(p, m_Unknown0x158);
    p += 2;
    serializeU16(p, m_Unknown0x15A);
    p += 2;
    serializeU8(p, m_Unknown0x15C);
    p += 1;
    serializeU16(p, m_Unknown0x15E);
    p += 2;
    serializeU16(p, m_Unknown0x160);
    p += 2;
    serializeU16(p, m_Unknown0x162);
    p += 2;
    serializeU8(p, m_Unknown0x164);
    p += 1;
    serializeU32(p, m_Unknown0x168);
    p += 4;
    serializeU8(p, m_Unknown0x16C);
    p += 1;
    serializeU8(p, m_Unknown0x16D);
    p += 1;
    serializeU8(p, m_Unknown0x16E);
    p += 1;
    serializeU8(p, m_Unknown0x16F);
    p += 1;
    serializeU32(p, m_Unknown0x170);
    p += 4;
    serializeU32(p, m_Unknown0x174);
    p += 4;
    serializeU32(p, m_Unknown0x178);
    p += 4;
    serializeU8(p, m_Unknown0x17C);
    p += 1;
    serializeU8(p, m_Unknown0x17D);
    p += 1;
    serializeU32(p, m_Unknown0x180);
    p += 4;
    serializeU32(p, m_Unknown0x184);
    p += 4;
    for (u32 i = 0; i < 6; i++) {
        serializeU32(p, m_Unknown0x188[i]);
        p += 4;
    }
    serializeU16(p, m_Unknown0x1A0);
    p += 2;
    serializeU32(p, m_Unknown0x1A4);
    p += 4;
    serializeU8(p, m_Unknown0x1A8);
    p += 1;
    serializeU8(p, m_Unknown0x1A9);
    p += 1;
    serializeU8(p, m_Unknown0x1AA);
    p += 1;
    serializeU8(p, m_Unknown0x1AB);
    p += 1;
    for (int i = 0; i < 6; i++) {
        serializeU8(p, m_Unknown0x1AC[i]);
        p += 1;
    }
    for (int i = 0; i < 6; i++) {
        serializeU32(p, m_Unknown0x1B4[i]);
        p += 4;
    }
    for (int i = 0; i < 6; i++) {
        serializeU32(p, m_Unknown0x1CC[i]);
        p += 4;
    }
    serializeU8(p, m_Unknown0x1E4);
    p += 1;
    serializeU8(p, m_Unknown0x1E5);
    p += 1;
    serializeU8(p, m_Unknown0x1E6);
    p += 1;
    serializeU32(p, m_Unknown0x1E8);
    p += 4;
    serializeU8(p, m_Unknown0x1EC);
    p += 1;
    serializeU8(p, m_Unknown0x1ED);
    p += 1;
    serializeU8(p, m_Unknown0x1EE);
    p += 1;
    serializeU8(p, m_Unknown0x1EF);
    p += 1;
    for (int i = 0; i < 6; i++) {
        serializeU8(p, m_Unknown0x1F0[i]);
        p += 1;
    }
    for (int i = 0; i < 6; i++) {
        serializeU32(p, m_Unknown0x1F8[i]);
        p += 4;
    }
    for (int i = 0; i < 6; i++) {
        serializeU32(p, m_Unknown0x210[i]);
        p += 4;
    }
    serializeU8(p, m_Unknown0x228);
    p += 1;
    serializeU8(p, m_Unknown0x229);
    p += 1;
    serializeU8(p, m_Unknown0x22A);
    p += 1;
    serializeU8(p, m_Unknown0x22B);
    p += 1;
    serializeU8(p, m_Unknown0x22C);
    p += 1;
    serializeU8(p, m_Unknown0x22D);
    p += 1;
    serializeU32(p, m_Unknown0x230);
    p += 4;
    serializeU32(p, m_Unknown0x234);
    p += 4;
    serializeU32(p, m_Unknown0x238);
    p += 4;
    serializeU8(p, m_Unknown0x23C);
    p += 1;
    serializeU8(p, m_Unknown0x23D);
    p += 1;
    serializeU32(p, m_Unknown0x240);
    p += 4;
    serializeU32(p, m_Unknown0x244);
    p += 4;
    serializeU32(p, m_Unknown0x248);
    p += 4;
    serializeU8(p, m_Unknown0x24C);
    p += 1;
    serializeU8(p, m_Unknown0x24D);
    p += 1;
    serializeU8(p, m_Unknown0x24E);
    p += 1;
    serializeU8(p, m_Unknown0x24F);
    p += 1;
    serializeU8(p, m_Unknown0x250);
    p += 1;
    serializeU8(p, m_Unknown0x251);
    p += 1;
    serializeU8(p, m_Unknown0x252);
    p += 1;
    serializeU8(p, m_Unknown0x253);
    p += 1;
    serializeU32(p, m_HostPrincipalId);
    p += 4;
    serializeU32(p, m_Unknown0x258);
    p += 4;
    serializeU8(p, m_Unknown0x25C);
    p += 1;
    serializeU8(p, m_Unknown0x25D);
    p += 1;
    serializeU8(p, m_Unknown0x25E);
    p += 1;
    serializeU8(p, m_Unknown0x25F);
    p += 1;
    serializeU8(p, m_JoinPhase);
    p += 1;
    serializeU8(p, m_Unknown0x261);
    p += 1;
    serializeU16(p, m_BandwidthCheckPacketSize);
    p += 2;
    serializeU16(p, m_BandwidthCheckPacketLoss);
    p += 2;
    serializeU32(p, m_BandwidthCheckBandwidth);
    p += 4;
    serializeU32(p, m_BandwidthCheckResult);
    p += 4;
    serializeU16(p, m_Unknown0x270);
    p += 2;
    serializeU16(p, m_Unknown0x272);
    p += 2;
    for (int i = 0; i < 23; i++) {
        serializeU32(p, m_Unknown0x274[i]);
        p += 4;
    }
    for (u32 i = 0; i < 23; i++) {
        serializeU32(p, m_Unknown0x2D0[i]);
        p += 4;
    }
    for (u32 i = 0; i < 23; i++) {
        serializeU32(p, m_Unknown0x32C[i]);
        p += 4;
    }
    for (u32 i = 0; i < 23; i++) {
        serializeU32(p, m_Unknown0x388[i]);
        p += 4;
    }
    for (u32 i = 0; i < 23; i++) {
        serializeU32(p, m_Unknown0x3E4[i]);
        p += 4;
    }
    for (u32 i = 0; i < 23; i++) {
        serializeU8(p, m_Unknown0x440[i]);
        p += 1;
    }
    for (u32 i = 0; i < 23; i++) {
        serializeU8(p, m_Unknown0x457[i]);
        p += 1;
    }
    for (u32 i = 0; i < 23; i++) {
        serializeU8(p, m_Unknown0x46E[i]);
        p += 1;
    }
    for (u32 i = 0; i < 23; i++) {
        serializeU8(p, m_Unknown0x485[i]);
        p += 1;
    }
    serializeU8(p, m_Unknown0x49C);
    p += 1;
    serializeU8(p, m_Unknown0x49D);
    p += 1;
    serializeU16(p, m_Unknown0x49E);
    p += 2;
    serializeU16(p, m_Unknown0x4A0);
    p += 2;
    serializeU16(p, m_Unknown0x4A2);
    p += 2;
    for (u32 i = 0; i < 8; i++) {
        serializeU8(p, m_Unknown0x4A4[i]);
        p += 1;
    }
    serializeU32(p, m_Terminator);
    p += 4;
    *pSize = p - pBuffer;
    return nn::Result();
}

} // namespace common
} // namespace pia
} // namespace nn

#include "nn/pia/common/common_SessionStateMonitoringContent.h"
#include "nn/pia/common/common_Api.h"
#include "nn/pia/common/common_Result.h"
#include <string.h>

namespace nn {
namespace pia {
namespace common {
namespace {
// the end of the block ("PiaM")
const u32 TERMINATOR = 0x5069614D;
const u8 INVALID_U8 = 0xFF;
const u16 INVALID_U16 = 0xFFFF;
const u32 INVALID_U32 = 0xFFFFFFFF;
const u64 INVALID_U64 = 0xFFFFFFFFFFFFFFFFull;
} // namespace

// 0x00AE70F0
SessionStateMonitoringContent g_SessionStateMonitoringContent;

// 0x004286D4 (name is ours)
void nn::pia::common::SessionStateMonitoringContent::Initialize()
{
    memset(this, 0xFF, sizeof(*this));
    m_Header.m_Version = MonitoringHeader::VERSION;
    m_Header.m_Type = 1;
    m_Header.m_Size = GetSerializedSize();
    m_Terminator = TERMINATOR;
}

// 0x00428710 (name is ours)
unsigned int nn::pia::common::SessionStateMonitoringContent::GetSerializedSize()
{
    return 0x3D1;
}

// 0x0042871C | fefates:callseq-callee [tier C]
void nn::pia::common::SessionStateMonitoringContent::Cleanup()
{
    m_DispatchCount = INVALID_U32;
    m_Unknown0x1C = INVALID_U32;
    m_Unknown0x3C9 = INVALID_U8;
    m_Unknown0x3CA = INVALID_U8;
    m_Unknown0x3CB = INVALID_U8;
    m_Unknown0x3CC = INVALID_U8;
    m_Unknown0x3CD = INVALID_U8;
    m_Unknown0x3D8 = INVALID_U8;
    for (int i = 0; i < 16; i++) {
        m_Unknown0x20[i] = INVALID_U32;
        m_Unknown0x60[i] = INVALID_U32;
        m_Unknown0xA0[i] = INVALID_U64;
        m_Unknown0x120[i] = INVALID_U32;
        m_Unknown0x160[i] = INVALID_U32;
        m_Unknown0x1A0[i] = INVALID_U64;
    }
    for (int i = 0; i < 23; i++) {
        m_Unknown0x220[i] = INVALID_U32;
        m_Unknown0x27C[i] = INVALID_U8;
        m_Unknown0x294[i] = INVALID_U16;
        m_Unknown0x2C2[i] = INVALID_U16;
    }
    m_Unknown0x2F0 = INVALID_U8;
    m_Unknown0x2F1 = INVALID_U8;
    m_Unknown0x2F2 = INVALID_U16;
    m_Unknown0x2F4 = INVALID_U32;
    m_Unknown0x2F8 = INVALID_U16;
    m_Unknown0x2FC = INVALID_U32;
    for (int i = 0; i < 4; i++) {
        m_Unknown0x308[i] = INVALID_U32;
    }
    for (int i = 0; i < 4; i++) {
        m_Unknown0x318[i] = INVALID_U32;
    }
    m_Unknown0x328 = INVALID_U32;
    m_Unknown0x32C = INVALID_U32;
    m_Unknown0x330 = INVALID_U8;
    m_Unknown0x3C4 = INVALID_U32;
    m_Unknown0x334 = INVALID_U32;
    m_Unknown0x338 = INVALID_U8;
    m_Unknown0x339 = INVALID_U8;
    m_Unknown0x33C = INVALID_U32;
    m_Unknown0x340 = INVALID_U32;
    m_Unknown0x344 = INVALID_U16;
    m_Unknown0x346 = INVALID_U16;
    m_Unknown0x348 = INVALID_U32;
    m_Unknown0x34C = INVALID_U8;
    m_Unknown0x34D = INVALID_U8;
    m_Unknown0x3D9 = INVALID_U8;
    m_Unknown0x3C8 = INVALID_U8;
    m_Unknown0x3D0 = INVALID_U32;
    m_Unknown0x3D4 = INVALID_U8;
    m_Unknown0x3D5 = INVALID_U8;
    m_Unknown0x3D6 = INVALID_U8;
    m_Unknown0x3D7 = INVALID_U8;
    m_Unknown0x34E = INVALID_U8;
    m_Unknown0x34F = INVALID_U8;
    m_Unknown0x350 = INVALID_U8;
    m_Unknown0x351 = INVALID_U8;
    m_Unknown0x354 = INVALID_U32;
    m_Unknown0x358 = INVALID_U8;
    m_Unknown0x35C = INVALID_U32;
    m_Unknown0x360 = INVALID_U8;
    m_Unknown0x361 = INVALID_U8;
    m_Unknown0x364 = INVALID_U32;
    m_Unknown0x368 = INVALID_U32;
    memset(m_Unknown0x36C, 0xFF, sizeof(m_Unknown0x36C));
    memset(m_Unknown0x370, 0xFF, sizeof(m_Unknown0x370));
    m_Unknown0x378 = INVALID_U32;
    m_Unknown0x37C = INVALID_U16;
    m_Unknown0x380 = INVALID_U32;
    m_Unknown0x384 = INVALID_U32;
}

// 0x00732A28 (name is ours)
nn::Result nn::pia::common::SessionStateMonitoringContent::Serialize(unsigned char* pBuffer, unsigned int* pSize, unsigned int bufferSize) const
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
    serializeU32(p, m_DispatchCount);
    p += 4;
    serializeU32(p, m_Unknown0x14);
    p += 4;
    serializeU32(p, m_Unknown0x18);
    p += 4;
    serializeU32(p, m_Unknown0x1C);
    p += 4;
    for (u32 i = 0; i < 16; i++) {
        serializeU32(p, m_Unknown0x20[i]);
        p += 4;
    }
    for (u32 i = 0; i < 16; i++) {
        serializeU32(p, m_Unknown0x60[i]);
        p += 4;
    }
    for (u32 i = 0; i < 16; i++) {
        serializeU64(p, m_Unknown0xA0[i]);
        p += 8;
    }
    for (u32 i = 0; i < 16; i++) {
        serializeU32(p, m_Unknown0x120[i]);
        p += 4;
    }
    for (u32 i = 0; i < 16; i++) {
        serializeU32(p, m_Unknown0x160[i]);
        p += 4;
    }
    for (u32 i = 0; i < 16; i++) {
        serializeU64(p, m_Unknown0x1A0[i]);
        p += 8;
    }
    for (u32 i = 0; i < 23; i++) {
        serializeU32(p, m_Unknown0x220[i]);
        p += 4;
    }
    for (u32 i = 0; i < 23; i++) {
        serializeU8(p, m_Unknown0x27C[i]);
        p += 1;
    }
    for (u32 i = 0; i < 23; i++) {
        serializeU16(p, m_Unknown0x294[i]);
        p += 2;
    }
    for (u32 i = 0; i < 23; i++) {
        serializeU16(p, m_Unknown0x2C2[i]);
        p += 2;
    }
    serializeU8(p, m_Unknown0x2F0);
    p += 1;
    serializeU8(p, m_Unknown0x2F1);
    p += 1;
    serializeU16(p, m_Unknown0x2F2);
    p += 2;
    serializeU32(p, m_Unknown0x2F4);
    p += 4;
    serializeU16(p, m_Unknown0x2F8);
    p += 2;
    serializeU32(p, m_Unknown0x2FC);
    p += 4;
    serializeU32(p, m_Unknown0x300);
    p += 4;
    serializeU32(p, m_Unknown0x304);
    p += 4;
    for (u32 i = 0; i < 4; i++) {
        serializeU32(p, m_Unknown0x308[i]);
        p += 4;
    }
    for (u32 i = 0; i < 4; i++) {
        serializeU32(p, m_Unknown0x318[i]);
        p += 4;
    }
    serializeU32(p, m_Unknown0x328);
    p += 4;
    serializeU32(p, m_Unknown0x32C);
    p += 4;
    serializeU8(p, m_Unknown0x330);
    p += 1;
    serializeU32(p, m_Unknown0x334);
    p += 4;
    serializeU8(p, m_Unknown0x338);
    p += 1;
    serializeU8(p, m_Unknown0x339);
    p += 1;
    serializeU32(p, m_Unknown0x33C);
    p += 4;
    serializeU32(p, m_Unknown0x340);
    p += 4;
    serializeU16(p, m_Unknown0x344);
    p += 2;
    serializeU16(p, m_Unknown0x346);
    p += 2;
    serializeU32(p, m_Unknown0x348);
    p += 4;
    serializeU8(p, m_Unknown0x34C);
    p += 1;
    serializeU8(p, m_Unknown0x34D);
    p += 1;
    serializeU8(p, m_Unknown0x34E);
    p += 1;
    serializeU8(p, m_Unknown0x34F);
    p += 1;
    serializeU8(p, m_Unknown0x350);
    p += 1;
    serializeU8(p, m_Unknown0x351);
    p += 1;
    serializeU32(p, m_Unknown0x354);
    p += 4;
    serializeU8(p, m_Unknown0x358);
    p += 1;
    serializeU32(p, m_Unknown0x35C);
    p += 4;
    serializeU8(p, m_Unknown0x360);
    p += 1;
    serializeU8(p, m_Unknown0x361);
    p += 1;
    serializeU32(p, m_Unknown0x364);
    p += 4;
    serializeU32(p, m_Unknown0x368);
    p += 4;
    for (u32 i = 0; i < 4; i++) {
        serializeU8(p, m_Unknown0x36C[i]);
        p += 1;
    }
    for (u32 i = 0; i < 5; i++) {
        serializeU8(p, m_Unknown0x370[i]);
        p += 1;
    }
    serializeU32(p, m_Unknown0x378);
    p += 4;
    serializeU16(p, m_Unknown0x37C);
    p += 2;
    serializeU8(p, m_Unknown0x37E);
    p += 1;
    serializeU32(p, m_Unknown0x380);
    p += 4;
    serializeU32(p, m_Unknown0x384);
    p += 4;
    serializeU16(p, m_Unknown0x388);
    p += 2;
    serializeU8(p, m_Unknown0x38A);
    p += 1;
    serializeU16(p, m_Unknown0x38C);
    p += 2;
    serializeU16(p, m_Unknown0x38E);
    p += 2;
    serializeU32(p, m_Unknown0x390);
    p += 4;
    serializeU16(p, m_Unknown0x394);
    p += 2;
    serializeU16(p, m_Unknown0x396);
    p += 2;
    serializeU8(p, m_Unknown0x398);
    p += 1;
    serializeU32(p, m_Unknown0x39C);
    p += 4;
    serializeU32(p, m_Unknown0x3A0);
    p += 4;
    serializeU32(p, m_Unknown0x3A4);
    p += 4;
    serializeU32(p, m_Unknown0x3A8);
    p += 4;
    serializeU64(p, m_Unknown0x3B0);
    p += 8;
    serializeU64(p, m_Unknown0x3B8);
    p += 8;
    serializeU8(p, m_Unknown0x3C0);
    p += 1;
    serializeU8(p, m_Unknown0x3C1);
    p += 1;
    serializeU8(p, m_Unknown0x3C2);
    p += 1;
    serializeU8(p, m_Unknown0x3C3);
    p += 1;
    serializeU32(p, m_Unknown0x3C4);
    p += 4;
    serializeU8(p, m_Unknown0x3C8);
    p += 1;
    serializeU8(p, m_Unknown0x3C9);
    p += 1;
    serializeU8(p, m_Unknown0x3CA);
    p += 1;
    serializeU8(p, m_Unknown0x3CB);
    p += 1;
    serializeU8(p, m_Unknown0x3CC);
    p += 1;
    serializeU8(p, m_Unknown0x3CD);
    p += 1;
    serializeU32(p, m_Unknown0x3D0);
    p += 4;
    serializeU8(p, m_Unknown0x3D4);
    p += 1;
    serializeU8(p, m_Unknown0x3D5);
    p += 1;
    serializeU8(p, m_Unknown0x3D6);
    p += 1;
    serializeU8(p, m_Unknown0x3D7);
    p += 1;
    serializeU8(p, m_Unknown0x3D8);
    p += 1;
    serializeU8(p, m_Unknown0x3D9);
    p += 1;
    serializeU16(p, m_Unknown0x3DA);
    p += 2;
    serializeU16(p, m_Unknown0x3DC);
    p += 2;
    for (int i = 0; i < 12; i++) {
        serializeU8(p, m_Unknown0x3DE[i]);
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

#pragma once

// The functions of common_Api.cpp (the file name is from the RTTI of its unnamed-namespace class
// Delegate).

#include "decomp.h"
#include "nn/Result.h"
#include "nn/pia/pia_Types.h"

namespace nn {
namespace pia {
namespace common {
// makes the heaps of pia in the memory (4-aligned) and the log; the module must be set up next
nn::Result Initialize(void* pMemory, unsigned int size); // 0x00425FDC (name is ours)
void Finalize(); // 0x00429490 | fefates:bytes [tier B]
// begin / end of the setup of the module: the instances of the common classes are created in
// between, on the common heap
nn::Result BeginSetup(); // 0x00425F88 | fefates:bytes [tier B]
nn::Result EndSetup(); // 0x00429438 (name is ours)
// the print callback of the application (not in the binary; name is ours)
void SetPrintCallback(void (*callback)(const char* str, s32 length));
bool IsInitialized(); // 0x00426DA0 | fefates:callgraph [tier C]
bool IsInSetupMode(); // 0x00426D90 | fefates:callgraph [tier C]

// the 32 bit hash of the value: the first 4 bytes of its MD5 hash, big endian
u32 hashWithMd5(unsigned int value); // 0x00426C68 | fefates:bytes [tier B]

// big endian (network byte order); serializeU8 is inline (the monitoring Serialize functions)
inline void serializeU8(unsigned char* pBuffer, u8 value)
{
    *pBuffer = value;
}
void serializeU16(unsigned char* pBuffer, unsigned short value); // 0x00426CF4 | fefates:callgraph [tier C]
void serializeU32(unsigned char* pBuffer, unsigned int value); // 0x00426D04 | fefates:bytes [tier B]
void serializeU64(unsigned char* pBuffer, unsigned long long value); // 0x00426D24 | fefates:bytes [tier B]
u16 deserializeU16(const unsigned char* pBuffer); // 0x004272BC | fefates:callgraph [tier C]
u32 deserializeU32(const unsigned char* pBuffer); // 0x004272C8 | fefates:bytes [tier B]
u64 deserializeU64(const unsigned char* pBuffer); // 0x004272E0 | fefates:bytes [tier B]

// a station index a packet may come from (a station or STATION_INDEX_UNIDENTIFIED)
bool isValidSourceStationIndex(nn::pia::StationIndex index); // 0x004283DC | fefates:bytes [tier B]
} // namespace common
} // namespace pia
} // namespace nn

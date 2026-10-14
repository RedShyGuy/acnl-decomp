#include "nn/nex/nex_qVector.h"
#include "nn/nex/nex_DataStoreKeyValue.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// 0x007E4C64 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE qVector<u8>::qVector(int num)
{
    InitializeInline(num);
}

// 0x007E4BA4 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE qVector<String>::qVector(int num)
{
    InitializeInline(num);
}

// 0x007E4894 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE qVector<DataStoreKeyValue>::qVector(int num)
{
    InitializeInline(num);
}
} // namespace nex
} // namespace nn

namespace std {
// 0x00837294 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE vector<u64, nn::nex::MemAllocator<u64> >::~vector()
{
    DestroyInline();
}

// 0x0083540C | fefates:callgraph [tier C]
template <>
DECOMP_NOINLINE void vector<u8, nn::nex::MemAllocator<u8> >::_C_insert_aux(u8* pos, const u8& value)
{
    InsertAuxInline(pos, value);
}

// 0x00835548 | mk7dlp:bytes [tier A]
template <>
DECOMP_NOINLINE void vector<u8, nn::nex::MemAllocator<u8> >::_C_insert_aux(u8* pos, u32 num, const u8& value)
{
    InsertFillInline(pos, num, value);
}

// 0x00836570 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE void vector<u32, nn::nex::MemAllocator<u32> >::_C_insert_aux(u32* pos, u32 num, const u32& value)
{
    InsertFillInline(pos, num, value);
}

// 0x008363F4 | fefates:bytes [tier B]
template <>
DECOMP_NOINLINE void vector<u32, nn::nex::MemAllocator<u32> >::_C_insert_aux(u32* pos, const u32& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std

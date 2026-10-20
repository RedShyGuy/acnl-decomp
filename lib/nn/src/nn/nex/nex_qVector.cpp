#include "nn/nex/nex_qVector.h"
#include "nn/nex/nex_DataStoreKeyValue.h"
#include "nn/nex/nex_DataStoreMetaInfo.h"
#include "nn/nex/nex_DataStoreRatingInfo.h"
#include "nn/nex/nex_DataStoreReqGetInfo.h"
#include "nn/nex/nex_DataStoreNotification.h"
#include "nn/nex/nex_DataStorePasswordInfo.h"
#include "nn/nex/nex_DataStoreNotificationV1.h"
#include "nn/nex/nex_DataStorePersistenceInfo.h"
#include "nn/nex/nex_DataStoreSpecificMetaInfo.h"
#include "nn/nex/nex_DataStoreSpecificMetaInfoV1.h"
#include "nn/nex/nex_String.h"
#include "nn/nex/nex_EncryptionAlgorithm.h"
#include "nn/nex/nex_qResult.h"

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

// 0x003D4708
template <>
DECOMP_NOINLINE qVector<u8>::qVector(const qVector<u8>& other)
{
    InitializeCopyInline(other);
}

// 0x003D4418
template <>
DECOMP_NOINLINE qVector<String>::qVector(const qVector<String>& other)
{
    InitializeCopyInline(other);
}

// 0x003D4070 (name is ours; the destructor of the temporary vectors of the protocol client)
template <>
DECOMP_NOINLINE qVector<DataStoreKeyValue>::~qVector()
{
    // nothing to do: the vector destroys its elements
}

// 0x003D40B0 (name is ours; the destructor of the temporary vectors of the protocol client)
template <>
DECOMP_NOINLINE qVector<DataStoreMetaInfo>::~qVector()
{
    // nothing to do: the vector destroys its elements
}

// 0x003D40F0 (name is ours; the destructor of the temporary vectors of the protocol client)
template <>
DECOMP_NOINLINE qVector<DataStoreRatingInfo>::~qVector()
{
    // nothing to do: the vector destroys its elements
}

// 0x003D4130 (name is ours; the destructor of the temporary vectors of the protocol client)
template <>
DECOMP_NOINLINE qVector<DataStoreReqGetInfo>::~qVector()
{
    // nothing to do: the vector destroys its elements
}

// 0x003D4170 (name is ours; the destructor of the temporary vectors of the protocol client)
template <>
DECOMP_NOINLINE qVector<DataStoreNotification>::~qVector()
{
    // nothing to do: the vector destroys its elements
}

// 0x003D41B0 (name is ours; the destructor of the temporary vectors of the protocol client)
template <>
DECOMP_NOINLINE qVector<DataStorePasswordInfo>::~qVector()
{
    // nothing to do: the vector destroys its elements
}

// 0x003D41F0 (name is ours; the destructor of the temporary vectors of the protocol client)
template <>
DECOMP_NOINLINE qVector<DataStoreNotificationV1>::~qVector()
{
    // nothing to do: the vector destroys its elements
}

// 0x003D4230 (name is ours; the destructor of the temporary vectors of the protocol client)
template <>
DECOMP_NOINLINE qVector<DataStorePersistenceInfo>::~qVector()
{
    // nothing to do: the vector destroys its elements
}

// 0x003D4270 (name is ours; the destructor of the temporary vectors of the protocol client)
template <>
DECOMP_NOINLINE qVector<DataStoreSpecificMetaInfo>::~qVector()
{
    // nothing to do: the vector destroys its elements
}

// 0x003D43D8 (name is ours; the destructor of the temporary vectors of the protocol client)
template <>
DECOMP_NOINLINE qVector<DataStoreSpecificMetaInfoV1>::~qVector()
{
    // nothing to do: the vector destroys its elements
}
// 0x003D44BC (name is ours; it calls the destructor of std::vector)
template qVector<qResult>::~qVector();

} // namespace nex
} // namespace nn

namespace std {
// 0x00836AD0 (name is ours)
template <>
DECOMP_NOINLINE void vector<u32, nn::nex::MemAllocator<u32> >::assign(const u32* pFirst, const u32* pLast)
{
    AssignRangeInline(pFirst, pLast);
}

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

// 0x008370F0
template <>
DECOMP_NOINLINE void vector<unsigned long long, nn::nex::MemAllocator<unsigned long long> >::_C_insert_aux(unsigned long long* pos, const unsigned long long& value)
{
    InsertAuxInline(pos, value);
}

// 0x0083476C (name is ours)
template <>
DECOMP_NOINLINE DECOMP_NOIPA vector<nn::nex::qResult, nn::nex::MemAllocator<nn::nex::qResult> >::~vector()
{
    DestroyInline();
}

// 0x008343B0 (name is ours)
template <>
DECOMP_NOINLINE void vector<nn::nex::qResult, nn::nex::MemAllocator<nn::nex::qResult> >::_C_destroy(nn::nex::qResult* pFirst, nn::nex::qResult* pLast)
{
    for (nn::nex::qResult* p = pFirst; p != pLast; p++) {
        p->~qResult();
    }
}

// 0x00834408 (name is ours)
template <>
DECOMP_NOINLINE void vector<nn::nex::qResult, nn::nex::MemAllocator<nn::nex::qResult> >::_C_insert_aux(nn::nex::qResult* pos, const nn::nex::qResult& value)
{
    InsertAuxInline(pos, value);
}

// 0x00835F8C (name is ours)
template void vector<u8, nn::nex::MemAllocator<u8> >::resize(u32 num, u8 value);
// 0x00835A6C (name is ours)
template void vector<u8, nn::nex::MemAllocator<u8> >::insert<const u8*>(u8* pPos, const u8* first, const u8* last);
// 0x00835CFC (name is ours)
template void vector<u8, nn::nex::MemAllocator<u8> >::insert<char*>(u8* pPos, char* first, char* last);
// 0x008367FC (name is ours)
template void vector<u32, nn::nex::MemAllocator<u32> >::_C_insert_aux2<u32*>(u32* pPos, u32* first, u32* last);
// 0x00836D94 (name is ours)
template void vector<u32, nn::nex::MemAllocator<u32> >::push_back(const u32& value);
// 0x00834978 (name is ours)
template void vector<nn::nex::EncryptionAlgorithm*, nn::nex::MemAllocator<nn::nex::EncryptionAlgorithm*> >::reserve(u32 num);
} // namespace std

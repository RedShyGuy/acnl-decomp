#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreRatingInfoWithSlot.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex27DataStoreRatingInfoWithSlotE @ 0x008CF024
// vtable 0x008FEF4C (vptr 0x008FEF54), offset_to_top 0, 2 entries
class DataStoreRatingInfoWithSlot : public ::nn::nex::_DDL_DataStoreRatingInfoWithSlot
{
public:
    // (inline)
    DataStoreRatingInfoWithSlot() {}
    virtual ~DataStoreRatingInfoWithSlot(); // 0x003BCFD4 slot 0x00
    // 0x003BCFC4 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStoreRatingInfoWithSlot, 0x28);

// (out of line)
template <>
DECOMP_NOINLINE qVector<qVector<DataStoreRatingInfoWithSlot> >::~qVector();
template <>
DECOMP_NOINLINE void _Type_qvector<DDLClassMetaType<DataStoreRatingInfoWithSlot>>::Extract(Message* pMessage, Type* pVector);
} // namespace nex
} // namespace nn

namespace std {
template <>
DECOMP_NOINLINE vector<nn::nex::DataStoreRatingInfoWithSlot, nn::nex::MemAllocator<nn::nex::DataStoreRatingInfoWithSlot> >::~vector();
} // namespace std

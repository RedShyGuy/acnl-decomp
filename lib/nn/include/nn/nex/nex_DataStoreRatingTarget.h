#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreRatingTarget.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex21DataStoreRatingTargetE @ 0x008CEA58
// vtable 0x008FDCBC (vptr 0x008FDCC4), offset_to_top 0, 2 entries
class DataStoreRatingTarget : public ::nn::nex::_DDL_DataStoreRatingTarget
{
public:
    // (inline)
    DataStoreRatingTarget() {}
    virtual ~DataStoreRatingTarget(); // 0x0039842C slot 0x00
    // 0x00398428 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStoreRatingTarget, 0x18);

// (out of line)
template <>
DECOMP_NOINLINE void _Type_qvector<DDLClassMetaType<DataStoreRatingTarget>>::Add(Message* pMessage, const Type& vector);
} // namespace nex
} // namespace nn

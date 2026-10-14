#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreKeyValue.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex17DataStoreKeyValueE @ 0x008CE698
// vtable 0x008FD3B8 (vptr 0x008FD3C0), offset_to_top 0, 2 entries
class DataStoreKeyValue : public ::nn::nex::_DDL_DataStoreKeyValue
{
public:
    // (inline)
    DataStoreKeyValue() {}
    virtual ~DataStoreKeyValue(); // 0x00386B14 slot 0x00
    // 0x00386AEC slot 0x04 (deleting dtor)
};
ASSERT_SIZE(DataStoreKeyValue, 0x18);

// (out of line)
template <>
DECOMP_NOINLINE void _Type_qvector<DDLClassMetaType<DataStoreKeyValue>>::Extract(Message* pMessage, Type* pVector);
} // namespace nex
} // namespace nn

#pragma once

#include "decomp.h"
#include "nn/nex/nex__DDL_DataStoreSearchParam.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex20DataStoreSearchParamE @ 0x008CE880
// vtable 0x008FD814 (vptr 0x008FD81C), offset_to_top 0, 2 entries
class DataStoreSearchParam : public ::nn::nex::_DDL_DataStoreSearchParam
{
public:
    // the defaults of a search
    DataStoreSearchParam(); // 0x00396564
    virtual ~DataStoreSearchParam(); // 0x003967B0 slot 0x00
    // 0x003967A0 slot 0x04 (deleting dtor)
    // (the name is ours)
    void SetDataTypes(const nn::nex::qVector<unsigned short>& dataTypes); // 0x00396384
};
ASSERT_SIZE(DataStoreSearchParam, 0x88);

// (out of line)
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStoreSearchParam>::Add(Message* pMessage, const nn::nex::DataStoreSearchParam& value);
} // namespace nex
} // namespace nn

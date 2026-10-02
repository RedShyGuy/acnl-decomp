#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex30_DDL_DataStorePreparePostParamE @ 0x008CF1F4
// vtable 0x008FF4C8 (vptr 0x008FF4D0), offset_to_top 0, 2 entries
class _DDL_DataStorePreparePostParam : public ::nn::nex::RootObject
{
public:
    _DDL_DataStorePreparePostParam(); // ctor address unknown
    virtual ~_DDL_DataStorePreparePostParam(); // 0x003C46F0 slot 0x00 | fefates:bytes
    // 0x003C46E0 slot 0x04 | slot vf_0x04 of nn::nex::_DDL_DataStorePreparePostParam (deleting dtor)
    void Add(nn::nex::Message*, const nn::nex::_DDL_DataStorePreparePostParam&); // 0x003C433C | fefates:bytes [tier B]
    void operator=(const nn::nex::_DDL_DataStorePreparePostParam&); // 0x003C47E0 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn

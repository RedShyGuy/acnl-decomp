#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16RefCountedObjectE @ 0x008CE614
// vtable 0x008FD234 (vptr 0x008FD23C), offset_to_top 0, 2 entries
class RefCountedObject : public ::nn::nex::RootObject
{
public:
    virtual ~RefCountedObject(); // 0x00383A10 slot 0x00 | fefates:callgraph
    // 0x003839E4 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    RefCountedObject(); // 0x003839C4 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn

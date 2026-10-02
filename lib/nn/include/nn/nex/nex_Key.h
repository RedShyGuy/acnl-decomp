#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex3KeyE @ 0x008CF464
// vtable 0x008FF894 (vptr 0x008FF89C), offset_to_top 0, 2 entries
class Key : public ::nn::nex::RefCountedObject
{
public:
    virtual ~Key(); // 0x003CCA8C slot 0x00 | fefates:bytes
    // 0x003CCA5C slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    void PrepareContentPtr(unsigned int); // 0x003CC824 | fefates:bytes [tier B]
    Key(const unsigned char*, unsigned); // 0x003CC840 | mk7dlp:callseq-callee [tier A]
    Key(const nn::nex::String&); // 0x003CC8A4 | fefates:bytes [tier B]
    Key(const nn::nex::Key&); // 0x003CC95C | fefates:bytes [tier B]
    Key(); // 0x003CCA2C | fefates:bytes [tier B]
    void operator=(const nn::nex::Key&); // 0x003CCB14 | fefates:bytes [tier B]
    void GetContentPtr() const; // 0x0072DF34 | fefates:bytes [tier B]
    void ExtractToString(nn::nex::String*) const; // 0x0072DF4C | fefates:bytes [tier B]
    void ToString() const; // 0x0072E040 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn

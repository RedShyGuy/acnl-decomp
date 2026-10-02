#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace transport {
// RTTI N2nn3pia9transport21TransportThreadStreamE @ 0x008D0254
// vtable 0x00901F88 (vptr 0x00901F90), offset_to_top 0, 2 entries
class TransportThreadStream : public ::nn::pia::common::RootObject
{
public:
    struct ThreadState { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual void vf_0x00(); // 0x0045BD00 slot 0x00 | virtual slot, introduced by nn::pia::transport::TransportThreadStream
    virtual void ProcessOne(); // 0x0011C12F slot 0x04 | slot vf_0x00 of ChangeRentalBase
    void ChangeState(nn::pia::transport::TransportThreadStream::ThreadState); // 0x0045B9D4 | fefates:bytes-fuzzy [tier B]
    void FinalizeCore(); // 0x0045BA6C | fefates:bytes [tier B]
    void isDropPacket(); // 0x0045BB14 | fefates:bytes [tier B]
    void InitializeCore(const char*, int, unsigned int, unsigned int, bool, unsigned int); // 0x0045BB50 | fefates:bytes [tier B]
    void Cleanup(); // 0x0045BC4C | fefates:bytes [tier B]
    void Startup(); // 0x0045BC98 | fefates:bytes [tier B]
    TransportThreadStream(); // 0x0045BD04 | fefates:bytes [tier B]
    ~TransportThreadStream(); // 0x0045BD90 | fefates:bytes [tier B]
    void GetLastResult() const; // 0x00736548 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn

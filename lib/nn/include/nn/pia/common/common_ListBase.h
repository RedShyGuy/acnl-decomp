#pragma once

#include "decomp.h"
#include "nn/pia/common/common_RootObject.h"

namespace nn {
namespace pia {
namespace common {
// RTTI N2nn3pia6common8ListBaseE @ 0x008CFF18
class ListBase : public ::nn::pia::common::RootObject
{
public:
    ListBase(); // ctor address unknown
    void PopBackNode(); // 0x0042955C | fefates:bytes [tier B]
    void PopFrontNode(); // 0x0042959C | fefates:bytes [tier B]
    void InsertAfterNode(nn::pia::common::ListNode*, nn::pia::common::ListNode*); // 0x004295F0 | fefates:bytes [tier B]
    void InsertBeforeNode(nn::pia::common::ListNode*, nn::pia::common::ListNode*); // 0x004296B4 | fefates:bytes [tier B]
    void Init(); // 0x00429740 | fefates:bytes [tier B]
    void EraseNode(nn::pia::common::ListNode*); // 0x00429758 | fefates:bytes [tier B]
    void IsIncludeNode(const nn::pia::common::ListNode*) const; // 0x007333D8 | fefates:bytes [tier B]
};
} // namespace common
} // namespace pia
} // namespace nn

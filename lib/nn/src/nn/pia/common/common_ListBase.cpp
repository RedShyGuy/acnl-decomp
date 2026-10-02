#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_ListBase.h"

namespace nn {
namespace pia {
namespace common {
// ctor address unknown
nn::pia::common::ListBase::ListBase()
{
}

// 0x0042955C | fefates:bytes [tier B]
void nn::pia::common::ListBase::PopBackNode()
{
}

// 0x0042959C | fefates:bytes [tier B]
void nn::pia::common::ListBase::PopFrontNode()
{
}

// 0x004295F0 | fefates:bytes [tier B]
void nn::pia::common::ListBase::InsertAfterNode(nn::pia::common::ListNode*, nn::pia::common::ListNode*)
{
}

// 0x004296B4 | fefates:bytes [tier B]
void nn::pia::common::ListBase::InsertBeforeNode(nn::pia::common::ListNode*, nn::pia::common::ListNode*)
{
}

// 0x00429740 | fefates:bytes [tier B]
void nn::pia::common::ListBase::Init()
{
}

// 0x00429758 | fefates:bytes [tier B]
void nn::pia::common::ListBase::EraseNode(nn::pia::common::ListNode*)
{
}

// 0x007333D8 | fefates:bytes [tier B]
void nn::pia::common::ListBase::IsIncludeNode(const nn::pia::common::ListNode*) const
{
}

} // namespace common
} // namespace pia
} // namespace nn

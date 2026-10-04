#include "nn/pia/common/common_ListBase.h"

namespace nn {
namespace pia {
namespace common {
// 0x0042955C | fefates:bytes [tier B]
ListNode* nn::pia::common::ListBase::PopBackNode()
{
    if (m_Count == 0) {
        return nullptr;
    }
    ListNode* node = m_StartEnd.m_pPrev;
    EraseNode(node);
    return node;
}

// 0x0042959C | fefates:bytes [tier B]
ListNode* nn::pia::common::ListBase::PopFrontNode()
{
    if (m_Count == 0) {
        return nullptr;
    }
    ListNode* node = m_StartEnd.m_pNext;
    EraseNode(node);
    return node;
}

// 0x004295F0 | fefates:bytes [tier B]
void nn::pia::common::ListBase::InsertAfterNode(nn::pia::common::ListNode* pos, nn::pia::common::ListNode* node)
{
    node->m_pPrev = pos;
    node->m_pNext = pos->m_pNext;
    pos->m_pNext->m_pPrev = node;
    pos->m_pNext = node;
    m_Count++;
}

// 0x004296B4 | fefates:bytes [tier B]
void nn::pia::common::ListBase::InsertBeforeNode(nn::pia::common::ListNode* pos, nn::pia::common::ListNode* node)
{
    node->m_pNext = pos;
    node->m_pPrev = pos->m_pPrev;
    pos->m_pPrev->m_pNext = node;
    pos->m_pPrev = node;
    m_Count++;
}

// 0x00429740 | fefates:bytes [tier B]
void nn::pia::common::ListBase::Init()
{
    m_StartEnd.m_pPrev = &m_StartEnd;
    m_StartEnd.m_pNext = &m_StartEnd;
    m_Count = 0;
}

// 0x00429758 | fefates:bytes [tier B]
void nn::pia::common::ListBase::EraseNode(nn::pia::common::ListNode* node)
{
    node->m_pPrev->m_pNext = node->m_pNext;
    node->m_pNext->m_pPrev = node->m_pPrev;
    node->m_pPrev = nullptr;
    node->m_pNext = nullptr;
    m_Count--;
}

// 0x007333D8 | fefates:bytes [tier B]
bool nn::pia::common::ListBase::IsIncludeNode(const nn::pia::common::ListNode* node) const
{
    if (node->m_pPrev == nullptr || node->m_pNext == nullptr) {
        return false;
    }
    for (const ListNode* it = m_StartEnd.m_pNext; it != &m_StartEnd; it = it->m_pNext) {
        if (it == node) {
            return true;
        }
    }
    return false;
}

} // namespace common
} // namespace pia
} // namespace nn

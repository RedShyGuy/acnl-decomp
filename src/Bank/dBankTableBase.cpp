#include "Bank/dBankTableBase.h"

// 0x0021384C slot 0x10 (name is ours)
void BankTableBase::ClearBit(u32 index)
{
    // empty (in the original too)
}

// 0x00213850 slot 0x14 (name is ours)
void BankTableBase::ResetBit()
{
    // empty (in the original too)
}

// 0x00213868 slot 0x00 (name is ours)
bool BankTableBase::Release(u32 index)
{
    return true;
}

// 0x00213870 (name is ours)
void BankTableBase::Clear()
{
    m_Size = 0;
    m_UsedNum = 0;
    m_Next = 0;
    ResetBit();
}

// 0x0021388C (name is ours)
bool BankTableBase::Alloc(BankHandle* handle)
{
    if (handle->m_pTable != NULL) {
        return true;
    }
    if (m_UsedNum >= m_Size) {
        return false;
    }
    if (static_cast<u32>(m_Next) >= m_Size) {
        m_Next = 0;
    }
    u32 index;
    for (index = m_Next + 1; index < m_Size; index++) {
        if (!CheckBit(index)) {
            goto found;
        }
    }
    for (index = 0; index <= static_cast<u32>(m_Next); index++) {
        if (!CheckBit(index)) {
            goto found;
        }
    }
    return false;
found:
    SetBit(index);
    m_UsedNum++;
    handle->m_pTable = this;
    handle->m_Index = index;
    return true;
}

// 0x002139BC (name is ours)
bool BankTableBase::Free(BankHandle* handle)
{
    const u32 index = handle->m_Index;
    if (handle->m_pTable != this || index >= m_Size || !CheckBit(index)) {
        return true;
    }
    if (!Release(index)) {
        return false;
    }
    ClearBit(index);
    m_Next = index;
    handle->m_pTable = NULL;
    handle->m_Index = -1;
    m_UsedNum--;
    return true;
}

// 0x00213A58 slot 0x0C (name is ours)
void BankTableBase::SetBit(u32 index)
{
    // empty (in the original too)
}

// 0x00213A5C (name is ours)
BankTableBase::BankTableBase() : m_Size(0), m_UsedNum(0), m_Next(0)
{
}

// 0x00713F78 slot 0x08 (name is ours)
bool BankTableBase::CheckBit(u32 index)
{
    return true;
}

// 0x00713F80 slot 0x04 (name is ours)
bool BankTableBase::IsValid(u32 index)
{
    return true;
}

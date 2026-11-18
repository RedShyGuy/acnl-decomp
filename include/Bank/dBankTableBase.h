#pragma once

#include "decomp.h"
#include "Bank/dBankTablePrivate.h"

class BankTableBase;

// a bank of a table that a user holds (the type and the names are ours; empty: no table, index -1)
struct BankHandle
{
    BankTableBase* m_pTable; // 0x0
    s32 m_Index;             // 0x4
};
ASSERT_SIZE(BankHandle, 0x8);

// The interface of the tables of banks (class name from the RTTI; the names of the bit slots and of m_Size after libgarden, the
// other names are ours): m_Size banks,
// of which m_UsedNum are handed out; the next search starts after m_Next.
// RTTI 13BankTableBase @ 0x008CB894
// vtable 0x008EEC74 (vptr 0x008EEC7C), offset_to_top 0, 6 entries
class BankTableBase : private ::BankTablePrivate
{
public:
    BankTableBase(); // 0x00213A5C

    // the resources of bank index are released (true: done)
    virtual bool Release(u32 index); // 0x00213868 slot 0x00
    // whether index is a bank of the table (the tables read the heaps of the bank, the result does not depend on them)
    virtual bool IsValid(u32 index); // 0x00713F80 slot 0x04
    // whether bank index is handed out
    virtual bool CheckBit(u32 index); // 0x00713F78 slot 0x08
    virtual void SetBit(u32 index); // 0x00213A58 slot 0x0C
    virtual void ClearBit(u32 index); // 0x0021384C slot 0x10
    virtual void ResetBit(); // 0x00213850 slot 0x14

    // no banks, none handed out
    void Clear(); // 0x00213870
    // a free bank for handle (true: handle holds one, also if it held one already)
    bool Alloc(BankHandle* handle); // 0x0021388C
    // the bank of handle is released and given back (false: its resources are not released yet)
    bool Free(BankHandle* handle); // 0x002139BC

    u32 m_Size;   // 0x04
    u16 m_UsedNum; // 0x08
    s16 m_Next;    // 0x0A
};
ASSERT_SIZE(BankTableBase, 0xC);

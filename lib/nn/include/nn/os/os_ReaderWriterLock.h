#pragma once

#include "decomp.h"

namespace nn {
namespace os {
// Many readers or one writer, built on one counter word and the address arbiter like SimpleLock.
// Only the parts ACNL uses are in the binary.
//
// Counter values (the member name is ours):
//      0       not initialized (constructor)
//      1 + n   free for writing, n readers inside
//     -1 - n   a writer took it and waits for the last of n readers to leave
//     -1       a writer is inside
class ReaderWriterLock
{
public:
    ReaderWriterLock(); // 0x0011F690 | fefates:bytes [tier B]
    ~ReaderWriterLock(); // 0x0034C18C (empty)
    void Initialize(); // 0x0011F678 | tier C
    void Finalize(); // 0x0034C174 (name is ours)
    void LockForWrite(); // 0x0013AF84 | fefates:bytes [tier B]
    void UnlockForWrite(); // 0x00136540 (name is ours)
    // takes it for writing if it is free, without waiting
    bool TryLockForWrite(); // 0x0013658C (name is ours)
    void LockForRead(); // 0x0034C0A8 (name is ours)
    void UnlockForRead(); // 0x0034C114 (name is ours)

private:
    volatile s32 mCounter;
};
ASSERT_SIZE(ReaderWriterLock, 4);
} // namespace os
} // namespace nn

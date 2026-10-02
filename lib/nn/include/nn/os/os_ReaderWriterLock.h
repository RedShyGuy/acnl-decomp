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
    void Initialize(); // 0x0011F678 | tier C
    void LockForWrite(); // 0x0013AF84 | fefates:bytes [tier B]

private:
    volatile s32 mCounter;
};
ASSERT_SIZE(ReaderWriterLock, 4);
} // namespace os
} // namespace nn

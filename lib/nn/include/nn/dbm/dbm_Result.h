#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace dbm {

// The results of the RomFs file table (module fs, 17). The values are from the binary, the names
// are ours.
const bit32 RESULT_FIND_FINISHED = 0x00004414;        // success, fs, 20: no more entries
const bit32 RESULT_KEY_NOT_FOUND = 0xC880446F;        // status, not found, fs, 111
const bit32 RESULT_FILE_NOT_FOUND = 0xC8804470;       // status, not found, fs, 112
const bit32 RESULT_DIRECTORY_NOT_FOUND = 0xC8804471;  // status, not found, fs, 113
const bit32 RESULT_WRONG_ENTRY_TYPE = 0xE0C04702;     // usage, not supported, fs, 770: a file where a
                                                      // directory is wanted or the other way round

const bit32 MODULE_FS = 17;

// inline (names are ours)
inline bool IsKeyNotFound(nn::Result result)
{
    return result.GetModule() == MODULE_FS && result.GetDescription() == 111;
}

// descriptions 20 to 29: the end of a search
inline bool IsFindFinished(nn::Result result)
{
    return result.GetModule() == MODULE_FS && 20 <= result.GetDescription() && result.GetDescription() <= 29;
}

} // namespace dbm
} // namespace nn

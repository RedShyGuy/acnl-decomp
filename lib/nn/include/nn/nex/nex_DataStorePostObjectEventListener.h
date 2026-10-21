#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// The source of the data of a DataStore post (the application implements it; no RTTI in nex). The
// names are ours.
class DataStorePostObjectEventListener
{
public:
    virtual ~DataStorePostObjectEventListener() {}
    // the next part of the data (its size); the last one sets *pIsLast
    virtual unsigned int GetData(unsigned char* pBuffer, unsigned int size, bool* pIsLast) = 0; // slot 0x08
};
} // namespace nex
} // namespace nn

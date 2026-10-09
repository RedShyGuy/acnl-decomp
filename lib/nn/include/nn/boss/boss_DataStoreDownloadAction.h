#pragma once

#include "decomp.h"
#include "nn/boss/boss_TaskAction.h"

namespace nn {
namespace boss {
// RTTI N2nn4boss23DataStoreDownloadActionE @ 0x008D0380
// Downloads from the DataStore server (action code 10; the data is DataStoreDownloadData).
class DataStoreDownloadAction : public ::nn::boss::TaskAction
{
public:
    DataStoreDownloadAction();
    virtual ~DataStoreDownloadAction();
    virtual nn::Result GetProperty(nn::boss::PropertyType type, void* pValue, unsigned size);

    nn::Result Initialize(unsigned int gameId, const wchar_t* pKey); // 0x0046BEE4 | fefates:bytes [tier B]
    nn::Result ClearData(); // 0x0046C124 (name is ours)
};
} // namespace boss
} // namespace nn

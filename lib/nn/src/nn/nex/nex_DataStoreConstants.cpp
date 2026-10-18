#include "nn/nex/nex_DataStoreConstants.h"

namespace nn {
namespace nex {
// (the static initializer of the DataStore file makes the constants)
// 0x0095F5F4 (name is ours)
const DateTime& g_DataStoreMaxTime = DateTime(9999, 12, 31, 23, 59, 59);
// 0x0095F5F8 (name is ours)
const DateTime& g_DataStoreMaxDate = DateTime(9999, 12, 31, 0, 0, 0);
} // namespace nex
} // namespace nn

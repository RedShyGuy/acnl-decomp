#pragma once

#include "decomp.h"
#include "nn/nex/nex_DateTime.h"

namespace nn {
namespace nex {
namespace DataStoreConstants {
// The permissions of DataStore objects (the namespace and type names are from the reference symbols;
// only the value of the DDL structures is known, its name is ours).
enum Permission : u8
{
    PERMISSION_DEFAULT = 3
};
} // namespace DataStoreConstants

// The latest time (9999-12-31 23:59:59: no limit of a search) and the latest date (9999-12-31) of
// DataStore (references to constants made by the static initializer; the names are ours).
extern const DateTime& g_DataStoreMaxTime;
extern const DateTime& g_DataStoreMaxDate;
} // namespace nex
} // namespace nn

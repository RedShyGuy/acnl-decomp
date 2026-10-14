#pragma once

#include "decomp.h"

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
} // namespace nex
} // namespace nn

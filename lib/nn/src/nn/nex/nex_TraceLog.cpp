#include "nn/nex/nex_TraceLog.h"

namespace nn {
namespace nex {
// 0x003D5FD4
// 0x003D5F58 (deleting dtor)
nn::nex::TraceLog::~TraceLog()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

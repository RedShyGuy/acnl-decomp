#include "nn/nex/nex_GameSession.h"

namespace nn {
namespace nex {
// 0x003576DC slot 0x00
// 0x003576BC (deleting dtor)
nn::nex::GameSession::~GameSession()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

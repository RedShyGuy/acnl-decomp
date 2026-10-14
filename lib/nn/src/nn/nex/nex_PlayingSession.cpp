#include "nn/nex/nex_PlayingSession.h"

namespace nn {
namespace nex {
// 0x00373B30 slot 0x00
// 0x00373AF8 (deleting dtor)
nn::nex::PlayingSession::~PlayingSession()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

#include "nn/nex/nex_NintendoNotificationEvent.h"

namespace nn {
namespace nex {
// 0x003B84EC slot 0x00
// 0x003B84B4 (deleting dtor)
nn::nex::NintendoNotificationEvent::~NintendoNotificationEvent()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
class MultiVoice
{
public:
    struct VoiceCallbackStatus { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void SetFrontBypass(bool); // 0x004CA7A8 | fefates:bytes [tier B]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

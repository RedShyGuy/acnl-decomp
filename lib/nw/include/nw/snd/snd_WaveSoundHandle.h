#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
class WaveSoundHandle
{
public:
    void DetachSound(); // 0x00132530 | fefates:bytes [tier B]

    nw::snd::internal::WaveSound* m_pSound; // 0x0 (name is ours)
};
} // namespace snd
} // namespace nw

#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
namespace internal {
class BankFileReader;
class WaveArchiveFileReader;

namespace driver {
class Channel;
struct NoteOnInfo;

class Bank
{
public:
    // the volume of a velocity (0 to 127 -> 0 to 1)
    static f32 CalcChannelVelocityVolume(unsigned char velocity); // 0x004D3C58 | fefates:bytes [tier B]
    // a channel of the note (symbols.json names other parameters, from a nintendogs call sequence)
    Channel* NoteOn(const BankFileReader& bankReader, const WaveArchiveFileReader& warcReader,
                    const NoteOnInfo& noteOnInfo) const; // 0x0074168C | nintendogs:callseq [tier A]
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw

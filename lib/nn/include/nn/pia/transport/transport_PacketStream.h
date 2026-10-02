#pragma once

#include "decomp.h"

namespace nn {
namespace pia {
namespace transport {
class PacketStream
{
public:
    class Reader;
    class Writer;
    class Accessor;
    void Initialize(unsigned int, unsigned int); // 0x0044DAAC | fefates:bytes [tier B]
    void Cleanup(); // 0x0044DD14 | fefates:bytes [tier B]
    void Startup(); // 0x0044DD38 | fefates:bytes [tier B]
    void Finalize(); // 0x0044DDAC | fefates:bytes [tier B]
    PacketStream(); // 0x0044DE40 | fefates:bytes [tier B]
};
} // namespace transport
} // namespace pia
} // namespace nn

#include "nn/hid/CTR/hid_Gyroscope.h"

namespace nn {
namespace hid {
namespace CTR {
// 0x003546C8 slot 0x00
// 0x00354698 (deleting dtor)
nn::hid::CTR::Gyroscope::~Gyroscope()
{
    // nothing to do: the members and bases are destroyed by the compiler
}

} // namespace CTR
} // namespace hid
} // namespace nn

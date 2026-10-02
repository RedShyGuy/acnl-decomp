#include "nn/hid/CTR/hid_PadReader.h"

namespace nn {
namespace hid {
namespace CTR {
// TODO: default ctor added so derived stubs compile - may not exist
nn::hid::CTR::PadReader::PadReader()
{
}

// 0x003546F0 | nintendogs:bytes [tier A]
void nn::hid::CTR::PadReader::ReadLatest(nn::hid::CTR::PadStatus*)
{
}

// 0x00354820 | nintendogs:bytes [tier B]
void nn::hid::CTR::PadReader::NormalizeStickWithScale(float*, float*, short, short)
{
}

// 0x00354838 | nintendogs:bytes [tier A]
nn::hid::CTR::PadReader::PadReader(nn::hid::CTR::Pad&)
{
}

} // namespace CTR
} // namespace hid
} // namespace nn

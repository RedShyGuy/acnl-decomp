#include "nn/nex/nex_ZLibPlugin.h"
#include "nn/nex/nex_ZLibCompression.h"

namespace nn {
namespace nex {
// 0x00356978 slot 0x00
// 0x00356960 (deleting dtor)
nn::nex::ZLibPlugin::~ZLibPlugin()
{
    // nothing to do
}

// 0x003568C0 slot 0x08 (name is ours)
const wchar_t* nn::nex::ZLibPlugin::GetGUID()
{
    return L"Quazal-ZLib-Fake-GUID";
}

// 0x003568F4 slot 0x0C (name is ours)
const wchar_t* nn::nex::ZLibPlugin::GetDescription()
{
    return L"Quazal ZLib Compression Plugin";
}

// 0x00356948 slot 0x10 (name is ours)
const wchar_t* nn::nex::ZLibPlugin::GetVendor()
{
    return L"Quazal";
}

// 0x00356878 slot 0x14 (name is ours)
u32 nn::nex::ZLibPlugin::GetMajorVersion()
{
    return 1;
}

// 0x00356880 slot 0x18 (name is ours)
u32 nn::nex::ZLibPlugin::GetMinorVersion()
{
    return 1;
}

// 0x00356940 slot 0x20 (name is ours)
bool nn::nex::ZLibPlugin::Load()
{
    return true;
}

// 0x0035693C slot 0x24 (name is ours)
void nn::nex::ZLibPlugin::Unload()
{
    // nothing to do
}

// 0x00356888 slot 0x38 (name is ours)
nn::nex::CompressionAlgorithm* nn::nex::ZLibPlugin::CreateAlgorithm()
{
    ZLibCompression* pAlgorithm = new ZLibCompression();
    pAlgorithm->m_pPlugin = this;
    return pAlgorithm;
}

// 0x003568A8 slot 0x3C (name is ours)
void nn::nex::ZLibPlugin::DeleteAlgorithm(nn::nex::CompressionAlgorithm* pAlgorithm)
{
    if (pAlgorithm != nullptr) {
        delete pAlgorithm;
    }
}

} // namespace nex
} // namespace nn

#include "nn/nex/nex__DDL_Gathering.h"
#include "nn/nex/nex_Gathering.h"

namespace nn {
namespace nex {
// ctor candidate(s) 0x003D6A90 (unverified)
nn::nex::Gathering::Gathering()
{
}

// 0x003D6B0C slot 0x00 | fefates:callgraph
nn::nex::Gathering::~Gathering()
{
}

// 0x0072EA04 slot 0x20 | virtual slot, introduced by nn::nex::Gathering
void nn::nex::Gathering::vf_0x20()
{
}

// 0x003D6A5C | fefates:bytes [tier B]
void nn::nex::Gathering::Reset()
{
}

// 0x003D6A14 (name is ours)
void nn::nex::Gathering::SetDescription(const String& description)
{
    m_Description = description;
}

} // namespace nex
} // namespace nn

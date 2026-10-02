#include "nn/boss/boss_NsData.h"

namespace nn {
namespace boss {
// 0x0046CF20 slot 0x00 | virtual slot, introduced by nn::boss::NsData
void nn::boss::NsData::vf_0x00()
{
}

// 0x0046CF1C slot 0x04 | virtual slot, introduced by nn::boss::NsData
void nn::boss::NsData::vf_0x04()
{
}

// 0x0046C854 | nintendogs:bytes [tier A]
void nn::boss::NsData::Initialize(unsigned)
{
}

// 0x0046C874 | nintendogs:bytes [tier A]
void nn::boss::NsData::GetReadFlag(bool*)
{
}

// 0x0046C920 | nintendogs:bytes [tier A]
void nn::boss::NsData::SetReadFlag(bool)
{
}

// 0x0046C9B8 | nintendogs:bytes [tier A]
void nn::boss::NsData::GetHeaderInfo(nn::boss::HeaderInfoType, void*, unsigned)
{
}

// 0x0046CA84 | nintendogs:bytes [tier B]
void nn::boss::NsData::GetLastUpdated(nn::fnd::DateTime*)
{
}

// 0x0046CC34 | nintendogs:bytes [tier A]
void nn::boss::NsData::ReadData(unsigned char*, unsigned)
{
}

// 0x0046CED8 | nintendogs:bytes [tier A]
nn::boss::NsData::NsData()
{
}

} // namespace boss
} // namespace nn

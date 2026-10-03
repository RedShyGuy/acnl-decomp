#include "nn/fnd/fnd_DateTime.h"

namespace nn {
namespace fnd {
// TODO: default ctor added so derived stubs compile - may not exist
nn::fnd::DateTime::DateTime()
{
}

// 0x0012468C | fefates:bytes [tier B]
void nn::fnd::DateTime::FromParameters(const nn::fnd::DateTimeParameters&)
{
}

// 0x001246C0 | fefates:bytes [tier B]
void nn::fnd::DateTime::GetNow()
{
}

// 0x00126C04 | nintendogs:bytes [tier A]
void nn::fnd::DateTime::GetParameters() const
{
}

// 0x0012A08C | nintendogs:bytes [tier A]
void nn::fnd::DateTime::DaysToDate(int*, int*, int*, int)
{
}

// 0x0012A21C | nintendogs:bytes [tier A]
void nn::fnd::DateTime::FromParameters(int, int, int, int, int, int, int)
{
}

// 0x0012A28C | fefates:bytes [tier B]
void nn::fnd::DateTime::operator+=(const nn::fnd::TimeSpan&)
{
}

// 0x00130ED4 | nintendogs:callgraph [tier A]
void nn::fnd::DateTime::DateToDays(int, int, int)
{
}

// 0x00130FCC | nintendogs:bytes [tier A]
nn::fnd::DateTime::DateTime(int, int, int, int, int, int, int)
{
}

// 0x00352450 | nintendogs:bytes [tier B]
void nn::fnd::DateTime::IsValidDate(int, int, int)
{
}

// 0x00352584 | nintendogs:bytes [tier B]
void nn::fnd::DateTime::IsValidParameters(int, int, int, int, int, int, int)
{
}

// 0x0072975C | fefates:bytes [tier B]
s32 nn::fnd::DateTime::GetMilliSecond() const
{
}

// 0x007297B0 | nintendogs:bytes [tier A]
s32 nn::fnd::DateTime::GetDay() const
{
}

// 0x00729808 | nintendogs:bytes [tier A]
s32 nn::fnd::DateTime::GetHour() const
{
}

// 0x00729900 | nintendogs:bytes [tier A]
s32 nn::fnd::DateTime::GetYear() const
{
}

// 0x00729958 | nintendogs:bytes [tier A]
s32 nn::fnd::DateTime::GetMonth() const
{
}

// 0x007299B0 | nintendogs:bytes [tier A]
s32 nn::fnd::DateTime::GetMinute() const
{
}

// 0x00729A1C | nintendogs:bytes [tier A]
s32 nn::fnd::DateTime::GetSecond() const
{
}

} // namespace fnd
} // namespace nn

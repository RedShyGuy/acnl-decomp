#include "nn/pia/common/common_StepSequenceJob_Step.h"

namespace nn {
namespace pia {
namespace common {
// 0x00427510
// 0x0042750C (deleting dtor)
nn::pia::common::StepSequenceJob::Step::~Step()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00731AF0 (name after StepSequenceJob::Trace)
void nn::pia::common::StepSequenceJob::Step::Trace(u64) const
{
    // empty (in the original too)
}

} // namespace common
} // namespace pia
} // namespace nn

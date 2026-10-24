#include "nw/snd/internal/snd_MoveValue.h"

namespace nw {
namespace snd {
namespace internal {
// the out-of-line copies of the members (the original has them in several places)
// 0x007E79F8 | nintendogs:bytes [tier B]
template nw::snd::internal::MoveValue<float, int>::MoveValue();
// 0x00135B60 | nintendogs:bytes [tier B]
template float nw::snd::internal::MoveValue<float, int>::GetValue() const;
// 0x007E79A8 | nintendogs:bytes [tier A]
template void nw::snd::internal::MoveValue<float, int>::SetTarget(float target, int frame);
} // namespace internal
} // namespace snd
} // namespace nw

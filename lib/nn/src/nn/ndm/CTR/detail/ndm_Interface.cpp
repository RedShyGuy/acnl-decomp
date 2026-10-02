#include "nn/ndm/CTR/detail/ndm_Interface.h"

namespace nn {
namespace ndm {
namespace CTR {
namespace detail {
// 0x00124760 | nintendogs:bytes [tier A]
void nn::ndm::CTR::detail::Interface::OverrideDefaultDaemons(unsigned)
{
}

// 0x00354AF4 | nintendogs:bytes [tier B]
void nn::ndm::CTR::detail::Interface::QueryExclusiveMode(int*)
{
}

// 0x00354B34 | nintendogs:bytes [tier A]
void nn::ndm::CTR::detail::Interface::EnterExclusiveState(int)
{
}

// 0x00354B78 | nintendogs:bytes [tier A]
void nn::ndm::CTR::detail::Interface::LeaveExclusiveState()
{
}

} // namespace detail
} // namespace CTR
} // namespace ndm
} // namespace nn

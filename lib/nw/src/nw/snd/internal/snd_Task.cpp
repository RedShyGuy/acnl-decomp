#include "nw/snd/internal/snd_Task.h"

namespace nw {
namespace snd {
namespace internal {
// 0x004C9B60 | fefates:bytes [tier B]
nw::snd::internal::Task::Task() : m_Status(STATUS_FREE), m_Id(0)
{
    m_Event.Initialize(true);
    m_Event.Signal();
}

// 0x004C9BBC slot 0x00 | fefates:callgraph
// 0x004C9BA8 (deleting dtor)
nw::snd::internal::Task::~Task()
{
    // empty (in the original too)
}

} // namespace internal
} // namespace snd
} // namespace nw

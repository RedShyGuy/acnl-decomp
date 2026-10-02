#include "nn/pia/local/local_LocalMessage.h"
#include "nn/pia/local/local_LocalStartHostMigrationMessage.h"

namespace nn {
namespace pia {
namespace local {
// ctor candidate(s) 0x0042394C (unverified)
nn::pia::local::LocalStartHostMigrationMessage::LocalStartHostMigrationMessage()
{
}

// 0x00423980 slot 0x00 | virtual slot, introduced by nn::pia::local::LocalMessage
void nn::pia::local::LocalStartHostMigrationMessage::vf_0x00()
{
}

// 0x0042397C slot 0x04 | virtual slot, introduced by nn::pia::local::LocalMessage
void nn::pia::local::LocalStartHostMigrationMessage::vf_0x04()
{
}

// 0x00423900 slot 0x08 | slot vf_0x08 of nn::pia::local::LocalMessage
void nn::pia::local::LocalStartHostMigrationMessage::UpdateMessageHeader()
{
}

// 0x004238C0 slot 0x0C | slot vf_0x0C of nn::pia::local::LocalMessage
void nn::pia::local::LocalStartHostMigrationMessage::ParseMessageHeader()
{
}

} // namespace local
} // namespace pia
} // namespace nn

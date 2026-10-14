#include "nn/nex/nex_Gathering.h"

namespace nn {
namespace nex {
// 0x003D6A90 | fefates:bytes (symbols.json picks a conflicting nintendogs name)
nn::nex::Gathering::Gathering()
{
    Reset();
}

// 0x003D6B0C slot 0x00 | fefates:callgraph
// 0x003D6AEC (deleting dtor)
nn::nex::Gathering::~Gathering()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0072EA04 slot 0x20
void nn::nex::Gathering::Trace() const
{
    // nothing to do: a gathering writes no trace
}

// 0x003D6A5C | fefates:bytes [tier B]
void nn::nex::Gathering::Reset()
{
    m_OwnerPrincipalId = 0;
    m_HostPrincipalId = 0;
    m_MinParticipants = 0;
    m_MaxParticipants = 0;
    m_ParticipationPolicy = DEFAULT_PARTICIPATION_POLICY;
    m_PolicyArgument = 0;
    m_Flags = DEFAULT_FLAGS;
    m_State = 0;
    m_Id = 0;
}

// 0x003D6A14 (name is ours)
void nn::nex::Gathering::SetDescription(const String& description)
{
    m_Description = description;
}

// 0x003D6A50 | fefates:callgraph [tier C]
void nn::nex::Gathering::SetParticipationPolicy(unsigned int policy, unsigned int argument)
{
    m_ParticipationPolicy = policy;
    m_PolicyArgument = argument;
}

} // namespace nex
} // namespace nn

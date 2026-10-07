#include "nn/pia/inet/inet_NatProperty.h"
#include "nn/nex/nex_NATProperties.h"

namespace nn {
namespace pia {
namespace inet {
// 0x003E3A4C | fefates:bytes [tier B]
void nn::pia::inet::NatProperty::SetNexNatProperties(const nn::nex::NATProperties& properties)
{
    m_PrivatePort = properties.m_PrivatePort;
    m_PublicPort = properties.m_PublicPort;
    m_NatMapping = properties.GetNATMapping();
    m_NatFiltering = properties.GetNATFiltering();
    m_PortIncrement = properties.m_PortIncrement;
    // (through a copy of the properties)
    nn::nex::NATProperties copy(properties);
    m_IsPortPreserved = copy.m_IsPortPreserved;
}

// 0x003E3B04 slot 0x08
void nn::pia::inet::NatProperty::Trace(u64) const
{
    // empty (in the original too)
}

// 0x003E3B08 | fefates:bytes [tier B]
nn::pia::inet::NatProperty::NatProperty()
    : m_PrivatePort(0), m_PublicPort(0), m_NatMapping(0), m_NatFiltering(0), m_PortIncrement(0), m_IsPortPreserved(0)
{
}

// 0x003E3B38
// 0x003E3B34 (deleting dtor)
nn::pia::inet::NatProperty::~NatProperty()
{
    // empty (in the original too)
}

} // namespace inet
} // namespace pia
} // namespace nn

#pragma once

#include "decomp.h"

class Field;

// Builds a field step by step; FieldDirector::Construct calls the steps in the order SetupBlocks, SetupHeights, Setup10,
// ClearItems, SetupItems, Setup20, Setup24, Setup1C, SetupGrass (class name from the RTTI, the rest is ours; the steps
// with numbers do different things in the builders: terrain changes, objects, ...).
// RTTI 12FieldBuilder @ 0x008CB7A0
// vtable 0x008EE6A0 (vptr 0x008EE6A8), offset_to_top 0, 11 entries
class FieldBuilder
{
public:
    // (inline in the constructors of the builders; a copy)
    DECOMP_EMIT_COPY explicit FieldBuilder(Field* field) : m_pField(field) {} // 0x00201138
    virtual ~FieldBuilder();
    // the grounds of the blocks (BgInfo::SetName)
    virtual void SetupBlocks() = 0;  // slot 0x08
    // the heights and attributes of the units (JmpBlock)
    virtual void SetupHeights() = 0; // slot 0x0C
    virtual void Setup10() = 0;      // slot 0x10
    // the items of all blocks are removed
    virtual void ClearItems() = 0;   // slot 0x14
    // the items of the blocks
    virtual void SetupItems() = 0;   // slot 0x18
    virtual void Setup1C() = 0;      // slot 0x1C
    virtual void Setup20() = 0;      // slot 0x20
    virtual void Setup24() = 0;      // slot 0x24
    // the grass of the field (Field::SetGrass)
    virtual void SetupGrass() = 0;   // slot 0x28

    Field* m_pField; // 0x4
};
ASSERT_SIZE(FieldBuilder, 0x8);

// calls the steps of a builder (no RTTI; name is ours)
class FieldDirector
{
public:
    void Construct(FieldBuilder* builder); // 0x0022CE80
};

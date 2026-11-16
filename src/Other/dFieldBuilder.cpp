#include "Other/dFieldBuilder.h"

// 0x00201154 slot 0x00
// 0x0020114C (deleting dtor)
FieldBuilder::~FieldBuilder()
{
    // empty (in the original too)
}

// 0x0022CE80 (name is ours)
void FieldDirector::Construct(FieldBuilder* builder)
{
    builder->SetupBlocks();
    builder->SetupHeights();
    builder->Setup10();
    builder->ClearItems();
    builder->SetupItems();
    builder->Setup20();
    builder->Setup24();
    builder->Setup1C();
    builder->SetupGrass();
}

#include "Other/dField.h"

// 0x002FEF9C | libgarden [tier A]
void Field::GetFg(long, long, unsigned char)
{
}

// (at least 32: the field code reads up to index 31; the next variable is 0x9B5ACC)
// 0x009B5A24
Field* Field::s_pFields[32];

#include "stage/stage_Extern.h"

// the size of a unit of the field in world units (names are ours)
// 0x008602D4
extern const f32 FIELD_UNIT_SIZE_X = 32.0f;
// 0x008602D8
extern const f32 FIELD_UNIT_SIZE_Z = 32.0f;
// 0x008602DC
extern const f32 FIELD_HALF_UNIT_SIZE_X = 16.0f;
// 0x008602E0
extern const f32 FIELD_HALF_UNIT_SIZE_Z = 16.0f;

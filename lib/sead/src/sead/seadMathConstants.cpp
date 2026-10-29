// The constants of the sead math types that the code uses. The original builds them (with the f64
// matrices and the Matrix22/33 constants) in the static initializers 0x00790D60 (matrices) and
// 0x007910F4.. (vectors); only the used ones are defined here (the name of the file is ours).
#include "sead/seadMatrix.h"
#include "sead/seadVector2.h"
#include "sead/seadVector3.h"

// (float in the names: check.py compares them with the demangled names)
namespace sead {
// 0x009830F0
template <>
const Vector2<float> Vector2<float>::ones(1.0f, 1.0f);
// 0x009830F8
template <>
const Vector2<float> Vector2<float>::zero(0.0f, 0.0f);

// 0x00AF8AE4
template <>
const Vector3<float> Vector3<float>::ones(1.0f, 1.0f, 1.0f);
// 0x00AF8AF0
template <>
const Vector3<float> Vector3<float>::zero(0.0f, 0.0f, 0.0f);

// 0x00AF8B68
template <>
const Vector3<unsigned int> Vector3<unsigned int>::zero(0, 0, 0);

// 0x00AF8F50
const Matrix34<float> Matrix34<float>::zero(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
// 0x00AF8F80
const Matrix34<float> Matrix34<float>::ident(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

// 0x00AF90B0
const Matrix44<float> Matrix44<float>::zero(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
// 0x00AF90F0
const Matrix44<float> Matrix44<float>::ident(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
} // namespace sead

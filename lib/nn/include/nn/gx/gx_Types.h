#pragma once

// The types of the OpenGL ES API (the public Khronos API that the gl* functions implement).

#include "decomp.h"

extern "C" {
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef void GLvoid;
typedef int GLintptr;
typedef int GLsizeiptr;
} // extern "C"

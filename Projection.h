#pragma once
#ifndef PROJH
#define PROJH
#include "vec3.h"
#include "mat4x4.h"

mat4x4 lookAt(const vec3 &camPos, const vec3 &target);
mat4x4 setProjMatrix(float w, float h, float n, float f, float fov);
float getScaleFromFOV(float fov, float n);

#endif // !PROJH


#pragma once

#include <cfloat>

namespace coriolis
{
typedef float real;
#define REAL_MAX FLT_MAX
#define real_sqrt sqrtf
#define real_abs fabsf
#define real_sin sinf
#define real_cos cosf
#define real_exp expf
#define real_pow powf
#define real_fmod fmodf
#define real_epislon FLT_EPSILON
#define R_PI 3.14159f
}
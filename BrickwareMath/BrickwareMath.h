#ifndef BRICKWAREMATH_H
#define BRICKWARE_MATH_H

//Only use this line to make sure VS creates an import lib
#ifdef BRICKWARE_MATH_EXPORTS
#define BRICKWARE_MATH_API __declspec(dllexport)
#else
#define BRICKWARE_MATH_API __declspec(dllimport)
#endif

#include "Matrix4.h"

#include "Quaternion.h"

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"

#endif
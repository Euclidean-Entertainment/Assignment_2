/**
 *  3 dimensional vector related functions and structures.
 */
#ifndef _VEC3_H_INCLUDED
#define _VEC3_H_INCLUDED

#include <stdbool.h>

/**
 *  Our standard vec3 structure. We use the mathematical notation i, j, k for the length, and x, y, z for the position
 */
typedef struct
{
    float x;    /**< Vector x position */
    float y;    /**< Vector y position */
    float z;    /**< Vector z position */

    float i;    /**< Length along x-axis */
    float j;    /**< Length along y-axis */
    float k;    /**< Length along z-axis */
} vec3_t;


/**
 *  Normalize a vector
 */
void vec3_normalize(vec3_t* vec);

/**
 *  Calculate the dot product of two 3D vectors
 */
float vec3_dot(vec3_t* vec1, vec3_t* vec2);

/**
 *  Vector cross product
 */
vec3_t vec3_cross(vec3_t* vec);














#endif

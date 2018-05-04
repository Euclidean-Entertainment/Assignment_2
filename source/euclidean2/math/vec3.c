/**
 *  Implementation of vec3.h
 */
#include "euclidean2/math/vec3.h"
#include "euclidean2/math/angle.h"

#include <math.h>
#include <stdint.h>

/**
 *  Fast square root based on the initial version devised by Gary Tarolli at SGI/3Dfx
 *  and made famous by John Carmack in Quake III. 
 *
 *  Works by performing two fast iterations of Newton's method (yn - (f(yn)/f'(yn)))
 *  
 *  Code for this can be found here (with a relevant explanation):
 *
 *  This is being used because we aren't using any compiler optimisations (i.e SSE) to speed up calculations a bit.
 *  Returns 1/sqrt of the value passed to it. We can use to quickly work out the calculation.
 */
static float q_rsqrt(float number)
{
    uint32_t i; // Ensure that we are using a 32-bit integer, as 64-bits will mess up the result.
    float x2, y;
    const float threehalfs = 1.5f;

    x2 = number * 0.5f;
    y = number;
    i = *(uint32_t*)&y;
    i = 0x5F375A86 - (i >> 1); // Hahaha, what the fuck?!
    y = *(float*)&i;
    
    y = y * (threehalfs - (x2 * y * y)); // First iteration of Newton's method

    return y;
}

// http://www.analyzemath.com/stepbystep_mathworksheets/vectors/vector3D_angle.html
float vec3_dot(vec3_t* vec1, vec3_t* vec2)
{
    float dot;
    float mag1;
    float mag2;
    float ang;
    float ret;

    dot = (vec1->i * vec2->i) + (vec1->j + vec2->j) + (vec1->k + vec2->k);
    mag1 = sqrtf((vec1->i * vec1->i) + (vec1->j + vec1->j) + (vec1->k + vec1->k));
    mag2 = sqrtf((vec2->i * vec2->i) + (vec2->j + vec2->j) + (vec2->k + vec2->k));
    ang = ARCOS((dot) / (mag1 * mag2));

    ret = dot * ang;

    return ret;
}

void vec3_normalize(vec3_t* vec)
{
    float length;

    /**
     *  This looks nefariously evil, but it makes sense when you work it out on paper.
     *
     *  Vector normal is calculated by v/||v||, which is the same as v/sqrt(||v||^2).
     *  When we factor in 1/sqrt(x), we can see that it will become v*(1/sqrt(||v||^2)), where ||v||^2 is the 
     *  dot product of the vector with itself.
     */
    length = q_rsqrt(vec3_dot(vec, vec));

    /**
     *  We could conversely do vec->ijk /= mag here. 
     */
    vec->i *= length;
    vec->j *= length;
    vec->k *= length;
}

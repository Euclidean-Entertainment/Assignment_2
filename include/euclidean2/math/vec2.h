/**
 *  2D Vector related structures and functions
 */
#ifndef _VEC2_INCLUDED_H
#define _VEC2_INCLUDED_H

#include <stdbool.h>

/**
 *  Our vec2 class. We're using 'traditional' mathematical notation of i and j for this.
 */
typedef struct 
{
    float i;    /**< Length along the x axis */
    float j;    /**< Length along the y axis */

    float x;    /**< Position of this vector on the x axis */ 
    float y;    /**< Position of this vector on the y axis */
} vec2_t;

/**
 *  Normalizes a vector
 *
 *  @param vec @ref vec2_t that we want to normalize
 */
void vec2_normalize(vec2_t* vec);

/**
 *  Add two vectors together
 *
 *  @param vec1 First vector
 *  @param vec2 Second vector
 */
void vec2_add(vec2_t* vec1, vec2_t* vec2);

/**
 *  Subtracts two vectors. Result in vec1
 *
 *  @param vec1 First vector
 *  @param vec2 Second vector
 */
void vec2_subtract(vec2_t* vec1, vec2_t* vec2);

/**
 *  Multiply a vector with a scalar constant
 */
void vec2_scale(vec2_t* vec, float constant);


/**
 *  Get the magnitude of a vec2_t
 *
 *  @param vec Vector whose magnitude we want to calculate
 */
float vec2_getMagnitude(vec2_t* vec);

/**
 *  Get the angle this vector makes with the x-axis
 *
 *  @param vec Vector whose angle we want to calculate
 */
float vec2_getAngle(vec2_t* vec);

/**
 *  Draw this vector to the screen
 *
 *  @param vec Vector we want to draw
 */
void vec2_draw(vec2_t* vec, float r, float g, float b, float s, bool normalize);


#endif

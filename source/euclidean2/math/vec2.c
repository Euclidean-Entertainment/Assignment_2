/**
 *  Implementation of vec2.h
 */
#include "euclidean2/math/vec2.h"

#include "gl_helper.h"
#include "platform.h"

#include <math.h>
#include <stdio.h>

void vec2_normalize(vec2_t* vec)
{
    float mag;

    if(vec == NULL)
    {
        printf("%s: vec == NULL!\n", __PRETTY_FUNCTION__);
        return;
    }

    mag = sqrtf((vec->i * vec->i) + (vec->j * vec->j));

    vec->i /= mag;
    vec->j /= mag;
}

void vec2_add(vec2_t* vec1, vec2_t* vec2)
{
    if(vec1 == NULL || vec2 == NULL)
        return;

    vec1->i += vec2->i;
    vec1->j += vec2->j;
}

void vec2_subtract(vec2_t* vec1, vec2_t* vec2)
{
    if(vec1 == NULL || vec2 == NULL)
        return;

    vec1->i -= vec2->i;
    vec1->j -= vec2->j;
}

float vec2_getMagnitude(vec2_t* vec)
{
    return sqrtf((vec->i * vec->i) + (vec->j * vec->j));
}

float vec2_getAngle(vec2_t* vec)
{
    return atanf(vec->j / vec->i);
}

void vec2_draw(vec2_t* vec, float r, float g, float b, float s, bool normalize)
{
    GLCall(glColor3f(r, g, b))

    if(normalize)
    {
        float mag = v_getMagnitude(vec);
        glBegin(GL_LINES);
            glVertex2f(vec->x-(vec->i/mag)*s, vec->y-(vec->j/mag)*s);
            glVertex2f(vec->x+(vec->i/mag)*s, vec->y+(vec->j/mag)*s);
        glEnd();
    }
    else
    {

        glBegin(GL_LINES);
            glVertex3f(vec->x, vec->y, 1.0f);
            glVertex3f(vec->x + vec->i, vec->y + vec->j, 1.0f);
        glEnd(); 
    }
}

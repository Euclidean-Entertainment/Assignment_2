/**
 *	Boat related stuff.
 */
#ifndef _BOAT_HPP_INCLUDED_
#define _BOAT_HPP_INCLUDED_

#include "euclidean2/system/material.hpp"
#include "euclidean2/math/vec3.hpp"
#include "euclidean2/object/projectile.hpp"

struct boat_t
{
	vec3_t      position;	     /**< Boat vector */
    vec3_t      normal;  
    vec3_t      velocity;         /**< Boat velocity vector */        
	material_t mat; 		     /**< Boat material */

	float mag;
	float stopping_pos;

    float heading;

    float zRot;
    float xRot;

    float dydx;
    float dydz;

    float cRotX;
    float cRotZ;

    float delayTime;

};

void boat_spawn(boat_t& b, float x, float y, float z);

void boat_draw(boat_t& b);

void boat_update(boat_t& b, float t, float dt, int numWaves);

void boat_shoot(boat_t& b);

#endif


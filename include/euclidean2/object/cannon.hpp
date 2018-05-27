/**
 *	Cannon related stuff
 */
#ifndef _CANNON_HPP_INCLUDED_
#define _CANNON_HPP_INCLUDED_

#include "euclidean2/system/texture.hpp"
#include "euclidean2/system/material.hpp"

/**
struct island_t
{
	float 		rad;
	texture_t 	tex;
	material_t	mat;
};
*/

struct cannon_t
{	
	float 		yaw;			/**< Cannon spin rotation (?) */
	float 		pitch;			/**< Actual cannon rotation (where we fire from) */
	
	int 		stacks;
	int			slices;
	float 		length;			/**< Length (height) of the cannon */	
	float 		height;			/**< Cannon Width */
	float		radius; 

	float 		islandRad;
	float		power;

	float 		cooldown = 0.0f;

	material_t	mat;
	material_t	islandMat;

	texture_t	islandTex;

	int 		hp = 200;
	int   		score = 0;
};

/**
 *	Island initilisation
 
void island_init(island_t& island, float radius, const char* tex_path);


 * Draw the island
 
void island_draw(island_t& island);
*/

/**
 *	Initialise a cannon
 */
void cannon_init(cannon_t& cannon, int stacks, int slices, float length, float width, float radius);

/**
 * Draw the cannon
 */
void cannon_draw(cannon_t& cannon);

/**
 * Pitch the cannon
 */
void cannon_pitch(cannon_t& cannon, float diff);

/**
 * Yaw the cannon
 */
void cannon_yaw(cannon_t& cannon, float diff);

#endif

/**
 *	Implementation of projectile.hpp
 */
#include "euclidean2/object/cannon.hpp"
#include "euclidean2/object/projectile.hpp"
#include "euclidean2/math/angle.hpp"
#include "platform.hpp"

#include "euclidean2/object/boat.hpp"
#include "euclidean2/system/material.hpp"

#include <vector>
#include <cstdio>



// TODO: Projectile list here

std::vector<projectile_t> active_list;

extern std::vector<boat_t> boats;
extern cannon_t cannon;

material_t pMat;
void projectile_create(float x, float y, float z, float pitch, float yaw, float power)
{
	projectile_t p;
	
	p.position.x = x;
	p.position.y = y;
	p.position.z = z;

	p.velocity.x = power * SIN_D(pitch) * SIN_D(yaw);
	p.velocity.y = power * 2 * COS_D(pitch);
	p.velocity.z = power * SIN_D(pitch) * COS_D(yaw);

	active_list.push_back(p);

	material_create(pMat, 0.1f, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 40.0f);
}

void projectile_create(float x, float y, float z, float pitch, float vx, float vz, float power)
{
	projectile_t p;
	
	p.position.x = x;
	p.position.y = y;
	p.position.z = z;

	p.velocity.x = vx;
	p.velocity.y = power * 2 * COS_D(pitch);
	p.velocity.z = vz;

	active_list.push_back(p);

	material_create(pMat, 0.1f, 0.1f, 0.1f, 1.0f, 1.0f, 1.0f, 40.0f);
}


void projectile_draw()
{
	material_bind(pMat);
	for(size_t i = 0; i < active_list.size(); i++)
	{
		projectile_t proj = active_list.at(i);

		glPushMatrix();
		glTranslatef(proj.position.x, proj.position.y, proj.position.z);
		glutSolidSphere(0.1f, 20, 20);
		glPopMatrix();
	}
}

/**
 *	This is the most inelegant, hacked together piece of code ever written.
 *	If your eyes start bleeding (@James, @Geoff) I apologise in advance...
 *	This was written at 2am Sunday night....
 *
 *	Also I'd like to thank Matt for convincing me that AABBs are too difficult to implement
 *	and that I should just use spherical collision instead!
 */
void projectile_update(float dt)
{
	// TODO: If projectile y == 0, cull this projectile

	for(size_t i = 0; i < active_list.size(); i++)
	{
		projectile_t& proj = active_list.at(i);

		if(proj.position.y <= -2.0f)
			active_list.erase(active_list.begin() + i);

		proj.position.x += proj.velocity.x * dt;
		proj.velocity.y += GRAVITY * dt;
		proj.position.y += proj.velocity.y * dt;
		proj.position.z += proj.velocity.z * dt;

		// Check for collision
		for(size_t j = 0; j < boats.size(); j++)
		{
			boat_t b = boats.at(j);

			float dist = sqrtf(pow(proj.position.x - b.position.x, 2) + pow(proj.position.y - b.position.y, 2) + pow(proj.position.z - b.position.z, 2));
			float rad = sqrtf(pow(0.1f, 2) + pow(0.2f, 2));

			if(dist < rad)
			{
				cannon.score++;
				boats.erase(boats.begin() + j);
				active_list.erase(active_list.begin() + i);
			}
		}

		{
			float dist = sqrtf(pow(proj.position.x - 0.0f, 2) + pow(proj.position.y - 0.0f, 2) + pow(proj.position.z - 0.0f, 2));
			float rad = sqrtf(pow(cannon.islandRad, 2) + (0.1f));

			if(dist < rad)
			{
				//printf("hit!\n");
				cannon.hp -= 1;
				active_list.erase(active_list.begin() + i);
			}
		}
	}
}

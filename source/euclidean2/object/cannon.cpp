/**
 *	Implementation of cannon.hpp
 */
#include "euclidean2/object/cannon.hpp"
#include "euclidean2/math/angle.hpp"
#include "platform.hpp"

#include <cstdio>

/**
void island_init(island_t& island, float radius, const char* tex_path)
{
	island.rad = radius;
	texture_load(island.tex, tex_path);

    material_create(island.mat, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f);
    material_setAlpha(island.mat, 1.0f);

}

void island_draw(island_t& island)
{
	glEnable(GL_TEXTURE_2D);
	material_bind(island.mat);
	texture_bind(island.tex);

	glPushMatrix();
	//glLoadIdentity();
	glutSolidSphere(island.rad, 100, 100);
	glPopMatrix();

	texture_unbind();
	glDisable(GL_TEXTURE_2D);
}

**/

void cannon_init(cannon_t& cannon, int stacks, int slices, float length, float height, float radius)
{
	cannon.stacks 	= stacks;
	cannon.slices 	= slices;
	cannon.length 	= length;
	cannon.height 	= height;
    cannon.radius   = radius;	
	cannon.pitch	= 45.0f;
	cannon.yaw		= 0.0f;

    cannon.islandRad = 1.6f;

    cannon.power    = 7.0f;

	material_create(cannon.mat, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 20.0f);
    material_create(cannon.islandMat, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 2.0f);

    texture_load(cannon.islandTex, "res/sand2.tga");
}

static void cannon_drawBase(cannon_t& cannon)
{
	float theta;
	float r;
	
	material_bind(cannon.mat);

    glPushMatrix();
    glTranslatef(0.0f, 0.2f, 0.0f);

    glBegin(GL_TRIANGLE_STRIP);
    for(int i = 0; i <= cannon.slices; i++)
    {
        theta = (static_cast<float>(i)/cannon.slices) * 2.0f * M_PI;
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(1.0f * sinf(theta), 0.0f, 1.0f * cosf(theta));
    }
    glEnd();

    for(int i = 0; i <= cannon.stacks; i++)
    {
        r = (static_cast<float>(i)/cannon.stacks) * 1.0f;
        glBegin(GL_TRIANGLE_STRIP);
        for(int j = 0; j <= cannon.slices; j++)
        {
            theta = (static_cast<float>(j)/cannon.slices) * 2.0f * M_PI;
            glNormal3f(sinf(theta), 0, cosf(theta));
            glVertex3f(sinf(theta) * 1.0f, r, cosf(theta) * 1.0f);
            glNormal3f(sinf(theta), 0, cosf(theta));
            glVertex3f(sinf(theta) * 1.0f, 0.0f + (1/cannon.stacks * 1.0f), cosf(theta) * 1.0f);
        }
        glEnd();
    }

    glBegin(GL_TRIANGLE_STRIP);
    for(int i = 0; i <= cannon.slices; i++)
    {
        theta = (static_cast<float>(i)/cannon.slices) * 2.0f * M_PI;
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(1.0f * sinf(theta), 1.0f, 1.0f * cosf(theta));
    }
    glEnd();

	glPopMatrix();

	glPushMatrix();
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glRotatef(cannon.yaw, 1.0f, 0.0f, 0.0f);
	glTranslatef(1.2f, -0.5, 0.0f);

    glBegin(GL_TRIANGLE_STRIP);
    for(int i = 0; i <= cannon.slices; i++)
    {
        theta = (static_cast<float>(i)/cannon.slices) * 2.0f * M_PI;
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.5f * sinf(theta), 0.0f, 0.5f * cosf(theta));
    }
    glEnd();

    for(int i = 0; i <= cannon.stacks; i++)
    {
        r = (static_cast<float>(i)/cannon.stacks) * 1.0f;
        glBegin(GL_TRIANGLE_STRIP);
        for(int j = 0; j <= cannon.slices; j++)
        {
            theta = (static_cast<float>(j)/cannon.slices) * 2.0f * M_PI;
            glNormal3f(sinf(theta), 0, cosf(theta));
            glVertex3f(sinf(theta) * 0.5f, r, cosf(theta) * 0.5f);
            glNormal3f(sinf(theta), 0, cosf(theta));
            glVertex3f(sinf(theta) * 0.5f, 0.0f + (1/cannon.stacks * 0.5f), cosf(theta) * 0.5f);
        }
        glEnd();
    }

    glBegin(GL_TRIANGLE_STRIP);
    for(int i = 0; i <= cannon.slices; i++)
    {
        theta = (static_cast<float>(i)/cannon.slices) * 2.0f * M_PI;
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.5f * sinf(theta), 1.0f, 0.5f * cosf(theta));
    }
    glEnd();

	glPopMatrix();
}

void cannon_draw(cannon_t& cannon)
{
	float theta;
	float r;

    glPushAttrib(GL_ENABLE_BIT);
    glDisable(GL_TEXTURE_2D);
	cannon_drawBase(cannon);

	material_bind(cannon.mat);
	
	glPushMatrix();
    glTranslatef(0.0f, 1.4f, 0.0f);
	glRotatef(cannon.yaw, 0.0f, 1.0f, 0.0f);
	glRotatef(cannon.pitch, 1.0f, 0.0f, 0.0f);
	glBegin(GL_TRIANGLE_STRIP);

	for(int i = 0; i <= cannon.slices; i++)
	{
		theta = (static_cast<float>(i)/cannon.slices) * 2.0f * M_PI;
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glNormal3f(0.0f, -1.0f, 0.0f);
		glVertex3f(cannon.radius * sinf(theta), 0.0f, cannon.radius * cosf(theta));
	}	
	glEnd();	

	for(int i = 0; i <= cannon.stacks; i++)
	{
		r = (static_cast<float>(i)/cannon.stacks) * cannon.height;
		glBegin(GL_TRIANGLE_STRIP);
		for(int j = 0; j <= cannon.slices; j++)
		{
			theta = (static_cast<float>(j)/cannon.slices) * 2.0f * M_PI;
			glNormal3f(sinf(theta), 0, cosf(theta));
			glVertex3f(sinf(theta) * cannon.radius, r, cosf(theta) * cannon.radius);
			glNormal3f(sinf(theta), 0, cosf(theta));
			glVertex3f(sinf(theta) * cannon.radius, 0.0f + (1/cannon.stacks * cannon.height), cosf(theta) * cannon.radius);	
		}
		glEnd();
	}
	
	glBegin(GL_TRIANGLE_STRIP);
    for(int i = 0; i <= cannon.slices; i++)
    {
        theta = (static_cast<float>(i)/cannon.slices) * 2.0f * M_PI;
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, cannon.height, 0.0f);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(cannon.radius * sinf(theta), cannon.height, cannon.radius * cosf(theta));
    }
    glEnd();

	
	glPopMatrix();

    glPopAttrib();
    // Now draw the island (this'll draw over the cylinders)
    texture_bind(cannon.islandTex);
    material_bind(cannon.islandMat);

    glPushMatrix();
    glTranslatef(0.0f, -0.6f, 0.0f);
    glutSolidSphere(cannon.islandRad, 40, 40);
    glPopMatrix();

    texture_unbind();

}

void cannon_pitch(cannon_t& cannon, float diff)
{
	if(cannon.pitch + diff > 90.0f)
		return;

	if(cannon.pitch + diff <= 0.0f)
		return;

	cannon.pitch += diff;
}

void cannon_yaw(cannon_t& cannon, float diff)
{
	if(cannon.yaw >= 360.0f || cannon.yaw <= -360.0f)
		cannon.yaw = 0.0f;

	cannon.yaw += diff;
}

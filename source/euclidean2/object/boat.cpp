/**
 *	Implementation of boat.hpp
 */
#include "euclidean2/object/boat.hpp"
#include "euclidean2/object/water.hpp"
#include "euclidean2/math/e_math.hpp"
#include "euclidean2/math/angle.hpp"
#include "euclidean2/math/vec3.hpp"

#include <vector>

extern float m_calculateSine(float a, float kx, float kz, float w, float x, float z, float t);
extern float m_calcDYDX(float a, float kx, float kz, float x, float z, float w, float t);
extern float m_calcDYDZ(float a, float kx, float kz, float x, float z, float w, float t);

extern float a[];
extern float kx[]; 
extern float kz[];
extern float w[];

extern vertex3f_t v1;

extern std::vector<boat_t> boats;

static constexpr int BOAT_FIRE_DELAY = 20.0f;

void boat_spawn(boat_t& b, float x, float y, float z)
{
	b.position.x = x;
	b.position.y = y;
	b.position.z = z;

	b.position.i = b.position.x;
	b.position.j = 0.0f;
	b.position.k = b.position.z;

    b.velocity.x = 1.0f;
    b.velocity.z = 1.0f;

    b.xRot = 0.0f;
    b.zRot = 0.0f;

    b.stopping_pos = (static_cast<float>(rand())/(static_cast<float>(RAND_MAX)/10.f))+2.0f;

	b.mag = v_Magnitude(b.position);

    b.delayTime = BOAT_FIRE_DELAY;

	material_create(b.mat, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 2.0f);
}

float radius = 0.5f;
float height = 1.5f;
int stacks = 10;
int slices = 10;
void boat_draw(boat_t& b)
{
	glPushMatrix();
	glTranslatef(b.position.x, b.position.y - 0.1f, b.position.z);
    glRotatef(ANG_2_DEGREES(b.dydz) * 0.5f, 1.0f, 0.0f, 0.0f);
    glRotatef(ANG_2_DEGREES(b.dydx) * 0.5f, 0.0f, 0.0f, 1.0f);

	material_bind(b.mat);
	glutSolidCube(0.2f);

    material_bind(b.mat);

    // Haha, this doesn't work at all!!
    glPushMatrix();
    glScalef(0.1f, 0.1f, 0.1f);
    glTranslatef(0.0f, 0.4f, 0.0f);

    float theta;
    float r;
    glBegin(GL_TRIANGLE_STRIP);
    for(int i = 0; i <= slices; i++)
    {
        theta = (static_cast<float>(i)/10) * 2.0f * M_PI;
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(radius * sinf(theta), 0.0f, radius * cosf(theta));
    }   
    glEnd();    

    for(int i = 0; i <= stacks; i++)
    {
        r = (static_cast<float>(i)/stacks) * height;
        glBegin(GL_TRIANGLE_STRIP);
        for(int j = 0; j <= slices; j++)
        {
            theta = (static_cast<float>(j)/slices) * 2.0f * M_PI;
            glNormal3f(sinf(theta), 0, cosf(theta));
            glVertex3f(sinf(theta) * radius, r, cosf(theta) * radius);
            glNormal3f(sinf(theta), 0, cosf(theta));
            glVertex3f(sinf(theta) * radius, 0.0f + (1/stacks * height), cosf(theta) * radius); 
        }
        glEnd();
    }
    
    glBegin(GL_TRIANGLE_STRIP);
    for(int i = 0; i <= slices; i++)
    {
        theta = (static_cast<float>(i)/slices) * 2.0f * M_PI;
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(0.0f, height, 0.0f);
        glNormal3f(0.0f, -1.0f, 0.0f);
        glVertex3f(radius * sinf(theta), height, radius * cosf(theta));
    }
    glEnd();


	glPopMatrix();
    glPopMatrix();
}

void boat_update(boat_t& b, float t, float dt, int numWaves)
{
    float wave = 0.0f;
    float dydx;
    float dydz;

	for(int n = 0; n < numWaves; n++)
	{
		wave += m_calculateSine(a[n], kx[n], kz[n], w[n], b.position.x, b.position.z, t);
        dydx += m_calcDYDX(a[n], kx[n], kz[n], w[n], b.position.x, 0, t);
        dydz += m_calcDYDZ(a[n], kx[n], kz[n], w[n], 0, b.position.z, t);
	}

    float theta = ARTAN(b.position.z/b.position.x);

    b.heading = theta;

    b.dydx = dydx;
    b.dydz = dydz;

    b.xRot = COS(dydx) * dt;
    b.zRot = COS(dydz) * dt;

    b.position.y = wave + 0.1f;

    float distance = v_Magnitude(b.position);

    /**
        Doesn't quite work right...
    // Remove the sign bit from the float 
    uint32_t xAbs = static_cast<uint32_t>(b.position.x) & 0x7fffffff;
    uint32_t zAbs = static_cast<uint32_t>(b.position.z) & 0x7fffffff;

    float xAbsF = static_cast<float>(xAbs);
    float zAbsF = static_cast<float>(zAbs);

    float distance = sqrtf(pow(xAbsF - 1.2f, 2) + pow(zAbsF - 1.2f, 2));
    **/

    b.delayTime -= 0.22f;

    if(distance <= 4.7f)
    {
        b.heading = ANG_2_DEGREES(theta);
        if(b.delayTime <= 0.0f)
        {
            boat_shoot(b);
            b.delayTime = BOAT_FIRE_DELAY;
        }
        return ;
    }

    // This acts _kinda_ strange, but it's actually kinda fun
    // The boats slow down as they come in, which presents a pretty fun
    // challenge. It's not like this is a really big hack or anything!
    // (yes James, I'm looking at you :] )
    b.velocity.z = -b.position.x * TAN(theta) * dt * 0.2f;
    b.velocity.x = -b.position.z / TAN(theta) * dt * 0.2f;

    b.position.x += b.velocity.x * 0.4f;
    b.position.z += b.velocity.z * 0.4f;
    b.position.i += b.velocity.x * 0.4f;
    b.position.k += b.velocity.z * 0.4f;
}

void boat_shoot(boat_t& b)
{
    float vz;
    float vx;

    /**
     *  Don't ask how this works.. Seriously...
     *  tan(90) == tan(270) which is UNDEFINED!
     *  Seriously, it's a division by zero... How this works is beyond me...
     */
    if(b.position.x < 0.0f)
    {
        vx = b.position.x / TAN(270.0f) * 0.1f;
        b.cRotX = b.position.x / TAN(270.0f);
    }
    else
    {
        vx = b.position.x / TAN(90.0f) * 1.1f;
        b.cRotX = b.position.x / TAN(90.0f);
    }

    if(b.position.z < 0.0f)
    {
        vz = b.position.z / TAN(270.0f) * 0.1f;
        b.cRotZ = b.position.z / TAN(270.0f);
    }
    else
    {
        vz = b.position.z / TAN(90.0f) * 1.1f;
        b.cRotZ = b.position.z / TAN(90.0f);
    }

    projectile_create(b.position.x, b.position.y + 0.2f, b.position.z, 35.0f, vx, vz, 4.5f);
}



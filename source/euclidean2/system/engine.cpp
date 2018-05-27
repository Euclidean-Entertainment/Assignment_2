/**
 *	Implementation of engine.h
 */
#include "euclidean2/system/engine.hpp"
#include "euclidean2/system/window.hpp"
#include "euclidean2/camera/camera.hpp"
#include "euclidean2/math/angle.hpp"
#include "euclidean2/system/text.hpp"

#include "euclidean2/system/light.hpp"
#include "euclidean2/system/material.hpp"

#include "euclidean2/object/boat.hpp"
#include "euclidean2/object/water.hpp"
#include "euclidean2/system/skybox.hpp"
#include "euclidean2/object/cannon.hpp"
#include "euclidean2/object/projectile.hpp"

#include "gl_helper.hpp"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <ctime>

#include <string>

static constexpr float MILLISECOND_TIME = 1000.0f;

const int NUM_SECONDS = 1;

engine_t    engine;	/**< Local engine structure. Not static so we can abuse 'extern'. */
camera_t    cam;
water_t     water;
cannon_t	cannon;

    // Sun (which is bound to GL_LIGHT0)
GLfloat sun_amb[]   = {0.1f, 0.0f, 0.1f, 1.0f};
GLfloat sun_dif[]   = {0.7f, 0.7f, 0.7f, 1.0f};
GLfloat sun_pos[]   = {-9.0f, 18.0f, -7.0f, 0.0f};
GLfloat sun_spec[]  = {1.0f, 1.0f, 1.0f, 1.0f};

static float t = 0.0f;

extern window_t* hwnd; // YEAH LIVE LIFE ON THE EDGE!

static bool gameOver = false;

static float zoomZ = 0.0f;
static void i_specialKeyPressed(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
			cannon_yaw(cannon, -1.0f);
			break;
		case GLUT_KEY_RIGHT:
			cannon_yaw(cannon, 1.0f);
			break;

		case GLUT_KEY_UP:
			cannon_pitch(cannon, 1.0f);
			break;
		
		case GLUT_KEY_DOWN:
			cannon_pitch(cannon, -1.0f);
			break;

		default:
			break;
	}
}

static void i_keyPressed(unsigned char c, int x, int y)
{
    static_cast<void>(x);
    static_cast<void>(y);

    switch(c)
    {
        /**
        case 'w':
            cam_translate(cam, 0.6f * engine.dt);
            break;

        case 's':
            cam_translate(cam, -0.6f * engine.dt);
            break;
        */

        case 'w':
            cannon.power += 1.0f;
            break;

        case 's':
            if(cannon.power > 1.0f)
                cannon.power -= 1.0f;
            break;
            
        case 'n':
            engine.drawNormals = !engine.drawNormals;
            break;

        case 'h':
            engine.running = !engine.running;
            break;

        case 'p':
            if(engine.polygonMode == GL_FILL)
                engine.polygonMode = GL_LINE;
            else
                engine.polygonMode = GL_FILL;

            glPolygonMode(GL_FRONT_AND_BACK, engine.polygonMode);
            break;

        case 'l':
            engine.lighting = !engine.lighting;
            break;

        case 'f':
            engine.fogEnable = !engine.fogEnable;
            break;

        case 'c':
            if(cam.mode == CAMERA_MODES::CAM_LOCKED)
                cam.mode = CAMERA_MODES::CAM_DEBUG;
            else if(cam.mode == CAMERA_MODES::CAM_DEBUG)
                cam.mode = CAMERA_MODES::CAM_LOCKED;
            break;

        case 'k':
            engine.l0Enable = !engine.l0Enable;
            break;

        case 't':
            engine.textures = !engine.textures;
            break;

        case 'm':
            engine.mouseControl = !engine.mouseControl;
            break;

        case ' ':
            vec3_t pos;
            pos.x = SIN_D(cannon.pitch) * SIN_D(cannon.yaw);
            pos.y = COS_D(cannon.pitch);
            pos.z = SIN_D(cannon.pitch) * COS_D(cannon.yaw);
            if(cannon.cooldown <= 0.0f)
            {
                projectile_create(pos.x, pos.y + 1.4f, pos.z, cannon.pitch, cannon.yaw, cannon.power);
                cannon.cooldown = 3.0f;
            }
            break;


        // If you're not using a keyboard with a numpad, I have some bad news for you....
        case '8':
            if(cam.mode == CAMERA_MODES::CAM_DEBUG)
                cam_translate(cam, 0.6f * engine.dt);
            break;

        case '2':
            if(cam.mode == CAMERA_MODES::CAM_DEBUG)
                cam_translate(cam, -0.6f * engine.dt);
            break;

        case '+':
            water_increaseTesselations(water);
            break;

        case '-':
            water_decreaseTesselations(water);
            break;

        case '\\':
            engine.holdMouse = !engine.holdMouse;
            break;

        case 127: // DEL
            engine.debug = !engine.debug;
            break;

        case 27: // ESC
        case 'q':
            #ifdef FREEGLUT
                glutLeaveMainLoop();
            #else
                exit(0);
            #endif
            break;

        default:
            break;
    }
}

float zoom = 1.0f;
static void i_mouse(int button, int state, int x, int y)
{
    if(cam.mode == CAMERA_MODES::CAM_LOCKED)
    {
        if(button == 3)
        {
            if(state == GLUT_DOWN)
            {
                glTranslatef(0.0f, 0.0f, 0.1f);
                zoomZ += 0.1f;
            }
        }
        else if(button == 4)
        {
            if(state == GLUT_DOWN)
            {
                glTranslatef(0.0f, 0.0f, -0.1f);
                zoomZ -= 0.1f;
            }
        }
    }
}

static void i_mouseMotion(int x, int y)
{
    float dx = static_cast<float>(x) - static_cast<float>(hwnd->width / 2);
    float dy = static_cast<float>(y) - static_cast<float>(hwnd->height / 2);

    cam_yaw(cam, dx * engine.dt);
    cam_pitch(cam, dy * engine.dt);
    cam_update(cam);

    if(cam.mode == CAMERA_MODES::CAM_LOCKED && engine.mouseControl)
    {
        cannon.yaw += dx * engine.dt;
        cannon.pitch  += dy * engine.dt;     
    }

}

// Draw 3D axes
#if 0
static void r_drawAxes(void)
{
	glDisable(GL_LIGHTING);
    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(1.0f, 0.0f, 0.0f); 
    glEnd();

    // Draw the y-axis in green
    glBegin(GL_LINES);
        glColor3f(0.0f, 1.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();

    // Draw the z-axis in blue
    glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(0.0f, 0.0f, 1.0f);
    glEnd();
	glEnable(GL_LIGHTING);
}
#endif

static void e_pumpGLError(void)
{
    GLenum error;
    while((error = glGetError()) != GL_NO_ERROR)
    {
        std::string err_str = "[GL ERROR]: ";

        switch(error)
        {
        case GL_INVALID_ENUM:
            err_str += "INVALID_ENUM\n";
            break;
        case GL_INVALID_VALUE:
            err_str += "INVALID_VALUE\n";
            break;
        case GL_INVALID_OPERATION:
            err_str += "INVALID_OPERATION\n";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            err_str += "GL_INVALID_FRAMEBUFFER_OPERATION\n";
            break;
        case GL_OUT_OF_MEMORY:
            err_str += "GL_OUT_OF_MEMORY\n";
            break;
        case GL_STACK_OVERFLOW:
            err_str += "GL_STACK_OVERFLOW\n";
            break;
        case GL_STACK_UNDERFLOW:
            err_str += "GL_STACK_UNDERFLOW\n";
            break;
        default:
            err_str += "UNKNOWN!!!\n";
            break;
        }

        printf("%s\n", err_str.c_str());
    }
}

static void r_reshapeFunc(int w, int h)
{
    hwnd->width = w;
    hwnd->height = h;
    cam_refresh(cam);
}

std::vector<boat_t> boats;

static void drawHealth()
{
    // DRAW HEALTH BAR
    // First we need to save some OpenGL states on the attribute stack
    glPushAttrib(GL_VIEWPORT_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT); // Save the current viewport info, enabled info and context information

    // Save the current projection matrix (so we can restore it later)
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glColor3f(0.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
        glVertex2f(0.7, 1.0);
        glVertex2f(0.7 + (cannon.hp * 0.003), 1.0f);
        glVertex2f(0.7 + (cannon.hp * 0.003), 0.95);
        glVertex2f(0.7, 0.95);
    glEnd();

    glPopMatrix();

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glPopAttrib();
}


float meme = 1.0f;
/**
 *	Engine draw function
 */
static void draw(void)
{
    char str[512];

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // View stuff
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(cam.mode == CAMERA_MODES::CAM_DEBUG)
    {
        gluLookAt(cam.x, cam.y, cam.z, cam.x + cam.lx, cam.y + cam.ly, cam.z + cam.lz, 0.0f, 1.0f, 0.0f);

        /**
         *  @James: Holy fuck I aactually got it to work...
         *  -Jesse
         */
        glPushMatrix();
        glTranslatef(cam.x, cam.y, cam.z);
        glDisable(GL_DEPTH_TEST);
        skybox_draw(); 
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
    }
    else if(cam.mode == CAMERA_MODES::CAM_LOCKED)
    {
        glTranslatef(0.5f, -1.0f, -5.0f);
        glRotatef(cam.pitch, 1.0f, 0.0f, 0.0f);
        glRotatef(cam.yaw * sin(5.0f), 0.0f, 1.0f, 0.0f);

        glPushMatrix();
        glTranslatef(1.0f, 1.0f, 1.0f);
        glDisable(GL_DEPTH_TEST);
        skybox_draw(); 
        glEnable(GL_DEPTH_TEST);
        glPopMatrix();
    }
	//gluLookAt(0.0f, -1.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f);



    if(engine.lighting)
        glEnable(GL_LIGHTING);
    else
        glDisable(GL_LIGHTING);

    if(engine.textures)
        glEnable(GL_TEXTURE_2D);
    else
        glDisable(GL_TEXTURE_2D);

    if(engine.l0Enable)
        glEnable(GL_LIGHT0);
    else
        glDisable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, sun_pos);
    

    if(engine.fogEnable)
    {
        glEnable(GL_FOG);
    }
    else
    {
        glDisable(GL_FOG);
    }

	cannon_draw(cannon);
    projectile_draw();

    for(size_t i = 0; i < boats.size(); i++)
    {
        boat_draw(boats.at(i));
    }

    water_draw(water, engine.drawNormals);

    if(gameOver)
    {
        r_drawString(hwnd->width / 2 - 10, hwnd->height / 2, "G A M E   O V E R ! ! !");
    }

    sprintf(str, "Score: %d", cannon.score);
    r_drawString(hwnd->width - 100, 26.,str);

    drawHealth();

    // Uncomment this for some Intestellar type shit
    //glScalef(zoom, zoom, 1.0f);

    if(engine.debug)
    {
        r_drawString(0, 0, "[ DEBUG ]");
		r_drawString(0, 13, "[ CAM ]");
        sprintf(str, "cam.x     = %f\ncam.y     = %f\ncam.z     = %f\ncam.lx    = %f\ncam.ly    = %f\ncam.lz    = %f\ncam.pitch = %f\ncam.yaw   = %f\n", cam.x, cam.y, cam.z, cam.lx, cam.ly, cam.lz, cam.pitch, cam.yaw);
        r_drawString(0, 26, str);
		r_drawString(0, 143, "[ LIGHTING ]");
		sprintf(str, "activeLights = %d", light_getActiveLightCount());
		r_drawString(0, 156, str);
        r_drawString(0, 182, "[ OSD ]");
        sprintf(str, "fps: %f\nft: %f\n", engine.framerate, engine.frame_interval);
        r_drawString(0, 195, str);
    }

    glDisable(GL_LIGHTING);
    e_pumpGLError();
	glutSwapBuffers();

    engine.frames++;
}

void e_shutdown()
{
    
    printf("---- e_shutdown ----\n");
    r_freeWindow();
}

static float timer_counter = 0;
int tmp = 0;
float divisor = 30.0f;

void e_update(void)
{    
    static float prev_t = -1.0f;
    float dt = 0.0f;
    std::string direction[4] = {"north", "south", "east", "west"};

    if(cannon.hp <= 0)
    {
        gameOver = true;
        engine.running = false;
    }

    t = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)) / MILLISECOND_TIME; // Number of ms since glutInit() was called

    // Skip the first frame to prevent / 0
    // Also keep the clock running even if the sim is paused to dt doesn't become huge
    if(prev_t < 0.0f)
    {
  		prev_t = t;	
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        return;
    } 

    if(engine.holdMouse)
    {
        glutSetCursor(GLUT_CURSOR_NONE);
        glutWarpPointer(hwnd->width/2, hwnd->height/2);      
    }
    else
    {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);  
    }
    
    if(engine.running)
    {
        dt = t - prev_t;
	    water_animate(water, t, 3);

        timer_counter += dt;

        projectile_update(dt);

        cannon.cooldown -= 0.1f;

        for(size_t i = 0; i < boats.size(); i++)
        {

            boat_update(boats.at(i), t, dt, 3);
        }

        if((timer_counter > 2.0f))
        {
            boat_t tmp_b;

            float pos = (static_cast<float>(rand())/(static_cast<float>(RAND_MAX)/divisor)) - 15.0f;

            if(strcmp(direction[tmp].c_str(), "north") == 0)
            {
                boat_spawn(tmp_b, pos, 0.0f, 15.0f);
            }
            else if(strcmp(direction[tmp].c_str(), "south") == 0)
            {
                boat_spawn(tmp_b, pos, 0.0f, -15.0f);
            }
            else if(strcmp(direction[tmp].c_str(), "east") == 0)
            {
                boat_spawn(tmp_b, 15.0f, 0.0f, pos);
            }
            else if(strcmp(direction[tmp].c_str(), "west") == 0)
            {
                boat_spawn(tmp_b, -15.0f, 0.0f, pos);
            }
            
            boats.push_back(tmp_b);

            tmp++;

            if(tmp == 4)
            {
                tmp = 0;
            }

            timer_counter = 0;
        }

        prev_t = t;
        dt = t - engine.last_frametime;
        if(dt > 0.2)
        {           
            engine.framerate = engine.frames/dt;
            engine.dt = dt;
            engine.last_frametime = t;
            engine.frames = 0;
        }
    }
    else
    {
        prev_t = t;
    }
    glutPostRedisplay();
}

static void e_glInit()
{
    glShadeModel(GL_SMOOTH);
    // Enable transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glShadeModel(GL_SMOOTH);    // Set the shading model
    glEnable(GL_DEPTH_TEST);    // Enable the Depth test for depth buffer
    glDepthFunc(GL_LEQUAL);

    // Material and  Lighting 
    glEnable(GL_NORMALIZE);

    engine.polygonMode = GL_FILL;

    glLightfv(GL_LIGHT0, GL_AMBIENT, sun_amb);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_dif);
    glLightfv(GL_LIGHT0, GL_POSITION, sun_pos);
    glLightfv(GL_LIGHT0, GL_SPECULAR, sun_spec);

    GLfloat fog[4] = {0.7f, 0.7f, 0.7f, 1.0f};
    // Fog
    glEnable(GL_FOG);
    glFogi(GL_FOG_MODE, GL_EXP);
    glFogfv(GL_FOG_COLOR, fog);
    glFogf(GL_FOG_DENSITY, 0.125);
    glFogf(GL_FOG_START, 9.0f);
    glFogf(GL_FOG_END, 25.0f);

    glDisable(GL_FOG);

}

void e_init(int argc, char** argv)
{
    // Walk the arguments list
    for(int i = 1; i < argc; i++) // Don't include execution string
    {
        std::string arg = std::string(argv[i]);
        printf("arg: %s\n", arg.c_str());
    }

	// Generate a random seed
	srand(time(NULL));

    printf("---- e_init ----\n");
    r_createWindow(1024, 768, "Test");
    r_setDrawFunction(&draw);

    // Initialize OpenGL paramaters
    e_glInit();
    
	// Engine stuff
    engine.debug            = false;
    engine.drawNormals      = false;
    engine.frames           = 0;
    engine.frame_interval   = 0.2f;
    engine.last_frametime   = 0.0f;
    engine.time_elapsed     = 0.0f;
	engine.framerate 		= 0.0f;
    engine.running          = true;
    engine.lighting         = true;
    engine.textures         = true;
    engine.l0Enable         = true;
    engine.fogEnable        = true;
    engine.holdMouse        = true;
    engine.mouseControl     = true;
    gameOver = false;

    cam_init(cam, hwnd->width, hwnd->height);

	GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

	glutIdleFunc(e_update);
    glutKeyboardFunc(i_keyPressed);
    glutSpecialFunc(i_specialKeyPressed);
    glutMouseFunc(i_mouse);
	glutPassiveMotionFunc(i_mouseMotion);
    glutReshapeFunc(r_reshapeFunc);

    #ifdef FREEGLUT
        glutCloseFunc(e_shutdown);
    #endif

    water_generate(water);

    cam.z = 5.0f;

	skybox_init();

	cannon_init(cannon, 10, 10, 1.0f, 1.0f, 0.1f);
    cannon.yaw = 90.0f;

    glutMainLoop();


}


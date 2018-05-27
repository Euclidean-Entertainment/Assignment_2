
/**
 *	Engine related structures and functions
 */
#ifndef _ENGINE_H_INCLUDED
#define _ENGINE_H_INCLUDED

#include "euclidean2/system/window.hpp"

#include "platform.hpp"

#include <cstdint>

/**
 *	Engine 'class'. Contains function pointers to the update, input and render methods.
 */
typedef struct
{
	void		(*update)();	/**< Engine idle update loop */
	
	bool        running;            /**< Is the game currently running??? */
    bool        debug;              /**< Is debug mode enabled? */
    bool        drawNormals;        /**< Should we draw the normals? */
    bool        lighting;           /**< Is lighting enabled or disabled? */
    bool        textures;           /**< Is texturing enabled or disabled? */
    bool        l0Enable;           /**< Should we enable GL_LIGHT0??? */
    bool        fogEnable;          /**< Did somebody say SILENT HILL?? */
    bool        holdMouse;          /**< Hold the mouse to the screen? */
    bool        mouseControl;       /**< Can we control the cannon with our mouse */

    // Timing related stuff
    float       time_elapsed;       /**< Elapsed time since our program started (in ms) */
    float       framerate;          /**< Current frame rate */
    float       dt;
    float       last_frametime;
    float       frame_interval;             
	uint64_t 	frames; 			/**< Frame count */

    GLenum      polygonMode;        /**< Current polygon mode */
} engine_t;


/**
 *  Engine initilisation routine. Gets us out of main.
 *
 *  @param argc Number of arguments passed
 *  @param argv Arguments passed from the command line
 */
void e_init(int argc, char** argv);

/**
 *  Engine shutdown function
 */
void e_shutdown(void);




#endif

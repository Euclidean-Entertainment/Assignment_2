/**
 *  Implementation of camera.hpp
 */
#include "euclidean2/camera/camera.hpp"
#include "euclidean2/math/angle.hpp"
#include "euclidean2/math/e_numeric.hpp"

#include "platform.hpp"

#include <cstdio>

void cam_init(camera_t& cam, int width, int height)
{
    // Reset the camera to the origin
    cam.x       = 0.0f;
    cam.y       = 0.0f;
    cam.z       = 0.0f;

    cam.lx      = 0.0f;
    cam.ly      = 0.0f;
    cam.lz      = 0.0f;
    cam.fov     = CAMERA_FOV_RESET;

    cam.pitch   = 0.0f;
    cam.yaw     = -90.0f;

    cam.mode    = CAMERA_MODES::CAM_LOCKED;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(cam.fov, static_cast<GLfloat>(width/height), 0.0001, 1000.0f);

    cam_update(cam);
}

void cam_update(camera_t& cam)
{
    cam.lx = COS_D(cam.yaw) * COS_D(cam.pitch);
    cam.ly = SIN_D(cam.pitch);
    cam.lz = SIN_D(cam.yaw) * COS_D(cam.pitch);
}

void cam_translate(camera_t& cam, float dv)
{  
    cam.x = cam.x + cam.lx * dv;
    cam.y = cam.y + cam.ly * dv;
    cam.z = cam.z + cam.lz * dv;
    cam_update(cam);
}

void cam_refresh(camera_t& cam)
{
    int width = glutGet(GLUT_WINDOW_WIDTH);
    int height = glutGet(GLUT_WINDOW_HEIGHT);

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(cam.fov, static_cast<GLfloat>(width/height), 0.0001, 1000.0f);
}

void cam_pitch(camera_t& cam, const float angle)
{
    cam.pitch = clamp<float>(cam.pitch + angle, -90.0f, 90.0f);
}

void cam_yaw(camera_t& cam, const float angle)
{
    if(cam.yaw >= 360.0f || cam.yaw <= -360.0f)
        cam.yaw = 0.0f;

    cam.yaw += angle;
}

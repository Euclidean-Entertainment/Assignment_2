/**
 *	Angle related functions and constants
 */
#include <math.h>

#define M_PI 3.14159265358979323846263383279f // My version of gcc doesn't have M_PI so let's just define it as a pre-processor constant

#define ANG_2_DEGREES(x) (float)(x*(180.0f/M_PI))
#define ANG_2_RAD(x) (float)(x*(M_PI/180.0f))

#define ARCOS(x) acosf(x) 

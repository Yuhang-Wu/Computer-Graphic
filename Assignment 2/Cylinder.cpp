/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cylinder class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cylinder.h"
#include <math.h>

/**
* Cylinder's intersection method.  The input is a ray (pos, dir). 
*/
float Cylinder::intersect(glm::vec3 posn, glm::vec3 dir)
{    
	float x = posn.x - center.x;
	float z = posn.z - center.z;
	
    float a = pow(dir.x,2) + pow(dir.z,2);
    float b = 2 * (dir.x * x + dir.z * z);
    float c = pow(x,2) + pow(z,2) - pow(radius,2);
    float delta = pow(b,2) - 4*a*c;
    
    if (fabs(delta) < 0.001) return -1.0;
    if(delta < 0.0) return -1.0;
    
    float front;
	float back;
    float t1 = (-b - sqrt(delta)) / (2*a);
    float t2 = (-b + sqrt(delta)) / (2*a);
	
    if (fabs(t1) < 0.001)
    {
		if(t2 > 0) front = t2;
		else t1 = -1.0;
	}
	if(fabs(t2) < 0.001 ) t2 = -1.0;
	
	
	
	if(t1 > t2) {
		front = t1;
		back = t2;
	} else {
		front = t2;
		back = t1;
	}
	
	float y1 = (posn.y + back * dir.y);
	float y2 = (posn.y + front * dir.y);
	
	if(y1 >= center.y && y1 <= center.y + height && back != -1.0) return back;
	else if(y2 >= center.y && y2 <= center.y + height && front != -1.0) return front;
	else return -1.0;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the cylinder.
*/
glm::vec3 Cylinder::normal(glm::vec3 p)
{
    glm::vec3 n = glm::vec3(p.x-center.x, 0, p.z-center.z);
    n = glm::normalize(n);
    return n;
}

/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The cone class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Cone.h"
#include <math.h>

/**
* Cone's intersection method.  The input is a ray (posn, dir). 
*/
float Cone::intersect(glm::vec3 posn, glm::vec3 dir)
{    
	float x = posn.x - center.x;
    float z = posn.z - center.z;
    float h = height - posn.y + center.y;
    float tan_theta = pow((radius / height),2);
    
    float a = pow(dir.x,2) + pow(dir.z,2) - (tan_theta*pow(dir.y,2));
    float b = 2 * ((x*dir.x) + (z*dir.z) + (tan_theta*dir.y*h));
    float c = pow(x,2) + pow(z,2) - (tan_theta*pow(h,2));
    
    float delta = pow(b,2) - 4*a*c;
	if(fabs(delta) < 0.001) return -1.0; 
    if(delta < 0.0) return -1.0;
    
    float front;
	float back;
    float t1 = (-b - sqrt(delta)) / (2*a);
    float t2 = (-b + sqrt(delta)) / (2*a);
    
    if (fabs(t1) < 0.001)
    {
		if(t2 > 0) back = t2;
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
	
    //~ float t;
    
    //~ if(fabs(t1) < 0.001 )
    //~ {
        //~ if (t2 > 0) t = t2;
        //~ else t1 = -1.0;
    //~ }
    //~ if(fabs(t2) < 0.001 ) t2 = -1.0;
    //~ t = (t1 < t2)? t1: t2;
    
    //~ float y = posn.y + t * dir.y;
    //~ if ((y >= center.y) and (y <= center.y + height)) return t;
	//~ else return -1.0;
}

/**
* Returns the unit normal vector at a given point.
* Assumption: The input point p lies on the Cone.
*/
glm::vec3 Cone::normal(glm::vec3 p)
{
    
    float r = sqrt((p.x-center.x)*(p.x-center.x) + (p.z-center.z)*(p.z-center.z));
    glm::vec3 n = glm::vec3 (p.x-center.x, r*(radius/height), p.z-center.z);
	n = glm::normalize(n);
    return n;
}

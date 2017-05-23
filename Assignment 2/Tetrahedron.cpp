/*----------------------------------------------------------
* COSC363  Ray Tracer
*
*  The Tetrahedron class
*  This is a subclass of Object, and hence implements the
*  methods intersect() and normal().
-------------------------------------------------------------*/

#include "Tetrahedron.h"
#include <math.h>


/**
* Checks if a point pt is inside the current polygon
* Implement a point inclusion test using 
* member variables a, b, c.
*/
bool Tetrahedron::isInside(glm::vec3 pt)
{

	glm::vec3 n = normal(pt);
	float A = glm::dot(cross((b-a),(pt-a)), n);
	float B = glm::dot(cross((c-b),(pt-b)), n);
	float C = glm::dot(cross((a-c),(pt-c)), n);
	
	if(A>0 && B>0 && C>0) return true;
	else return false;
	
}

/**
* Tetrahedron's intersection method.  The input is a ray (pos, dir). 
*/
float Tetrahedron::intersect(glm::vec3 posn, glm::vec3 dir)
{
	glm::vec3 n = normal(posn);
	glm::vec3 vdif = a - posn;
	float vdotn = glm::dot(dir, n);
	if(fabs(vdotn) < 1.e-4) return -1;
    float t = glm::dot(vdif, n)/vdotn;
	if(fabs(t) < 0.0001) return -1;
	glm::vec3 q = posn + dir*t;
	if(isInside(q)) return t;
    else return -1;
}

/**
* Returns the unit normal vector at a given point.
* Compute the Tetrahedron's normal vector using 
* member variables a, b, c, d.
* The parameter pt is a dummy variable and is not used.
*/
glm::vec3 Tetrahedron::normal(glm::vec3 pt)
{
	glm::vec3 n = cross((b - a), (c - a));
	n = glm::normalize(n);	
    return n;
}




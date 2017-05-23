/*========================================================================
*  COSC363: Computer Graphics (2016);  University of Canterbury.
*  Assignment 2
*
*  Student Name: Yuhang Wu
*  Student ID: 31992820
* 
*  g++ -Wall -o "%e" RayTracer.cpp Ray.cpp SceneObject.cpp Sphere.cpp Plane.cpp Cylinder.cpp Cone.cpp Tetrahedron.cpp TextureBMP.cpp -lm -lGL -lGLU -lglut
*=========================================================================
*/

#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "SceneObject.h"
#include "Ray.h"
#include <GL/glut.h>
#include "Plane.h"
#include "TextureBMP.h"

#include "Cone.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Tetrahedron.h"



using namespace std;

const float WIDTH = 20.0;  
const float HEIGHT = 20.0;
const float EDIST = 40.0;
const int NUMDIV = 500;
const int MAX_STEPS = 5;

const float XMIN = -WIDTH * 0.5;
const float XMAX =  WIDTH * 0.5;
const float YMIN = -HEIGHT * 0.5;
const float YMAX =  HEIGHT * 0.5;

vector<SceneObject*> sceneObjects;
TextureBMP earth;
TextureBMP box;

glm::vec3 trace(Ray ray, int step)
{
	
	float ambientTerm = 0.2;
	glm::vec3 colorSum;
	glm::vec3 backgroundCol(0);
	glm::vec3 light(10, 40, -3);
	glm::vec3 light2(-10, 20, 23); // light2 
    ray.closestPt(sceneObjects);

    if(ray.xindex == -1) return backgroundCol;

    glm::vec3 col = sceneObjects[ray.xindex]->getColor(); 
	glm::vec3 normalVector = sceneObjects[ray.xindex]->normal(ray.xpt);
	
	// light1 reflection
	glm::vec3 lightVector = light - ray.xpt;
	glm::vec3 normalLight = glm::normalize(lightVector);
	float lDotn = glm::dot(normalLight, normalVector);
	
	glm::vec3 reflVector = glm::reflect(-lightVector, normalVector);
	glm::vec3 normalReflVector = glm::normalize(reflVector);
	float rDotv = glm::dot(normalReflVector,normalVector);
	
	//light2 reflection
	glm::vec3 lightVector2 = light2 - ray.xpt;
	glm::vec3 normalLight2 = glm::normalize(lightVector2);
	float l2Dotn = glm::dot(normalLight2, normalVector);
	
	glm::vec3 reflVector2 = glm::reflect(-lightVector2, normalVector);
	glm::vec3 normalReflVector2 = glm::normalize(reflVector2);
	float r2Dotv = glm::dot(normalReflVector2,normalVector);
	
	//specularCol for light1 && light2
	glm::vec3 white(1);
	glm::vec3 specularCol(0);
	glm::vec3 specularCol2(0);
	if (rDotv >= 0) specularCol = pow(rDotv, 40) * white;
	if (r2Dotv >= 0) specularCol2 = pow(r2Dotv, 40) * white;
	
	
	// -------------------- pattern cylinder && cone && floor -------------------- 
	if (ray.xindex == 5) //cylinder
	{ 
		if((int)(ray.xpt.y) % 2 == 0)
			col = glm::vec3(1.0, 0.2, 0.6);
	}
	
	if (ray.xindex == 6) //cone
	{
		if((int)(ray.xpt.z + ray.xpt.x) % 2 == 0) {
			col = glm::vec3(1, 0.6, 0.8);
		}
	}
	
	if (ray.xindex == 4) //floor
	{
		if((int)(ray.xpt.x + ray.xpt.z) % 5 == 0 || (int)(ray.xpt.x - ray.xpt.z) % 5 == 0 )
			col = glm::vec3(0.6, 1.0, 0.2);
	}
	
	// -------------------- texture sphere && box_F -------------------- 
	if(ray.xindex == 2) //sphere
	{
		float s = asin(normalVector.x)/M_PI + 0.5;
		float t = asin(normalVector.y)/M_PI + 0.5;
		col = earth.getColorAt(s,t);
	}

	if (ray.xindex == 8) //box_F
	{	
		float textcoords = (ray.xpt.x - 10) / 5;
		float textcoordt = (ray.xpt.y + 12) / 5 ;
		col = box.getColorAt(textcoords, textcoordt);
	}
	
	// -------------------- light1 shadow --------------------
	Ray shadow(ray.xpt, normalLight);
	shadow.closestPt(sceneObjects);
	float lightDist = glm::length(lightVector);
	
	if ((0 >= lDotn) || (shadow.xindex > -1 && shadow.xdist < lightDist))
		 col =  ambientTerm*col;
	else col =  ambientTerm*col + lDotn*col + specularCol;
	
	// -------------------- light2 shadow --------------------
	Ray shadow2(ray.xpt, normalLight2);
	shadow2.closestPt(sceneObjects);
	float light2Dist = glm::length(lightVector2);
	
	if ((0 >= l2Dotn) || (shadow2.xindex > -1 && shadow2.xdist < light2Dist))
		 col =  ambientTerm*col;
	else col =  ambientTerm*col + l2Dotn*col + specularCol;
	colorSum = col;
	
	// -------------------- refraction --------------------
	float air = 1.0;
	float material = 1.5;
	
	if((ray.xindex == 1 || ray.xindex == 3)  && step < MAX_STEPS) //sphere
	{	
		if (ray.xindex == 1) material = 1.5;
		else if (ray.xindex == 3) material = 1.01;
		
		float eta = air / material;
		
		glm::vec3 refractDir = glm::refract(ray.dir, normalVector, eta);
		Ray refractRay(ray.xpt, refractDir);
		refractRay.closestPt(sceneObjects);
		glm::vec3 normalRefracted = sceneObjects[refractRay.xindex] -> normal(refractRay.xpt);
		glm::vec3 outRayDir = glm::refract(refractDir, -normalRefracted, 1.0f);
		
		Ray outRay(refractRay.xpt, outRayDir);
		outRay.closestPt(sceneObjects);
		if(outRay.xindex == -1) return backgroundCol;
		glm::vec3 refractCol = trace(outRay, step+1);
		
		if (ray.xindex == 1) colorSum = refractCol; // refraction
		else if (ray.xindex == 3)
			colorSum = colorSum * glm::vec3(0.7) + refractCol* glm::vec3(0.5); // transparent
	}
	
	// -------------------- reflect --------------------
	if((ray.xindex == 0 || ray.xindex == 4 )&& step < MAX_STEPS)
	{
		glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVector);
		Ray reflectedRay(ray.xpt, reflectedDir);
		glm::vec3 reflectedCol = trace(reflectedRay, step+1);
		colorSum = colorSum + (0.8f*reflectedCol);
	}
	
	return colorSum;
}

// -------------------- Diaplay Function -------------------- 
void display()
{
	float xp, yp;
	float cellX = (XMAX-XMIN)/NUMDIV;
	float cellY = (YMAX-YMIN)/NUMDIV;

	glm::vec3 eye(0., -5., 0.);

	glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

	glBegin(GL_QUADS);

	for(int i = 0; i < NUMDIV; i++)
	{
		xp = XMIN + i*cellX;
		
		for(int j = 0; j < NUMDIV; j++)
		{
			yp = YMIN + j*cellY;
			
			// -------------------- No Anti-Aliasing --------------------
		    //~ glm::vec3 dir(xp+0.5*cellX, yp+0.5*cellY, -EDIST);	//direction of the primary ray
		    //~ Ray ray = Ray(eye, dir);		//Create a ray originating from the camera in the direction 'dir'
			//~ ray.normalize();				//Normalize the direction of the ray to a unit vector
		    //~ glm::vec3 col = trace (ray, 1); //Trace the primary ray and get the colour value
		    //~ glColor3f(col.r, col.g, col.b);
		    
		    // -------------------- Anti-Aliasing -------------------- 
		    glm::vec3 dir1(xp+0.25*cellX, yp+0.25*cellY, -EDIST);
		    glm::vec3 dir2(xp+0.75*cellX, yp+0.25*cellY, -EDIST);
		    glm::vec3 dir3(xp+0.75*cellX, yp+0.75*cellY, -EDIST);
		    glm::vec3 dir4(xp+0.25*cellX, yp+0.75*cellY, -EDIST);
		     
		    Ray ray1 = Ray(eye, dir1);
		    Ray ray2 = Ray(eye, dir2);
		    Ray ray3 = Ray(eye, dir3);
		    Ray ray4 = Ray(eye, dir4);
		    
		    ray1.normalize();
		    ray2.normalize();
		    ray3.normalize();
		    ray4.normalize();
		    
		    glm::vec3 col1 = trace (ray1, 1);
		    glm::vec3 col2 = trace (ray2, 1);
		    glm::vec3 col3 = trace (ray3, 1);
		    glm::vec3 col4 = trace (ray4, 1);
		    
		    float R = (col1.r + col2.r + col3.r + col4.r) / 4;
		    float G = (col1.g + col2.g + col3.g + col4.g) / 4;
		    float B = (col1.b + col2.b + col3.b + col4.b) / 4;
			glColor3f(R, G, B);
			
			glVertex2f(xp, yp);				//Draw each cell with its color value
			glVertex2f(xp+cellX, yp);
			glVertex2f(xp+cellX, yp+cellY);
			glVertex2f(xp, yp+cellY);
        }
    }

    glEnd();
    glFlush();
}

//-------------------- Objects --------------------
void objects()
{
	//reflaction
	Sphere *sphere1 = new Sphere(glm::vec3(-5.0, 5.0, -150.0), 25.0, glm::vec3(0.4, 0.7, 1));
	sceneObjects.push_back(sphere1); 
	
	//refraction
	Sphere *sphere2 = new Sphere(glm::vec3(0.0, -10.0, -40.0), 2.0, glm::vec3(1, 0, 0));
	sceneObjects.push_back(sphere2); 
	
	//texture
	Sphere *sphere3 = new Sphere(glm::vec3(-10.0, 5.0, -90.0), 5.0, glm::vec3(0, 0, 1));
	sceneObjects.push_back(sphere3); 
	
	//transparent
	Sphere *sphere4 = new Sphere(glm::vec3(0.0, -7.0, -50.0), 2.0, glm::vec3(1, 0.5, 0));
	sceneObjects.push_back(sphere4);
	
	Plane *plane1 = new Plane (glm::vec3(-40., -20, -40),
							  glm::vec3(40., -20, -40),
							  glm::vec3(40., -20, -200),
							  glm::vec3(-40., -20, -200),
							  glm::vec3(0.8, 1.0, 0.6));
	sceneObjects.push_back(plane1);
	
	Cylinder *cylinder = new Cylinder(glm::vec3(-10, -20, -95), 3, 7, glm::vec3(1, 0.6, 0.8));
	sceneObjects.push_back(cylinder);
	
	Cone *cone = new Cone(glm::vec3(-10, -13, -95), 5, 5, glm::vec3(1.0, 0.2, 0.6));
	sceneObjects.push_back(cone);
}

void cube()
{
	float x = 10.0;
	float y = -12.0;
	float z = -80;
	float len = 5;
	
	Plane *cube_Bt = new Plane (glm::vec3(x,y,z), 		
								glm::vec3(x+len,y,z), 
								glm::vec3(x+len,y,z-len),	
								glm::vec3(x,y,z-len),
								glm::vec3(0.0, 1.0, 1.0));
	
	Plane *cube_F = new Plane (glm::vec3(x,y,z), 		
								glm::vec3(x+len,y,z), 
								glm::vec3(x+len,y+len,z),	
								glm::vec3(x,y+len,z),
								glm::vec3(0.0, 1.0, 1.0));
	
	Plane *cube_R = new Plane (glm::vec3(x+len,y,z),
								glm::vec3(x+len,y,z-len), 
								glm::vec3(x+len,y+len,z-len),
								glm::vec3(x+len,y+len,z),
								glm::vec3(0.0, 1.0, 1.0));		
	Plane *cube_B = new Plane (glm::vec3(x,y,z-len), 		
								glm::vec3(x+len,y,z-len), 
								glm::vec3(x+len,y+len,z-len),	
								glm::vec3(x,y+len,z-len),
								glm::vec3(0.0, 1.0, 1.0));
	Plane *cube_L = new Plane (glm::vec3(x,y,z-len),
								glm::vec3(x,y,z), 
								glm::vec3(x,y+len,z),
								glm::vec3(x,y+len,z-len),
								glm::vec3(0.0, 1.0, 1.0));
	Plane *cube_T = new Plane (glm::vec3(x,y+len,z),
								glm::vec3(x+len,y+len,z), 
								glm::vec3(x+len,y+len,z-len),
								glm::vec3(x,y+len,z-len),
								glm::vec3(0.0, 1.0, 1.0));
	
	sceneObjects.push_back(cube_Bt);
	sceneObjects.push_back(cube_F);
	sceneObjects.push_back(cube_R);	
	sceneObjects.push_back(cube_B);
	sceneObjects.push_back(cube_L);
	sceneObjects.push_back(cube_T);
}

void tetrahedron()
{
	glm::vec3 color = glm::vec3(1.0, 0.0, 0.0);
	
	glm::vec3 T(2.0, 0.0, -65.0);
	glm::vec3 A(-3, -8.0, -70.0);
	glm::vec3 B(7, -6.0, -70.0);
	glm::vec3 C(2.0, -5.0, -60);
	
	
	Tetrahedron *tetra1 = new Tetrahedron(T, A, B, color);
	Tetrahedron *tetra2 = new Tetrahedron(A, C, B, color);
	Tetrahedron *tetra3 = new Tetrahedron(T, A, C, color);
	Tetrahedron *tetra4 = new Tetrahedron(T, C, B, color);

	
	sceneObjects.push_back(tetra1);
	sceneObjects.push_back(tetra2);
	sceneObjects.push_back(tetra3);
	sceneObjects.push_back(tetra4);
}


//-------------------- Initialization --------------------
void initialize()
{
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);
    glClearColor(0, 0, 0, 1);
    earth = TextureBMP((char*)"Earth.bmp");
    box = TextureBMP((char*)"Box.bmp");
    
    objects();
    cube();
    tetrahedron();
    
}

//-------------------- Main function --------------------
int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(20, 20);
    glutCreateWindow("Raytracer");

    glutDisplayFunc(display);
    initialize();

    glutMainLoop();
    return 0;
}

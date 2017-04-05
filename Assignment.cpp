//  ========================================================================
//  COSC363: Computer Graphics (2016);  University of Canterbury.
//  Assignment 1
//
//  Student Name: Yuhang Wu
//  Student ID: 31992820
//  ========================================================================

#include <iostream>
#include <math.h>
#include <GL/freeglut.h>
#include "loadTGA.h"
using namespace std;

GLuint txId[10];
GLUquadric *q;

// -------------------- global variable --------------------
bool first_person_view = false;
float cam_dis = -300;
float scene_angle = 0.0;

// circling airplane
float airplaneAngle = 0.0;

// moveable robot position
float robot_x = 0.0;
float robot_z = 100.0;
float robot_y = 0.0;
float robotArm = 15;
bool forward = true;

// light color and position
float white[4] = {1.0, 1.0, 1.0, 1.0};
float black[4] = {0.0, 0.0, 0.0, 1.0};
float gray[4] = {0.5, 0.5, 0.5, 1.0};
float lgt_pos[] = {0.0f, 150.0f, 0.0f, 1.0f}; // Light0 position
int twinkle = 0;

// first person view camera position
float eye_x = 120,  eye_y = 150,  eye_z = 120;    //Initial camera position
float look_x = 120, look_y = 150, look_z = 120;    //"Look-at" point along -z direction
float theta = 0;
int step = 0;

// bullet location
float bullet_x = 0;
float bullet_y = 320;
float bullet_z = -250;

// lauch a bullet
int timeline = 0;
float launch_x = 0;
float launch_y = 300;
float launch_z = 0;

// Lighthouse vertex
const int N = 35;
float vx[N] = {0, 60, 60, 60, 40, 38.35, 36.68, 35, 33.34, 31.67, 
	30, 28, 26, 24, 22, 20, 28, 30, 40, 30, 30, 15, 15, 30,
	27.5, 25, 22.5, 17.5, 15, 12.5, 0, 2.5, 5, 2.5, 0};
float vy[N] = {0, 0, 10, 20, 20, 30, 40, 50, 60, 70, 80, 90, 100, 
	110, 120, 130, 140, 145, 150, 150, 160, 160, 170, 170,
	180, 182.5, 185, 192.5, 195, 197.5, 200, 202.5, 205, 207.5, 210};
float vz[N] = {0};

//Flag
int shift = 0;
float flagTime=20;

// -------------------- Load textures --------------------
void loadTexture(void)
{
	glGenTextures(10, txId);
	 	
	glBindTexture(GL_TEXTURE_2D, txId[0]);
	loadTGA("SkyboxBack.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[1]);
	loadTGA("SkyboxL.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[2]);
	loadTGA("SkyboxR.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	
	glBindTexture(GL_TEXTURE_2D, txId[3]);
	loadTGA("SkyboxTop.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	
	glBindTexture(GL_TEXTURE_2D, txId[4]);
	loadTGA("SkyboxBottom.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); 
	
	glBindTexture(GL_TEXTURE_2D, txId[5]);
	loadTGA("SkyboxFront.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, txId[6]);
	loadTGA("Lighthouse.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); 
	
	glBindTexture(GL_TEXTURE_2D, txId[7]);
	loadTGA("Airplane.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[8]);
	loadTGA("Bullet.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, txId[9]);
	loadTGA("Flag.tga");
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

// -------------------- Skybox --------------------
void skybox(void)
{
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
		
	glPushMatrix();
	
		glBindTexture(GL_TEXTURE_2D, txId[0]);
		glBegin(GL_QUADS); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-400, -0.1, -400);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(400, -0.1, -400); 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(400, 400, -400);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-400, 400, -400);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D, txId[1]);
		glBegin(GL_QUADS); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-400, -0.1, 400);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-400, -0.1, -400); 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-400, 400, -400);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-400, 400, 400);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D, txId[2]);
		glBegin(GL_QUADS); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(400, -0.1, -400);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(400, -0.1, 400); 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(400, 400, 400);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(400, 400, -400);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D, txId[3]);
		glBegin(GL_QUADS); 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-400, 400, -400);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(400, 400, -400); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(400, 400, 400);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-400, 400, 400);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D, txId[4]);
		glBegin(GL_QUADS); 
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-400, -0.1, 400);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(400, -0.1, 400); 
			glTexCoord2f(0.0f, 1.0f); glVertex3f(400, -0.1, -400);
			glTexCoord2f(0.0f, 0.0f); glVertex3f(-400, -0.1, -400);
		glEnd();
		
		glBindTexture(GL_TEXTURE_2D, txId[5]);
		glBegin(GL_QUADS); 
			glTexCoord2f(0.0f, 0.0f); glVertex3f(400, -0.1, 400);
			glTexCoord2f(1.0f, 0.0f); glVertex3f(-400, -0.1, 400); 
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-400, 400, 400);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(400, 400, 400);
		glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
}

// -------------------- normal --------------------
void normal(float x1, float y1, float z1, 
            float x2, float y2, float z2,
		      float x3, float y3, float z3 )
{
	  float nx, ny, nz;
	  nx = y1*(z2-z3)+ y2*(z3-z1)+ y3*(z1-z2);
	  ny = z1*(x2-x3)+ z2*(x3-x1)+ z3*(x1-x2);
	  nz = x1*(y2-y3)+ x2*(y3-y1)+ x3*(y1-y2);

      glNormal3f(nx, ny, nz);
}

// -------------------- Robot --------------------
void robot(bool colored)
{
	glNormal3f(0,1,0);
	q = gluNewQuadric();
	
	if(colored) glColor3f(0.0, 0.4, 0.4);
	else glColor4f(0.2, 0.2, 0.2, 1.0);
		
	glPushMatrix();				//head
		glTranslatef(0, 80, 0);
		glutSolidSphere(20, 36, 18);
	glPopMatrix();
	
	if(colored) glColor3f(1, 0, 0);
	else glColor4f(0.2, 0.2, 0.2, 1.0);
	
	glPushMatrix();				//dot
		glTranslatef(20, 75, 0);
		glutSolidSphere(5, 36, 18);
	glPopMatrix();
	
	if(colored) glColor3f(0.4, 0.4, 1);
	else glColor4f(0.2, 0.2, 0.2, 1.0);
	glPushMatrix();				//body
		glTranslatef(0, 35, 0);
		glRotatef(-90, 1, 0, 0);
		gluCylinder(q, 20, 15, 35, 36, 18);
		gluQuadricDrawStyle(q, GLU_FILL);
	glPopMatrix();
	
	if(colored) glColor3f(0., 0, 0.);
	else glColor4f(0.2, 0.2, 0.2, 1.0);
	glPushMatrix();				//eye1
		glTranslatef(18, 85,-8);
		glutSolidSphere(2, 36, 18);
	glPopMatrix();
	
	glPushMatrix();				//eye2
		glTranslatef(18, 85, 8);
		glutSolidSphere(2, 36, 18);
	glPopMatrix();
	
	if(colored) glColor3f(1, 0., 0.);
	else glColor4f(0.2, 0.2, 0.2, 1.0);
	glPushMatrix();				//wheel1
		glTranslatef(0, 25, 20);
		glutSolidTorus(5, 10, 36, 18);
	glPopMatrix();
	
	glPushMatrix();				//wheel2
		glTranslatef(0, 25, -20);
		glutSolidTorus(5, 10, 36, 18);
	glPopMatrix();
	
	if(colored) glColor3f(0., 0., 1);
	else glColor4f(0.2, 0.2, 0.2, 1.0);
	glPushMatrix();				//arm2
		glTranslatef(0, 55, -20);
		glRotatef(robotArm, 0, 0, 1);
		glScalef(1, 4, 1);
		glutSolidSphere(4, 36, 18);
	glPopMatrix();
	
	
	if(colored) glColor3f(0., 0., 1);
	else glColor4f(0.2, 0.2, 0.2, 1.0);	
	glPushMatrix();				//arm2
		glTranslatef(0, 55, 20);
		glRotatef(-robotArm, 0, 0, 1);
		glScalef(1, 4, 1);
		glutSolidSphere(4, 36, 18);
	glPopMatrix();
	
	glFlush();
}

// -------------------- Robot Shadow --------------------
void robotShadow(void)
{
    float shadowMat[16] = {lgt_pos[1],0,0,0,
						-lgt_pos[0],0,-lgt_pos[2],-1,
						0,0,lgt_pos[1],0,
						0,0,0,lgt_pos[1]};
	
    // Draw shadoW
    glDisable(GL_LIGHTING);
	glPushMatrix();
		glMultMatrixf(shadowMat);
		robot(0);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	
}

// -------------------- Lighthouse  --------------------
void lighthouse(void)
{
	float block = 360.;
	float wx[N], wy[N], wz[N];
	float angStep = -1.0*3.1415926/180.0;
	glColor4f (0.87, 0.87, 0.87, 1.0);
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[6]);
	
	for(int j = 0; j < block; j++){

		glBegin(GL_TRIANGLE_STRIP);
		for(int i = 0; i < N; i++)
		{
			wx[i] = vx[i] * cos(angStep) + vz[i] * sin(angStep);
			wy[i] = vy[i];
			wz[i] = -vx[i] * sin(angStep) + vz[i] * cos(angStep);
				
			if (i > 0) normal(wx[i-1], wy[i-1], wz[i-1],
							   vx[i-1], vy[i-1], vz[i-1],
							   vx[i], vy[i], vz[i]);
			glTexCoord2f(j/block, i/(float)N); 
			glVertex3f(vx[i], vy[i], vz[i]);
			
			if (i > 0) normal(wx[i-1], wy[i-1], wz[i-1],
							   vx[i], vy[i], vz[i],
							   wx[i], wy[i], wz[i]);
			glTexCoord2f((j+1)/block, i/(float)N); 				   
			glVertex3f(wx[i], wy[i], wz[i]);
		}
		glEnd();
			
		for(int i = 0; i < N; i++) {
			vx[i] = wx[i];
			vy[i] = wy[i];
			vz[i] = wz[i];
		}
	}
	glFlush();
	glDisable(GL_TEXTURE_2D);
}

// -------------------- Launch a bullet from lighthouse --------------------
void launch(int value)
{
	launch_x = 20 * timeline;
	launch_y -= 0.05 * timeline * timeline;
	timeline++;

	glutPostRedisplay();
	glutTimerFunc(50, launch, 0);
}

// -------------------- Launch a bullet by airplane --------------------
void fire(int value)
{
		bullet_y-= 2;
		bullet_x-=0.1;
	
	glutPostRedisplay();
	glutTimerFunc(50, fire, 0);
}

// -------------------- AirPlane --------------------
void airpalne(bool colored)
{	
	glNormal3f(0, 1, 0);
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glColor4f (0.87, 0.87, 0.87, 1.0);
	
	if(colored) glEnable(GL_TEXTURE_2D);
	else glColor4f(0.2, 0.2, 0.2, 1.0);
	glBindTexture(GL_TEXTURE_2D, txId[7]);	
	
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, 0);		glVertex3f(-43, 15, 10);
		glTexCoord2f(0, 1);		glVertex3f(-45, 5, 10);
		glTexCoord2f(0.14, 1); 	glVertex3f(-25, 25, 10);
		glTexCoord2f(0.14, 0); 	glVertex3f(-25, 0, 10);
		glTexCoord2f(0.26, 1); 	glVertex3f(-10, 45, 10);
		glTexCoord2f(0.33, 0); 	glVertex3f(0, 5, 10);
		glTexCoord2f(0.41, 1); 	glVertex3f(10, 45, 10);
		glTexCoord2f(0.74,0); 	glVertex3f(55, 0, 10);
		glTexCoord2f(0.81, 1);	glVertex3f(65, 15, 10);
		glTexCoord2f(1, 0); 	glVertex3f(85, 5, 0);
		glTexCoord2f(0.93, 1);	glVertex3f(75, 30, 0);
		glTexCoord2f(1, 1); 	glVertex3f(85, 30, 0);	
	glEnd();
	
	
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, 0);		glVertex3f(-43, 15, -10);
		glTexCoord2f(0, 1);		glVertex3f(-45, 5, -10);
		glTexCoord2f(0.14, 1);	glVertex3f(-25, 25, -10);
		glTexCoord2f(0.14, 0);	glVertex3f(-25, 0, -10);
		glTexCoord2f(0.26, 1);	glVertex3f(-10, 45, -10);
		glTexCoord2f(0.33, 0); 	glVertex3f(0, 5, -10);
		glTexCoord2f(0.41, 1); 	glVertex3f(10, 45, -10);
		glTexCoord2f(0.74,0); 	glVertex3f(55, 0, -10);
		glTexCoord2f(0.81, 1);	glVertex3f(65, 15, -10);
		glTexCoord2f(1, 0); 	glVertex3f(85, 5, 0);
		glTexCoord2f(0.93, 1);	glVertex3f(75, 30, 0);
		glTexCoord2f(1, 1); 	glVertex3f(85, 30, 0);
	glEnd();
	
	if(colored) glColor4f(0.3, 0.3, 0.3, 1.0);
	else glColor4f(0.2, 0.2, 0.2, 1.0);	
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-10, 15, 10);
		glVertex3f(-10, 25, 10);
		glVertex3f(0, 10, 50);
		glVertex3f(20, 25, 10);
		glVertex3f(10, 10, 50);
		glVertex3f(20, 15, 10);
		glVertex3f(0, 10, 50);
		glVertex3f(-10, 15, 10);
	glEnd();
	
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-10, 15, -10);
		glVertex3f(-10, 25, -10);
		glVertex3f(0, 10, -50);
		glVertex3f(20, 25, -10);
		glVertex3f(10, 10, -50);
		glVertex3f(20, 15, -10);
		glVertex3f(0, 10, -50);
		glVertex3f(-10, 15, -10);
	glEnd();
	
	if(colored) glColor4f(0.7, 0.7, 0.7, 1.0);
	else glColor4f(0.2, 0.2, 0.2, 1.0);	
	glBegin(GL_QUAD_STRIP);
		glVertex3f(-45, 5, 10);		glVertex3f(-45, 5, -10);
		glVertex3f(-43, 15, 10);	glVertex3f(-43, 15, -10);
		glVertex3f(-25, 25, 10);	glVertex3f(-25, 25, -10);
		glVertex3f(-10, 45, 10);	glVertex3f(-10, 45, -10);
		glVertex3f(10, 45, 10);		glVertex3f(10, 45, -10);
		glVertex3f(65, 15, 10);		glVertex3f(65, 15, -10);
		glVertex3f(75, 30, 0);		glVertex3f(75, 30, 0);
		glVertex3f(85, 30, 0);		glVertex3f(85, 30, 0);
		glVertex3f(85, 5, 0);		glVertex3f(85, 5, 0);
		glVertex3f(55, 0, 10);		glVertex3f(55, 0, -10);
		glVertex3f(0, 5, 10);		glVertex3f(0, 5, -10);
		glVertex3f(-25, 0, 10);		glVertex3f(-25, 0 ,-10);
		glVertex3f(-45, 5, 10);		glVertex3f(-45, 5, -10);
	glEnd();	

	glDisable(GL_TEXTURE_2D);
}

//  -------------------- Flag --------------------
void flag()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[9]);
	
	glBegin(GL_QUADS);
	for(float i = 0; i < 20; i++)
	{
		for(float j = 0; j < 50; j++)
		{
			glTexCoord2f((i+0)/20, (j+0)/50);
			glVertex3f(sin(0.25*(i+0+shift))+cos(0.25*(j+0+shift)), 1.5*(i+0), 1.5*(j+0));
			glTexCoord2f((i+1)/20, (j+0)/50);
			glVertex3f(sin(0.25*(i+1+shift))+cos(0.25*(j+0+shift)), 1.5*(i+1), 1.5*(j+0));
			glTexCoord2f((i+1)/20, (j+1)/50);
			glVertex3f(sin(0.25*(i+1+shift))+cos(0.25*(j+1+shift)), 1.5*(i+1), 1.5*(j+1));
			glTexCoord2f((i+0)/20, (j+1)/50);
			glVertex3f(sin(0.25*(i+0+shift))+cos(0.25*(j+1+shift)), 1.5*(i+0), 1.5*(j+1));
		}
	}
	shift += 5 * flagTime;
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

// -------------------- Airplane Shadow --------------------
void airplaneShadow(void)
{
    float shadowMat[16] = {lgt_pos[1],0,0,0,
						-lgt_pos[0],0,-lgt_pos[2],-1,
						0,0,lgt_pos[1],0,
						0,0,0,lgt_pos[1]};
    // Draw shadow
    glDisable(GL_LIGHTING);
	glPushMatrix();
		glMultMatrixf(shadowMat);
		airpalne(0);
	glPopMatrix();
	glEnable(GL_LIGHTING);
	
}

// -------------------- Airplane Pathway --------------------
void airplaneTimer(int value)
{
	airplaneAngle++;
	if(airplaneAngle > 360) airplaneAngle = 0.0;
	glutTimerFunc(50, airplaneTimer, value);
	glutPostRedisplay();
}

// -------------------- keyboard call back function --------------------
void keyboard(unsigned char key, int x, int y)
{

	switch(key)
	{
		case 'a': robot_z++; robot_y = -90; break;
		case 'A': robot_z++; robot_y = -90; break;
		
		case 'd': robot_z--; robot_y = 90; break;
		case 'D': robot_z--; robot_y = 90; break;
		
		case 'w': robot_x++; robot_y = 0.0; break;
		case 'W': robot_x++; robot_y = 0.0; break;
		
		case 's': robot_x--; robot_y = 180; break;						
		case 'S': robot_x--; robot_y = 180; break;
					
		case 'f': glutTimerFunc(50, fire, 0); break;
		case 'F': glutTimerFunc(50, fire, 0); break;
						
		case ' ': first_person_view = !first_person_view; break;
	}
}

// ---------- move the camera with the direction keys ----------
void camerPosition(int key, int x, int y)
{
	step = 0;
	if (!first_person_view){
		if(key == GLUT_KEY_LEFT) theta += 0.1;
		else if(key == GLUT_KEY_RIGHT) theta -= 0.1;
		else if(key == GLUT_KEY_DOWN) step = -1;
		else if(key == GLUT_KEY_UP) step = 1;
	}
	
	// Launch a bullet from lighthouse.
	if (key == GLUT_KEY_INSERT)
		glutTimerFunc(50, launch, 0);
	
	glutPostRedisplay();
}

// -------------------- Robot Timer --------------------
void robotTimer(int value)
{
	if (forward) {
		if(robotArm > -15 and robotArm <= 15) robotArm--;
		else forward = !forward;
	} else {
		if(robotArm < 15 and robotArm >= -15) robotArm++;
		else forward = !forward;
	}
	
	glutPostRedisplay();
	glutTimerFunc(20, robotTimer, 0);
}

// -------------------- robot spot light --------------------
void lighting(int value)
{   
	if (twinkle % 5 == 0){
		twinkle++;
		glEnable(GL_LIGHT1);
	} else {
		twinkle = 0;
		glDisable(GL_LIGHT1);
	}
	
	glutPostRedisplay();
	glutTimerFunc(50, lighting, 0);
	
}

// -------------------- Initialization --------------------
void initialize(void) 
{
    float red[4] = {1.0, 0.0, 0.0, 1.0};    
	q = gluNewQuadric();
	loadTexture();
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glLightfv(GL_LIGHT0, GL_SPECULAR, white);
    
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, red);
    glLightfv(GL_LIGHT1, GL_SPECULAR, red);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,0.01);
    glShadeModel(GL_SMOOTH);
	
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glMaterialfv(GL_FRONT, GL_SPECULAR, white);
    glMaterialf(GL_FRONT, GL_SHININESS, 50);
    
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
    
    glPolygonMode(GL_FRONT_AND_BACK, GLU_FILL);
	glClearColor (0.0, 0.0, 0.0, 0.0);
	
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60., 1.0, 10.0, 1000.0);
}


// -------------------- Diaplay Function -------------------- 
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glLightfv(GL_LIGHT0, GL_POSITION, lgt_pos);   //light position

	float lgt1_pos[] = {20.0, 75.0, 0.0, 1.0f}; //light1 position
	float spotdir[] = {1.0, 0.0, 1.0};
	
	if (!first_person_view){
		// general view
		float dir_x = -sin(theta), dir_y = 0,  dir_z = -cos(theta);
		float s = 1;
		eye_x += step * s * dir_x;
		eye_z += step * s * dir_z;
		step = 0;
		
		if (eye_x > 350) eye_x = 350;
		else if (eye_x < -350 ) eye_x = -350;
		
		if (eye_z > 380) eye_z = 380;
		else if (eye_z < -350) eye_z = -350;
			  
		float d = 2.0;
		look_x = eye_x + d * dir_x;
		look_y = eye_y + d * dir_y;
		look_z = eye_z + d * dir_z;
			
		gluLookAt(eye_x, eye_y, eye_z, look_x, look_y, look_z, 0, 1, 0);	
		
	} else {
		
		// FIRST PERSON VIEW
		glTranslatef(-robot_x, -85, -robot_z);
		glRotatef(-robot_y, 0, 1, 0);
		
		if(robot_x > 350) robot_x = 350;
		else if(robot_x < -350) robot_x = -350;
		
		if(robot_z > 350) robot_z = 350;
		else if(robot_z < -350) robot_z = -350;
	}

	glColor4f(1.0, 1.0, 1.0, 1.0);        //Base colour
	skybox();
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, txId[8]);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	glPushMatrix();			//draw airplane's bullet
		if(bullet_y > 0) {
			glRotatef(airplaneAngle, 0, 1, 0); 
			glTranslatef(bullet_x, bullet_y, bullet_z);
			glutSolidSphere(10, 36, 18);
		}
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	
	glPushMatrix();			//draw lighthouse
		glTranslatef(-300.0, 0.0, -300.0); 
		glScalef(1.5,1.7,1.5);
		lighthouse();
	glPopMatrix();
	
	glPushMatrix();			//draw lighthouse's bullet
		if(launch_y > 0) {
			glTranslatef(launch_x, launch_y, launch_z);
			glTranslatef(-300, 0.0, -300);
			glutSolidSphere(5, 36, 18);	
		}
	glPopMatrix();	
	
	if(!first_person_view){
		glEnable(GL_LIGHTING);
		glPushMatrix();			//draw robot
			glTranslatef(robot_x, 0, robot_z);
			glRotatef(robot_y, 0, 1, 0);
			glLightfv(GL_LIGHT1, GL_POSITION, lgt1_pos);
			glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spotdir);
			robot(1);
		glPopMatrix();
							
		glPushMatrix();			//draw robot shadow
			glTranslatef(robot_x, 0, robot_z);
			glRotatef(robot_y, 0, 1, 0);
			robotShadow();
		glPopMatrix();
	}
		
	glPushMatrix();			//draw robot backup
		glTranslatef(300, 0.0, -250);
		glRotatef(-90, 0, 1, 0);
		robot(0);
	glPopMatrix();
	
	glPushMatrix();			//draw robot shadow
		glTranslatef(300, 0.0, -250);
		glRotatef(-90, 0, 1, 0);
		robotShadow();
	glPopMatrix();
	
	glPushMatrix();			//draw robot backup
		glTranslatef(250, 0.0, -250);
		glRotatef(-90, 0, 1, 0);
		robot(0);
	glPopMatrix();
	
	glPushMatrix();			//draw robot shadow
		glTranslatef(250, 0.0, -250);
		glRotatef(-90, 0, 1, 0);
		robotShadow();
	glPopMatrix();
	
	glPushMatrix();			//draw robot backup
		glTranslatef(200, 0.0, -250);
		glRotatef(-90, 0, 1, 0);
		robot(0);
	glPopMatrix();
	
	glPushMatrix();			//draw robot shadow
		glTranslatef(200, 0.0, -250);
		glRotatef(-90, 0, 1, 0);
		robotShadow();
	glPopMatrix();
	
	glPushMatrix();			//draw airplane
		glRotatef(airplaneAngle, 0, 1, 0); 
	    glTranslatef(0.0, 300, -250);
		airpalne(1);
    glPopMatrix();
    
    glPushMatrix();			//draw flag
		glRotatef(airplaneAngle, 0, 1, 0); 
		glTranslatef(0.0, 300, -250);
		glTranslatef(85, 0, 0);
		glRotatef(90, 0, 1, 0);
		flag();
    glPopMatrix();
    
    glPushMatrix();			//draw airplane shadow
		glRotatef(airplaneAngle, 0, 1, 0);
		glTranslatef(0.0, 0.0, -250);
		airplaneShadow();
    glPopMatrix();
    
    glPushMatrix();			//draw backup airplane
	    glTranslatef(350.0, 0.0, 350);
	    glRotatef(-150, 0, 1, 0);
		airpalne(1);
    glPopMatrix();
    
    glPushMatrix();			//draw backup airplane
	    glTranslatef(300.0, 0.0, 280);
	    glRotatef(-150, 0, 1, 0);
		airpalne(1);
    glPopMatrix();
    
	glutSwapBuffers();
}

//-------------------- Main function --------------------
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE|GLUT_DEPTH);
	glutInitWindowSize (800, 800); 
	glutInitWindowPosition (50, 50);
	glutCreateWindow ("Assignment-1 Yuhang Wu - 31992820");
	initialize ();
	
	glutDisplayFunc(display); 
	glutSpecialFunc(camerPosition);
	glutKeyboardFunc(keyboard);
	
	glutTimerFunc(50, airplaneTimer, 0);
	glutTimerFunc(20, robotTimer,0);
	glutTimerFunc(50, lighting, 0);
	glutMainLoop();
	return 0;
}


/* 
* File:   main.cpp
* Author: Robert Quan
*
* Created on September 6, 2017, 4:11 PM
*
* Comand to compile in Linux and run the OPEN GL code in C++
* g++ RQ_Hw1.cpp -lglut -lGLU -lGL
*/

#include <GL/glut.h>
#include <iostream>
#include "game.h"
#include "cyCore.h"
#include "cyPoint.h"

double mu = 0.1;
double b = 0.00001; //surface area friction coefficient smooth sphere
int rotateon;
bool magnetization;

double xmin, xmax, ymin, ymax, zmin, zmax;
double maxdiff;
Game game;
// double bax, bay, baz;
cyPoint3d p1(0,50,0);
cyPoint3d p2(100.0,-50.0,50.0);
cyPoint3d p3(50.0,-50.0,-50.0);
cyPoint3d e1;
cyPoint3d Mag_For;

double coff_rest;
// double coff_rest = 3;
// double coeff_friction = 0.01;
double coeff_friction;
double gravity;
cyPoint3d B(0.0,5.0,0.0);	//magnetic field vector


double v00;
double v01;
double v11;
double v02;
double v12;
double invDenom;
cyPoint3d v1;
cyPoint3d v0;
int lastx, lasty;
int xchange, ychange;
float spin = 0.0;
float spinup = 0.0;
int frame =0;
int created = 1000;
int modulo =0;

double dn1;  //distance from next to plane


using namespace std;

void beginInit(){
	cout << "Hi, set default values? (y/n):";
	char a;
	cin >> a;
	if(a=='y'){
		coff_rest = 3;
		coeff_friction = 0.01;
		gravity = 10.0;
	}
	else{
		cout << "Coefficient of restitution: " << endl;
		cin >> coff_rest;
		cout << endl;
		cout << "Coefficient of friction: " << endl;
		cin >> coeff_friction;
		cout << endl;
		cout << "Gravity: " << endl;
		cin >> gravity;
		cout << endl;
	}

}

cyPoint3d getNormal(cyPoint3d p1, cyPoint3d p2, cyPoint3d p3){
	//Gets the normal vector and sets up the conditions for barycentric collisions.
	v1 = p2-p1;
	v0 = p3-p1;
	v00 = v0.Dot(v0);
	v01 = v0.Dot(v1);
	v11 = v1.Dot(v1);
	invDenom = 1/(v00 * v11 - v01*v01);
	cyPoint3d cross;
	cross = v1.Cross(v0);
	cross.Normalize();
	return cross;
}

void magnetic_field(double velx, double vely, double velz){
	cyPoint3d vel(velx, vely, velz);
	vel.Normalize();

	Mag_For = vel.Cross(B);
}

int barycentric(double px, double py, double pz){
	//checks to see if the collision was actually on the triangle
	cyPoint3d P(px,py,pz);
	cyPoint3d v2;
	v2 = P - p1;
	v02 = v0.Dot(v2);
	v12 = v1.Dot(v2);
	double u = (v11 * v02 - v01 * v12) * invDenom;
	double v = (v00 * v12 - v01 * v02) * invDenom;
	bool temp = (u >= 0) && (v >= 0) && (u + v < 1);
	// Check if point is in triangle
	return temp;	
}

bool collisionTest(double lastx, double lasty, double lastz, double nextx, double nexty, double nextz){
	//lets find the distance from the veloctiy vector and the normal vector
	//(x-p)*n
	cyPoint3d P(lastx,lasty,lastz);
	cyPoint3d v2 = P - p1;
	double k = e1.Dot(v2);
	cyPoint3d N(nextx,nexty,nextz);
	cyPoint3d v4 = N - p1;
	double j = e1.Dot(v4);
	dn1 = j;
	if ( k*j<=0){
		return 1*barycentric(lastx,lasty,lastz);}
		return false;
}
	
	
void display(void) {
	GLfloat box_ambient[] = {0.1, 0.1, 0.1};
	GLfloat smallr00[] = {0.1, 0.0, 0.0};
	GLfloat small0g0[] = {0.0, 0.1, 0.0};
	GLfloat small00b[] = {0.0, 0.0, 0.1};
	GLfloat smallrg0[] = {0.1, 0.1, 0.0};
	GLfloat smallr0b[] = {0.1, 0.0, 0.1};
	GLfloat small0gb[] = {0.0, 0.1, 0.1};
	GLfloat smallrgb[] = {0.1, 0.1, 0.1};
	GLfloat ball_ambient[] = {0.4, 0.0, 0.0};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	//rotate the view
	glRotatef(spinup, 1.0, 0.0, 0.0);
	glRotatef(spin, 0.0, 1.0, 0.0);
  
	glBegin(GL_TRIANGLES);           
	   
	   // Right
	   glMaterialfv(GL_FRONT, GL_AMBIENT, smallrgb);
	   glVertex3f(p1.x, p1.y, p1.z);
	   glVertex3f(p2.x, p2.y, p2.z);
	   glVertex3f(p3.x, p3.y, p3.z);

	glEnd();  

	 glBegin(GL_POINTS);
	 	for(int i=0; i<created+frame; i++){
	 		if(game.phys.generator.ballvec[i].isalive){
	 			glColor3f(0.0f,0.0f,1.0f);
	 			glVertex3f(game.phys.generator.ballvec[i].x,game.phys.generator.ballvec[i].y,game.phys.generator.ballvec[i].z);
	 		}
	 	}
	 glEnd();

	glPopMatrix();
	glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
	// Compute aspect ratio of the new window
	if (height == 0) height = 1;                // To prevent divide by 0
	GLfloat aspect = (GLfloat)width / (GLfloat)height;
  
	// Set the viewport to cover the new window
	glViewport(0, 0, width, height);
  
	// Set the aspect ratio of the clipping volume to match the viewport
	glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
	glLoadIdentity();             // Reset
	// Enable perspective projection with fovy, aspect, zNear and zFar
	gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}


void iterate(void){
		//Lets see if we want magnetization
		if(!magnetization){
			for(int i=0; i<frame+created;i++){
				if(game.phys.generator.ballvec[i].isalive){
						game.phys.positions(game.phys, game.kTimeStep, frame+created,i);
						// cout << "Particle: " << i << endl;
						if(!collisionTest(game.phys.generator.ballvec[i].lastx, game.phys.generator.ballvec[i].lasty, 
						game.phys.generator.ballvec[i].lastz, game.phys.generator.ballvec[i].nextx, game.phys.generator.ballvec[i].nexty, game.phys.generator.ballvec[i].nextz)){
							game.phys.generator.ballvec[i].x += game.phys.generator.ballvec[i].vx*game.kTimeStep;
							game.phys.generator.ballvec[i].y += game.phys.generator.ballvec[i].vy*game.kTimeStep;
							game.phys.generator.ballvec[i].z += game.phys.generator.ballvec[i].vz*game.kTimeStep;
							game.phys.generator.ballvec[i].vz -= gravity*game.kTimeStep*0.02;
							} else{
								// x'(n+1) = x(n+1) - (1-p)d(n+1)*n
								game.phys.generator.ballvec[i].x = game.phys.generator.ballvec[i].x - (1+coff_rest)*(dn1)*e1.x;
								game.phys.generator.ballvec[i].y = game.phys.generator.ballvec[i].y - (1+coff_rest)*(dn1)*e1.y;
								game.phys.generator.ballvec[i].z = game.phys.generator.ballvec[i].z - (1+coff_rest)*(dn1)*e1.z;

								game.phys.generator.ballvec[i].vx = -coff_rest*game.phys.generator.ballvec[i].vx*e1.x*e1.x + (1-coeff_friction)*game.phys.generator.ballvec[i].vx - game.phys.generator.ballvec[i].vx*e1.x*e1.x;
								game.phys.generator.ballvec[i].vy = -coff_rest*game.phys.generator.ballvec[i].vy*e1.y*e1.y + (1-coeff_friction)*game.phys.generator.ballvec[i].vy - game.phys.generator.ballvec[i].vy*e1.y*e1.y;
								game.phys.generator.ballvec[i].vz = -coff_rest*game.phys.generator.ballvec[i].vz*e1.z*e1.z + (1-coeff_friction)*game.phys.generator.ballvec[i].vz - game.phys.generator.ballvec[i].vz*e1.z*e1.z;
							}
					}
			}
		}
		else {
			for(int i=0; i<frame+created;i++){
				if(game.phys.generator.ballvec[i].isalive){
						game.phys.positions(game.phys, game.kTimeStep, frame+created,i);
						if(!collisionTest(game.phys.generator.ballvec[i].lastx, game.phys.generator.ballvec[i].lasty, 
						game.phys.generator.ballvec[i].lastz, game.phys.generator.ballvec[i].nextx, game.phys.generator.ballvec[i].nexty, game.phys.generator.ballvec[i].nextz)){
							magnetic_field(game.phys.generator.ballvec[i].vx,game.phys.generator.ballvec[i].vy,game.phys.generator.ballvec[i].vz);
							
							game.phys.generator.ballvec[i].x += game.phys.generator.ballvec[i].vx*game.kTimeStep;
							game.phys.generator.ballvec[i].y += game.phys.generator.ballvec[i].vy*game.kTimeStep;
							game.phys.generator.ballvec[i].z += game.phys.generator.ballvec[i].vz*game.kTimeStep;
							game.phys.generator.ballvec[i].vx += Mag_For.x*game.kTimeStep;
							game.phys.generator.ballvec[i].vy += Mag_For.y*game.kTimeStep;
							game.phys.generator.ballvec[i].vz += Mag_For.z*game.kTimeStep - gravity*game.kTimeStep*0.02;

							} else{
								// x'(n+1) = x(n+1) - (1-p)d(n+1)*n
								magnetic_field(game.phys.generator.ballvec[i].vx,game.phys.generator.ballvec[i].vy,game.phys.generator.ballvec[i].vz);
								game.phys.generator.ballvec[i].x = game.phys.generator.ballvec[i].x - (1+coff_rest)*(dn1)*e1.x;
								game.phys.generator.ballvec[i].y = game.phys.generator.ballvec[i].y - (1+coff_rest)*(dn1)*e1.y;
								game.phys.generator.ballvec[i].z = game.phys.generator.ballvec[i].z - (1+coff_rest)*(dn1)*e1.z;
								game.phys.generator.ballvec[i].vx = -coff_rest*game.phys.generator.ballvec[i].vx*e1.x*e1.x + (1-coeff_friction)*game.phys.generator.ballvec[i].vx - game.phys.generator.ballvec[i].vx*e1.x*e1.x;
								game.phys.generator.ballvec[i].vy = -coff_rest*game.phys.generator.ballvec[i].vy*e1.y*e1.y + (1-coeff_friction)*game.phys.generator.ballvec[i].vy - game.phys.generator.ballvec[i].vy*e1.y*e1.y;
								game.phys.generator.ballvec[i].vz = -coff_rest*game.phys.generator.ballvec[i].vz*e1.z*e1.z + (1-coeff_friction)*game.phys.generator.ballvec[i].vz - game.phys.generator.ballvec[i].vz*e1.z*e1.z;
								game.phys.generator.ballvec[i].vx += Mag_For.x*game.kTimeStep;
								game.phys.generator.ballvec[i].vy += Mag_For.y*game.kTimeStep;
								game.phys.generator.ballvec[i].vz += Mag_For.z*game.kTimeStep- gravity*game.kTimeStep*0.02;
							}
					}
			}
		}
	frame++;
	glutPostRedisplay();
}

void init(void) {

	glClearDepth(1.0f);
	// Enable Z-buffering, backface culling, and lighting
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, 1.0, 1, 600);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glutSetCursor(GLUT_CURSOR_INFO);

	// Set eye point and look at point
	gluLookAt(0, 300, 40, 0, 0, 0, 0, 0, 1);

	// Set up lights
	GLfloat light0color[] = {1.0, 1.0, 1.0};
	GLfloat light0pos[] = {0, 0, 0};
	GLfloat light1color[] = {1.0, 1.0, 1.0};
	GLfloat light1pos[] = {300, 300, 300};
	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0color);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0color);
	glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1color);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1color);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light1color);



}

void reshapeFunc(GLint newWidth, GLint newHeight) {
	if (newWidth > newHeight) // Keep a square viewport
		glViewport((newWidth - newHeight) / 2, 0, newHeight, newHeight);
	else
		glViewport(0, (newHeight - newWidth) / 2, newWidth, newWidth);
	init();
	glutPostRedisplay();
}

void rotateview(void) {
	
	if (rotateon) {
		spin += xchange / 250.0;
		if (spin >= 360.0) spin -= 360.0;
		if (spin < 0.0) spin += 360.0;
		spinup -= ychange / 250.0;
		if (spinup > 89.0) spinup = 89.0;
		if (spinup < -89.0) spinup = -89.0;
	}

	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {

	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN) {
				lastx = x;
				lasty = y;
				xchange = 0;
				ychange = 0;
				rotateon = 1;
			} else if (state == GLUT_UP) {
				xchange = 0;
				ychange = 0;
				rotateon = 0;
			}
			break;

		default:
			break;
	}
}

void motion(int x, int y) {
	xchange = x - lastx;
	ychange = y - lasty;

	rotateview();
}

void keyboard(unsigned char key, int x, int y){
    
    switch (key){
		case 'm':
		//lets turn on the magnetic field
		magnetization = true;
		std::cout << "Magnetization on" << std::endl;
		break;
		case 'n':
		//lets turn off the magnetic field
		magnetization = false;
		std::cout << "Magnetization off" << std::endl;
		break;
        case 'w':
            spinup -= 5;
        break;
        case 's':
            spinup += 5;
        break;
        case 'a':
            spin += 5;
        break;
        case 'd':
            spin -= 5;
        break;
        case ' ':
		game.phys.reset(game.phys);	//rese the motion of the particles!
		created = 1000; 
		magnetization = false;
		break;
	}
}

int main(int argc, char** argv) {
	GLint SubMenu1, SubMenu2, SubMenu3, SubMenu4;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(1022, 100);
	glutCreateWindow("Particle Generation");
	e1 = getNormal(p1,p2,p3);	//lets get the normal vector to the plane.
	beginInit();
	init();
	rotateon = 0;
	
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutIdleFunc(iterate); //The idle funtion could be restarted.
	glutReshapeFunc(reshapeFunc);
	glutMainLoop();

	return 0;
};

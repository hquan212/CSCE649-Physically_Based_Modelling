
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
#include <string>
#include <math.h>
#include "ball.h"

using namespace std;

double ballx, bally, ballz, ball2x, ball2y, ball2z; //The position of the ball - you can set this in your code
double ballradius = 14;		//radius of each ball
double ballvx, ballvy, ballvz, ball2vx, ball2vy, ball2vz;  //Initial velocities
double boxxl, boxxh, boxyl, boxyh, boxzl, boxzh; // The low and high x, y, z values for the box sides
int g = 10;     //value of gravity
double h = 0.01;	//timestep
double mu = 0.1;
double b = 0.00001; //surface area friction coefficient smooth sphere
bool revx, revy, revz, revzl; //Boolean to see if the ball is traveling in negative directiosn
bool rev2x, rev2y, rev2z; //Boolean to see if the ball is traveling in negative directiosn
int rotateon;
double epsilon = 0.005; //global constant
int balls =0; 	//Number of balls to create

double xmin, xmax, ymin, ymax, zmin, zmax;
double maxdiff;

int lastx, lasty;
int xchange, ychange;
float spin = 0.0;
float spinup = 0.0;

void wind(int xchange, int ychange){
	cout << endl;
	cout << "Wind X!:\t" << xchange*0.01 << endl; 
	cout << "Wind Y!:\t" << ychange*0.01 << endl;

	ballvz += abs(ychange)*0.01;
	ballvx += abs(xchange)*0.01;
	ballvy += abs(sin(xchange*ychange));

	ball2vz += abs(ychange)*0.01;
	ball2vx += abs(xchange)*0.01;
	ball2vy += abs(sin(xchange*ychange));

};

void ball_collision(double x, double x2, double y, double y2, double z, double z2){
	if(abs(x-x2) <= ballradius){
		ballvx = -ballvx;
		ball2vx = -ball2vx;
		cout << "Collision" << endl;
	} else if(abs(y-y2) <= ballradius){
		ballvy = -ballvy;
		ball2vy = -ball2vy;
		cout << "Collision" << endl;
	} else if(abs(z-z2) <= ballradius) {
		ballvz = -ballvz;
		ball2vz = -ball2vz;
		cout << "Collision" << endl;
	} else {}
}

void move2(){

	//ball_collision(ball2x,ballx,ball2y,bally,ball2z,ballz);
	
	if (ball2x > boxxh - ballradius) {
		revx = 1;	
		ball2vy = ball2vy*(1-mu);
		ball2vz = ball2vz*(1-mu); //friction
		ball2vx = -ball2vx;
		
	} else if (ball2x < boxxl + ballradius) {
		revx = 0;
		ball2vy = ball2vy*(1-mu);
		ball2vz = ball2vz*(1-mu); //friction
		ball2vx = -ball2vx;
		
	} else if (ball2y > boxyh - ballradius) {
		revy = 1;
		ball2vx = ball2vx*(1-mu);
		ball2vz = ball2vz*(1-mu); //friction
		ball2vy = -ball2vy;
	} else if(ball2y < boxyl + ballradius) {
		revy = 0;
		ball2vx = ball2vx*(1-mu);
		ball2vz = ball2vz*(1-mu); //friction
		ball2vy = -ball2vy;
	} else if (ball2z > boxzh - ballradius) {
		revz = 1;
		revzl = 0;
		ball2vx = ball2vx*(1-mu);
		ball2vy = ball2vy*(1-mu); //friction
		ball2vz = -0.98*ball2vz;
	} else if (ball2z < boxzl + ballradius) {
		revz = 0;
		revzl = 1;
		ball2vx = ball2vx*(1-mu);
		ball2vy = ball2vy*(1-mu); //friction
		ball2vz = -0.98*ball2vz;		
	}

	//-------------------------------------------------------------------

	if(abs(ball2vx) > epsilon){		//See if the velocity of ball stops in X direction.
		ball2x = ball2x + ball2vx*h;
		if(ball2vx >= 0){
			ball2vx = ball2vx - b*abs(ball2vx);
		} else {
			ball2vx = ball2vx + b*abs(ball2vx);
		}
	} else {
		ball2vx = 0;
	}	

	//See if the velocity of ball stops in Y direction.
	if(abs(ball2vy) > epsilon){		
		ball2y = ball2y + ball2vy*h;
		if(ball2vy >= 0){
			ball2vy = ball2vy - b*abs(ball2vy);
		} else {
			ball2vy = ball2vy + b*abs(ball2vy);
		}
	} else {
		ball2vy = 0;				//Stop the ball in Y
	}	


	//Update Z
		
	ball2z = ball2z + ball2vz*h;		//Euler Integratoin
	ball2vz = ball2vz - g*h;

	if(ball2vz >= 0){
		ball2vz = ball2vz - b*abs(ball2vz);
	} else {
		ball2vz = ball2vz + b*abs(ball2vz);
	}

};

void move1(){
	
	//ball_collision(ballx,ball2x,bally,ball2y,ballz,ball2z);

	if (ballx > boxxh - ballradius) {
		revx = 1;	
		ballvy = ballvy*(1-mu);
		ballvz = ballvz*(1-mu); //friction
		ballvx = -ballvx;
		
	} else if (ballx < boxxl + ballradius) {
		revx = 0;
		ballvy = ballvy*(1-mu);
		ballvz = ballvz*(1-mu); //friction
		ballvx = -ballvx;
		
	} else if (bally > boxyh - ballradius) {
		revy = 1;
		ballvx = ballvx*(1-mu);
		ballvz = ballvz*(1-mu); //friction
		ballvy = -ballvy;
	} else if(bally < boxyl + ballradius) {
		revy = 0;
		ballvx = ballvx*(1-mu);
		ballvz = ballvz*(1-mu); //friction
		ballvy = -ballvy;
	} else if (ballz > boxzh - ballradius) {
		revz = 1;
		revzl = 0;
		ballvx = ballvx*(1-mu);
		ballvy = ballvy*(1-mu); //friction
		ballvz = -0.98*ballvz;
	} else if (ballz < boxzl + ballradius) {
		revz = 0;
		revzl = 1;
		ballvx = ballvx*(1-mu);
		ballvy = ballvy*(1-mu); //friction
		ballvz = -0.98*ballvz;		
	}
	
	//-------------------------------------------------------------------
	
	if(abs(ballvx) > epsilon){		//See if the velocity of ball stops in X direction.
		ballx = ballx + ballvx*h;
		if(ballvx >= 0){
			ballvx = ballvx - b*abs(ballvx);
		} else {
			ballvx = ballvx + b*abs(ballvx);
		}
	} else {
		ballvx = 0;
	}	

	//See if the velocity of ball stops in Y direction.
	if(abs(ballvy) > epsilon){		
		bally = bally + ballvy*h;
		if(ballvy >= 0){
			ballvy = ballvy - b*abs(ballvy);
		} else {
			ballvy = ballvy + b*abs(ballvy);
		}
	} else {
		ballvy = 0;				//Stop the ball in Y
	}	


	//Lets Check to see the bounce height difference
	/*
	if(ballz < (boxzl + ballradius + 0.02) && abs(ballz - (ballz-ballvz*h)) < 0.002){
		ballvz += 0.8*ballvz;
		cout << "Stoppping" << endl;
		cout << endl;
	}*/

	//Update Z
		
	ballz = ballz + ballvz*h;		//Euler Integratoin
	ballvz = ballvz - g*h;

	if(ballvz >= 0){
		ballvz = ballvz - b*abs(ballvz);
	} else {
		ballvz = ballvz + b*abs(ballvz);
	}

	move2();
};

void display(void) {
	GLfloat box_ambient[] = {0.1, 0.1, 0.1};
	GLfloat smallr00[] = {0.1, 0.0, 0.0};
	GLfloat small0g0[] = {0.0, 0.1, 0.0};
	GLfloat small00b[] = {0.0, 0.0, 0.1};
	GLfloat smallrg0[] = {0.1, 0.1, 0.0};
	GLfloat smallr0b[] = {0.1, 0.0, 0.1};
	GLfloat small0gb[] = {0.0, 0.1, 0.1};
	GLfloat smallrgb[] = {0.1, 0.1, 0.1};

	GLfloat box_diffuse[] = {0.7, 0.7, 0.7};
	GLfloat box_specular[] = {0.1, 0.1, 0.1};
	GLfloat box_shininess[] = {1.0};
	GLfloat greenEmissiveMaterial[] = {0.0, 1.0, 0.0};
	GLfloat ball_ambient[] = {0.4, 0.0, 0.0};
	GLfloat ball_diffuse[] = {0.3, 0.0, 0.0};
	GLfloat ball_specular[] = {0.3, 0.3, 0.3};
	GLfloat ball_shininess[] = {10.0};

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	//rotate the view
	glRotatef(spinup, 1.0, 0.0, 0.0);
	glRotatef(spin, 0.0, 1.0, 0.0);

	/*
		//8 vertices:
		glVertex3f(boxxl, boxyl, boxzl);
		glVertex3f(boxxh, boxyl, boxzl);
		glVertex3f(boxxh, boxyh, boxzl);
		glVertex3f(boxxl, boxyh, boxzl);
		glVertex3f(boxxl, boxyl, boxzh);
		glVertex3f(boxxh, boxyl, boxzh);
		glVertex3f(boxxh, boxyh, boxzh);
		glVertex3f(boxxl, boxyh, boxzh);

	*/
	//Draw the box
	//set material parameters
	glMaterialfv(GL_FRONT, GL_AMBIENT, box_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, box_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, box_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, box_shininess);

	glBegin(GL_QUADS);
	//back face
	glMaterialfv(GL_FRONT, GL_AMBIENT, smallrgb);
	glVertex3f(boxxl, boxyl, boxzl);
	glVertex3f(boxxh, boxyl, boxzl);
	glVertex3f(boxxh, boxyh, boxzl);
	glVertex3f(boxxl, boxyh, boxzl);

	//left face
	glMaterialfv(GL_FRONT, GL_AMBIENT, small0g0);
	glVertex3f(boxxl, boxyl, boxzh);
	glVertex3f(boxxl, boxyl, boxzl);
	glVertex3f(boxxl, boxyh, boxzl);
	glVertex3f(boxxl, boxyh, boxzh);

	//right face
	glMaterialfv(GL_FRONT, GL_AMBIENT, small00b);
	glVertex3f(boxxh, boxyl, boxzh);
	glVertex3f(boxxh, boxyh, boxzh);
	glVertex3f(boxxh, boxyh, boxzl);
	glVertex3f(boxxh, boxyl, boxzl);

	//bottom face
	glMaterialfv(GL_FRONT, GL_AMBIENT, smallrg0);
	glVertex3f(boxxh, boxyl, boxzh);
	glVertex3f(boxxh, boxyl, boxzl);
	glVertex3f(boxxl, boxyl, boxzl);
	glVertex3f(boxxl, boxyl, boxzh);

	//top face
	glMaterialfv(GL_FRONT, GL_AMBIENT, smallr0b);
	glVertex3f(boxxh, boxyh, boxzh);
	glVertex3f(boxxl, boxyh, boxzh);
	glVertex3f(boxxl, boxyh, boxzl);
	glVertex3f(boxxh, boxyh, boxzl);

	//front face
	glMaterialfv(GL_FRONT, GL_AMBIENT, small0gb);
	glVertex3f(boxxh, boxyl, boxzh);
	glVertex3f(boxxl, boxyl, boxzh);
	glVertex3f(boxxl, boxyh, boxzh);
	glVertex3f(boxxh, boxyh, boxzh);

	glEnd();

	//draw the ball
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ball_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ball_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ball_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, ball_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, greenEmissiveMaterial);
	glPushMatrix();
	glTranslatef(-ballx, bally, ballz);
	glutSolidSphere(ballradius, 10, 10);
	glPopMatrix();

	//draw the ball2
	
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ball_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, ball_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, ball_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, ball_shininess);
	glPushMatrix();
	glTranslatef(-ball2x, ball2y, ball2z);
	glutSolidSphere(ballradius, 10, 10);
	glPopMatrix();
	
	glPopMatrix();
	glutSwapBuffers();
};

void iterate(void){
	//Ball b1;
	//Ball b2;
	for(int i=0;i<40;i++){
		move1();
	}
	glutPostRedisplay();
}

void init(void) {
	glClearColor(0.0, 0.0, 0.0, 0.0);
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
	gluLookAt(0, 400, 40, 0, 0, 0, 0, 0, 1);

	// Set up lights
	GLfloat light0color[] = {1.0, 1.0, 1.0};
	GLfloat light0pos[] = {0, 500, 300};
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


	//Initialize ball position
	ballx = 0.0;
	bally = 0.0;
	ballz = 0.0;
	//Velocity of first ball
	ballvx = 12.0;
	ballvy = 15.0;
	ballvz = 10.0;
	//Initialize the boolean bounce conditions
	revx = 0.0;
	revy = 0.0;
	revz = 0.0;

	//-----------------------------------------------------------
	//Initialize ball position
	ball2x = 5.0;
	ball2y = 20.0;
	ball2z = 30.0;
	//Bolean conditions
	rev2x = 0;
	rev2y = 0;
	rev2z = 0;
	//Initialize ball velocity
	ball2vx = 15.0;
	ball2vy = 20.0;
	ball2vz = 30.0;

	//-----------------------------------------------------------
	//Initialize box boundaries
	boxxl = -100;
	boxxh = 100;
	boxyl = -100;
	boxyh = 100;
	boxzl = -100;
	boxzh = 100;

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

	wind(xchange,ychange);
}

int main(int argc, char** argv) {
	GLint SubMenu1, SubMenu2, SubMenu3, SubMenu4;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(1022, 100);
	glutCreateWindow("Ball in Cube Demo");
	init();
	rotateon = 0;
	
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	cout << "Enter the your desired timestep (choose a number below 0.025):\t" << endl;
	cin >> h;
	glutIdleFunc(iterate); //The idle funtion could be restarted.
	glutReshapeFunc(reshapeFunc);
	glutMainLoop();

	return 0;
};

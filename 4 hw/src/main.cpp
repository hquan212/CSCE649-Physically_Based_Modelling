#include <GL/glut.h>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>

#include "cyCore.h"
#include "cyPoint.h"


double boxxl, boxxh, boxyl, boxyh, boxzl, boxzh; // The low and high x, y, z values for the box sides
int gravity = 30;     //value of gravity
double h = 0.01;	//timestep
int rotateon;
double xmin, xmax, ymin, ymax, zmin, zmax;
double maxdiff;
bool euler;

cyPoint3d face1(-100,0.0,0.0);
cyPoint3d face2(100,0.0,0.0);
cyPoint3d face3(0.0,-100,0.0);
cyPoint3d face4(0.0,100,0.0);
cyPoint3d face5(0.0,0.0,-100);
cyPoint3d face6(0.0,0.0,100);

cyPoint3d e1 = face1.GetNormalized();
cyPoint3d e2 = face2.GetNormalized();
cyPoint3d e3 = face3.GetNormalized();
cyPoint3d e4 = face4.GetNormalized();
cyPoint3d e5 = face5.GetNormalized();
cyPoint3d e6 = face6.GetNormalized();

std::vector<cyPoint3d> e_vector;


double coeff_friction = 0.02;
double coff_rest =0.001;

std::vector<std::vector<double>> struct_length(8,std::vector<double>(8,0));

int lastx, lasty;
int xchange, ychange;
float spin = 0.0;
float spinup = 0.0;

struct Vertex{
    double x;
    double y;
    double z;
    double vx;
    double vy;
	double vz;
	double ax;
	double ay;
	double az;
	double k = 7000.0;
	double mass = 2.0;
	double d = 1003.2;
};

std::vector<Vertex> v;

void euclidian_dist(){
	for(int i=0;i<v.size();i++){
		cyPoint3d a(v[i].x,v[i].y,v[i].z);
		for(int j=0;j<v.size();j++){
			if(i==j) continue;
			cyPoint3d b(v[j].x,v[j].y,v[j].z);
			cyPoint3d r = a-b;
			struct_length[i][j] = r.Length();
		}
	}
};

void spring_damper_euler(){
	for(int i=0;i<v.size();i++){
		cyPoint3d a(v[i].x,v[i].y,v[i].z);
		cyPoint3d SpringrunningSum(0.0,0.0,0.0);
		cyPoint3d DamperRunningSum(0.0,0.0,0.0);
		cyPoint3d vela(v[i].vx,v[i].vy,v[i].vz);
		for(int j=0;j<v.size();j++){
			if(i!=j){
				cyPoint3d b(v[j].x,v[j].y,v[j].z);
				cyPoint3d velb(v[j].vx,v[j].vy,v[j].vz);
				cyPoint3d r = a-b;
				double eu_dist = r.Length();
				SpringrunningSum += ((v[i].k / v[j].mass) * (eu_dist - struct_length[i][j] ) * (r / pow(eu_dist,2) ) ) ;//get the accel force
				DamperRunningSum += (v[i].d * ((velb - vela)* (r / pow(eu_dist,2) ) )* (r / pow(eu_dist,2) ) );
			}
		}
		v[i].ax = - SpringrunningSum[0]  + DamperRunningSum[0];
		v[i].ay = - SpringrunningSum[1]  + DamperRunningSum[1];
		v[i].az = - SpringrunningSum[2]  + DamperRunningSum[2];
	}
}

cyPoint3d spring_damper_rk(cyPoint3d a, cyPoint3d vela, int i, double h){
	cyPoint3d newPos(v[i].vx*h,v[i].vy*h,v[i].vz*h);
	cyPoint3d newVel(v[i].ax*h,v[i].ay*h,v[i].az*h);
	a += newPos;
	vela += newVel;
	cyPoint3d SpringrunningSum(0.0,0.0,0.0);
	cyPoint3d DamperRunningSum(0.0,0.0,0.0);
	for(int j=0;j<v.size();j++){
		if(i!=j){
			cyPoint3d b(v[j].x,v[j].y,v[j].z);
			cyPoint3d velb(v[j].vx,v[j].vy,v[j].vz);
			cyPoint3d r = a-b;
			double eu_dist = r.Length();
			SpringrunningSum += ((v[i].k / v[j].mass) * (eu_dist - struct_length[i][j] ) * (r / pow(eu_dist,2) ) ) ;//get the accel force
			DamperRunningSum += (v[i].d * ((velb - vela)* (r / pow(eu_dist,2) ) )* (r / pow(eu_dist,2) ) );
		}
	}
	double s1 = -SpringrunningSum[0]+DamperRunningSum[0];
	double s2 = -SpringrunningSum[1]+DamperRunningSum[1];
	double s3 = -SpringrunningSum[2]+DamperRunningSum[2];
	cyPoint3d accel(s1,s2,s3);
	return accel;
}

void rungeKutta(Vertex vert, int i){

	cyPoint3d x1(vert.x, vert.y, vert.z);
	cyPoint3d v1(vert.vx, vert.vy, vert.vz);
	cyPoint3d a1 = spring_damper_rk(x1, v1, i, h);

	cyPoint3d x2 = x1 + 0.5*v1*h;
	cyPoint3d v2 = v1 + 0.5*a1*h;
	cyPoint3d a2 = spring_damper_rk(x2,v2,i, h/2.0);

	cyPoint3d x3 = x1 + 0.5*v2*h;
	cyPoint3d v3 = v1 + 0.5*a2*h;
	cyPoint3d a3 = spring_damper_rk(x3,v3,i, h/2.0);

	cyPoint3d x4 = x1 + v3*h;
	cyPoint3d v4 = v1 + a3*h;
	cyPoint3d a4 = spring_damper_rk(x4,v4,i, h);

	cyPoint3d pos = x1 + (h/6.0)*(v1 + 2*v2 + 2*v3 + v4);
	cyPoint3d vel = v1 + (h/6.0)*(a1 + 2*a2 + 2*a3 + a4);

	v[i].x = pos.x;	//update the new position from rk4
	v[i].y = pos.y;
	v[i].z = pos.z;

	v[i].vx = vel.x;	//update new velocities using rk4
	v[i].vy = vel.y;
	v[i].vz = vel.z - gravity*h;

}	

void collision_detection(){
	for(int i=0;i<v.size();i++){

		// Get the vector subtraction for the vertex to each face
		// P- F1
		cyPoint3d Previos_pos(v[i].x,v[i].y,v[i].z);
		cyPoint3d k1 = Previos_pos - face1;
		cyPoint3d k2 = Previos_pos - face2;
		cyPoint3d k3 = Previos_pos - face3;
		cyPoint3d k4 = Previos_pos - face4;
		cyPoint3d k5 = Previos_pos - face5;
		cyPoint3d k6 = Previos_pos - face6;

		std::vector<double> distance_from_planes;
		// Vector of distances in direction with unit vectors.
		distance_from_planes.push_back(e1.Dot(k1));
		distance_from_planes.push_back(e2.Dot(k2));
		distance_from_planes.push_back(e3.Dot(k3));
		distance_from_planes.push_back(e4.Dot(k4));
		distance_from_planes.push_back(e5.Dot(k5));
		distance_from_planes.push_back(e6.Dot(k6));
	
		double collision_nextx = v[i].x + v[i].vx*h*4;
		double collision_nexty = v[i].y + v[i].vy*h*4;
		double collision_nextz = v[i].z + v[i].vz*h*4;
		
		cyPoint3d Next_pos(collision_nextx, collision_nexty, collision_nextz);
		cyPoint3d c1 = Next_pos - face1;
		cyPoint3d c2 = Next_pos - face2;
		cyPoint3d c3 = Next_pos - face3;
		cyPoint3d c4 = Next_pos - face4;
		cyPoint3d c5 = Next_pos - face5;
		cyPoint3d c6 = Next_pos - face6;
		
		std::vector<double> next_dist;
		next_dist.push_back(e1.Dot(c1));
		next_dist.push_back(e2.Dot(c2));
		next_dist.push_back(e3.Dot(c3));
		next_dist.push_back(e4.Dot(c4));
		next_dist.push_back(e5.Dot(c5));
		next_dist.push_back(e6.Dot(c6));

		for(int j=0;j<distance_from_planes.size();j++){
			if(distance_from_planes[j]*next_dist[j] <= 0){
				// x'(n+1) = x(n+1) - (1-p)d(n+1)*n
				v[i].x = v[i].x - (1+coff_rest)*e_vector[j].x;
				v[i].y = v[i].y - (1+coff_rest)*e_vector[j].y;
				v[i].z = v[i].z - (1+coff_rest)*e_vector[j].z;

				// v'(n+1) = -r*v(n)*e*e + (1-p)*v(n) - v*e*e
				v[i].vx = -coff_rest*v[i].vx*e_vector[j].x*e_vector[j].x + (1-coeff_friction)*v[i].vx - v[i].vx*e_vector[j].x*e_vector[j].x;
				v[i].vy = -coff_rest*v[i].vy*e_vector[j].y*e_vector[j].y + (1-coeff_friction)*v[i].vy - v[i].vy*e_vector[j].y*e_vector[j].y;
				v[i].vz = -coff_rest*v[i].vz*e_vector[j].z*e_vector[j].z + (1-coeff_friction)*v[i].vz - v[i].vz*e_vector[j].z*e_vector[j].z;
			}
		}

	}
};

void positionstep(){    //Euler integration for position.
    for(int i =0; i<v.size(); i++){
        v[i].x = v[i].x+v[i].vx*h; 
        v[i].y = v[i].y+v[i].vy*h; 
        v[i].z = v[i].z+v[i].vz*h; 
    }
};

void velocitystep(){    //Euler integration for velocity
    for(int i =0; i<v.size(); i++){
        v[i].vx = v[i].vx+v[i].ax*h; 
        v[i].vy = v[i].vy+v[i].ay*h; 
		v[i].vz = v[i].vz+v[i].az*h-gravity*h; 
    }
};

void setupcube(){
    v[0].x = 20.0;   //TTT
    v[0].y = 20.0;
    v[0].z = 20.0;

    v[1].x = 20.0;   //TTF
    v[1].y = 20.0;
    v[1].z = -20.0;

    v[2].x = 20.0;   //TFF
    v[2].y = -20.0;
    v[2].z = -20.0;

    v[3].x = 20.0;   //TFT
    v[3].y = -20.0;
    v[3].z = 20.0;

    v[4].x = -20.0;   //FTT
    v[4].y = 20.0;
    v[4].z = 20.0;

    v[5].x = -20.0;   //FTF
    v[5].y = 20.0;
    v[5].z = -20.0;

    v[6].x = -20.0;   //FFT
    v[6].y = -20.0;
    v[6].z = 20.0;

    v[7].x = -20.0;  //FFF
    v[7].y = -20.0;
	v[7].z = -20.0;
	
	for(int i = 0; i < 8 ; i++){
		v[i].vx = 38.8;
		v[i].vy = 35.3;
		v[i].vz = 30.5;
	}

	v[2].vx += 8.8;
	v[2].vy += 5.3;
	v[2].vz += 4.5;

	e_vector.push_back(e1);
	e_vector.push_back(e2);
	e_vector.push_back(e3);
	e_vector.push_back(e4);
	e_vector.push_back(e5);
	e_vector.push_back(e6);

	euclidian_dist();
};

void euler_step(){
	euler = true;
	positionstep();
	spring_damper_euler();
	velocitystep();
	collision_detection();
	positionstep();
}

void rk4_step(){
	for(int i=0;i<v.size();i++){
		rungeKutta(v[i], i);
	}
	collision_detection();

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
	glLineWidth(3.5); 
	glBegin(GL_LINES);                // Begin drawing the color cube with 6 quads
		glMaterialfv(GL_FRONT, GL_AMBIENT, smallrgb);
		glVertex3f(v[0].x,v[0].y,v[0].z);
		glVertex3f(v[1].x,v[1].y,v[1].z);
		
		glVertex3f(v[1].x,v[1].y,v[1].z);
		glVertex3f(v[2].x,v[2].y,v[2].z);

		glVertex3f(v[2].x,v[2].y,v[2].z);
		glVertex3f(v[3].x,v[3].y,v[3].z);

		glVertex3f(v[0].x,v[0].y,v[0].z);
		glVertex3f(v[4].x,v[4].y,v[4].z);

		glVertex3f(v[5].x,v[5].y,v[5].z);
		glVertex3f(v[4].x,v[4].y,v[4].z);
		
		glVertex3f(v[5].x,v[5].y,v[5].z);
		glVertex3f(v[1].x,v[1].y,v[1].z);

		glVertex3f(v[6].x,v[6].y,v[6].z);
		glVertex3f(v[4].x,v[4].y,v[4].z);

		glVertex3f(v[7].x,v[7].y,v[7].z);
		glVertex3f(v[2].x,v[2].y,v[2].z);

		glVertex3f(v[7].x,v[7].y,v[7].z);
		glVertex3f(v[5].x,v[5].y,v[5].z);    

		glVertex3f(v[0].x,v[0].y,v[0].z);
		glVertex3f(v[3].x,v[3].y,v[3].z);

		glVertex3f(v[7].x,v[7].y,v[7].z);
		glVertex3f(v[6].x,v[6].y,v[6].z); 
		
		glVertex3f(v[6].x,v[6].y,v[6].z); 
		glVertex3f(v[3].x,v[3].y,v[3].z);
	glEnd();
	glPopMatrix();
	glutSwapBuffers();
};

void iterate(void){			// Change From Euler to RK4 for explosion ================================================================
	euler_step();
	// rk4_step();
	glutPostRedisplay();
};

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


	//-----------------------------------------------------------

    for(int i =0; i<8; i++){
		v.push_back(Vertex());
    }
	setupcube();
	//-----------------------------------------------------------
	//Initialize box boundaries
	boxxl = -100;
	boxxh = 100;
	boxyl = -100;
	boxyh = 100;
	boxzl = -100;
	boxzh = 100;

};

void reshapeFunc(GLint newWidth, GLint newHeight) {
	if (newWidth > newHeight) // Keep a square viewport
		glViewport((newWidth - newHeight) / 2, 0, newHeight, newHeight);
	else
		glViewport(0, (newHeight - newWidth) / 2, newWidth, newWidth);
	glutPostRedisplay();
};

void rotateview(void) {
	if (rotateon) {
		spin += xchange / 250.0;
		if (spin >= 360.0) spin -= 360.0;
		if (spin < 0.0) spin += 360.0;
		spinup -= ychange / 250.0;
		if (spinup > 89.0) spinup = 89.0;
		if (spinup < -89.0) spinup = -89.0;
	}
	iterate();
	glutPostRedisplay();
};

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
};

void motion(int x, int y) {
	xchange = x - lastx;
	ychange = y - lasty;
};

float RandomNumber(float Min, float Max){
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

void keyboard(unsigned char key, int x, int y){
    
    switch (key){

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
			setupcube();
			double random_num = RandomNumber(-1000,1000);
			std::cout << random_num <<std::endl;
			for(int i=0;i<v.size();i++){
				v[i].vx = random_num; 
			}
			random_num = RandomNumber(-1000,1000);
			std::cout << random_num <<std::endl;
			for(int i=0;i<v.size();i++){
				v[i].vy = random_num; 
			}
			random_num = RandomNumber(-1000,1000);
			std::cout << random_num <<std::endl;
			for(int i=0;i<v.size();i++){
				v[i].vz = random_num; 
			}
			if(euler){
				v[1].k -= 200.1;
				v[2].k -= 200.1;
				v[3].k -= 200.1;
				v[4].k -= 200.1;
			}
		break;

	}
};

int main(int argc, char** argv) {
	GLint SubMenu1, SubMenu2, SubMenu3, SubMenu4;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(720, 720);
	glutInitWindowPosition(1000, 100);
	glutCreateWindow("Jelly Cube");
	init();
	rotateon = 0;
	
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(motion);
	glutIdleFunc(rotateview); //The idle funtion could be restarted.
	glutReshapeFunc(reshapeFunc);
	glutMainLoop();

	return 0;
};

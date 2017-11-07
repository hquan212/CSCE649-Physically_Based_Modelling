#pragma once
#include <GL/glut.h>

class Ball 
{
public: 
    double x, y, z;     //Balls position
    double vx, vy, vz;  //Balls veloctiy
    double color;
    double lastx, lasty, lastz;
    double nextx, nexty, nextz;
    bool isalive;
    bool collided;
    int time;
    Ball();
};


// set particles to true, 
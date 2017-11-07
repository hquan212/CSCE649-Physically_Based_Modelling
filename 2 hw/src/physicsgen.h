#pragma once
#include <vector>
#include "ballgen.h"
#include <GL/glut.h>
#include "cyCore.h"
#include "cyPoint.h"

//--------------This file  should create all the movements, collisions necessary for the particles in space also link together the object and particles.


class PhysGen
{
public:
    BallGen generator;
    std::vector<double> B = {0,5.0,0}; //magnetic field strength

    void reset(PhysGen& phys);
    void movement(PhysGen& phys, double h, int moveballnum);
    void positions(PhysGen& phys, double h, int moveballnum, int i);
    void magnetic_movement(PhysGen& phys, double h, int moveballnum);
    void collision_movement(PhysGen& phys, double h, int moveballnum);
    bool isalive(PhysGen& phys, int moveballnum);
};
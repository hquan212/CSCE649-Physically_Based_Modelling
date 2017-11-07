#include "physicsgen.h"


void PhysGen::movement(PhysGen& phys, double h, int moveballnum){
    //Euler integration
    for(int i=0; i<moveballnum;i++){
            phys.generator.ballvec[i].x += phys.generator.ballvec[i].vx*h;
            phys.generator.ballvec[i].y += phys.generator.ballvec[i].vy*h;
            phys.generator.ballvec[i].z += phys.generator.ballvec[i].vz*h;
    }
}

void PhysGen::magnetic_movement(PhysGen& phys, double h, int moveballnum){
    //Euler integration
    for(int i=0; i<moveballnum;i++){
        phys.generator.ballvec[i].x += phys.generator.ballvec[i].vx*h;
        phys.generator.ballvec[i].y += phys.generator.ballvec[i].vy*h*B[1];
        phys.generator.ballvec[i].z += phys.generator.ballvec[i].vz*h*B[1];
    }
}


void PhysGen::reset(PhysGen& phys){
    for(int i=0; i<phys.generator.ballvec.size();i++){
        phys.generator.ballvec[i].x = 0;
        phys.generator.ballvec[i].y = 0;
        phys.generator.ballvec[i].z = 0;
        phys.generator.ballvec[i].vx = phys.generator.normal_distribution_generator();
        phys.generator.ballvec[i].vy = phys.generator.normal_distribution_generator()*0.1;
        phys.generator.ballvec[i].vz = phys.generator.normal_distribution_generator()*0.1;
    }
}

void PhysGen::collision_movement(PhysGen& phys, double h, int moveballnum){
    for(int i=0; i<moveballnum;i++){
        if(phys.generator.ballvec[i].collided){
        phys.generator.ballvec[i].x -= phys.generator.ballvec[i].vx*h;
        phys.generator.ballvec[i].y -= phys.generator.ballvec[i].vy*h;
        phys.generator.ballvec[i].z -= phys.generator.ballvec[i].vz*h;
        }
    }
    std::cout << "Inside collion functoin" << std::endl;
}

void PhysGen::positions(PhysGen& phys, double h, int moveballnum, int i){
    // for(int i=0; i<moveballnum;i++){
        phys.generator.ballvec[i].lastx = phys.generator.ballvec[i].x;
        phys.generator.ballvec[i].lasty = phys.generator.ballvec[i].y;
        phys.generator.ballvec[i].lastz = phys.generator.ballvec[i].z;
        phys.generator.ballvec[i].nextx = phys.generator.ballvec[i].lastx + phys.generator.ballvec[i].vx*h;
        phys.generator.ballvec[i].nexty = phys.generator.ballvec[i].lasty + phys.generator.ballvec[i].vy*h;
        phys.generator.ballvec[i].nextz = phys.generator.ballvec[i].lastz + phys.generator.ballvec[i].vz*h;
    // }
        // std::cout << "last: " << phys.generator.ballvec[i].lastx << std::endl;
        // std::cout << "next: " << phys.generator.ballvec[i].nextx << std::endl;
}

bool PhysGen::isalive(PhysGen& phys, int moveballnum){
    for(int i=0; i< moveballnum; i++){
        if(phys.generator.ballvec[i].isalive){
        }
        else {return false;}
    }
}
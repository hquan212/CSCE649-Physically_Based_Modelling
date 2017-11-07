#include "ballgen.h"


BallGen::BallGen() : rng_(std::random_device()()), distribution_(5.0,2.0)
{
    int n = 20000;
    ballvec.resize(n);

    for(auto& b : ballvec){ //push the balls into ballvec
        BallInit(&b);   
    }

}

void BallGen::BallInit(Ball* ballobj)
{   

    ballobj->x = uniform_real_distribution()*5;
    ballobj->y = uniform_real_distribution()*5;
    ballobj->z = uniform_real_distribution()*5;
    // ballobj->vx = - normal_distribution_generator()*2;
    ballobj->vx = normal_distribution_generator() ;
    ballobj->vy = normal_distribution_generator()*0.3;
    ballobj->vz = normal_distribution_generator()*0.1;
    ballobj->color = log(uniform_(rng_)*2);

}

double BallGen::normal_distribution_generator(){
    return distribution_(rng_);
}

double BallGen::uniform_real_distribution(){
    return uniform_(rng_);
}
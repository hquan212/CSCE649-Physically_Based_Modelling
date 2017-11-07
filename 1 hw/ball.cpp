#include "ball.h"
#include <random>
#include <iostream>

using namespace std;

Ball::Ball() : x(0), y(0), z(0), vx(0), vy(0), vz(0)
{
    randomVel();
    init();
    cout << "Vx is: " << x << endl;
    cout << "Vy is: " << y << endl;
    cout << "Vz is: " << z << endl;
}

/*Ball::Ball(int inx,int iny,int inz,int invx,int invy,int invz){
    x= inx;
    y = iny;
    z = inz;
    vx = invx;
    vy = invy;
    vz = invz;
}*/

void Ball::randomVel(){
    vx = ((double)rand()/2184210012)*10;
    vy = ((double)rand()/2184210012)*10;
    vz = ((double)rand()/2184210012)*10;
}

void Ball::init(){
    x = ((double)rand()/2184210012)*70;
    y = ((double)rand()/2184210012)*70;
    z = ((double)rand()/2184210012)*70;
}

int main(){

    Ball b1;
    return 0;
}
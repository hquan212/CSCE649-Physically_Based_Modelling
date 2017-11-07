
class Ball 
{
    public: 
        double x, y, z;     //Balls position
        double vx, vy, vz;  //Balls veloctiy

        Ball();
        //Ball(int inx,int iny,int inz,int invx,int invy,int invz);

        void randomVel();   //init random velocities
        void init();        //Init random positions

};
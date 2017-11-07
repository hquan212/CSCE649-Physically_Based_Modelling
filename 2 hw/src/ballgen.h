#pragma once
#include <random>
#include <iostream>
#include "ball.h"
#include <vector>
#include <algorithm>
#include <random>
#include "math.h"


class BallGen
{
    std::mt19937_64 rng_;
    std::uniform_real_distribution<> uniform_;

    std::normal_distribution<double> distribution_;

public:
    int vec_count, last;
    BallGen();
    std::vector<Ball> ballvec;
    void BallInit(Ball* ballobj);

    double normal_distribution_generator();
    double uniform_real_distribution();
};
#include "robot.h"
#include <cmath>
#include <random>
#include <iostream>

#include "config.h"

using namespace std;

Robot::Robot()
{
    m_random_engine.seed(m_random_device());
    m_dice = normal_distribution<double>(0.0 ,1.0);
}

void Robot::update(double dt)
{
    m_yaw = m_yaw + m_yaw_vel*dt;

    m_x = m_x + m_vel*cos(m_yaw)*dt;
    m_y = m_y + m_vel*sin(m_yaw)*dt;
}


double Robot::vel_noisy()
{
    double sigma = ALPHA1*fabs(m_vel) + ALPHA2*fabs(m_yaw);

    return m_vel + sigma*m_dice(m_random_engine);
}

double Robot::yaw_vel_noisy()
{
    double sigma = ALPHA3*fabs(m_vel) + ALPHA4*fabs(m_yaw);

    return m_yaw_vel + sigma*m_dice(m_random_engine);
}

bool Robot::is_moving()
{
    static const double eps = 0.0001; // kinda arbitrary

    if (fabs(m_vel) > eps || fabs(m_yaw_vel) > eps) {
        return true;
    }

    return false;
}

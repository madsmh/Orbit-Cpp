//
// Created by mads on 14-08-17.
//

#ifndef ORBIT3D_VERLET_H
#define ORBIT3D_VERLET_H

#include "vector3.h"
#include "system.h"
#include "trajectory.h"
#include "planetdata.h"
#include "propertiesfile.h"


class Verlet {

public:
    void setup(int days, int detail);
    void run(System &sys, Trajectory &tra);

private:
    int m_rows;
    int m_bodies;
    double m_delta;
    bool m_abort = false;
};


#endif //ORBIT3D_VERLET_H

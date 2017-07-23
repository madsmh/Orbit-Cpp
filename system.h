//
// Created by mads on 23-07-17.
//

#ifndef ORBIT3D_SYSTEM_H
#define ORBIT3D_SYSTEM_H

#include <vector>
#include <string>
#include "body.h"
#include "vector3.h"

class System {

    std::vector<std::string> self_names;
    std::vector<Body> self_bodies;

    long self_n;

public:
    System(std::vector<std::string> names, std::vector<Vector3> pos0,
           std::vector<Vector3> vel0, std::vector<double>, std::vector<double>);

    std::vector<Vector3> get_positions();

    std::vector<Vector3> get_velocities();

    Vector3 acceleration(Body, Body );

    std::vector<Vector3> get_accelerations();

    void set_positions(std::vector<Vector3>);

    void set_velocities(std::vector<Vector3>);

    long get_number_of_bodies(){ return self_n; }
};


#endif //ORBIT3D_SYSTEM_H

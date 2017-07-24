/* Copyright (c) 2017 By the Authors
 *
 * This file is part of Orbit Simulator.

    Orbit Simulator is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Orbit Simulator is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Orbit Simulator.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */


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

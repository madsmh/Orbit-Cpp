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


#include "system.h"
#include <vector>
#include <string>
#include "vector3.h"

System::System(std::vector<std::string> names, std::vector<Vector3> pos0,
               std::vector<Vector3> vel0, std::vector<double> gms,
               std::vector<double> radii){

    self_n = names.size();

    // Initialize self_n obejects of Body type
    for (int i=0; i < self_n; i++){
    self_bodies.emplace_back(Body(names[i], pos0[i], vel0[i], gms[i], radii[i]));
    }

    for (int j = 0; j < self_n; ++j) {
        self_names.push_back(self_bodies[j].get_name());
    }
};
std::vector<Vector3> System::get_positions(){

    std::vector<Vector3> positions;

    for (int i = 0; i < self_n; ++i) {
        positions.push_back(self_bodies[i].get_position());
    }

    return positions;
};

std::vector<Vector3> System::get_velocities(){

    std::vector<Vector3> velocities;

    for (int i = 0; i < self_n; ++i) {
        velocities.push_back(self_bodies[i].get_velocity());
    }

    return velocities;
};

Vector3 System::acceleration(Body body1, Body body2){
    // Function to calulate the vector force on body2 from
    // body 1

    Vector3 pos1 = body1.get_position();
    Vector3 pos2 = body2.get_position();

    // If the positions are equal return the zero-vector
    if(pos1 == pos2) {
        return Vector3 (0, 0, 0);
    } else {

        Vector3 r12 = pos2 - pos1;

        double f = -body1.get_GM()/pow(r12.norm(), 3);

        return r12*f ;
    }
}

std::vector<Vector3> System::get_accelerations(){

    // Initialize the matrix that will hold the force vectors
    double accels[self_n][self_n][3];

    std::vector<Vector3> accelerations;

    // Enter the values into the acceleration matrix
    for (int i = 0; i < self_n; ++i) {
        for (int j = 0; j < self_n; ++j){
            Vector3 temp_accel = acceleration(self_bodies[i], self_bodies[j]);
            accels[i][j][0] = temp_accel.x();
            accels[i][j][1] = temp_accel.y();
            accels[i][j][2] = temp_accel.z();
        }
    }

    for (int i = 0; i < self_n; ++i) {
        double a = 0;
        double b = 0;
        double c = 0;

        for (int j = 0; j < self_n; ++j) {
            a += accels[i][j][0];
            b += accels[i][j][1];
            c += accels[i][j][2];
        }

        accelerations.emplace_back(std::vector<double> {a, b, c});
    }

    return accelerations;
}

void System::set_positions(std::vector<Vector3> positions){
    for (int i = 0; i < self_n; ++i) {
        self_bodies[i].set_position(positions[i]);
    }
}

void System::set_velocities(std::vector<Vector3> velocities){
    for (int i = 0; i < self_n; ++i) {
        self_bodies[i].set_velocity(velocities[i]);
    }
}



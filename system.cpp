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


System::System(std::vector<std::string> names, std::vector<Vector3> pos0,
               std::vector<Vector3> vel0, std::vector<double> gms,
               std::vector<double> radii){

    self_n = names.size();

    // Initialize self_n obejects of Body type
    for (int i = 0; i < self_n; ++i){
        self_bodies.emplace_back(Body(names[i], pos0[i], vel0[i], gms[i], radii[i]));
    }

    for (int j = 0; j < self_n; ++j) {
        self_names.push_back(self_bodies[j].get_name());
    }
};
std::vector<Vector3> System::get_positions() const {

    std::vector<Vector3> positions;

    for (int i = 0; i < self_n; ++i) {
        positions.push_back(self_bodies[i].get_position());
    }

    return positions;
};

std::vector<Vector3> System::get_velocities() const {

    std::vector<Vector3> velocities;

    for (int i = 0; i < self_n; ++i) {
        velocities.push_back(self_bodies[i].get_velocity());
    }

    return velocities;
};

Vector3 System::acceleration(const Body& body1, const Body& body2) const {
    // Function to calulate the vector force on body2 from
    // body 1

    Vector3 pos1 = body1.get_position();
    Vector3 pos2 = body2.get_position();

    // If the positions are equal return the zero-vector
    if (pos1 == pos2) {
        return Vector3 (0, 0, 0);
    }

    Vector3 r12 = pos2 - pos1;

    return (-1*body1.get_GM()/(std::pow(r12.norm(), 3))) * r12 ;
}

std::vector<Vector3> System::get_accelerations() const {

    std::vector<Vector3> accelerations {};

    for (int i = 0; i < self_n; ++i) {
        Vector3 temp_accel (0, 0, 0);

        for (int j = 0; j < self_n; ++j) {
            temp_accel += acceleration(self_bodies[j], self_bodies[i]);
        }

        accelerations.emplace_back(temp_accel);
    }

    return accelerations;
}

double System::get_total_kinetic_energy() const {
    double total_kinetic = 0;

    for (int i = 0; i < self_n; ++i) {
        total_kinetic += self_bodies[i].get_kinetic_energy();
    }

    return total_kinetic;
}


void System::set_positions(const std::vector<Vector3>& positions){
    for (int i = 0; i < self_n; ++i) {
        self_bodies[i].set_position(positions[i]);
    }
}

void System::set_velocities(const std::vector<Vector3>& velocities){
    for (int i = 0; i < self_n; ++i) {
        self_bodies[i].set_velocity(velocities[i]);
    }
}

Vector3 System::get_total_momentum() const {
    Vector3 total_momentum (0, 0, 0);

    for (int i = 0; i < self_n; ++i) {
        total_momentum += self_bodies[i].get_momentum();
    }

    return total_momentum;
}

Vector3 System::get_total_angular_momentum() const {
    Vector3 total_ang_momentum (0, 0, 0);

    for (int i = 0; i < self_n; ++i) {
        total_ang_momentum += self_bodies[i].get_angular_momentum();
    }

    return total_ang_momentum;
}



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


void System::set_param(std::vector<std::string> names, std::vector<Vector3> pos0,
               std::vector<Vector3> vel0, std::vector<double> gms,
               std::vector<double> radii, std::vector<double> j2s){

    self_n = names.size();

    // Initialize self_n obejects of Body type
    for (int i = 0; i < self_n; ++i){
        self_bodies.emplace_back(Body(names[i], pos0[i], vel0[i], gms[i], radii[i], j2s[i]));
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

Vector3 System::nodal_correction(const Body &body1, const Body &body2) const {
    // Function to calulate the vector force on body2 from
    // body 1

    Vector3 pos1 = body1.get_position();
    Vector3 pos2 = body2.get_position();

    // If the positions are equal return the zero-vector
    if (pos1 == pos2) {
        return Vector3 (0, 0, 0);
    }

    Vector3 r12 = pos2 - pos1;

    double R2 = std::pow(body1.get_radius(), 2);
    double z2 = std::pow(r12.z(), 2);
    double r3 = std::pow(r12.norm(), 3);
    double r2 = r12.norm2();
    double r5 = std::pow(r12.norm(), 5);

    double a1 = -body1.get_GM()*r12.x()/r3 - ( 3 * body1.get_j2() * body1.get_GM()* R2)/(2*r5) * r12.x() * (1-5*z2/r2);
    double a2 = -body1.get_GM()*r12.y()/r3 - ( 3 * body1.get_j2() * body1.get_GM()* R2)/(2*r5) * r12.y() * (1-5*z2/r2);
    double a3 = -body1.get_GM()*r12.z()/r3 - ( 3 * body1.get_j2() * body1.get_GM()* R2)/(2*r5) * r12.z() * (3-5*z2/r2);

    return Vector3 (a1, a2, a3);
}


std::vector<Vector3> System::get_accelerations() const {

    std::vector<Vector3> accelerations {};

    for (auto body1 : self_bodies) {
        Vector3 temp_accel (0, 0, 0);

        for (auto body2 : self_bodies) {

            if (body2.get_j2() != 0) {
                temp_accel += nodal_correction(body2, body1);
            } else { temp_accel += acceleration(body2, body1); }
        }

        accelerations.emplace_back(temp_accel);
    }

    return accelerations;
}

double System::get_total_kinetic_energy() const {
    double total_kinetic = 0;

    for (auto body : self_bodies) {
        total_kinetic += body.get_kinetic_energy();
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

    for (auto body : self_bodies) {
        total_momentum += body.get_momentum();
    }

    return total_momentum;
}

Vector3 System::get_total_angular_momentum() const {
    Vector3 total_ang_momentum (0, 0, 0);

    for (auto body : self_bodies) {
        total_ang_momentum += body.get_angular_momentum();
    }

    return total_ang_momentum;
}

double System::potential_energy(Body body1, Body body2) const {
    // Gravitational potential of body 2 with respect to body 1.

    if (body1.get_position() == body2.get_position()){
        return 0;
    }

    double dist = (body2.get_position()-body1.get_position()).norm();

    return -body1.get_GM()*body2.get_mass()/dist;
}

// TODO Need to modify the energy calculations to they take the nodal correction into account.

double System::get_total_potential_energy() const {

    double total_potential_energy = 0;

    for (auto body1 : self_bodies) {
        for (auto body2 : self_bodies) {
            total_potential_energy += potential_energy(body1, body2);
        }
    }

    return total_potential_energy/2;
}

double System::get_total_mechanical_energy() const {
    return get_total_kinetic_energy() + get_total_potential_energy();
}



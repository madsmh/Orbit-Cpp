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

    Vector3 acceleration(const Body& body1, const Body& body2) const;

    const double self_AU = 149597870700.0;
    const double self_day = 86400.0;

    double potential_energy(Body body1, Body body2) const;

    double get_total_potential_energy() const;

    double get_total_kinetic_energy() const;

public:
    void set_param(std::vector<std::string> names, std::vector<Vector3> pos0,
           std::vector<Vector3> vel0, std::vector<double> gms, std::vector<double> radii);

    std::vector<Vector3> get_positions() const;

    std::vector<Vector3> get_velocities() const;

    std::vector<Vector3> get_accelerations() const;

    Vector3 get_total_momentum() const;

    Vector3 get_total_angular_momentum () const;

    void set_positions(const std::vector<Vector3>& positions);

    void set_velocities(const std::vector<Vector3>& velocities);

    long get_number_of_bodies() const{ return self_n; }

    double get_total_mechanical_energy () const;

    std::vector<Vector3> convert_pos_to_AU(std::vector<Vector3> pos);
    std::vector<Vector3> convert_vel_to_AU_per_day(std::vector<Vector3> vel);
};


#endif //ORBIT3D_SYSTEM_H

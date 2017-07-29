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
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */

#ifndef ORBIT3D_PLANETDATA_H
#define ORBIT3D_PLANETDATA_H

#include <vector>
#include "vector3.h"

struct HorizonsFile {
    double x;
    double y;
    double z;
    double vx;
    double vy;
    double vz;
};

class PlanetData {
    std::vector<std::string> self_planet_names;
    std::vector<std::vector<Vector3> > self_positions;
    std::vector<std::vector<Vector3> > self_velocities;

    void structs_to_arrays();

    std::vector<HorizonsFile> horizons_to_structs(std::string planet);
public:
    explicit PlanetData(std::vector<std::string> &names);

    std::vector<Vector3> get_body_positions(int body);

    std::vector<Vector3> get_body_velocities(int body);

    std::vector<Vector3> get_starting_positions();

    std::vector<Vector3> get_starting_velocities();

};

#endif //ORBIT3D_PLANETDATA_H

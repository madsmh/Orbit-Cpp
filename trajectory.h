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


#ifndef ORBIT3D_TRAJECTORY_H
#define ORBIT3D_TRAJECTORY_H

#include "vector3.h"
#include <vector>

class Trajectory{

    long self_n_rows;
    long self_n_trajectories;
    std::vector<std::vector<Vector3>> self_positions;
    std::vector<std::vector<Vector3>>  self_velocities;

public:
    Trajectory(long n_trajectories, long n_rows);

    void set_position(std::vector<Vector3> pos, std::vector<Vector3> vel);

    std::vector<Vector3> get_trajectory_positions(int body);

    std::vector<Vector3> get_trajectory_velocities(int body);

    std::vector<Vector3 > get_positions_at_index(int index);

    std::vector<Vector3 > get_velocities_at_index(int index);

    long get_number_of_rows(){ return self_n_rows; }
};


#endif //ORBIT3D_TRAJECTORY_H

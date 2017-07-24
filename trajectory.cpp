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


#include "trajectory.h"

Trajectory::Trajectory(long n_trajectories, long n_rows) {

    self_n_trajectories = n_trajectories;

    for (unsigned int i = 0; i < self_n_trajectories; ++i) {
        self_positions.emplace_back(std::vector<Vector3> {});
        self_velocities.emplace_back(std::vector<Vector3> {});
    }
    self_n_rows = n_rows;
}

void Trajectory::set_position(std::vector<Vector3> pos, std::vector<Vector3> vel){
    for (unsigned int i = 0; i< self_n_trajectories ; ++i) {
        self_positions[i].emplace_back(pos[i]);
        self_velocities[i].emplace_back(vel[i]);
    }
}

std::vector<Vector3> Trajectory::get_trajectory_positions(int body){
    std::cout << self_positions[body].size() << "\n";
    return self_positions[body];
}

std::vector<Vector3> Trajectory::get_trajectory_velocities(int body) {
    return self_velocities[body];
}
std::vector<Vector3 > Trajectory::get_positions_at_index(int index){

    std::vector<Vector3 > positions;

    for (int j = 0; j < self_n_trajectories; ++j) {
        positions.emplace_back(self_positions[j][index]);
    }

    return positions;
}

std::vector<Vector3> Trajectory::get_velocities_at_index(int index) {

    std::vector<Vector3> velocities;

    for (int j = 0; j < self_n_trajectories; ++j) {
        velocities.emplace_back(self_velocities[j][index]);
    }

    return velocities;
}


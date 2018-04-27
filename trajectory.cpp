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

void Trajectory::setup(long n_trajectories, std::vector<std::string> names) {

    self_n_trajectories = n_trajectories;

    self_positions.clear();
    self_velocities.clear();

    for (unsigned int i = 0; i < self_n_trajectories; ++i) {
        // Create vectors for coordinates, position and velocity
        self_positions.emplace_back(std::vector<Vector3>{});
        self_velocities.emplace_back(std::vector<Vector3>{});

        // Create and open ofstreams, position and velocity
        self_pos_streams.emplace_back(new std::ofstream);
        self_pos_streams[i].open(self_trajectory_dir + "pos/" + names[i] + ".csv");

        self_vel_streams.emplace_back(new std::ofstream);
        self_vel_streams[i].open(self_trajectory_dir + "vel/" + names[i] + ".csv");
    }
}

void Trajectory::set_position(const std::vector<Vector3>& pos, const std::vector<Vector3>& vel){
    for (unsigned int i = 0; i< self_n_trajectories ; ++i) {
        self_positions[i].emplace_back(pos[i]);
        self_velocities[i].emplace_back(vel[i]);
    }
}

std::vector<Vector3> Trajectory::get_trajectory_positions(int body) const {
    return self_positions[body];
}

std::vector<Vector3> Trajectory::get_trajectory_velocities(int body)  const {
    return self_velocities[body];
}

std::vector<Vector3> Trajectory::get_positions_at_index(int index) const {

    std::vector<Vector3 > positions;

    for (int j = 0; j < self_n_trajectories; ++j) {
        positions.emplace_back(self_positions[j][index]);
    }

    return positions;
}

std::vector<Vector3> Trajectory::get_velocities_at_index(int index)  const {

    std::vector<Vector3> velocities;

    for (int j = 0; j < self_n_trajectories; ++j) {
        velocities.emplace_back(self_velocities[j][index]);
    }

    return velocities;
}

void Trajectory::save_to_csv() {
    for (int j = 0; j < self_n_trajectories; ++j) {
        for (int k = 0; k < self_positions[j].size(); ++k) {
            self_pos_streams[j] << self_positions[j][k].x()
                                << ", "
                                << self_positions[j][k].y()
                                << ", "
                                << self_positions[j][k].z()
                                << std::endl;
            self_vel_streams[j] << self_velocities[j][k].x()
                                << ", "
                                << self_velocities[j][k].y()
                                << ", "
                                << self_velocities[j][k].z()
                                << std::endl;
        }
        
    }
}

void Trajectory::close_streams() {
    for (int j = 0; j < self_n_trajectories; ++j) {
        self_pos_streams[j].close();
        self_vel_streams[j].close();
    }

}

void Trajectory::clear_coordinates() {
    for (int j = 0; j < self_n_trajectories; ++j) {
        self_positions[j].clear();
        self_positions[j].clear();
    }

}



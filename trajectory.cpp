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

void Trajectory::save_trajectory_positions(int tra, std::string name, double start_time, double dt) {

    std::string path = "trajectory/" + name + ".csv";
    std::ofstream myfile (path);

    std::vector<Vector3> current_trajectory = self_positions[tra];

    std::cout << "Writing data for " << name << "." << std::endl;

    if (myfile.good()) {

        for (int i = 0; i < self_n_rows; ++i) {
            std::string t = boost::lexical_cast<std::string>(start_time + (double) i * dt);
            std::string x_coord = boost::lexical_cast<std::string>(current_trajectory[i].x());
            std::string y_coord = boost::lexical_cast<std::string>(current_trajectory[i].y());
            std::string z_coord = boost::lexical_cast<std::string>(current_trajectory[i].z());
            myfile << t << "," << x_coord << "," << y_coord << "," << z_coord << std::endl;
        }
    } else {
        std::cout << "Error opening file." << std::endl;
    }

    myfile.close();
    std::cout << "Finshed writing data for " << name << "." << std::endl;
}



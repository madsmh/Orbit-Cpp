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

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
#include <iomanip>
#include "trajectory.h"
#include "vector3.h"

void Trajectory::setup(long n_trajectories, std::vector<std::string> names) {

    self_n_trajectories = n_trajectories;

    self_positions.clear();
    self_velocities.clear();

    self_names = std::move(names);

    for (int j = 0; j < self_n_trajectories; ++j) {
        self_positions.emplace_back(std::vector<Vector3> {});
        self_velocities.emplace_back(std::vector<Vector3> {});
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

void Trajectory::save_to_file() {

    for (int j = 0; j < self_n_trajectories; ++j) {
        for (int k = 0; k < self_positions[j].size(); ++k) {
            self_pos_ofstreams[j] << std::scientific
                                  << std::setprecision(16)
                                  << self_positions[j][k].x()
                                  << ", "
                                  << self_positions[j][k].y()
                                  << ", "
                                  << self_positions[j][k].z()
                                  << std::endl;

            self_vel_ofstreams[j] << std::scientific
                                  << std::setprecision(16)
                                  << self_velocities[j][k].x()
                                  << ", "
                                  << self_velocities[j][k].y()
                                  << ", "
                                  << self_velocities[j][k].z()
                                  << std::endl;
        }
    }
}

void Trajectory::close_ofstreams() {
    for (int j = 0; j < self_n_trajectories; ++j) {
        self_pos_ofstreams[j].close();
        self_vel_ofstreams[j].close();
    }
}

void Trajectory::clear_coordinates() {
    for (int j = 0; j < self_n_trajectories; ++j) {
        self_positions[j].clear();
        self_positions[j].clear();
    }
}

void Trajectory::read_from_csv(int skip) {

    clear_coordinates();

    std::ifstream filereader_pos;
    std::ifstream filereader_vel;
    std::string buffer_pos;
    std::string buffer_vel;

    typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;

    for (int j = 0; j < self_n_trajectories; ++j) {

        std::string path_pos = self_pos_dir + self_names[j] + self_file_extension;
        boost::algorithm::to_lower(path_pos);
        boost::replace_all(path_pos, " ", "_");

        std::string path_vel = self_vel_dir + self_names[j] + self_file_extension;
        boost::algorithm::to_lower(path_vel);
        boost::replace_all(path_vel, " ", "_");


        filereader_pos.open(path_pos);
        filereader_vel.open(path_vel);

        long counter = 0;

        if (filereader_pos.is_open() and filereader_vel.is_open()) {

            while (std::getline(filereader_pos, buffer_pos)) {

                if (((counter+1) % skip !=0) and counter != 0){
                    ++counter;
                    continue;
                }

                tokenizer tok(buffer_pos);

                auto tok_it = tok.begin();

                double x_buffer = std::stod(*tok_it);

                ++tok_it;

                double y_buffer = std::stod(*tok_it);

                ++tok_it;

                double z_buffer = std::stod(*tok_it);

                self_positions[j].push_back(Vector3(x_buffer, y_buffer, z_buffer));

                ++counter;
            }

            counter = 0;

            while (std::getline(filereader_vel, buffer_vel)) {

                if (((counter+1) % skip !=0) and counter != 0){
                    ++counter;
                    continue;
                }

                tokenizer tok(buffer_vel);

                auto tok_it = tok.begin();

                double vx_buffer = std::stod(*tok_it);

                ++tok_it;

                double vy_buffer = std::stod(*tok_it);

                ++tok_it;

                double vz_buffer = std::stod(*tok_it);

                self_velocities[j].push_back(Vector3(vx_buffer, vy_buffer, vz_buffer));

                ++counter;
            }

        } else {
            std::cout << "Error opening file(s) for " << self_names[j] << std::endl;
        }

        filereader_pos.close();
        filereader_vel.close();
    }

}


void Trajectory::open_ofstreams() {
    for (unsigned int i = 0; i < self_n_trajectories; ++i) {
        // Create and open ofstreams, position and velocity

        std::string path_pos = self_pos_dir + self_names[i] + self_file_extension;
        boost::algorithm::to_lower(path_pos);
        boost::replace_all(path_pos, " ", "_");

        std::string path_vel = self_vel_dir + self_names[i] + self_file_extension;
        boost::algorithm::to_lower(path_vel);
        boost::replace_all(path_vel, " ", "_");

        std::ofstream out1;

        self_pos_ofstreams.push_back(std::move(out1));
        self_pos_ofstreams[i].open(path_pos);

        std::ofstream out2;

        self_vel_ofstreams.push_back(std::move(out2));
        self_vel_ofstreams[i].open(path_vel);
    }
}
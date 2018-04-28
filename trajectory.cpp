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


#include <boost/token_functions.hpp>
#include <boost/tokenizer.hpp>
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

void Trajectory::read_from_csv(int skip) {

    clear_coordinates();

    std::ifstream filereader_pos;
    std::ifstream filereader_vel;
    std::string buffer_pos;
    std::string buffer_vel;

    typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;

    for (int j = 0; j < self_n_trajectories; ++j) {

        filereader_pos.open(self_pos_dir + self_names[j] + self_file_extension);
        filereader_vel.open(self_vel_dir + self_names[j] + self_file_extension);

        // TODO Implement line-spik feature.

        if (filereader_pos.is_open() and filereader_vel.is_open()) {
            while (!filereader_pos.eof() or !filereader_vel.eof()) {

                if(!filereader_pos.eof()){
                    std::getline(filereader_pos, buffer_pos);
                }

                if (!filereader_vel.eof()) {
                    std::getline(filereader_vel, buffer_vel);
                }

                if (!buffer_pos.empty() and !filereader_pos.eof()) {
                    tokenizer tok(buffer_pos);

                    auto tok_it = tok.begin();

                    double x_buffer = std::stod(*tok_it);

                    ++tok_it;

                    double y_buffer = std::stod(*tok_it);

                    ++tok_it;

                    double z_buffer = std::stod(*tok_it);

                    self_positions[j].push_back(Vector3(x_buffer, y_buffer, z_buffer));
                }

                if (!buffer_vel.empty() and !filereader_vel.eof()) {
                    tokenizer tok(buffer_vel);

                    auto tok_it = tok.begin();

                    double vx_buffer = std::stod(*tok_it);

                    ++tok_it;

                    double vy_buffer = std::stod(*tok_it);

                    ++tok_it;

                    double vz_buffer = std::stod(*tok_it);

                    self_velocities[j].push_back(Vector3(vx_buffer, vy_buffer, vz_buffer));
                }

            }

            filereader_pos.close();
            filereader_vel.close();

        }

    }

}

void Trajectory::open_streams() {
    for (unsigned int i = 0; i < self_n_trajectories; ++i) {
        // Create and open ofstreams, position and velocity

        std::ofstream out1;

        self_pos_streams.push_back(std::move(out1));
        self_pos_streams[i].open(self_pos_dir + self_names[i] + self_file_extension);

        std::ofstream out2;

        self_vel_streams.push_back(std::move(out2));
        self_vel_streams[i].open(self_vel_dir + self_names[i] + self_file_extension);
    }
}



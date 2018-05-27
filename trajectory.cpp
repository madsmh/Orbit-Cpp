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

    m_n_trajectories = n_trajectories;

    m_positions.clear();
    m_velocities.clear();

    m_names = std::move(names);

    for (int j = 0; j < m_n_trajectories; ++j) {
        m_positions.emplace_back(std::vector<Vector3> {});
        m_velocities.emplace_back(std::vector<Vector3> {});
    }

}

void Trajectory::set_position(const std::vector<Vector3>& pos, const std::vector<Vector3>& vel){
    for (unsigned int i = 0; i< m_n_trajectories ; ++i) {
        m_positions[i].emplace_back(pos[i]);
        m_velocities[i].emplace_back(vel[i]);
    }
}

std::vector<Vector3> Trajectory::get_trajectory_positions(int body) const {
    return m_positions[body];
}

std::vector<Vector3> Trajectory::get_trajectory_velocities(int body)  const {
    return m_velocities[body];
}

std::vector<Vector3> Trajectory::get_positions_at_index(int index) const {

    std::vector<Vector3 > positions;

    for (int j = 0; j < m_n_trajectories; ++j) {
        positions.emplace_back(m_positions[j][index]);
    }

    return positions;
}

std::vector<Vector3> Trajectory::get_velocities_at_index(int index)  const {

    std::vector<Vector3> velocities;

    for (int j = 0; j < m_n_trajectories; ++j) {
        velocities.emplace_back(m_velocities[j][index]);
    }

    return velocities;
}

void Trajectory::save_to_file() {

    for (int j = 0; j < m_n_trajectories; ++j) {
        for (int k = 0; k < m_positions[j].size(); ++k) {
            m_pos_ofstreams[j] << std::scientific
                                  << std::setprecision(16)
                                  << m_positions[j][k].x()
                                  << ", "
                                  << m_positions[j][k].y()
                                  << ", "
                                  << m_positions[j][k].z()
                                  << std::endl;

            m_vel_ofstreams[j] << std::scientific
                                  << std::setprecision(16)
                                  << m_velocities[j][k].x()
                                  << ", "
                                  << m_velocities[j][k].y()
                                  << ", "
                                  << m_velocities[j][k].z()
                                  << std::endl;
        }
    }
}

void Trajectory::close_ofstreams() {
    for (int j = 0; j < m_n_trajectories; ++j) {
        m_pos_ofstreams[j].close();
        m_vel_ofstreams[j].close();
    }
}

void Trajectory::clear_coordinates() {
    for (int j = 0; j < m_n_trajectories; ++j) {
        m_positions[j].clear();
        m_positions[j].clear();
    }
}

void Trajectory::read_from_csv(int skip) {

    clear_coordinates();

    std::ifstream filereader_pos;
    std::ifstream filereader_vel;
    std::string buffer_pos;
    std::string buffer_vel;

    std::cout << "Reading files." << std::endl;

    typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;

    for (int j = 0; j < m_n_trajectories; ++j) {

        std::string path_pos = m_pos_dir + m_names[j] + m_file_extension;
        boost::algorithm::to_lower(path_pos);
        boost::replace_all(path_pos, " ", "_");

        std::string path_vel = m_vel_dir + m_names[j] + m_file_extension;
        boost::algorithm::to_lower(path_vel);
        boost::replace_all(path_vel, " ", "_");


        filereader_pos.open(path_pos);
        filereader_vel.open(path_vel);

        long counter = 0;

        if (filereader_pos.is_open() and filereader_vel.is_open()) {

            while (std::getline(filereader_pos, buffer_pos)) {

                if ((counter % skip != 0) and counter != 0){
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

                m_positions[j].push_back(Vector3(x_buffer, y_buffer, z_buffer));

                ++counter;
            }

            counter = 0;

            while (std::getline(filereader_vel, buffer_vel)) {

                if ((counter % skip != 0) and counter != 0){
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

                m_velocities[j].push_back(Vector3(vx_buffer, vy_buffer, vz_buffer));
                ++counter;
            }

        } else {
            std::cout << "Error opening file(s) for " << m_names[j] << std::endl;
        }

        filereader_pos.close();
        filereader_vel.close();
    }

}


void Trajectory::open_ofstreams() {
    for (unsigned int i = 0; i < m_n_trajectories; ++i) {
        // Create and open ofstreams, position and velocity

        std::string path_pos = m_pos_dir + m_names[i] + m_file_extension;
        boost::algorithm::to_lower(path_pos);
        boost::replace_all(path_pos, " ", "_");

        std::string path_vel = m_vel_dir + m_names[i] + m_file_extension;
        boost::algorithm::to_lower(path_vel);
        boost::replace_all(path_vel, " ", "_");

        std::ofstream out1;

        m_pos_ofstreams.push_back(std::move(out1));
        m_pos_ofstreams[i].open(path_pos);

        std::ofstream out2;

        m_vel_ofstreams.push_back(std::move(out2));
        m_vel_ofstreams[i].open(path_vel);
    }
}
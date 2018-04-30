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
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

#include <boost/lexical_cast.hpp>

class Trajectory{

    long m_n_trajectories;

    const std::string m_trajectory_dir = "trajectory/";
    const std::string m_vel_dir = m_trajectory_dir + "vel/";
    const std::string m_pos_dir = m_trajectory_dir + "pos/";
    const std::string m_file_extension = ".csv";

    std::vector<std::vector<Vector3>> m_positions;
    std::vector<std::vector<Vector3>> m_velocities;

    std::vector<std::ofstream> m_pos_ofstreams;
    std::vector<std::ofstream> m_vel_ofstreams;

    std::vector<std::string> m_names;

public:
    void setup(long n_trajectories, std::vector<std::string> names );

    void open_ofstreams();

    void close_ofstreams();

    void set_position(const std::vector<Vector3>& pos, const std::vector<Vector3> &vel);

    void clear_coordinates();

    void save_to_file();

    void read_from_csv(int skip);

    std::vector<Vector3> get_trajectory_positions(int body) const;

    std::vector<Vector3> get_trajectory_velocities(int body) const;

    std::vector<Vector3> get_positions_at_index(int index) const;

    std::vector<Vector3> get_velocities_at_index(int index) const;

    long get_number_of_rows() const { return m_positions[0].size(); }

    long get_number_of_trajectories(){ return m_n_trajectories; }
};


#endif //ORBIT3D_TRAJECTORY_H

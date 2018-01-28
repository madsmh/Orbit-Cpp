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

#include "vector3.h"
#include "system.h"
#include "trajectory.h"
#include "planetdata.h"
#include "propertiesfile.h"
#include "verlet.h"

#include <iostream>
#include <fstream>

Vector3 change_origin(Vector3 new_orgin, Vector3 old_coords){

    return old_coords-new_orgin;

};

void write_table(const std::vector<double > &data, const std::string &file_name){
    std::ofstream file;

    file.open(file_name);

    for (int j = 0; j < data.size(); ++j) {
        file << j << ", " << data[j] << "\n";
    }

    file.close();
}

void compare_with_horizon(Trajectory tra, PlanetData data,
                          std::vector<std::string> const &names,
                          int detail,
                          int days)
{
    int ref_rows = (int) data.get_body_positions(0).size();

    std::cout << "Absolute error for the bodies are" << std::endl;

    for (int j = 0; j < tra.get_number_of_trajectories(); ++j) {
        std::vector<Vector3> sim_vector = tra.get_trajectory_positions(j);
        std::vector<Vector3> ref_vector = data.get_body_positions(j);

        std::vector<double> dists;


        for (int i = 0; i < ref_rows; ++i) {
            double error = (sim_vector[i*detail]-ref_vector[i]).norm();
            dists.emplace_back(error);
        }

        double max_dist = *std::max_element(dists.begin(), dists.end())/1000;
        std::cout << j << " " << names[j] << ": " << max_dist << " km" << std::endl;
    }

    // Make table with error of Europas position with respect to Jupiter as a function
    // of time in days

    auto europa_ref_positions = data.get_body_positions(25);
    auto jupiter_ref_positions = data.get_body_positions(6);

    auto europa_sim_positions = tra.get_trajectory_positions(25);
    auto jupiter_sim_positions = tra.get_trajectory_positions(6);

    std::vector<double> europa_error_table {};

    std::cout << "Calculating Europas error in position with respect to Jupiter." << std::endl;
    for (int k = 0; k < days; ++k) {
        europa_error_table.emplace_back(
                (change_origin(jupiter_sim_positions[k*detail], europa_sim_positions[k*detail])-
                        change_origin(jupiter_ref_positions[k], europa_ref_positions[k])).norm()/1000);
    }

    double max_europa_jupiter_error = *std::max_element(europa_error_table.begin(), europa_error_table.end());

    std::cout << "Maximum error in Europas position with respect to jupiter (km): " <<
              max_europa_jupiter_error << std::endl;

    std::cout << "Writing table." << std::endl;

    write_table(europa_error_table,"europa_error.txt");
}


void help(char *name) {
    std::cout << "Usage: " << name << " <options>" << std::endl;
    std::cout << "-d <detail> : Number of time steps each day (default: 64)." << std::endl;
}

int main(int argc, char **argv) {
    // Adapted from https://doc.qt.io/qt-5/qt3d-basicshapes-cpp-main-cpp.html

    Trajectory trajectory;
    PhysicalProperties physicalProperties;

    physicalProperties.get_data();

    PlanetData planetData(physicalProperties.get_names());

    planetData.read_data();

    System sol;

    sol.set_param(physicalProperties.get_names(),
                  planetData.get_starting_positions(),
                  planetData.get_starting_velocities(),
                  physicalProperties.get_GMs(),
                  physicalProperties.get_radii());


    trajectory.setup(physicalProperties.get_names().size());

    int detail = 100;
    auto days_to_sim = (int) planetData.get_body_positions(0).size();

    Verlet integrator;

    integrator.setup(days_to_sim, detail);

    integrator.run(sol,trajectory);

    compare_with_horizon(trajectory,
                         planetData,
                         physicalProperties.get_names(),
                         detail,
                         days_to_sim);
}


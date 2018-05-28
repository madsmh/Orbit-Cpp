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
#include <iomanip>

Vector3 change_origin(Vector3 new_orgin, Vector3 old_coords){
    return old_coords-new_orgin;
};

double angle(Vector3 vector1, Vector3 vector2){
    double a = (vector1.x()*vector2.x()+vector1.y()*vector2.y()+vector1.z()*vector2.z())/(vector1.norm()*vector2.norm());
    double radians = std::acos(a);
    return radians*180.0/3.141592653589793238463;
}

void write_table(const std::vector<double > &data, const std::string &file_name){
    std::ofstream file;

    file.open(file_name);

    for (double j : data) {
        file << j << std::endl;
    }

    file.close();
}

void write_table(const std::vector<Vector3 > &data, const std::string &file_name){
    std::ofstream file;

    file.open(file_name);

    for (const auto &j : data) {
        file << j.x() << ", " << j.y() << ", " << j.z() << std::endl;
    }

    file.close();
}

void diagnostics(Trajectory &tra,
                 PlanetData data,
                 std::vector<std::string> const &names,
                 int detail) {

    tra.read_from_csv(detail);

    int ref_rows = (int) data.get_body_positions(0).size();

    std::cout << "Absolute error for the bodies are" << std::endl;
    std::cout << "Number of rows (positions): " << tra.get_trajectory_positions(0).size() << std::endl;
    std::cout << "Number of rows (velocity): " << tra.get_trajectory_velocities(0).size() << std::endl;
    std::cout << std::endl << "Maximum errors:" << std::endl;
    for (int j = 0; j < tra.get_number_of_trajectories(); ++j) {
        std::vector<Vector3> sim_vector = tra.get_trajectory_positions(j);
        std::vector<Vector3> ref_vector = data.get_body_positions(j);

        std::vector<double> dists;


        for (int i = 0; i < ref_rows; ++i) {
            double error = (sim_vector[i]-ref_vector[i]).norm();
            dists.emplace_back(error);
        }

        double max_dist = *std::max_element(dists.begin(), dists.end())/1000;

        if (j == 1){
            std::cout << std::endl << "Planets (+Pluto): " << std::endl;
        } else if (j == 10){
            std::cout << std::endl << "Earth's moon: " << std::endl;
        } else if (j == 11){
            std::cout << std::endl << "Jovian moons: " << std::endl;
        } else if (j == 17){
            std::cout << std::endl << "Saturnian moons: " << std::endl;
        } else if (j == 32){
            std::cout << std::endl << "Plutonian moons: " << std::endl;
        } else if (j == 36){
            std::cout << std::endl << "Martian moons: " << std::endl;
        } else if (j == 38){
            std::cout << std::endl << "Neptunian moons" << std::endl;
        } else if (j == 39){
            std::cout << std::endl << "Uranian moons" << std::endl;
        } else if (j == 44){
            std::cout << std::endl << "Asteroids" << std::endl;
        }

        std::cout << j << " " << names[j] << ": " << max_dist << " km" << std::endl;
    }

    // Make table with error of Europas position with respect to Jupiter as a function
    // of time in hours

    /*auto target_ref_positions = data.get_body_positions(target);
    auto origin_ref_positions = data.get_body_positions(origin);

    auto target_sim_positions = tra.get_trajectory_positions(target);
    auto origin_sim_positions = tra.get_trajectory_positions(origin);

    auto target_ref_velocities = data.get_body_velocities(target);
    auto origin_ref_velocities = data.get_body_velocities(origin);

    auto target_sim_velocities = tra.get_trajectory_velocities(target);
    auto origin_sim_velocities = tra.get_trajectory_velocities(origin);

    std::vector<double> error_pos_table {};
    std::vector<double> dist_table {};
    std::vector<double> angle_table {};
    std::vector<Vector3> delta_table {};
    std::vector<Vector3> pos_sim_table {};
    std::vector<Vector3> pos_ref_table {};
    std::vector<double> vel_sim_table {};
    std::vector<double> vel_ref_table {};


    std::cout << "Calculating " << names[target] << "s " << "error in position with respect to " <<
              names[origin] << "." << std::endl;
    for (int k = 0; k < hours; ++k) {

        Vector3 new_sim =change_origin(origin_sim_positions[k*detail], target_sim_positions[k*detail]);
        Vector3 new_ref = change_origin(origin_ref_positions[k], target_ref_positions[k]);
        Vector3 delta = new_sim-new_ref;

        Vector3 new_sim_vel = change_origin(origin_sim_velocities[k*detail], target_sim_velocities[k*detail]);
        Vector3 new_ref_vel = change_origin(origin_ref_velocities[k], target_ref_velocities[k]);

        error_pos_table.emplace_back(delta.norm()/1000);
        dist_table.emplace_back(new_sim.norm()/1000);
        angle_table.emplace_back(angle(new_ref, new_sim));
        delta_table.emplace_back(delta);
        pos_ref_table.emplace_back(new_ref);
        pos_sim_table.emplace_back(new_sim);
        vel_ref_table.emplace_back(new_ref_vel.norm());
        vel_sim_table.emplace_back(new_sim_vel.norm());
    }

    double max_error = *std::max_element(error_pos_table.begin(), error_pos_table.end());

    std::cout << "Maximum error in " << names[target] << "s " << "position with respect to " << names[origin] <<
              ": " << max_error << "." << std::endl;
    std::cout << "Initial conditions for " << names[target] << " is" << std::endl <<
              "   Position: " << std::scientific << std::setprecision(15)
              << tra.get_trajectory_positions(target)[0].x()*1e-3 << ", "
              << tra.get_trajectory_positions(target)[0].y()*1e-3 << ", "
              << tra.get_trajectory_positions(target)[0].z()*1e-3 << std::endl;
    std::cout << "   Velocity: " << std::scientific << std::setprecision(15)
                    << tra.get_trajectory_velocities(target)[0].x()*1e-3 << ", "
                    << tra.get_trajectory_velocities(target)[0].y()*1e-3 << ", "
                    << tra.get_trajectory_velocities(target)[0].z()*1e-3 << std::endl;

    std::cout << "Writing table." << std::endl;

    write_table(error_pos_table, "error_pos.csv");
    write_table(dist_table, "dist.csv");
    write_table(angle_table, "angles.csv");
    write_table(delta_table, "delta.csv");
    write_table(pos_ref_table, "pos_ref.csv");
    write_table(pos_sim_table, "pos_sim.csv");
    write_table(vel_ref_table, "ref_velocity.csv");
    write_table(vel_sim_table, "sim_velocity.csv");
*/
}


void help(char *name) {
    std::cout << "Usage: " << name << " <options>" << std::endl;
    std::cout << "-d <detail> : Number of time steps each day (default: 64)." << std::endl;
}

int main(int argc, char **argv) {

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
                  physicalProperties.get_radii(),
                  physicalProperties.get_j2s());


    trajectory.setup(physicalProperties.get_names().size(), physicalProperties.get_names());

    int detail;
    int origin;
    int target;
    auto hours_to_sim = planetData.get_body_positions(0).size();
    std::string sim_choice("y");
    std::string diag_choice("y");

    sim_choose:
    std::cout << "Do you wish to run simulation? (Y/n) ";
    std::cin >> sim_choice;

    std::cout << "Enter number of integration steps per hour: ";
    std::cin >> detail;

    if ( sim_choice == "y" or sim_choice == "Y"){

        Verlet integrator;

        integrator.setup(hours_to_sim, detail);

        integrator.run(sol,trajectory);
    } else if ( sim_choice != "n"){
        goto sim_choose;
    }

    diag_choose:
    std::cout << "Do you wish to run diagnostics? (Y/n) ";
    std::cin >> diag_choice;

    if (diag_choice == "y" or diag_choice == "Y"){

        /* TODO Update diagnostics to use less RAM.
        */
         diagnostics(trajectory,
                   planetData,
                   physicalProperties.get_names(),
                   detail);

    } else if (diag_choice != "n"){
        goto diag_choose;
    }
}


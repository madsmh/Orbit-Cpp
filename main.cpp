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


#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>
#include <getopt.h>
#include <boost/timer.hpp>

#include "vector3.h"
#include "system.h"
#include "trajectory.h"
#include "planetdata.h"
#include "propertiesfile.h"

void compare_with_horizon(Trajectory tra, PlanetData data,
                          std::vector<std::string> names,double detail, long ref_rows){

    for (int j = 0; j < tra.get_number_of_trajectories(); ++j) {
        std::vector<Vector3> sim_vector = tra.get_trajectory_positions(j);
        std::vector<Vector3> ref_vector = data.get_body_positions(j);

        std::vector<double> dists;

        for (int i = 0; i < ref_rows; ++i) {
            double error = (sim_vector[i*detail]-ref_vector[i]).norm();
            dists.emplace_back(error);
        }

        double max_dist = *std::max_element(dists.begin(), dists.end())/1000;
        std::cout << "Error for " << names[j] << " is : " << max_dist << " km" << std::endl;

    }

}

void verlet(System &system, Trajectory &trajectory, double delta){
    boost::timer t;
    long n = trajectory.get_number_of_rows();
    long n_bodies = system.get_number_of_bodies();

    std::vector<double> mechanical_energy;
    std::vector<Vector3> momentum;
    std::vector<Vector3> angular_momentum;

    double delta2 = std::pow(delta, 2);

    std::cout << "Starting integrator." << std::endl;

    for (int i = 0; i < n; ++i) {
        if(i == 0){
            std::vector<Vector3 > x0 = system.get_positions();
            std::vector<Vector3> v0 = system.get_velocities();

            trajectory.set_position(x0, v0);
            system.set_positions(x0);
            system.set_velocities(v0);

            mechanical_energy.emplace_back(system.get_total_mechanical_energy());
        }
        else {

            std::vector<Vector3> x0 = trajectory.get_positions_at_index(i-1);
            std::vector<Vector3> v0 = trajectory.get_velocities_at_index(i-1);

            std::vector<Vector3> a0 = system.get_accelerations();

            std::vector<Vector3> x1;

            for (long j = 0; j < n_bodies; ++j) {
                x1.emplace_back(x0[j] + v0[j] * delta + delta2 * 0.5 * a0[j]);
            }

            system.set_positions(x1);

            std::vector<Vector3 > a1 = system.get_accelerations();

            std::vector<Vector3 > v1;

            for (long k = 0; k < n_bodies; ++k) {
                v1.emplace_back(v0[k]+ 0.5 * delta *( a0[k]+a1[k]) );
            }

            system.set_velocities(v1);

            mechanical_energy.emplace_back(system.get_total_mechanical_energy());

            trajectory.set_position(x1, v1);
        }
    }
    std::cout << "Integration finished in " << t.elapsed() << " seconds."<< std::endl;
    double max_mech_energy = *std::max_element(mechanical_energy.begin(), mechanical_energy.end());
    double min_mech_energy = *std::min_element(mechanical_energy.begin(), mechanical_energy.end());
    double diff_mech_energy =  std::abs(max_mech_energy-min_mech_energy);
    double rel_mech_energy = diff_mech_energy/std::abs(max_mech_energy);

    std::cout << "Mechanical energy, relative difference "<< rel_mech_energy << std::endl;

}

void help(char *name) {
    std::cout << "Usage: " << name << " <options>" << std::endl;
    std::cout << "-d <detail> : Number of time steps each day (default: 64)." << std::endl;
}

int main(int argc, char *argv[]) {

    double detail = 64.0;
    long bodies = 0;
    while (1)
    {
        char opt = getopt(argc, argv, "d:b:");
        if (opt == -1)
            break;
        switch (opt)
        {
            case 'd' : detail = atof(optarg); break;
            case 'b' : bodies = atol(optarg); break;
            default  : help(argv[0]); exit(-1); break;
        }
    }

    double dt = 86400.0/detail;
    long rows = 1131*detail;


    PhysicalProperties prop(bodies);
    PlanetData horizons (prop.get_names());


    System sol (prop.get_names(),
                horizons.get_starting_positions(),
                horizons.get_starting_velocities(),
                prop.get_GMs(),
                prop.get_radii());


    long n_bodies = prop.get_names().size();

    Trajectory tra (n_bodies,rows);

    std::cout << std::endl;
    std::cout << "detail = " << detail << std::endl;

    verlet(sol, tra, dt);

    compare_with_horizon(tra, horizons, prop.get_names(), detail, 1131);

    // Saving the trajectories to file
    for (int j = 0; j < n_bodies; ++j) {
        tra.save_trajectory_positions(j, prop.get_names()[j], 0.0, dt);
    }
    return 0;
}


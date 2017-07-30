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

#include "vector3.h"
#include "system.h"
#include "trajectory.h"
#include "planetdata.h"
#include "propertiesfile.h"


void verlet(System &system, Trajectory &trajectory, double delta){

    long n = trajectory.get_number_of_rows();
    long n_bodies = system.get_number_of_bodies();

    double delta2 = std::pow(delta, 2);

    std::cout << "Starting integrator." << std::endl;

    for (int i = 0; i < n; ++i) {
        if(i == 0){
            std::vector<Vector3 > x0 = system.get_positions();
            std::vector<Vector3> v0 = system.get_velocities();

            trajectory.set_position(x0, v0);
            system.set_positions(x0);
            system.set_velocities(v0);

            std::cout << "Total momentum         : " << system.get_total_momentum() << "\n";
            std::cout << "Total angular momentum : " << system.get_total_angular_momentum() << "\n";
            std::cout << "Total mechanical energy: " << system.get_total_mechanical_energy() << "\n";
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

            std::cout << "Total momentum         : " << system.get_total_momentum() << "\n";
            std::cout << "Total angular momentum : " << system.get_total_angular_momentum() << "\n";
            std::cout << "Total mechanical energy: " << system.get_total_mechanical_energy() << "\n";

            trajectory.set_position(x1, v1);
        }
    }
    std::cout << "Integration finished." << std::endl;
}

void help(char *name) {
    std::cout << "Usage: " << name << " <options>" << std::endl;
    std::cout << "-d <detail> : Number of time steps each day (default: 64)." << std::endl;
}

int main(int argc, char *argv[]) {

    auto detail = 64;
    long bodies = 0;
    while (1)
    {
        char opt = getopt(argc, argv, "d:b:");
        if (opt == -1)
            break;
        switch (opt)
        {
            case 'd' : detail = atol(optarg); break;
            case 'b' : bodies = atol(optarg); break;
            default  : help(argv[0]); exit(-1); break;
        }
    }

    double dt = 86400/detail;
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

    std::vector<double> dists;

    // Get the earths trajectory
    std::vector<Vector3> earth_sim = tra.get_trajectory_positions(3);
    std::vector<Vector3> earth_ref = horizons.get_body_positions(3);
    std::vector<Vector3> sun_sim = tra.get_trajectory_positions(0);

    std::vector<Vector3> earth_corrected {};

    for (int j = 0; j < rows; ++j) {
        earth_corrected.push_back(earth_sim[j]-sun_sim[j]);
    }


    for (int i = 0; i < 1131; ++i) {
        Vector3 error = earth_corrected[i*detail]-earth_ref[i];

        double error_norm = error.norm();
        dists.emplace_back(error_norm);
    }

    std::cout << "Calculated the errors. \n";

     double max_dist = *std::max_element(dists.begin(), dists.end())/1000;
    std::cout << max_dist << std::endl;

    return 0;
}


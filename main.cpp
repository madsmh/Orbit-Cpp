#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>
#include <fstream>
#include <algorithm>

#include "vector3.h"
#include "system.h"
#include "trajectory.h"
#include "planetdata.h"
#include "propertiesfile.h"


void verlet(System system, Trajectory trajectory, double delta){

    long n = trajectory.get_number_of_rows();
    long n_bodies = system.get_number_of_bodies();

    double delta2 = pow(delta, 2);

    for (int i = 0; i < n; ++i) {
        if(i == 0){
            std::vector<Vector3 > x0 = system.get_positions();
            std::vector<Vector3> v0 = system.get_velocities();

            trajectory.set_position(x0, v0);
        }
        else {

            std::vector<Vector3> x0 = trajectory.get_positions_at_index(i-1);
            std::cout << "Starting round " << i << std::endl;
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
                v1.emplace_back(v0[k]+ 0.5* (a0[k]+a1[k]) * delta);
            }

            system.set_velocities(v1);

            trajectory.set_position(x1, v1);
            std::cout << " Round " << i << std::endl;

        }
    }
}


int main() {

    int detail = 64;
    double dt = 86400/detail;
    long rows = 1131*detail;


    PhysicalProperties prop;
    PlanetData planet_data (prop.get_names());


    System sol (prop.get_names(), planet_data.get_starting_positions(), planet_data.get_starting_velocities(),
                prop.get_GMs(), prop.get_radii());


    long n_bodies = prop.get_names().size();

    Trajectory tra (n_bodies,rows);


    verlet(sol, tra, dt);

    std::vector<double> dists;

    std::vector<Vector3 > earth = tra.get_trajectory_velocities(3);

    std::vector<Vector3 > earth_ref = planet_data.get_body_positions(3);

    for (int i = 0; i < rows; i += detail) {
        Vector3 error = earth[i]-earth_ref[i];
        dists.emplace_back(error.norm() );

    }
     double max_dist = *max_element(dists.begin(), dists.end());
    std::cout << max_dist << std::endl;

    return 0;
}


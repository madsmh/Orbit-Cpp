//
// Created by mads on 14-08-17.
//
#include <boost/timer.hpp>
#include "trajectory.h"
#include "verlet.h"
#include "system.h"

void Verlet::setup(int days, int detail) {

    m_rows = days * detail;
    m_delta = ((double) days)/((double) detail);

    /*std::cout << "Starting integrator." << std::endl;


    std::cout << "Integration finished in " << t.elapsed() << " seconds."<< std::endl;
    double max_mech_energy = *std::max_element(mechanical_energy.begin(), mechanical_energy.end());
    double min_mech_energy = *std::min_element(mechanical_energy.begin(), mechanical_energy.end());
    double diff_mech_energy =  std::abs(max_mech_energy-min_mech_energy);
    double rel_mech_energy = diff_mech_energy/std::abs(max_mech_energy);

    std::cout << "Mechanical energy, relative difference "<< rel_mech_energy << std::endl;
*/
}


void Verlet::run(System &sys, Trajectory &tra) {

    std::vector<double> mechanical_energy;
    std::vector<Vector3> momentum;
    std::vector<Vector3> angular_momentum;

    double delta = m_delta;
    int n_bodies = (int) sys.get_number_of_bodies();
    double delta2 = std::pow(delta, 2);


    for (int i = 0; i < m_rows; ++i) {
        if(i == 0){
            std::vector<Vector3 > x0 = sys.get_positions();
            std::vector<Vector3> v0 = sys.get_velocities();

            tra.set_position(x0, v0);
            sys.set_positions(x0);
            sys.set_velocities(v0);

            //mechanical_energy.emplace_back(system.get_total_mechanical_energy());

            emit progress(i);
        }
        else {

            std::vector<Vector3> x0 = tra.get_positions_at_index(i-1);
            std::vector<Vector3> v0 = tra.get_velocities_at_index(i-1);

            std::vector<Vector3> a0 = sys.get_accelerations();

            std::vector<Vector3> x1;

            for (long j = 0; j < n_bodies; ++j) {
                x1.emplace_back(x0[j] + v0[j] * delta + delta2 * 0.5 * a0[j]);
            }

            sys.set_positions(x1);

            std::vector<Vector3 > a1 = sys.get_accelerations();

            std::vector<Vector3 > v1;

            for (long k = 0; k < n_bodies; ++k) {
                v1.emplace_back(v0[k]+ 0.5 * delta *( a0[k]+a1[k]) );
            }

            sys.set_velocities(v1);

            mechanical_energy.emplace_back(sys.get_total_mechanical_energy());

            tra.set_position(x1, v1);

            emit progress(i);
        }
    }

    emit success(true);
}

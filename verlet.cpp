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

#include "trajectory.h"
#include "verlet.h"
#include "system.h"
#include <boost/progress.hpp>

void Verlet::setup(int days, int detail) {

    m_rows = days * detail;
    m_delta = ((double) 3600)/((double) detail);

    /*


    std::cout << "Integration finished in " << t.elapsed() << " seconds."<< std::endl;
    double max_mech_energy = *std::max_element(mechanical_energy.begin(), mechanical_energy.end());
    double min_mech_energy = *std::min_element(mechanical_energy.begin(), mechanical_energy.end());
    double diff_mech_energy =  std::abs(max_mech_energy-min_mech_energy);
    double rel_mech_energy = diff_mech_energy/std::abs(max_mech_energy);

    std::cout << "Mechanical energy, relative difference "<< rel_mech_energy << std::endl;
*/
}


void Verlet::run(System &sys, Trajectory &tra) {
    std::cout << "Starting integrator." << std::endl;


    boost::progress_display progress(static_cast<unsigned long>(m_rows));


    auto n_bodies = (int) sys.get_number_of_bodies();
    double delta2 = std::pow(m_delta, 2);

    tra.open_streams();

    for (int i = 0; i < m_rows; ++i) {
        if(i == 0){

            std::vector<Vector3 > x0 = sys.get_positions();
            std::vector<Vector3> v0 = sys.get_velocities();

            tra.set_position(x0, v0);
            tra.save_to_csv();
            tra.clear_coordinates();

            ++progress;
        }

        else {

            std::vector<Vector3> x0 = sys.get_positions();
            std::vector<Vector3> v0 = sys.get_velocities();

            std::vector<Vector3> a0 = sys.get_accelerations();

            std::vector<Vector3> x1;

            for (long j = 0; j < n_bodies; ++j) {
                x1.emplace_back( x0[j] + v0[j] * m_delta + delta2 * 0.5 * a0[j] );
            }

            sys.set_positions(x1);

            std::vector<Vector3 > a1 = sys.get_accelerations();

            std::vector<Vector3 > v1;

            for (long k = 0; k < n_bodies; ++k) {
                v1.emplace_back(v0[k] + 0.5 * m_delta * ( a0[k] + a1[k] ) );
            }

            sys.set_positions(x1);
            sys.set_velocities(v1);

            tra.set_position(x1, v1);
            tra.save_to_csv();
            tra.clear_coordinates();

            ++progress;
        }
    }

    tra.close_streams();
    std::cout << "Integration finished." << std::endl;
}

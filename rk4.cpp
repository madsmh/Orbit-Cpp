/* Copyright (c) 2018 By the Authors
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

#include "rk4.h"
#include <boost/progress.hpp>

void RK4::run(System &system, Trajectory &trajectory) {

    long n = system.get_number_of_bodies();

    std::cout << "Starting integrator." << std::endl;
    std::cout << "Timestep: " << m_timestep << std::endl;

    boost::progress_display progress(static_cast<unsigned long>(m_rows));

    for (int j = 0; j < m_rows; ++j) {
        if (j==0){
            // Initial setup.

            std::vector<Vector3> r = system.get_positions();
            std::vector<Vector3> v = system.get_velocities();

            trajectory.set_position(r, v);

            ++progress;

        } else {
            std::vector <Vector3> r = trajectory.get_positions_at_index(j - 1);
            std::vector <Vector3> v = trajectory.get_velocities_at_index(j - 1);

            std::vector <Vector3> kv1 = system.get_accelerations();
            std::vector <Vector3> kv2{};
            std::vector <Vector3> kv3{};
            std::vector <Vector3> kv4{};

            std::vector <Vector3> kr1 = v;
            std::vector <Vector3> kr2{};
            std::vector <Vector3> kr3{};
            std::vector <Vector3> kr4{};

            std::vector <Vector3> r2{};
            for (int k = 0; k < n; ++k) {
                r2.emplace_back(r[k] + kr1[k] * m_timestep*0.5);
            }
            system.set_positions(r2);
            kv2 = system.get_accelerations();

            for (int l = 0; l < n; ++l) {
                kr2.emplace_back(v[l] + kv1[l] * m_timestep*0.5);
            }
            std::vector <Vector3> r3{};
            for (int m = 0; m < n; ++m) {
                r3.emplace_back(r[m] + kr2[m] * m_timestep *0.5);
            }
            system.set_positions(r3);
            kv3 = system.get_accelerations();

            for (int i1 = 0; i1 < n; ++i1) {
                kr3.emplace_back(v[i1] + kv2[i1] * m_timestep*0.5);
            }

            std::vector <Vector3> r4{};
            for (int k1 = 0; k1 < n; ++k1) {
                r4.emplace_back(r[k1] + kr3[k1] * m_timestep);
            }
            system.set_positions(r4);
            kv4 = system.get_accelerations();

            for (int l1 = 0; l1 < n; ++l1) {
                kr4.emplace_back(v[l1]+kv3[l1]*m_timestep);
            }

            std::vector<Vector3> v_new {};
            std::vector<Vector3> r_new {};

            for (int m1 = 0; m1 < n; ++m1) {
                v_new.emplace_back(v[m1]+(m_timestep/6.0)*kv1[m1]+2.0*kv2[m1]+2.0*kv3[m1]+kv4[m1]);
                r_new.emplace_back(r[m1]+(m_timestep/6.0)*kr1[m1]+2.0*kr2[m1]+2.0*kr3[m1]+kr4[m1]);
            }

            trajectory.set_position(r_new, v_new);
            system.set_positions(r_new);
            system.set_velocities(v_new);

            ++progress;
        }
    }

    std::cout << "Integration finished." << std::endl;
}

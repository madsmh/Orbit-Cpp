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

void rk4::run(Trajectory &trajectory, System &system) {

    long n = system.get_number_of_bodies();
    
    for (int j = 0; j < m_rows; ++j) {
        if (j==0){
            // Initial setup.

            std::vector<Vector3> r = system.get_positions();
            std::vector<Vector3> v = system.get_velocities();

            trajectory.set_position(r, v);
        } else {
            std::vector<Vector3> r = trajectory.get_positions_at_index(i-1);
            std::vector<Vector3> v = trajectory.get_velocities_at_index(i-1);

            std::vector<Vector3> kv1 = system.get_accelerations();
            std::vector<Vector3> kv2 {};
            std::vector<Vector3> kv3 {};
            std::vector<Vector3> kv4 {};

            std::vector<Vector3> kr1 = v;
            std::vector<Vector3> kr2 {};
            std::vector<Vector3> kr3 {};
            std::vector<Vector3> kr4 {};

            std::vector<Vector3>  r2 {};
            for (int k = 0; k < n; ++k) {
                r2.emplace_back(r[k]+kr1[k]*m_timestep/2);
            }
            system.set_positions(r2);
            kv2 =  system.get_accelerations();

            for (int l = 0; l < n; ++l) {
                kr2.emplace_back(v[l]*kv1[l]*m_timestep/2);
            }
            std::vector<Vector3> r3 {};
            for (int m = 0; m < n; ++m) {
                r3.emplace_back(r[m]+kr2[m]*m_timestep/2);
            }
            system.set_positions(r3);
            kv3 = system.get_accelerations();

            for (int i1 = 0; i1 < n; ++i1) {
                kr3.emplace_back(v[i1]*kv2[i1]*m_timestep/2);
            }


        }
    }

}

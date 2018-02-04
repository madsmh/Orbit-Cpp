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

#ifndef ORBIT3D_RK4_H
#define ORBIT3D_RK4_H

#include "trajectory.h"
#include "system.h"

class rk4 {

    double m_timestep;
    long m_rows = 0;
    int m_detail;
public:
    void setup(int detail, long rows){
        m_timestep = ((double) 3600.0)/((double) detail);
        m_rows = rows;
    }

    void run(Trajectory &trajectory, System &system);

};


#endif //ORBIT3D_RK4_H

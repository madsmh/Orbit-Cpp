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
    along with Orbit Simulator.  If not, see <http://www.gnu.org/licenses/>.
 *
 * */

#include "vector3.h"

class Body {
    Vector3 m_r, m_v, m_r0, m_v0;
    double m_radius, m_gm, m_mass;

    std::string m_name;

public:
    Body(std::string name, Vector3 pos0, Vector3 vel0,
         double gm, double radius) {
        m_r0 = pos0;
        m_r = m_r0;

        m_v0 = vel0;
        m_v = m_v0;

        m_gm = gm;

        m_mass = m_gm/6.67259e-11;

        m_radius = radius;

        m_name = name;
    }

    Vector3 get_position() const{
        return m_r;
    };

    Vector3 get_velocity() const{
        return m_v;
    };


    void set_position(const Vector3& pos) {
        m_r = pos;
    };

    void set_velocity(const Vector3& vel) {
        m_v = vel;
    };


    double get_radius() const{
        return m_radius;
    };

    double get_GM() const{
        return m_gm;
    };

    std::string get_name() const{
        return m_name;
    };

    double get_kinetic_energy() const {
        return 0.5 * m_mass * m_v.norm2();
    }
    Vector3 get_momentum() const {
        return m_mass * m_v;
    }

    Vector3 get_angular_momentum() const {
        return m_r * get_momentum();
    }

};


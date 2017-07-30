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

#ifndef ORBIT3D_PROPERTIESFILE_H
#define ORBIT3D_PROPERTIESFILE_H

#include <vector>
#include "vector3.h"

struct PropertiesFile {
    std::string name{};
    double r{};
    double GM{};
};



class PhysicalProperties {
    std::vector<std::string> self_names;
    std::vector<double> self_radii;
    std::vector<double> self_GMs;

public:
    PhysicalProperties(long n = 0);
    std::vector<std::string> get_names(){
        return self_names;
    };

    std::vector<double> get_radii(){
        return self_radii;
    };

    std::vector<double> get_GMs(){
        return self_GMs;
    };
};


#endif //ORBIT3D_PROPERTIESFILE_H

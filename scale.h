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

#ifndef ORBIT3D_SCALE_H
#define ORBIT3D_SCALE_H

#include "vector3.h"

class Scale {
    double factor;

public:

    explicit Scale (double s){
        factor = s;
    }

    float scalar(double x){
        return x*factor;
    }

    Vector3 vector(Vector3 v){

        return v * factor;
    }

};


#endif //ORBIT3D_SCALE_H

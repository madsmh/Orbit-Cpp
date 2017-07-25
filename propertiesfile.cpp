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


#include <fstream>
#include "propertiesfile.h"

PhysicalProperties::PhysicalProperties(){
    std::string path = "../physical_properties/properties.csv";
    std::ifstream filereader;

    PropertiesFile p;

    std::vector<PropertiesFile> data;
    std::string buffer;

    filereader.open(path);
    if(filereader.is_open()){
        getline(filereader, buffer, '\n');
        while (!filereader.eof()){
            getline(filereader, buffer, ',');
            p.name = buffer;
            std::cout << "Reading physical constants for object " << buffer << std::endl;
            getline(filereader, buffer, ',');
            p.GM = stod(buffer);
            getline(filereader, buffer, ',');
            p.r = stod(buffer);
            getline(filereader, buffer, '\n');

            data.emplace_back(p);
        }
        filereader.close();
    }

    long n = data.size();
    for (int i = 0; i < n; ++i) {

        PropertiesFile d = data[i];
        self_names.emplace_back(d.name);
        self_radii.emplace_back(d.r);
        self_GMs.emplace_back(d.GM);
    }
};

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
#include <sstream>

#include "propertiesfile.h"

void PhysicalProperties::get_data(long n)  {
    std::string path = "physical_properties/properties.csv";
    std::ifstream filereader;

    PropertiesFile p;

    std::vector<PropertiesFile> data;
    std::string buffer;
    QString out;

    filereader.open(path);

    if(filereader.is_open()){
        getline(filereader, buffer, '\n');
        while (!filereader.eof()){
            getline(filereader, buffer, ',');
            if (buffer.empty()) break;
            p.name = buffer;
            emit (QString::fromUtf8(std::string("Reading physical constants for object " + buffer + "\n").c_str()));
            getline(filereader, buffer, ',');
            std::stringstream(buffer) >> p.r;
            emit (QString::fromUtf8(std::string("Radius = " + std::to_string(p.r) + "\n").c_str()));
            getline(filereader, buffer, ',');
            std::stringstream(buffer) >> p.GM;
            QString::fromUtf8(std::string("GM = " + std::to_string(p.GM) + "\n").c_str());
            getline(filereader, buffer, ',');
            getline(filereader, buffer, '\n');

            data.emplace_back(p);
        }
        filereader.close();
    }

    if (n==0)
        n = data.size();
    for (int i = 0; i < n; ++i) {

        PropertiesFile d = data[i];
        self_names.emplace_back(d.name);
        self_radii.emplace_back(d.r);
        self_GMs.emplace_back(d.GM);
    }
};

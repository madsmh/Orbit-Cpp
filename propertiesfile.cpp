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
#include <boost/tokenizer.hpp>

#include "propertiesfile.h"

void PhysicalProperties::get_data(long n)  {
    std::string path = "physical_properties/properties.csv";
    std::ifstream filereader;

    PropertiesFile p;

    std::vector<PropertiesFile> data;
    std::string buffer;

    filereader.open(path);

    typedef boost::tokenizer<boost::escaped_list_separator<char> > tokenizer;

    if(filereader.is_open()){
        getline(filereader, buffer); // Ignore first line of file.
        while (!filereader.eof()){
            getline(filereader, buffer);
            if(!buffer.empty()) {
                tokenizer tok(buffer);

                auto tok_it = tok.begin();

                p.name = *tok_it;

                ++tok_it;

                p.r = std::stod(*tok_it);

                ++tok_it;

                p.GM = std::stod(*tok_it)*1.0e9;

                ++tok_it;
                ++tok_it;

                p.j2 = std::stod(*tok_it);

                // std::cout << p.name << ", " << p.r << ", " << p.GM << std::endl;
                data.emplace_back(p);
            }
        }
        filereader.close();
    }

    if (n==0)
        n = data.size();

    for (int i = 0; i < n; ++i) {

        self_names.emplace_back(data[i].name);
        self_radii.emplace_back(data[i].r);
        self_GMs.emplace_back(data[i].GM);
        self_j2s.emplace_back(data[i].j2);

    }

    std::cout << "Loaded physical constants for " << self_names.size() << " bodies." << std::endl;
};

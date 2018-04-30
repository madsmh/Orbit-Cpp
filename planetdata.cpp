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

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include "planetdata.h"



PlanetData::PlanetData(const std::vector<std::string> &names){
    m_planet_names = names;

    for (unsigned int i = 0; i < m_planet_names.size(); ++i) {
        m_positions.emplace_back(std::vector<Vector3> {});
        m_velocities.emplace_back(std::vector<Vector3> {});
    }
}

// Adapted from: https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
std::vector<std::string> split(std::string str, const std::string& delim) {
    size_t pos = 0;
    std::vector<std::string> result;
    while ((pos = str.find(delim)) != std::string::npos) {
        result.push_back(str.substr(0, pos));
        str.erase(0, pos + delim.length());
    }
    return result;
}

std::vector<HorizonsFile> PlanetData::horizons_to_structs(const std::string planet) {
    std::string path = "data/" + planet + ".csv";
    boost::algorithm::to_lower(path);
    boost::replace_all(path, " ", "_");

    std::ifstream filereader;
    HorizonsFile h = {};
    std::vector<HorizonsFile> data;

    std::string buffer;

    typedef boost::tokenizer<boost::escaped_list_separator<char>> tokenizer;

    filereader.open(path);

    if (filereader.is_open()) {
        while (!filereader.eof()) {
            std::getline(filereader, buffer);
            if (!buffer.empty()) {
                tokenizer tok(buffer);

                auto tok_it = tok.begin();

                h.x = std::stod(*tok_it) * 1000.0;
                ++tok_it;
                h.y = std::stod(*tok_it) * 1000.0;
                ++tok_it;
                h.z = std::stod(*tok_it) * 1000.0;
                ++tok_it;
                h.vx = std::stod(*tok_it) * 1000.0;
                ++tok_it;
                h.vy = std::stod(*tok_it) * 1000.0;
                ++tok_it;
                h.vz = std::stod(*tok_it) * 1000.0;

                data.emplace_back(h);
            }
        }
        filereader.close();
    }

    return data;
}


void PlanetData::structs_to_arrays() {

    long n = m_planet_names.size();

    std::string out;

    for (int j = 0; j < n; ++j) {
        std::vector<HorizonsFile> h_structs = horizons_to_structs(m_planet_names[j]);

        out = "Initialized vector of structs with " +  std::to_string(h_structs.size()) +
         " elements, self-positons have size " + std::to_string(m_positions.size()) + "\n";

        long n_of_structs = h_structs.size();

        for (int i = 0; i < n_of_structs; ++i) {
            HorizonsFile h = h_structs[i];
            m_positions[j].emplace_back(Vector3 (h.x, h.y, h.z));
            m_velocities[j].emplace_back(Vector3 (h.vx, h.vy, h.vz));
        }

    }
    std::cout << "Imported position and velocity data for " << m_positions.size()
              << " bodies." << std::endl;
}

std::vector<Vector3> PlanetData::get_body_positions(int body){
    return m_positions[body];
};

std::vector<Vector3> PlanetData::get_body_velocities(int body) {
    return m_velocities[body];
}

std::vector<Vector3 > PlanetData::get_starting_positions(){
    long n = m_planet_names.size();
    std::vector<Vector3> starting_positions;

    for (int i = 0; i < n; ++i) {
        starting_positions.emplace_back(get_body_positions(i)[0]);
    }

    return starting_positions;
}

std::vector<Vector3 > PlanetData::get_starting_velocities() {

    long n = m_planet_names.size();

    std::vector<Vector3> starting_velocities;

    for (int i = 0; i < n; ++i) {
        starting_velocities.emplace_back(get_body_velocities(i)[0]);
    }

    return starting_velocities;
}

void PlanetData::read_data() {

    structs_to_arrays();

}

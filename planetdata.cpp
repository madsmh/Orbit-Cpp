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


#include "planetdata.h"



PlanetData::PlanetData(const std::vector<std::string> &names){
    self_planet_names = names;

    for (unsigned int i = 0; i < self_planet_names.size(); ++i) {
        self_positions.emplace_back(std::vector<Vector3> {});
        self_velocities.emplace_back(std::vector<Vector3> {});
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
    std::string path = "data/" + planet + ".txt";
    std::transform(path.begin(), path.end(), path.begin(), ::tolower);

    std::ifstream filereader;
    HorizonsFile h = {};
    std::vector<HorizonsFile> data;

    std::string buffer;

    std::string out;

    out = "Reading position and velocity data for " + planet + "\n";

    filereader.open(path);

    if (filereader.is_open()) {
        // Use a state machine to control
        // the file reading.
        bool reading = false;
        while (!filereader.eof()) {

            std::getline(filereader, buffer);

            if (buffer == "$$SOE") {
                reading = true;
            } else if (buffer == "$$EOE") {
                reading = false;
            } else if (reading) {
                std::vector<std::string> vals = split(buffer, ",");
                std::istringstream (vals[2]) >> h.x;
                h.x *= 1000.0;

                std::istringstream(vals[3])>> h.y;
                h.y *= 1000.0;

                std::istringstream (vals[4]) >> h.z;
                h.z *= 1000.0;

                std::istringstream (vals[5]) >> h.vx;
                h.vx *= 1000.0;

                std::istringstream (vals[6]) >> h.vy;
                h.vy *= 1000.0;

                std::istringstream (vals[7]) >> h.vz;
                h.vz *= 1000.0;
                data.emplace_back(h);
            }
        }

        out = "Closing file.\n";

    }

    return data;
}


void PlanetData::structs_to_arrays() {

    long n = self_planet_names.size();

    std::string out;

    for (int j = 0; j < n; ++j) {
        std::vector<HorizonsFile> h_structs = horizons_to_structs(self_planet_names[j]);
        out = "Initialized vector of structs with " +  std::to_string(h_structs.size()) +
         " elements, self-positons have size " + std::to_string(self_positions.size()) + "\n";

        long n_of_structs = h_structs.size();

        for (int i = 0; i < n_of_structs; ++i) {
            HorizonsFile h = h_structs[i];
            self_positions[j].emplace_back(Vector3 (h.x, h.y, h.z));
            self_velocities[j].emplace_back(Vector3 (h.vx, h.vy, h.vz));
        }
        out = "Saved structs to vector\n";

    }

}

std::vector<Vector3> PlanetData::get_body_positions(int body){
    return self_positions[body];
};

std::vector<Vector3> PlanetData::get_body_velocities(int body) {
    return self_velocities[body];
}

std::vector<Vector3 > PlanetData::get_starting_positions(){
    long n = self_planet_names.size();
    std::vector<Vector3> starting_positions;

    for (int i = 0; i < n; ++i) {
        starting_positions.emplace_back(get_body_positions(i)[0]);
    }

    return starting_positions;
}

std::vector<Vector3 > PlanetData::get_starting_velocities() {

    long n = self_planet_names.size();

    std::vector<Vector3> starting_velocities;

    for (int i = 0; i < n; ++i) {
        starting_velocities.emplace_back(get_body_velocities(i)[0]);
    }

    return starting_velocities;
}

void PlanetData::read_data() {

    structs_to_arrays();

}

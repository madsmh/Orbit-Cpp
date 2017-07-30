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
    structs_to_arrays();
}

std::vector<HorizonsFile> PlanetData::horizons_to_structs(const std::string planet) {
    std::string path = "../data/" + planet + ".txt";
    std::transform(path.begin(), path.end(), path.begin(), ::tolower);

    std::ifstream filereader;
    HorizonsFile h = {};
    std::vector<HorizonsFile> data;

    std::string buffer;

    std::cout << "Reading position and velocity data for " << planet << std::endl;

    filereader.open(path);

    if (filereader.is_open()) {
        while (!filereader.eof()) {

            std::getline(filereader, buffer);

            if (buffer == "$$SOE") {
                std::getline(filereader, buffer, ',');
                std::getline(filereader, buffer, ',');
                std::getline(filereader, buffer, ',');
                h.x = stod(buffer)*1000;
                std::getline(filereader, buffer, ',');
                h.y = stod(buffer)*1000;
                std::getline(filereader, buffer, ',');
                h.z = stod(buffer)*1000;
                std::getline(filereader, buffer, ',');
                h.vx = stod(buffer)*1000;
                std::getline(filereader, buffer, ',');
                h.vy = stod(buffer)*1000;
                std::getline(filereader, buffer, ',');
                h.vz = stod(buffer)*1000;
                std::getline(filereader, buffer, ',');
                std::getline(filereader, buffer, ',');
                std::getline(filereader, buffer, ',');
                std::getline(filereader, buffer);
                data.emplace_back(h);

                while (buffer != "$$EOE"){
                    std::getline(filereader, buffer, ',');
                    std::getline(filereader, buffer, ',');
                    std::getline(filereader, buffer, ',');
                    h.x = stod(buffer)*1000;
                    std::getline(filereader, buffer, ',');
                    h.y = stod(buffer)*1000;
                    std::getline(filereader, buffer, ',');
                    h.z = stod(buffer)*1000;
                    std::getline(filereader, buffer, ',');
                    h.vx = stod(buffer)*1000;
                    std::getline(filereader, buffer, ',');
                    h.vy = stod(buffer)*1000;
                    std::getline(filereader, buffer, ',');
                    h.vz = stod(buffer)*1000;
                    std::getline(filereader, buffer, ',');
                    std::getline(filereader, buffer, ',');
                    std::getline(filereader, buffer, ',');
                    std::getline(filereader, buffer);
                    data.emplace_back(h);
                }
                filereader.close();
            }

        }

        std::cout << "Closing file" << std::endl;
    }
    std::cout << "Returned vector is of size: " << data.size() << std::endl;
    return data;
}


void PlanetData::structs_to_arrays() {

    long n = self_planet_names.size();

    for (int j = 0; j < n; ++j) {
        std::vector<HorizonsFile> h_structs = horizons_to_structs(self_planet_names[j]);
        std::cout << "Initialized vector of structs with " <<  h_structs.size()
        << " elements" << " self-positons have size " << self_positions.size() << std::endl;
        long n_of_structs = h_structs.size();

        for (int i = 0; i < n_of_structs; ++i) {
            HorizonsFile h = h_structs[i];
            self_positions[j].emplace_back(Vector3 (h.x, h.y, h.z));
            self_velocities[j].emplace_back(Vector3 (h.vx, h.vy, h.vz));
        }
        std::cout << "Saved structs to vector" << std::endl;
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

    std::cout << "Inserted starting values into vector" << std::endl;

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

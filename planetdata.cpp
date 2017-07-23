//
// Created by mads on 23-07-17.
//

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>

#include "planetdata.h"
#include "vector3.h"



PlanetData::PlanetData(std::vector<std::string> names){
    self_planet_names = std::move(names);

    structs_to_arrays();
}

std::vector<HorizonsFile> PlanetData::horizons_to_structs(const std::string planet) {
    std::string path = "data/" + planet + ".csv";
    std::transform(path.begin(), path.end(), path.begin(), ::tolower);

    std::ifstream filereader;
    HorizonsFile h = {};
    std::vector<HorizonsFile> data;

    std::string buffer;

    std::cout << "Reading data for " << planet << std::endl;

    filereader.open(path);
    if (filereader.is_open()) {
        std::getline(filereader, buffer);
        while (!filereader.eof()) {
            std::getline(filereader, buffer, ',');
            if (buffer == "") break;
            std::getline(filereader, buffer, ',');
            std::getline(filereader, buffer, ',');
            h.x = stod(buffer);
            std::getline(filereader, buffer, ',');
            h.y = stod(buffer);
            std::getline(filereader, buffer, ',');
            h.z = stod(buffer);
            std::getline(filereader, buffer, ',');
            h.vx = stod(buffer);
            std::getline(filereader, buffer, ',');
            h.vy = stod(buffer);
            std::getline(filereader, buffer, ',');
            h.vz = stod(buffer);
            std::getline(filereader, buffer, ',');
            std::getline(filereader, buffer, ',');
            std::getline(filereader, buffer, ',');
            std::getline(filereader, buffer, '\n');
            data.push_back(h);
        }
        filereader.close();
    }
    return data;
}


void PlanetData::structs_to_arrays() {

    long n = self_planet_names.size();

    for (int j = 0; j < n; ++j) {
        std::vector<HorizonsFile> h_structs = horizons_to_structs(self_planet_names[j]);

        long n_of_structs = h_structs.size();

        for (int i = 0; i < n_of_structs; ++i) {
            HorizonsFile h = h_structs[i];
            self_positions[j].emplace_back(Vector3(h.x, h.y, h.z));
            self_velocities[j].emplace_back(Vector3(h.vx, h.vy, h.vz));
        }
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
